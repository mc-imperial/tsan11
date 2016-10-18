#!/bin/bash
set -x
set -e
set -u

svn checkout http://llvm.org/svn/llvm-project/llvm/tags/RELEASE_390/final llvm
cd llvm/tools
svn checkout http://llvm.org/svn/llvm-project/cfe/tags/RELEASE_390/final clang
cd ../projects
svn checkout --revision 272792 http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
svn checkout http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_390/final libcxx
svn checkout http://llvm.org/svn/llvm-project/libcxxabi/tags/RELEASE_390/final libcxxabi
#cd compiler-rt/lib/tsan
#svn patch tsan11.diff
cd ..
mkdir build272792
cd build272792
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make -j4

