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