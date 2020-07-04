https://docs.microsoft.com/en-us/windows/apps/winui/winui2/getting-started

in App.xaml file 

please add 
    <Application.Resources>
        <XamlControlsResources xmlns="using:Microsoft.UI.Xaml.Controls" />
    </Application.Resources>


localhost connection setup
keyword loopback

first open cmd and do 
checknetisolation loopbackexempt -s
find out your uwp app name
then do as below
checknetisolation LoopbackExempt -is -n=YourAppName
checknetisolation LoopbackExempt -is -n=com.fm_r7ytfjdgrj97w
https://docs.microsoft.com/en-us/windows/uwp/communication/interprocess-communication







custom delegete 
https://docs.microsoft.com/en-us/cpp/cppcx/delegates-c-cx?view=vs-2019