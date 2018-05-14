#!/bin/bash -e

PROJECT_DIR=$(cd "$(dirname ${BASH_SOURCE[0]})" && pwd)

set -o pipefail
set -o errexit

GCC=`which gcc`
GXX=`which g++`

# Check deps 
source $PROJECT_DIR/cmake/seastar.conf
if [[ "${SEASTAR}" == "" ]]; then 
  echo "you need to specify seastar project path in cmake/seastar.conf"
  exit 1
fi 
echo "SEASTAR: $SEASTAR"

# Env variable 
export SEASTAR

# Build openmit-ps 
if [ "X$is_all_build" == "X1" ] || [ ! -d $PROJECT_DIR/build ]; then
  echo "all build ..."
  rm -rf $PROJECT_DIR/build || true
  mkdir -p $PROJECT_DIR/build 
  cd $PROJECT_DIR/build
  cmake -D CMAKE_C_COMPILER=${GCC} -D CMAKE_CXX_COMPILER=${GXX} $PROJECT_DIR
else
  cd $PROJECT_DIR/build
fi

make 

echo "======== $0 done ========="
