

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

�ƥ�޲z���x�s�F�Ҧ����ݭn����ƥH�έ�l��xml�ɮ�,�]�t�F���w���ثe�ƥ�]�W�٥H�Ψƥ󪫥󪬺A�H�Ψƥ����
	5�ӥD�n���ɮ�
						1.EventPackages.xml
						2.EventObjectStatus.xml
						3.EventInstanceList.xml
						4.EventVarialbe.xml
						5.font file		
�ƥ�]�����Ū���ƥ�]�������w�ƥ�]�åB�q�ƥ�޲z�̤��������ϥΪ����,

�åB���_����s�ƥ�]������T����ƥ�]�w�g��������b�hŪ���U�@�ӷs���ƥ�]

�ƥ�]�O�ڭ̷Q�n�ݨ쪺�F��,���]�t�F�ƥ󪫥���骺�C��

�ƥ󪫥����]�t�F�ƥ󪫥󪬺A�C��H�Ψƥ����

�ƥ󪫥󪬺A�O�D�n���e����s����,�L�O�@�Ӫ��A�C���i�H�O�Ϥ��άOmpdi�άO�ɤl

�ƥ���� �@�ӥ]�tĲ�o�H�ΰ��檺�ƥ�C,���p�Ҧ���Ĳ�o�ƥ󳣧����F����ƥ�N����

�ƥ󫬺A  Ĳ�o�ƥ�  ����ƥ�


//sample for list select
	<cEventInstance Name="VendorMenu_GameSetup_8_CoinToScore" Repeat="1" >
		<EventTrigger>
			<cEventMultiButton Name="79979" cEventVariable="CoinToScore" ButtonSelectionType="One" StayAtLastSelect="1" PrefixButtonName="Prefix" >
				<cEventButton Name="Prefix" Text="��h���C�T:" BGColor="0.3,0.3,0.0,1" Pos="0,300,0" />
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