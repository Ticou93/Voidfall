cd ../

git clone git://github.com/bkaradzic/bx.git
git clone git://github.com/bkaradzic/bimg.git
git clone git://github.com/bkaradzic/bgfx.git

cd bgfx

..\bx\tools\bin\windows\genie vs2017
start .build\projects\vs2017\bgfx.sln