#! /usr/bin/bash
DEBUG_DIR=../debug/
RELEASE_DIR=../release/
echo "Building ..."
if [ ! -d "${DEBUG_DIR}" ] ; then
echo "Generating Directory at ${DEBUG_DIR}"
mkdir $DEBUG_DIR
fi
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B $DEBUG_DIR
cmake --build $DEBUG_DIR
echo "Built Debug at ${DEBUG_DIR}"

if [ ! -d "${RELEASE_DIR}" ] ; then
echo "Generating Directory at ${RELEASE_DIR}"
mkdir $RELEASE_DIR
fi
cmake -DCMAKE_BUILD_TYPE=Release -S . -B $RELEASE_DIR
cmake --build $RELEASE_DIR
echo "Built Release at ${RELEASE_DIR}"
