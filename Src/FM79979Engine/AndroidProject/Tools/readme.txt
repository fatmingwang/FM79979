http://stackoverflow.com/questions/8300822/android-how-to-find-which-platform-version-an-apk-targets


java -jar apktool.jar d app.apk
Then look in the generated apktool.yml file for this:

sdkInfo:
  minSdkVersion: '11'
  targetSdkVersion: '17'
You can match the SDK version to Android version here . In the above example minimum Android version is 3.0 and target version is 4.2.