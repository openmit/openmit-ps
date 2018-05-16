#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname ${BASH_SOURCE[0]})" && pwd)

thrift=`which thrift`

$thrift -r --gen cpp -o $SCRIPT_DIR $SCRIPT_DIR/tutorial.thrift
$thrift -r --gen cpp -o $SCRIPT_DIR $SCRIPT_DIR/shared.thrift
