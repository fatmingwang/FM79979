cd C:\NVPACK\jdk1.6.0_45\bin
jarsigner -verbose -keystore abc.keystore -signedjar 123x.apk 123.apk abc.keystore
cd C:\NVPACK\android-sdk-windows\build-tools\19.1.0
zipalign -v 4 C:\NVPACK\jdk1.6.0_45\bin\123x.apk C:\NVPACK\jdk1.6.0_45\bin\456.apk