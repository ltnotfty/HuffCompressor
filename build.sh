# !/bin/bash

cwd=`pwd`

proj_name=`basename ${cwd}`
echo "${proj_name}"

if [ -d build ]; then
	rm -rf ./build/*
else
	mkdir build
fi
cd build
cmake ../ -DCMAKE_BUILD_TYPE::STRING=Release

make -j4

[ -f "${proj_name}" ] &&  mv ${proj_name} ../ 





