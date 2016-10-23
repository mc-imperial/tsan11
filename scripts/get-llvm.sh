#!/bin/bash
set -x
set -e
set -u

svn checkout http://llvm.org/svn/llvm-project/llvm/tags/RELEASE_390/final .
cd tools
svn checkout http://llvm.org/svn/llvm-project/cfe/tags/RELEASE_390/final clang
cd ..
cd projects
svn checkout --revision 272792 http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
svn checkout http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_390/final libcxx
svn checkout http://llvm.org/svn/llvm-project/libcxxabi/tags/RELEASE_390/final libcxxabi
cd ..



