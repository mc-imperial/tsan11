import os
import subprocess
from os import path, chdir
import shutil
import errno
import io

source_root = os.environ["TSAN11_SOURCE_ROOT"]
build_root = os.environ["TSAN11_BUILD_ROOT"]

llvm = path.join(build_root, "llvm")
llvm_lib_tsan = path.join(llvm, "projects", "compiler-rt", "lib", "tsan")
llvm_build = path.join(build_root, "llvm_build")
llvm_build_bin = path.join(llvm_build, "bin")

llvm_patched = path.join(build_root, "llvm_patched")
llvm_patched_lib_tsan = path.join(llvm_patched, "projects", "compiler-rt", "lib", "tsan")
llvm_patched_build = path.join(build_root, "llvm_patched_build")
llvm_patched_build_bin = path.join(llvm_patched_build, "bin")
patch_file = path.join(source_root, "scripts", "tsan11.diff")

cdschecker = path.join(source_root, "cdschecker_tests")

class CDSCheckerBuildVersion(object):

    def __init__(self, suffix: str, my_llvm_build_bin: str, sanitize: bool):
        self.build = path.join(build_root, "cdschecker_build" + suffix)
        self.test_all = path.join(self.build, "test_all.sh")
        self.llvm_build_bin = my_llvm_build_bin
        self.sanitize = sanitize
        self.results = path.join(build_root, "cdschecker" + suffix + "_results.txt")


cdschecker_build_normal = CDSCheckerBuildVersion("", llvm_build_bin, False)
cdschecker_build_tsan = CDSCheckerBuildVersion("tsan", llvm_build_bin, True)
cdschecker_build_tsan11 = CDSCheckerBuildVersion("tsan11", llvm_patched_build_bin, True)


path = os.environ["PATH"]

# Utils #

def mkdir_p(path):
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

def add_path(directory: str):
    os.environ["PATH"] = directory + os.pathsep + os.environ["PATH"]
def reset_path():
    os.environ["PATH"] = path

# Main targets #

def get_llvm():
    mkdir_p(llvm)
    chdir(llvm)
    subprocess.check_call("get-llvm-original.sh")


def build_llvm():
    get_llvm()
    mkdir_p(llvm_build)
    chdir(llvm_build)
    subprocess.check_call(["cmake", "-G", "Unix Makefiles", "-DCMAKE_BUILD_TYPE=Release", llvm])
    subprocess.check_call(["make", "-j8"])


def get_llvm_patched():
    get_llvm()
    shutil.rmtree(llvm_patched_lib_tsan)
    copytree(llvm, llvm_patched)
    chdir(llvm_patched_lib_tsan)
    subprocess.check_call(["svn", "patch", patch_file])


def build_llvm_patched():
    get_llvm_patched()
    mkdir_p(llvm_patched_build)
    chdir(llvm_patched_build)
    subprocess.check_call(["cmake", "-G", "Unix Makefiles", "-DCMAKE_BUILD_TYPE=Release", llvm_patched])
    subprocess.check_call(["make", "-j8"])


def build_cdschecker_tsan(cdschecker_build: CDSCheckerBuildVersion):
    build_llvm()
    copytree(cdschecker, cdschecker_build.build)
    chdir(cdschecker_build.build)
    try:
        add_path(cdschecker_build.llvm_build_bin)
        # TODO: use cdschecker_build.sanitize
        subprocess.check_call(["make"])
    finally:
        reset_path()


def run_cdschecker_tsan(cdschecker_build: CDSCheckerBuildVersion):
    build_cdschecker_tsan(cdschecker_build)
    chdir(cdschecker_build.build)
    os.remove(cdschecker_build.results)
    with io.open(cdschecker_build.results) as f:
        subprocess.run(["cdschecker_build_tsan_test_all"], check=True, stdout=f, stderr=subprocess.STDOUT)
