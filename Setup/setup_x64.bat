cd ../

git submodule update --init --recursive

mkdir build
cd build/
cmake ..
start deps/deps.sln
