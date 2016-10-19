#!/bin/bash
set -x
set -e
set -u

svn checkout --revision 272792 http://llvm.org/svn/llvm-project/llvm/trunk .
cd tools
svn checkout --revision 272792 http://llvm.org/svn/llvm-project/cfe/trunk clang
cd ..
cd projects
svn checkout --revision 272792 http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
svn checkout --revision 272792 http://llvm.org/svn/llvm-project/libcxx/trunk libcxx
svn checkout --revision 272792 http://llvm.org/svn/llvm-project/libcxxabi/trunk libcxxabi
cd ..



