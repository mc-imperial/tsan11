# TSan11

Skip to `Using the build shell...` if not using docker.


## Building the docker image from source

In the repository root:

```bash
docker build -t tsan11-image .
```

## Starting the docker container and sshing in

```bash
# Remove the container if you have created it previously
docker rm tsan11-container

# Create the container:
docker create -it --name tsan11-container tsan11-image

# Start the container
docker start -ia tsan11-container
```

In a different terminal in a temporary directory:

```bash
# Download the "insecure key".
curl -o insecure_key -fSL https://github.com/phusion/baseimage-docker/raw/master/image/services/sshd/keys/insecure_key
chmod 600 insecure_key

# Find the IP address of the container:
docker inspect tsan11-container

# ssh into the container as paul (replace 172.17.0.3).
ssh -i insecure_key paul@172.17.0.3

# Or, ssh in with X forwarding and compression.
ssh -YCi insecure_key paul@172.17.0.3
```

You can also `ssh` in as root.

## Using the build shell to build everything except the browsers

Note that everything is pre-built in the docker container
by default,
but you can delete the `/data/tsan11_build` directory
if you want to build from scratch.
Or, you can build outside the docker container;
in this case,
you may need to manually install the dependencies
found in the [Dockerfile](Dockerfile).

In the repository root (`/data/tsan11` if inside the docker container):

```bash
# Copy the build environment template and edit appropriately.
# No editing is required inside the docker container.
cp build_env.sh.template build_env.sh

# Start the build shell
./build_env.sh

# Type build:
build
```

The build shell starts `bash` with certain environment variables set.
See [scripts/](scripts/) for available commands.
The build command executes `python3`,
imports `scripts/tsan11.py`,
and calls `build()`.
Refer to [scripts/tsan11.py](scripts/tsan11.py).
The script downloads and builds LLVM (and other programs)
in the `TSAN11_BUILD_ROOT` directory (`/data/tsan11_build` by default).

Note that this Python script
will skip downloading/building/running
projects based on the existence of files/directories,
so if a step fails
you may need to remove certain files/directories.
For example, if building LLVM fails, correct the issues and
remove `$TSAN11_BUILD_ROOT/llvm_build`,
and then execute `build` again.

## Running the non-browser benchmarks

From the build shell:

```bash
# Type run
run
```

The test results will be output to `.txt` files in
`TSAN11_BUILD_ROOT`.

## Building Firefox

Again, Firefox should be pre-built in the docker container,
but we describe how to build it from scratch.

From the build shell:

```bash
# Type:
build_firefox
```

This will build Firefox three times.
You will have to run it manually from
`TSAN11_BUILD_ROOT/firefox_build{,_tsan,_tsan11}/dist/bin/firefox`.

## Building Chromium

From the build shell:

```bash
# Type:
build_chromium
```

This will build Chromium three times.
You will have to run it manually from
`TSAN11_BUILD_ROOT/chromium_build{,_tsan,_tsan11}/src/out/build/chrome`.

## Performing any command

From the build shell:

```
# Start IPython 3 interpreter.
ipython3

# Import the build script:
import tsan11

# Type `tsan.` and press tab:
tsan.[TAB]
tsan11.build                             tsan11.cdschecker_modified_bench_build   tsan11.get_depot_tools                   tsan11.llvm_patched_build_lib
tsan11.build_all_chromium                tsan11.chromium                          tsan11.get_firefox                       tsan11.llvm_patched_lib_tsan
tsan11.build_all_firefox                 tsan11.chromium_build                    tsan11.get_llvm                          tsan11.mkdir_p
...

# Check scripts/tsan.py for details.

# E.g. to build Chromium using tsan11:
# LLVM with tsan11 will be built if required.
tsan11.build_chromium(tsan11.config_tsan11)

# E.g. Run the litmus tests with tsan03.
# LLVM will be built if required.
tsan11.run_litmus_tests(tsan11.config_tsan)
```

Again, remember that the build script will
naively check whether certain files/directories
exist and potentially skip targets.
E.g. if you don't delete 
`TSAN11_BUILD_ROOT/cdschecker_modified_bench_results_tsan11.txt`,
then the litmus tests for tsan11 will be skipped.
