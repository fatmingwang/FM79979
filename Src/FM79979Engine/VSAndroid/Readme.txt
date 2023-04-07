if clang.exe not found please ensure NDK and ANDROID_SDK and something related system environment variables setup well
VS default installed directory
C:\Microsoft\AndroidNDK

https://learn.microsoft.com/en-us/xamarin/android/troubleshooting/questions/android-sdk-location?tabs=windows


if you suffered some unreference symbol
change lib input order?


ensure you have take a look at out console window
all the message you have readed.
because the error messages it should be there.







https://msdn.microsoft.com/en-us/library/dn707591.aspx


https://www.visualstudio.com/en-us/docs/build/apps/mobile/xcode-ios


gradle with your project so
https://stackoverflow.com/questions/44929511/specify-so-files-in-build-gradle-file-android-studio



If you want to use it you can call it in your class like this:

static {
    System.loadLibrary("libSONAME");
}
Add the following in your gradle:

jniLibs.srcDirs = ['libs']






STL use 
LLVM libc++ shared library (c++_shared)

or you will lost some so file while your app start.


somehow crash and have no idea
please check AndroidManifest.xml


      <!-- must add this or your so won't be loaded. -->
      <meta-data android:name="android.app.lib_name"
              android:value="CarGoApp" />

ensure you so file name is correct!