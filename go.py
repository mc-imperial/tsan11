import os
import subprocess
import shutil
import errno
import io

source_root = os.environ["TSAN11_SOURCE_ROOT"]
build_root = os.environ["TSAN11_BUILD_ROOT"]

llvm = os.path.join(build_root, "llvm")
llvm_lib_tsan = os.path.join(llvm, "projects", "compiler-rt", "lib", "tsan")
llvm_build = os.path.join(build_root, "llvm_build")
llvm_build_bin = os.path.join(llvm_build, "bin")

llvm_patched = os.path.join(build_root, "llvm_patched")
llvm_patched_lib_tsan = os.path.join(llvm_patched, "projects", "compiler-rt", "lib", "tsan")
llvm_patched_build = os.path.join(build_root, "llvm_patched_build")
llvm_patched_build_bin = os.path.join(llvm_patched_build, "bin")
patch_file = os.path.join(source_root, "scripts", "tsan11.diff")

cdschecker = os.path.join(source_root, "cdschecker_tests")
cdschecker_build = os.path.join(build_root, "cdschecker_build")

litmus_tests = os.path.join(source_root, "litmus_tests")
litmus_tests_build = os.path.join(build_root, "litmus_tests_build")

firefox = os.path.join(build_root, "firefox")
firefox_build = os.path.join(build_root, "firefox_build")
mozconfig = os.path.join(source_root, "scripts", "mozconfig")


# class CDSCheckerBuildVersion(object):
#
#     def __init__(self, suffix: str, sanitize: bool, patched_llvm: bool):
#         self.suffix = suffix
#         self.build = os.path.join(build_root, "cdschecker_build" + suffix)
#         self.test_all = os.path.join(self.build, "test_all.sh")
#         self.llvm_build_bin = llvm_patched_build_bin if patched_llvm else llvm_build_bin
#         self.sanitize = sanitize
#         self.results = os.path.join(build_root, "cdschecker" + suffix + "_results.txt")
#         self.patched_llvm = patched_llvm


class BuildConfig(object):
    def __init__(self, suffix: str, sanitize: bool, patched_llvm: bool):
        self.suffix = suffix
        self.sanitize = sanitize
        self.patched_llvm = patched_llvm


config_normal = BuildConfig("", False, False)
config_tsan = BuildConfig("_tsan", True, False)
config_tsan11 = BuildConfig("_tsan11", True, True)


saved_path = os.environ["PATH"]

fast_check = True

# Utils #


def mkdir_p(path: str):
    try:
        os.makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


def copytree(src, dst, symlinks=False, ignore=None):
    if not os.path.exists(dst):
        os.makedirs(dst)
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            copytree(s, d, symlinks, ignore)
        else:
            if not os.path.exists(d) or os.stat(s).st_mtime - os.stat(d).st_mtime > 1:
                shutil.copy2(s, d)


def rmtree(path: str):
    try:
        shutil.rmtree(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.ENOENT:
            pass
        else:
            raise


def remove(path: str):
    try:
        os.remove(path)
    except OSError as exc:
        if exc.errno == errno.ENOENT:
            pass
        else:
            raise



# Main targets #


def get_llvm():
    print("get_llvm")
    if fast_check and os.path.exists(llvm):
        print("skipping")
        return
    mkdir_p(llvm)
    subprocess.check_call("get-llvm.sh", cwd=llvm)


def build_llvm():
    print("build_llvm")
    if fast_check and os.path.exists(llvm_build):
        print("skipping")
        return
    get_llvm()
    mkdir_p(llvm_build)
    subprocess.check_call(["cmake", "-G", "Unix Makefiles", "-DCMAKE_BUILD_TYPE=Release", llvm], cwd=llvm_build)
    subprocess.check_call(["make", "-j8"], cwd=llvm_build)


def get_llvm_patched():
    print("get_llvm_patched")
    if fast_check and os.path.exists(llvm_patched):
        print("skipping")
        return
    get_llvm()
    rmtree(llvm_patched_lib_tsan)
    copytree(llvm, llvm_patched)
    subprocess.check_call(["svn", "patch", patch_file], cwd=llvm_patched_lib_tsan)


def build_llvm_patched():
    print("build_llvm_patched")
    if fast_check and os.path.exists(llvm_patched_build):
        print("skipping")
        return
    get_llvm_patched()
    mkdir_p(llvm_patched_build)
    subprocess.check_call(["cmake", "-G", "Unix Makefiles", "-DCMAKE_BUILD_TYPE=Release", llvm_patched], cwd=llvm_patched_build)
    subprocess.check_call(["make", "-j8"], cwd=llvm_patched_build)


def build_cdschecker(config: BuildConfig):
    print("build_cdschecker" + config.suffix)
    if config.patched_llvm:
        build_llvm_patched()
    else:
        build_llvm()
    copytree(cdschecker, cdschecker_build + config.suffix)
    new_env = os.environ.copy()
    new_env["PATH"] = (llvm_patched_build_bin if config.patched_llvm else llvm_build_bin) + os.pathsep + new_env["PATH"]
    new_env["SANITIZE"] = "-fsanitize=thread" if config.sanitize else ""
    subprocess.check_call(["make"], cwd=cdschecker_build+config.suffix, env=new_env)


def run_cdschecker(config: BuildConfig):
    print("run_cdschecker" + config.suffix)
    results_file = os.path.join(build_root, "cdschecker_results" + config.suffix + ".txt")
    if fast_check and os.path.exists(results_file):
        print("skipping")
        return
    build_cdschecker(config)
    remove(results_file)
    with io.open(results_file, "w+") as f:
        subprocess.run(
            [os.path.join(cdschecker_build + config.suffix, "test_all.sh")],
            check=True,
            stdout=f,
            stderr=subprocess.STDOUT,
            cwd=cdschecker_build + config.suffix)


def build_litmus_tests(config: BuildConfig):
    print("build litmus tests")
    if config.patched_llvm:
        build_llvm_patched()
    else:
        build_llvm()
    copytree(litmus_tests, litmus_tests_build + config.suffix)
    new_env = os.environ.copy()
    new_env["PATH"] = (llvm_patched_build_bin if config.patched_llvm else llvm_build_bin) + os.pathsep + new_env["PATH"]
    new_env["SANITIZE"] = "-fsanitize=thread" if config.sanitize else ""
    subprocess.check_call(["make"], cwd=litmus_tests_build+config.suffix, env=new_env)


def run_litmus_tests(config: BuildConfig):
    print("run_litmus_tests" + config.suffix)
    results_file = os.path.join(build_root, "litmus_results" + config.suffix + ".txt")
    if fast_check and os.path.exists(results_file):
        print("skipping")
        return
    build_litmus_tests(config)
    remove(results_file)
    with io.open(results_file, "w+") as f:
        subprocess.run(
            [os.path.join(litmus_tests_build + config.suffix, "run_all.sh")],
            check=True,
            stdout=f,
            stderr=subprocess.STDOUT,
            cwd=litmus_tests_build+config.suffix)


def get_firefox():
    print("get_firefox")
    if fast_check and os.path.exists(firefox):
        print("skipping")
        return
    mkdir_p(firefox)
    subprocess.check_call(["hg", "clone", "https://hg.mozilla.org/mozilla-central/", os.curdir], cwd=firefox)
    subprocess.check_call(["hg", "update", "298600"], cwd=firefox)
    shutil.copyfile(mozconfig, os.path.join(firefox, "mozconfig"))


def build_firefox(config: BuildConfig):
    print("build_firefox" + config.suffix)
    if fast_check and os.path.exists(firefox_build + config.suffix):
        print("skipping")
    get_firefox()
    if config.patched_llvm:
        build_llvm_patched()
    else:
        build_llvm()
    mkdir_p(firefox_build + config.suffix)
    new_env = os.environ.copy()
    new_env["MOZ_OBJDIR"] = firefox_build + config.suffix
    new_env["PATH"] = (llvm_patched_build_bin if config.patched_llvm else llvm_build_bin) + os.pathsep + new_env["PATH"]
    new_env["CFLAGS"] = "-fsanitize=thread -fPIC -pie" if config.sanitize else ""
    new_env["CXXFLAGS"] = "-fsanitize=thread -fPIC -pie" if config.sanitize else ""
    new_env["LDFLAGS"] = "-fsanitize=thread -fPIC -pie" if config.sanitize else ""
    subprocess.check_call(
        ["make", "-f", "client.mk"],
        env=new_env,
        cwd=firefox)



if __name__ == "__main__":
    # for config in [config_normal, config_tsan, config_tsan11]:
    #     run_cdschecker(config)
    #     run_litmus_tests(config)
    build_firefox()
