#!/bin/bash -e

PROJECT_DIR=$(cd "$(dirname ${BASH_SOURCE[0]})" && pwd)

set -o pipefail
set -o errexit

GCC=`which gcc`
GXX=`which g++`

# Check deps 
source $PROJECT_DIR/cmake/deps.conf
if [[ "${SEASTAR_HOME}" == "" ]]; then 
  echo "you need to specify seastar project path in cmake/deps.conf"
  exit 1
fi 
echo "SEASTAR_HOME: $SEASTAR_HOME"

# Env variable 
export SEASTAR_HOME

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
