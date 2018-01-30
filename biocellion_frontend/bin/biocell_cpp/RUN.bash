#!/bin/bash

BIOCELLION_ROOT="$1"
DEBUG="$2"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/opt/tbb:."

if [ -z "$BIOCELLION_ROOT" ]; then
    BIOCELLION_ROOT="/opt/biocellion"
fi

if [ ! -d "$BIOCELLION_ROOT" ]; then
    echo "usage: $0 BIOCELLION_ROOT_DIRECTORY yes/no(DEBUG)"
    exit 1
fi

rm -rf output
if [ ! -d output ]; then
    mkdir output
fi

if [ "$DEBUG" == "yes" ]; then
    valgrind "${BIOCELLION_ROOT}"/framework/main/biocellion.DP.SPAGENT.DEBUG config.xml
else
    time "${BIOCELLION_ROOT}"/framework/main/biocellion.DP.SPAGENT.OPT config.xml
fi

