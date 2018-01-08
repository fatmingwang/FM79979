#ifndef _DOT_NET_COMMON_TOOLS_H_
#define _DOT_NET_COMMON_TOOLS_H_
#include "windows.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <xstring>
#include "assert.h"
#include <stdio.h>
#include <functional>
#include "DotNetDefine.h"
//#include "PropertyGridTypedef/AllPropertyGridInclude.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Diagnostics;
#ifndef CHAR_TO_WCHAR
#define	CHAR_TO_WCHAR( p,q )	 MultiByteToWideChar( CP_ACP, 0, p, -1, q, 512 );//q for return p for convert
#endif
#ifndef WCHAR_TO_CHAR
#define	WCHAR_TO_CHAR( p,q )	 WideCharToMultiByte( CP_ACP, 0, p, -1, q, 256, nullptr, nullptr );//q for return p for convert
#endif
#define XML_COMMENT "#Comment"
#define XML_TEXT	"#Text"

#ifndef DOT_NET_TEMP_SIZE
#define DOT_NET_TEMP_SIZE 4096*5
#endif

// http://stackoverflow.com/questions/13315940/apps-exe-file-missing-net-targetframework-but-only-on-clean-builds
//if here occur crash clear temp file
//C:\Users\UserName\AppData\Local\Temp
#define GetMyElementHost(p)	nullptr;\
	try\
	{\
		p = gcnew System::Windows::Forms::Integration::ElementHost();\
	}\
	catch(System::Exception^l_Exp)\
	{\
		String^l_strOriginal = "[assembly: System::Runtime::Versioning::TargetFrameworkAttribute(L"""", FrameworkDisplayName=L"".NET Framework 4"")];";\
		String^l_strNewOne = "[assembly: System::Runtime::Versioning::TargetFrameworkAttribute(L"".NETFramework,Version=v4.0"", FrameworkDisplayName=L"".NET Framework 4"")];";\
		System::Windows::Forms::MessageBox::Show("please open C:/Users/UserName/AppData/Local/Temp/.NETFramework,Version=v4.0.AssemblyAttributes.cpp\n\n\n"+l_Exp,"waring",GCFORM::MessageBoxButtons::OK);\
		System::Windows::Forms::MessageBox::Show("replace"+l_strOriginal+"\n to \n"+l_strNewOne,"waring",GCFORM::MessageBoxButtons::OK);\
		Debug::WriteLine(l_Exp);\
	}


extern bool g_bLanguageChinese;

namespace DNCT
{
	using namespace System::Xml;

	//p System.String  q char*
	#define GCSTRING_TO_WCHAR(p,q){assert(q && "nullptr Execption");						\
	WCHAR* stringPointer = (WCHAR*) System::Runtime::InteropServices::Marshal::StringToCoTaskMemUni(p).ToPointer();	\
		swprintf_s(q,DOT_NET_TEMP_SIZE,L"%ls\0",stringPointer);									\
		System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr(stringPointer));}
	//beware while the gc string contain unicode it may conver error,
	#define GCSTRING_TO_CHAR(p,q){WCHAR	l_strForLocalTemp[DOT_NET_TEMP_SIZE];GCSTRING_TO_WCHAR(p,l_strForLocalTemp);		\
								WCHAR_TO_CHAR(l_strForLocalTemp,q);}

	#define	TO_GCSTRING(p)gcnew System::String(p)

	#define	GCFORM System::Windows::Forms

	#define WARNING_MSG(p)GCFORM::MessageBox::Show(p,"waring",GCFORM::MessageBoxButtons::OK)
	#define WARING_YES_NO(p)GCFORM::MessageBox::Show(p,"waring",GCFORM::MessageBoxButtons::YesNo)
	#define WARING_YES_NO_TO_YES(p)if(WARING_YES_NO(p) == GCFORM::DialogResult::Yes)
	#define WARING_YES_NO_TO_NO(p)if(WARING_YES_NO(p) == GCFORM::DialogResult::No)

	
	#define DIALOG_OK(p) if( p->ShowDialog() == System::Windows::Forms::DialogResult::OK )
	
	#define GCNEW(p,q) p^ q = gcnew p;//class and named object

	std::string		GcStringToChar(System::String^e_str);
	std::wstring	GcStringToWchar(System::String^e_str);
	String^	WcharToGcstring(const WCHAR*e_str);
	inline String^	WcharToGcstring(std::wstring e_str) { return WcharToGcstring(e_str.c_str()); }
	String^	ForceAddExtenName(String^e_strDest,char*e_strExtensionName);
	//
	std::vector<int>	GetIntegerListByCommaDivide(String^e_str,int* e_piSize);
	std::vector<std::string >	GCArrayToSTDStringVector(cli::array<String^>^e_str);
	std::vector<std::string >	GCStringToSTDStringVector(String^e_str);
	std::vector<std::wstring >	GCArrayToSTDWStringVector(cli::array<String^>^e_str);
	std::vector<std::wstring >	GCStringToSTDWStringVector(String^e_str);
	//===============================================
	//eplace hashtable value by key
	//===============================================
	System::Void	ReplaceValuInHashTable(System::String^e_pKey,System::Object^e_pNewValue,System::Collections::Hashtable^e_pHashTable);
	//===============================================
	//
	//==============================================
	System::Windows::Forms::TextBox^	CreateDefaultTextBox(char*e_pName,int	e_iXPOs = 45,int e_iYPos = 45);
	//===============================================
	//
	//==============================================
	System::Windows::Forms::ComboBox^	CreateDefaultCombox(char*,System::Collections::ArrayList^ e_pStringForText,int	e_iXPOs = 45,int e_iYPos = 5);
	float	CreateNumericFormAndGetValue(float e_fMax = 100.f,float e_fIncreaseValue = 1.f,int e_fDecimal = 0);
	//===============================================
	// 1st listbox name
	// 2nd controlcollection to add listbox
	//===============================================
	System::Windows::Forms::ListBox^	CreateDefaultListBox(char*,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX = 90,int e_iPosY = 90);
	System::Windows::Forms::ListBox^	CreateDefaultListBox(System::String^,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX = 90,int e_iPosY = 90);
	GCFORM::ListBox^	GetActuallyListBoxFromHashTable(System::String^,System::Collections::Hashtable^,System::Windows::Forms::Control::ControlCollection^ e_p);
	//===============================================
	// for temp form to decide something to do ,ex decide listbox add or del
	//or textbox to input text to add data
	//===============================================
	System::Windows::Forms::Form^	CreateTempFromToDecideSelect( char* e_pAcceptString,char*e_pChanelString,System::Collections::ArrayList^);


	void	CreateDirectory(String^e_strDirectory);
	System::String^	SelectDirectory(const char*e_strSelectDirectory = nullptr);
	//filter string
	//"Model Files(*.dae;*.xml;*.fat)|*.dae;*.xml;*.fat|All files (*.*)|*.*"
	//"txt files (*.txt)|*.txt|All files (*.*)|*.*"
	//"image files (*.png)|*.png|All files (*.*)|*.*"
	//"xml files (*.xml)|*.xml|All files (*.*)|*.*"
	//"png files (*.png)|*.png|All files (*.*)|*.*"
	cli::array<String^>^ OpenFileAndGetNames(const char*e_pFileFilter = 0);
	System::String^	OpenFileAndGetName(const char*e_pFileFilter = 0);
	//"XML	Files(*.dae;*.xml;*.fat;prt)|*.dae;*.xml;*.fat;*.prt|All files (*.*)|*.*"
	std::string	ConvertExtensionToFilter(const char*e_strExtension,bool e_bWithAllFileFilter = false);
	std::string	ConbineFileDescribtionWithExtensionToFilter(const char*e_strDescribtion,const char*e_strExtension,bool e_bWithAllFileFilter = false);
	System::String^	SaveFileAndGetName(const char*e_pFileFilter = 0);
	System::String^	OpenFileGetText(String^e_strFileName = nullptr,String^e_strCodeName = "UTF-16");
	System::String^	GetChanglineString();


	bool						AssignDragEnterAndDropEventToTextboxWithFileName(GCFORM::TextBox^e_pTextBox,GCFORM::Control::ControlCollection^e_pControlCollection);
	cli::array<String^>^		DragDropEventWhileFileDrop(System::Windows::Forms::DragEventArgs^  e);
	GCFORM::TreeNode^			DragDropEventWhileTreeNodeDrop(System::Windows::Forms::DragEventArgs^  e,bool e_bForceToFetch = false);

	//===============================================
	//swap listbox content by two value,if the index content is not exist,return false,or index is the same
	//===============================================
	template <class T>void	SwapIndex( int e_i1, int e_i2 ,T e_pArray)
	{
	    System::Object^l_pData1 = e_pArray[e_i1];
	    System::Object^l_pData2 = e_pArray[e_i2];
	    e_pArray[e_i1] = l_pData2;
	    e_pArray[e_i2] = l_pData1;
	}
	bool	SwapListBoxValue( int e_i1, int e_i2 ,GCFORM::ListBox^e_pListBox);
	//===============================================
	//if contain the string will return true or not
	//===============================================
	bool	CheckListContainStringAndAdd(GCFORM::ListBox^,System::String^,bool e_bShowAlreadExistMessage = false);
	//===============================================
	//if listbox contain the string than will return selectindex value or will return the -1
	//===============================================
	bool	ReplaceListContainString(GCFORM::ListBox^,System::String^);
	bool	ReplaceListBoxStringByIndex(GCFORM::ListBox^,int e_iIndex,System::String^);
	//===============================================
	//
	//===============================================
	System::Void	StreamWriteFileFromString(System::String^e_pString,System::String^e_pFileName="",System::Text::Encoding^ e_pEncoding = System::Text::Encoding::GetEncoding(TO_GCSTRING("UTF-16")));
	//===============================================
	//
	//===============================================
	System::Drawing::Font^ GetFontFromFontDialog();
	//===============================================
	//
	//===============================================
	bool	IsNodesContainName(System::String^e_pString,GCFORM::TreeNodeCollection^e_pNodeCollection);
	//===============================================
	//It will check the node exist in its children if no it will add and retur true or return false
	//===============================================	
	bool	TreeNodeAddChild(System::String^e_pString,GCFORM::TreeNode^e_pTreeNode);
	//===============================================
	//TreeView Add Root
	//===============================================	
	bool	TreeViewAddRoot(System::String^e_pString,GCFORM::TreeView^e_pTreeView,bool e_bDoNotAcceptSameName = false);
	//===============================================
	//recrsive all child and slib
	//===============================================
	typedef void    (*TREE_NODE_CALL_BACK)( GCFORM::TreeNode^,void*e_pVoid);
	void	SearchAllNodes(GCFORM::TreeNode^e_pNode,void*e_pVoid,TREE_NODE_CALL_BACK);
	//===============================================
	//strip the string don't need,if e_bStripExtensionFileName = true the extension file will strip,ex: Abcd.bmp will become Abcd
	//===============================================
	//System::IO::Path::GetExtension(l_strName)
	System::String^	GetFileNameWithoutFullPath(System::String^e_pString,bool e_bStripExtensionFileName = false);
	//
	System::String^	GetDirectoryWithoutFileName(System::String^e_pString);
	//
	System::String^	ChangeFileExtensionName(System::String^e_strSource,System::String^e_pExtensionName);
	//===============================================
	//how many slash does string have
	//===============================================
	int		GetSlashCount(System::String^e_pString);

	//===============================================
	//here for combine xml and treenode
	//===============================================
	//for load  but only support element if the character out of rule will crash
	// SECTION 1. Create a DOM Document and load the XML data into it.
	//xmldoc->LoadXml(l_pString);

	//// SECTION 2. Initialize the TreeView control.
	//GCFORM::TreeView^l_pTreeView = gcnew GCFORM::TreeView();
	//TextEDitor_treeView->Nodes->Clear();
	//l_pTreeView->Nodes->Add(gcnew TreeNode(xmldoc->DocumentElement->Name));
	//TreeNode^ tNode = l_pTreeView->Nodes[0];
	//// SECTION 3. Populate the TreeView with the DOM nodes.
	//DNCT::AddNode(xmldoc->DocumentElement, tNode);
	System::Void AddNode(XmlNode^ inXmlNode, TreeNode^ inTreeNode);
	//for load all attributes as the abvoe just change AddNode to self
	void AddAttributesAndChildren(XmlNode^ inXmlNode, TreeNode^ inTreeNode);
	//for save
	//
	//XmlDocument^ d = gcnew XmlDocument();
	//XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");
	//for each (TreeNode^ t in TextEDitor_treeView->Nodes)
	//{
	// n->AppendChild(DNCT::getXmlNode(t, d));
	//}
	// d->AppendChild(n);
	// d->Save(l_pString);
	XmlNode^ getXmlNode(TreeNode^ tnode,XmlDocument^ d);
	//u can use here to do some easy save and open file 
	//just use treenode to get xml data is good enough for easy save
	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode);
	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode,String^e_strFileName);
	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView);
	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView,String^e_strFileName);
	//if input nullptr,it will automaticly open file dialog
	System::Xml::XmlDocument^OpenXmlFile(String^e_strFileName,bool e_bUnidoe = true);

	GCFORM::TreeNode^OpenXmlFileToNode(String^e_strFileName,bool e_bUnidoe = true);

	POINT	ImageGetAlphaOffsetPosLeftTop(System::Drawing::Bitmap^e_pImage);
	//beware this is the point right buttom point,not the width and height value we want to strip
	//the value should be ImageWidth-Point.x,ImageHeight-Point.y
	POINT	ImageGetAlphaOffsetPosRightDown(System::Drawing::Bitmap^e_pImage);//,POINT e_LTPoint);
	//input data ControlCollection that contain conponent that have string like "Add/�W�[",
	//and specific string e_strSplitCharacter,and any of graphic object
	//if the global variable g_bLanguageChinese is it would be split by '/'
	void	LanguageSwitch(GCFORM::Control::ControlCollection^e_pControls,String^e_strSplitCharacter,System::IntPtr e_pHwnd);
	//copy all floder file into new directory
	void	CopyDirectory(String^ e_strSourceDirectoryName,String^e_strTargetDirectoryName);
	//get all files in the directory,u have to new a e_pArrayList and input
	void	GetFilesNameByRecursivelyDirectory(String^ e_strSourceDirectoryName,System::Collections::ArrayList^e_pArrayListS,String^e_strSpecificExtensionName = nullptr);
	//we could simply to fetch all file and get new directory for all files,see cpp for use
	void	GetFilesNameByRecursivelyDirectoryAndAssignNewFolderToIt(String^ e_strSourceDirectoryName,System::Collections::ArrayList^e_pArrayList,String^e_strNewDirectoryName,System::Collections::ArrayList^e_pArrayList2);
	//get all files in the directory,and get all directories name
/*    DirectoryInfo dirInfo = new DirectoryInfo(@"\\server\folder");
    FileInfo[] wordFiles = dirInfo.GetFiles("*.doc");*/	
	void	GetDiectoriesAndFilesNameByRecursivelyDirectory(String^ e_strSourceDirectoryName,System::Collections::ArrayList^e_pFileArrayList,System::Collections::ArrayList^e_pDirecoryArrayList);
	//open file and resave it as unicode(utf-16),if e_bXML is true it will treat as xml file
	//CultureInfo.CurrentCulture.TextInfo.OEMCodePage
	//traditional chinese is "big5"
	//OR U COULD WORK JUST LIKET BELOW.....FUCK
	//#if defined WIN32 || defined LINUX
	//	        char *locale;
	//            locale = setlocale(LC_ALL, "");
	//#endif
	//if you want to specify the file encoding format just give it name
	//bool	FileToUnicode(String^e_strFileName,char*e_strCodingName = "big5");//"utf-32","utf-8"
	bool	FileToUnicode(String^e_strFileName,char*e_strCodingName = nullptr,String^e_strNewFileName = nullptr);
	//DNCT::TempFileToUnicodeParseFunction(gcnew String(e_strFileName),l_Fun1,"utf-8",false);
	bool	TempFileToUnicodeParseFunction(String^e_strFileName,std::function<bool(const char*)>e_ParseFunction,char*e_strencodingName = nullptr,bool e_bAutoDeleteFile = true);
	//configFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location
	String^	GetUseerNameAndTime();

	template<class T1,class T2> void	ListBoxSelectedItemWithObjectListMove(bool e_bUp,GCFORM::ListBox^e_pListBox,T1* e_pList,T2*e_pList2)
	{
		 int	l_iSelectIndex = e_pListBox->SelectedIndex;
		 if( l_iSelectIndex == -1 )
			return;
		 std::list<int>	l_iAllSelectedIndex;
		 bool	l_bContainTop = false;
		 bool	l_bContainDown = false;
		 for each(int l_iIndex in e_pListBox->SelectedIndices)
			l_iAllSelectedIndex.push_back(l_iIndex);
		 l_iAllSelectedIndex.sort();
		 e_pListBox->SelectedIndices->Clear();
		 if( !e_bUp )
		 {
			while( l_iAllSelectedIndex.size() )
			{
				int	l_iIndex = l_iAllSelectedIndex.back();
				l_iAllSelectedIndex.pop_back();
				 //it's bottom
				if( l_iIndex >= e_pListBox->Items->Count-1 )
				{
					WARNING_MSG("buttom one can't be down any more");
					return;
				}
				e_pList->SwapIndex(l_iIndex,l_iIndex+1);
				if( e_pList2 )
					e_pList2->SwapIndex(l_iIndex,l_iIndex+1);
				DNCT::SwapListBoxValue(l_iIndex,l_iIndex+1,e_pListBox);
				e_pListBox->SelectedIndices->Add(l_iIndex+1);
			}
		 }
		 else
		 {
			//it's top
			while( l_iAllSelectedIndex.size() )
			{
				int	l_iIndex = l_iAllSelectedIndex.front();
				l_iAllSelectedIndex.pop_front();
				 //it's top
				if( l_iIndex == 0 )
				{
					WARNING_MSG("top one can't be up any more");
					return;
				}
				e_pList->SwapIndex(l_iIndex,l_iIndex-1);
				if( e_pList2 )
					e_pList2->SwapIndex(l_iIndex,l_iIndex-1);
				DNCT::SwapListBoxValue(l_iIndex,l_iIndex-1,e_pListBox);
				e_pListBox->SelectedIndices->Add(l_iIndex-1);
			}
		 }
	}

	bool	IsTextLegalForXMLElement(GCFORM::TextBox^e_pTextBox,bool e_bShowMsg = true);
	
	bool    IsTreeNodeContainText(GCFORM::TreeNodeCollection^e_pTreeNodeCollection,String^e_strText);
	//http://www.codeproject.com/KB/GDI-plus/imageresize.aspx
	System::Drawing::Image^ ScaleByPercent(System::Drawing::Image^imgPhoto,float e_fPercentX,float e_fPercentY);
    System::Drawing::Image^ FixedSize(System::Drawing::Image^imgPhoto,int Width, int Height);
    System::Drawing::Image^ FixedSize2(System::Drawing::Image^imgPhoto,int Width, int Height);


	void	ParseXMALFileToHandl(HWND e_pHwand,String^e_strFileName);

	System::IO::Stream^ GenerateStreamFromString(String^e_str);
}
using namespace DNCT;
//http://www.dotblogs.com.tw/larrynung/archive/2009/10/13/11033.aspx
//u could oveerider the ToString function
//then the output sring would be just like what u override
//
//
//class Program  
//{  
//    static void Main(string[] args)  
//    {  
//        Customer larry = new Customer() { Name = "larry"};  
//        Console.WriteLine(larry);  
//    }  
//}  
//  
//public class Customer  
//{  
//    public string Name { get; set; }  
//    public Decimal Revenue { get; set; }  
//    public string ContactPhone { get; set; }  
//  
//    public override string ToString()  
//    {  
//        return string.Format("Customer{{Name = {0}, Revenue = {1}, ContactPhone = {2}}}",Name,Revenue.ToString (),ContactPhone);  
//    }  
//} 
//and u also could fetch data from list and set the container(listbox or combobox) content just u expected.
//public partial class XMLForm : Form  
//{  
//    List<Customer> _Customers = new List<Customer>() { new Customer{Name = "larry"}};  
//    public XMLForm()  
//    {  
//        InitializeComponent();  
//    }  
//  
//    private void XMLForm_Load(object sender, EventArgs e)  
//    {  
//        this.listBox1.DataSource = _Customers;  
//    }  
//}  


//list box with scrollbar event
//using System;
//using System.Windows.Forms;
//
//public class BetterListBox : ListBox {
//  // Event declaration
//  public delegate void BetterListBoxScrollDelegate(object Sender, BetterListBoxScrollArgs e);
//  public event BetterListBoxScrollDelegate Scroll;
//  // WM_VSCROLL message constants
//  private const int WM_VSCROLL = 0x0115;
//  private const int SB_THUMBTRACK = 5;
//  private const int SB_ENDSCROLL = 8;
//
//  protected override void WndProc(ref Message m) {
//    // Trap the WM_VSCROLL message to generate the Scroll event
//    base.WndProc(ref m);
//    if (m.Msg == WM_VSCROLL) {
//      int nfy = m.WParam.ToInt32() & 0xFFFF;
//      if (Scroll != null && (nfy == SB_THUMBTRACK || nfy == SB_ENDSCROLL))
//        Scroll(this, new BetterListBoxScrollArgs(this.TopIndex, nfy == SB_THUMBTRACK));
//    }
//  }
//  public class BetterListBoxScrollArgs {
//    // Scroll event argument
//    private int mTop;
//    private bool mTracking;
//    public BetterListBoxScrollArgs(int top, bool tracking) {
//      mTop = top;
//      mTracking = tracking;
//    }
//    public int Top {
//      get { return mTop; }
//    }
//    public bool Tracking {
//      get { return mTracking; }
//    }
//  }
//}
//
//Build your project, then drop a BetterListBox from the top of your toolbox onto your form.  Add several items, enough to get a scrollbar.  Write a handler for the new Scroll event, something like this:
//
//    private void betterListBox1_Scroll(object Sender, BetterListBox.BetterListBoxScrollArgs e) {
//      Console.WriteLine("Scroll to {0}, tracking={1}", e.Top, e.Tracking);
//    }
//
//The e.Top property tells you which item is now displayed at the top of the ListBox.  e.Tracking is true when the user is dragging the thumb.  Note that you'll get at least two Scroll events when the user is dragging, one with e.Tracking = true and another with e.Tracking = false when the user releases the thumb.
//
//Hope this helps.
//

//
//byte[] header=new byte[4];
//Encoding enc=Encoding.Default;
//using (FileStream fs = File.Open(openfiledialog.FullName, FileMode.Open,FileAccess.Read))
//{
//    if (fs.Length > 3)//�P�_�ɮת��׻ݤj��3
//    {
//  fs.Read(header, 0, 4);//Ū���}�Y���e4��byte��header��byte array
//  //�H�U�X�ؽs�X���P�_�ӷ�,�i�H�ѦҤ峹�᪺�Ѧ�.
//  if ((header[0] == 0xef && header[1] == 0xbb && header[2] == 0xbf))
//      enc = Encoding.UTF8;
//  else if ((header[0] == 0xfe && header[1] == 0xff))
//      enc = Encoding.BigEndianUnicode;
//  else if ((header[0] == 0xff && header[1] == 0xfe))
//      enc = Encoding.Unicode;//LittleEndianUnicode
//  else if ((header[0] == 0 && header[1] == 0 && header[2] == 0xfe && header[3] == 0xff) ||
//     (header[0] == 0xff && header[1] == 0xfe && header[2] == 0 && header[3] == 0))
//      enc = Encoding.UTF32;
//  else
//      enc = Encoding.Default;
//    }
//    else
//    {
//  enc = Encoding.Default;
//    }
//    fs.Close();
//}

//void TestMethod()
//{
//    string currentName = new StackTrace(true).GetFrame(0).GetMethod().Name;
//    string callName = new StackTrace(true).GetFrame(1).GetMethod().Name;
//    Trace.WriteLine("current line: " + new StackTrace(true).GetFrame(0).GetFileLineNumber().ToString());
//    Trace.WriteLine("call name: " + callName);
//    Trace.WriteLine("current name: " + currentName);
//    CallMethod();
//
//
//}
//
//Trace.WriteLine("�ثe�渹: " + new StackTrace(true).GetFrame(0).GetFileLineNumber().ToString());
//static string[] pool = "1 2 3 4 5 6 7 8 9".Split(' ');
//
//wake the application up
//private void cPIEditor_Load(object sender, EventArgs e)
//{
//    Process process = new Process();
//    process.StartInfo.FileName = Environment.GetEnvironmentVariable("WinDir") + "\\Notepad.exe";
//    process.Start();
//    process.Exited += new EventHandler(process_Exited);
//    process.SynchronizingObject = this;
//    process.EnableRaisingEvents = true;
//} 
//
//void process_Exited(object sender, EventArgs e)
//{
//2.���o�n���檺���ε{�����|
    //myProcess.StartInfo.FileName = Environment.GetEnvironmentVariable("WinDir") + "\\Notepad.exe";
    ////3.�}�l����
    //myProcess.Start();
    ////4.���ݸ��J
    //myProcess.WaitForInputIdle();
    ////5.���ݮɶ�
    //myProcess.WaitForExit(5000);
//    this.textBox1.Text = "Notepad.exe is closed!!!";
//    MessageBox.Show("Notepad.exe is closed!!!");
//}  
//1.�ޥ�Process���O����GetProcesses��k�A���o���椤���{��
//foreach (Process p in Process.GetProcesses())
//{
//    //�ޥ�ListViewItem���O
//    ListViewItem item = new ListViewItem(); 
// 
//    //���oProcess�����ݩʫ�A�[�J��ListViewItem���O
//    item.Text = p.ProcessName;
//    item.SubItems.Add(p.Id.ToString());
//    item.SubItems.Add((p.PrivateMemorySize64 / 1024) + " Kbyte");
//    item.SubItems.Add((p.VirtualMemorySize / 1024) + " Kbyte");
//    //�[�J���
//    ListView1.Items.Add(item);
//}
// 
//�p����o���ε{��ID�A�òפ�
//Process proc = Process.GetProcessById(Convert.ToInt32(ListView1.SelectedItems[0].SubItems[1].Text));
//try
//{ 
// 
//    ////�P�_���{���O�_�w����
//    if (!proc.HasExited)
//    {
//        //�������ε{��
//        proc.CloseMainWindow();
//        //��������
//        proc.WaitForExit();
//        //����귽
//        proc.Close();
//    }
//    else
//        MessageBox.Show("�w����");
//}
//catch
//{
//    MessageBox.Show("�L�k����!!");
//}
// 
//�p��I�s�~��������
////ProcessStartInfo info = new ProcessStartInfo(@"C:\windows\system32\notepad.exe");
////Process.Start(info);
////�I�s�~���{��
//Process.Start(@"C:\windows\system32\notepad.exe");
////��ø
//button1.PerformClick();
// 
//�p��C�X�]�t���Ҳ�
//if (p.Length > 0) listBox1.Items.Clear();
//{
//    try
//    {
//        //�C�X���ε{���]�t���Ҳ�
//        foreach (ProcessModule m in p[0].Modules)
//        {
//            listBox1.Items.Add("ModuleName: " + m.ModuleName + "\t" + " , " + "FileName: " + m.FileName);
//            //listBox1.Items.Add(m.FileVersionInfo);
//        }
//    }
//    catch
//    {
//    }
//}
//find file and copy
//string FormDirectory = @"C:\From\";
//string ToDirectory = @"C:\To\";
//string Filter = @"*.txt";
//string[] FileList = System.IO.Directory.GetFiles(FormDirectory, Filter);
//foreach (string File in FileList)
//{
//     System.IO.FileInfo fi = new System.IO.FileInfo(File);
//     fi.CopyTo(ToDirectory + fi.Name);

//
//using System.Data;
//using System.IO;
//using NPOI.HSSF.UserModel;
//using NPOI.SS.UserModel;
// 
//public class NPOIHelper
//{
//    public static DataTable ReadExcelAsTableNOPI(string fileName)
//    {
//        using (FileStream fs = new FileStream(fileName, FileMode.Open))
//        {
//            HSSFWorkbook wb = new HSSFWorkbook(fs);
//            Sheet sheet = wb.GetSheetAt(0);
//            DataTable table = new DataTable();
//            //�ѲĤ@�C�����D�������W��
//            Row headerRow = sheet.GetRow(0);
//            int cellCount = headerRow.LastCellNum;
//            for (int i = headerRow.FirstCellNum; i < cellCount; i++)
//                //�H����r���W�s�W���A���B�������r�ꫬ�O�H�D²��
//                table.Columns.Add(
//                    new DataColumn(headerRow.GetCell(i).StringCellValue));
//            
//            //���L�Ĺs�C(���D�C)�A�@���B�z�̫ܳ�@�C
//            for (int i = (sheet.FirstRowNum + 1); i < sheet.LastRowNum; i++)
//            {
//                Row row = sheet.GetRow(i);
//                if (row == null) continue;
//                DataRow dataRow = table.NewRow();
//                //�̥��e���o�����Ƴv�@�]�w��줺�e
//                for (int j = row.FirstCellNum; j < cellCount; j++)
//                    if (row.GetCell(j) != null)
//                        //�p�n�w�藍�P���O���ӧO�B�z�A�i����.CellType�P�_���O
//                        //�A��.StringCellValue, .DateCellValue, .NumericCellValue...����
//                        //���B�u²���ন�r��
//                        dataRow[j] = row.GetCell(j).ToString();
//                table.Rows.Add(dataRow);
//            }
//            return table;
//        }
//    }
//}

//http://www.dotblogs.com.tw/yc421206/archive/2011/06/03/26979.aspx

//Dim appWord As New Word.Application
//Dim docWord As New Word.Document
#endif