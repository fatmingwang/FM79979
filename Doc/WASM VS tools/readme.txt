1.install emscripten
https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm#Emscripten_environment_setup


2.use VS tools
the offical emscripten seems only support VS2010 vs-tool-master.zip
but someone wrote the tools which is support to VS2015
but now the start browser only accept IE...-_-


my setp is first install VS Tools
then 
vs-toolsets


https://github.com/crosire/vs-toolsets

3.
open Src\FM79979Engine\EmptyProject\EmptyProject.sln

select emscripten build target and make WASM as start up project
I have add flags
on WASM project open property
to to Emscripten Linker->Command Line  

-s ALLOW_MEMORY_GROWTH=1

also 

Emscripten Linker->Input 
preloaded Resource Files I have made ../asserts as preload files
if any file you want to add please add into this folder


4.
aftern built you have to start python simple htt server in the folder you generate html(python -m SimpleHTTPServer)

then type the 127.0.0.1:port/ooxx.html(127.0.0.1:8000/WASM.html)
after few seconds download then it should works.