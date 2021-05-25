https://khiav223577.github.io/blog/2018/01/20/%E5%A6%82%E4%BD%95%E4%BD%BF%E7%94%A8-SteamPipe-%E4%B8%8A%E6%9E%B6%E9%81%8A%E6%88%B2%E5%88%B0-Steam-%E5%B9%B3%E5%8F%B0/

download steamworks SDK
https://partner.steamgames.com/doc/sdk

copy your game to

Steamworks_sdk_151\sdk\tools\ContentBuilder\content


/tools/ContentBuilder/scripts，這裡預設會有二個檔案，
一個是 APP 建置設定檔，一個是 depot 建置設定檔。
檔案名字可以自由取，不一定要把 ID 寫在檔名後面。



DepotID: 改成實際上的 DepotID
ContentRoot: 建議改成相對路徑，路徑與此設定檔相對，因此設成 ..\content\
LocalPath: 建議寫相對路徑，例如英文語言包的話，可以寫成 ..\content\en\*。
註解說路徑會相對於 ContentRoot，但實際測試發現是相對於此設定檔的所在地。
如果抓不到檔案的話，可能要檢查一下是否是這裡的問題。

upload files with steamworks_sdk_151\sdk\tools\SteamPipeGUI.zip
unzip  follow readme