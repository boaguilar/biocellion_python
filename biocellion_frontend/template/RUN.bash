#!/bin/bash

BIOCELLION_ROOT="$1"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/opt/tbb:."

if [ -z "$BIOCELLION_ROOT" ]; then
    BIOCELLION_ROOT="/opt/biocellion"
fi

if [ ! -d "$BIOCELLION_ROOT" ]; then
    echo "usage: $0 BIOCELLION_ROOT_DIRECTORY"
    exit 1
fi

rm -rf output
if [ ! -d output ]; then
    mkdir output
fi
#valgrind "${BIOCELLION_ROOT}"/framework/main/biocellion.DP.SPAGENT.DEBUG config.xml
time "${BIOCELLION_ROOT}"/framework/main/biocellion.DP.SPAGENT.OPT config.xml
