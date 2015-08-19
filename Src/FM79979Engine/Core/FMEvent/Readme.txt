

cEventManager  store all necessary data and original xml data
	five major xml data,
						1.EventPackages.xml
						2.EventObjectStatus.xml
						3.EventInstanceList.xml
						4.EventVarialbe.xml
						5.font file
cEventPackageRunner load eventpackage to get current eventpackage and remove old resource at cEventManager,and update current 

eventpackage until satisfied condition and load next eventpackage.

cEventPackage is what we want to view,it contain a list of cEventObjectInstance

cEventObjectInstance contain 2 objects	cEventObjectStatus and cEventInstance

cEventObjectStatus for the major rendering object it is a status list,could be image or mpdi or particle

cEventInstance	a instance contain a trigger event list and event activer list,if all trigger is done activer will be called

event type  trigger,activer

事件管理者儲存了所有必需要的資料以及原始的xml檔案,包含了指定的目前事件包名稱以及事件物件狀態以及事件實體
	5個主要的檔案
						1.EventPackages.xml
						2.EventObjectStatus.xml
						3.EventInstanceList.xml
						4.EventVarialbe.xml
						5.font file		
事件包執行者讀取事件包中的指定事件包並且從事件管理者中移除不使用的資料,

並且不斷的更新事件包中的資訊直到事件包已經滿足條件在去讀取下一個新的事件包

事件包是我們想要看到的東西,它包含了事件物件實體的列表

事件物件實體包含了事件物件狀態列表以及事件實體

事件物件狀態是主要的畫面更新物件,他是一個狀態列表它可以是圖片或是mpdi或是粒子

事件實體 一個包含觸發以及執行的事件列,假如所有的觸發事件都完成了執行事件將執行

事件型態  觸發事件  執行事件


//sample for list select
	<cEventInstance Name="VendorMenu_GameSetup_8_CoinToScore" Repeat="1" >
		<EventTrigger>
			<cEventMultiButton Name="79979" cEventVariable="CoinToScore" ButtonSelectionType="One" StayAtLastSelect="1" PrefixButtonName="Prefix" >
				<cEventButton Name="Prefix" Text="投退幣每枚:" BGColor="0.3,0.3,0.0,1" Pos="0,300,0" />
				<cEventButton Name="1"    Text="1" BGColor="1,0,0.5,1" Pos="260,300,0" />
				<cEventButton Name="2"    Text="2" BGColor="1,0,0.5,1" Pos="290,300,0" />
				<cEventButton Name="5"    Text="5" BGColor="1,0,0.5,1" Pos="320,300,0"  />
				<cEventButton Name="10"   Text="10" BGColor="1,0,0.5,1" Pos="350,300,0" />
				<cEventButton Name="20"   Text="20" BGColor="1,0,0.5,1" Pos="410,300,0" />
				<cEventButton Name="25"   Text="25" BGColor="1,0,0.5,1" Pos="470,300,0" />
				<cEventButton Name="50"   Text="50" BGColor="1,0,0.5,1" Pos="530,300,0"  />
				<cEventButton Name="100"  Text="100" BGColor="1,0,0.5,1" Pos="620,300,0"/>
				<cEventButton Name="200"  Text="200" BGColor="1,0,0.5,1" Pos="260,350,0" />
				<cEventButton Name="500"  Text="500" BGColor="1,0,0.5,1" Pos="350,350,0" />
				<cEventButton Name="1000" Text="1000" BGColor="1,0,0.5,1" Pos="440,350,0"  />
			</cEventMultiButton>
		</EventTrigger>
		<EventActiver>
			<cEventButtonSelectedActiver Name="79979" cEventVariable="Language" Result="1,2,5,10,20,25,50,100,200,500,1000"/>
		</EventActiver>
	</cEventInstance>

	<cEventInstance Name="VendorMenu_GameSetup_8_CoinToScoreSelectLeft" Repeat="1" AllTriggetDoneAtSameTime="1" >
		<EventTrigger>
			<cEventValueCheck Value="2" cEventVariable="AllSubMenuResetIndex" Same="1" DataType="wstring" />
			<cEventKeyUpDectect KeyData="B" />
		</EventTrigger>
		<EventActiver>
			<cEventListValueSelectionActiver Name="Test" ValueChangeType="Decrease" cEventVariable="CoinToScore"  >
				<cListValueChanger  DataType="wstring" Data="1,2,5,10,20,25,50,100,200,500,1000" Loop="1" />
			</cEventListValueSelectionActiver>
		</EventActiver>
	</cEventInstance>
	<cEventInstance Name="VendorMenu_GameSetup_8_CoinToScoreSelectRight" Repeat="1" AllTriggetDoneAtSameTime="1" >
		<EventTrigger>
			<cEventValueCheck Value="2" cEventVariable="AllSubMenuResetIndex" Same="1" DataType="wstring" />
			<cEventKeyUpDectect KeyData="N" />
		</EventTrigger>
		<EventActiver>
			<cEventListValueSelectionActiver Name="Test" ValueChangeType="Increase" cEventVariable="CoinToScore"  >
				<cListValueChanger  DataType="wstring" Data="1,2,5,10,20,25,50,100,200,500,1000" Loop="1" />
			</cEventListValueSelectionActiver>
		</EventActiver>
	</cEventInstance>