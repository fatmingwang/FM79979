because 
opencv_world440.dll
opencv_world440d.dll" 
size over 50MB so please unzip Media/opencv_world440_dll.7z

Copy the DLL file to the project root (same folder as the Package.appxmanifest, not the .sln file).



https://social.msdn.microsoft.com/Forums/windowsapps/en-US/26013e5e-3ff1-44a0-8797-0474dabd12d0/uwpincluding-dlls-in-a-uwp-application-in-visual-studio?forum=wpdevelop

How can I place DLLs in a sub-folder? I can only get my method to work in the project root folder. But what if I would like to place them in a sub-folder like Assets/ or something custom like Lib/ or DLLs/?
My method (which I swear I tried before but failed):
1. Copy the DLL file to the project root (same folder as the Package.appxmanifest, not the .sln file).
2. Right click on project -> Add -> Existing Item..., and add the DLL file.
3. Right click on the DLL file and select Properties. Change "Content" to "Yes".
That's it!  No modifications to the manifest are required.



fatal error LNK1104: cannot open file '../../../lib/tess/Debug/leptonica-1.78.0d.lib'

or 

miss leptonica-1.78.0d.dll

take a look at

FM79979\Doc\opencv\OCR

use vcpkg to build tess

copy the lib and dll to FM79979\Media