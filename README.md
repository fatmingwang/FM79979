# FM79979
FM79979 cross platform game engine(Android,Windows,iOS,Linux)


Game Project
	FM79979\Src\FM79979Engine\FishingFun
	FM79979\Src\FM79979Engine\BluffingGirl
	FM79979\Src\FM79979Engine\MagicTower     
	FM79979\Src\FM79979Engine\WavParser

Game Editor	
	FM79979\Src\FM79979Engine\TextureEditor

Texture resize tool
	TextureScale(required DirectX9)

opencv and UDP streaming test
	UDPOpenCV

Book Editor



![Editor](../master/Doc/FM79979/FMBook/Old/1FMBook1.png?raw=true)
	FM79979\Src\FM79979Engine\FMBookEditor


AndroidProject
	FM79979\Src\FM79979Engine\AndroidProject
	FM79979\Src\FM79979Engine\VSAndroid
		Nvidia CodeWorks no more update since 2019 so Android project convert to MS Androis project.
		Android Project is via Nvidia's CodeWorks(https://developer.nvidia.com/android-m-support-now-available-new-nvidia-codeworks-android-1r4)


Core(openGL,openGL ES,utility IO,logic.....)
	FM79979\Src\FM79979Engine\Core for cross platform framework
	It also contain AI and particle and some useful project to speed game develop.



Emscripten test project(web assembly)
	Code
	FM79979\Src\FM79979Engine\BluffingGirl\BluffingGirl.sln
	Demo
	FM79979\Media\Emscripten




Because I am lazy to fix dependency...
Please unzip Media/opencv_world440_dll.7z or all applucation can't work


some clips and demo

https://1drv.ms/f/s!AlnnK63qVSovjivk-gls9XCARW8v

PI.exe for textures pack to one texture
MPDI.exe for 2D animation

FM79979\Doc\FM79979\TextureEditorDoc\±Ð¾Ç-TextureEditor

ParticalEditor
	FM79979\Doc\FM79979\Particle
	http://wordpress-tw.marcpov.com/first-smoke-fx.htm
![Editor](../master/Doc/FM79979/Particle/Particle.JPG?raw=true)

=========================================
WASM build tools
pleae take look at 
Doc\WASM VS tools
if compile failed make sure /MP and setting(commandline) is suite for emscripten

Emscripten game demo
https://1drv.ms/f/s!AlnnK63qVSovg8BJYiTS-LGA9R9bfA?e=GPGNsh
![Editor](../master/Doc/BluffingGirl/gameplay.jpg?raw=true)
![Editor](../master/Doc/WASM VS tools/FishGame.JPG?raw=true)

