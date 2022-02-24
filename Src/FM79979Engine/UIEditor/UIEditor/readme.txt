https://docs.microsoft.com/en-us/windows/apps/winui/winui2/getting-started

in App.xaml file 

please add 
    <Application.Resources>
        <XamlControlsResources xmlns="using:Microsoft.UI.Xaml.Controls" />
    </Application.Resources>
OR
    <Application.Resources>
        <ResourceDictionary>
            <ResourceDictionary.MergedDictionaries>
                <XamlControlsResources xmlns="using:Microsoft.UI.Xaml.Controls" />
            </ResourceDictionary.MergedDictionaries>
        </ResourceDictionary>
    </Application.Resources>


localhost connection setup
keyword loopback

first open cmd and do
checknetisolation loopbackexempt -s
find out your uwp app name
then do as below
open terminal as administrator 
checknetisolation LoopbackExempt -is -n=YourAppName
checknetisolation LoopbackExempt -is -n=com.fm_r7ytfjdgrj97w
https://docs.microsoft.com/en-us/windows/uwp/communication/interprocess-communication

if some files can't be read
please select it and make its type as xml

packing

https://stackoverflow.com/questions/32144287/copying-assets-into-uwp-application-package
ensure your content copy into Assets folder.

Create a certificate for package signing
https://docs.microsoft.com/en-us/windows/msix/package/create-certificate-package-signing
or 
select Any CPU not x64(don't ask me why.)
1.right click on the project
2.publish->create app packages
3.Sideloading
4.Yes,select a certificate->create or select from File
https://stackoverflow.com/questions/53749304/unable-to-install-uwp-side-loading-builds
install cer in your client computer
After you successfully package the app with visual Studio,
there will be a security certificate file in the form of .cer under the *_Test folder.
You need to install the security certificate file before sideload your app.
I often follow these steps to install the certificate:
double click .cer file > choose Install certificate > choose Local machine > press the Browse button >
choose Trusted Root Certification Authorities(or Trusted people)

mouse click Add-AppDevPackage.ps1,right click->install->require permission press y

in the last please ensure you have install OpenAL driver...
https://www.openal.org/downloads/


file access
1.
https://docs.microsoft.com/en-us/windows/uwp/files/file-access-permissions
2.
https://support.microsoft.com/en-us/windows/-windows-10-file-system-access-and-privacy-a7d90b20-b252-0e7b-6a29-a3a688e5c7be
3.
https://stackoverflow.com/questions/50559764/broadfilesystemaccess-uwp





WPF

https://riptutorial.com/wpf/example/29837/creating-a-resource-dictionary

use aff new file and select type as XAML->ResourceDictionary
go to App.xaml
add below code.
    <Application.Resources>
        <ResourceDictionary>
            <ResourceDictionary.MergedDictionaries>
                <ResourceDictionary Source="ShareResource.xaml"/>
                <XamlControlsResources xmlns="using:Microsoft.UI.Xaml.Controls" />
            </ResourceDictionary.MergedDictionaries>
        </ResourceDictionary>
    </Application.Resources>


https://riptutorial.com/wpf/example/29835/style-applied-to-all-buttons

combox binding
https://www.c-sharpcorner.com/article/explain-combo-box-binding-in-mvvm-wpf/


custom binding
https://stackoverflow.com/questions/2478738/add-collection-or-array-to-wpf-resource-dictionary




file access
https://github.com/microsoft/Windows-universal-samples/tree/master/Samples/FileAccess

	String^ l_strPath = "D:\\Shipping\\List.txt";
	try
	{
		concurrency::create_task(StorageFile::GetFileFromPathAsync(l_strPath)).then([this](StorageFile^ file)
			{
				auto lllll = file->Name;
				create_task(FileIO::ReadTextAsync(file)).then([this](Platform::String^ l_strContent)
					{
						auto ll = l_strContent;
						int a = 0;
					});
			});
	}
	catch (Exception^ e)
	{
		auto ll = e->Message;
		int a = 0;
	}


if something wrong wwith UWP versio please copy dlls into folder(contain package.manifext)
libcurl.dll,libcurl-d.dll zlibd1.dll(these 3 dll please compile from vcpkg)