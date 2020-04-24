cd ../

git submodule update --init --recursive

cd deps/bgfx/bgfx/

..\bx\tools\bin\windows\genie --with-glfw vs2017
start .build\projects\vs2017\bgfx.sln