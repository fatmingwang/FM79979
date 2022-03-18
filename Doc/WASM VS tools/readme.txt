1.install emscripten
https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm#Emscripten_environment_setup
https://emscripten.org/docs/getting_started/downloads.html#mac-os-x

git pull

use latest-upstream with 
emsdk install latest-upstream
emsdk activate latest-upstream

or latest

# Download and install the latest SDK tools.
emsdk install latest
# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
emsdk activate latest
# Activate PATH and other environment variables in the current terminal
emsdk_env.sh
!!!!!!!!!!!!
emsdk install latest
if download file failed it's because old python version not match SSL so download manually and put it into zips folder
!!!!!!!!!!!!
2.use VS tools
the offical emscripten seems only support VS2010 vs-tool-master.zip
but someone wrote the tools which is support to VS2015
but now the start browser only accept IE...-_-


my setp is first install VS Tools
then 
vs-toolsets


https://github.com/crosire/vs-toolsets


it also works fine in VS2019
copy file(vs-toolsets) into
C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Microsoft\VC\v160

but you have to install individual components 
MSVC v140 - VS 2015 c++ build tools(v14.00)

now 
a Japanese make a install file for VS2019 and Vs2022

https://github.com/nokotan/VSExtForEmscripten



3.
open Src\FM79979Engine\EmptyProject\EmptyProject.sln

select emscripten build target and make WASM as start up project
I have add flags
on WASM project open property
to to Emscripten Linker->Command Line  
to enable memory grow
-s ALLOW_MEMORY_GROWTH=1
or indicate how many you need
-s TOTAL_MEMORY=2013265920

get more detail
  --profiling-funcs

also 

Emscripten Linker->Input 
preloaded Resource Files I have made ../asserts as preload files
if any file you want to add please add into this folder


4.
aftern built you have to start python simple htt server in the folder you generate html(python -m SimpleHTTPServer)

then type the 127.0.0.1:port/ooxx.html(127.0.0.1:8000/WASM.html)
after few seconds download then it should works.




emcc --clear-cache


-s SAFE_HEAP=1 -s TOTAL_MEMORY=2013265920 -s FETCH=1 -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=2  -s FULL_ES2=1 -s ASSERTIONS=1 --profiling


https://github.com/emscripten-core/emscripten/blob/incoming/ChangeLog.md?fbclid=IwAR1TE8Q0yyFYhoJUuXLbObzoZJTywUtqsniVx0ZS-2xnW_h12i2gwh8Fkq4#v13827-02102019

Breaking change with -s USE_PTHREADS=1 + -s FETCH=1: When building with -o a.html, the generated worker script is now named "a.fetch.js" according to the base name of the specified output, instead of having a fixed name "fetch-worker.js".
BluffingGirlWASM.worker.js rename to BluffingGirlWASM.fetch.js



