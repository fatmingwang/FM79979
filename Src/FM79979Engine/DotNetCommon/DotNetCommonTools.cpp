#include "stdAfx.h"
#include "DotNetCommonTools.h"
bool g_bLanguageChinese = false;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing;

namespace DNCT
{

	ref class cDNCTFunctionDragEnterAndDrop:public GCFORM::Control
	{
		public: System::Void DNCTFunction_textBox_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
			 {
				GCFORM::TextBox^l_pTextbox = (GCFORM::TextBox^)sender;
				l_pTextbox->Text = "";
				auto l_pData = DNCT::DragDropEventWhileFileDrop(e);
				for each(auto l_strFileName in l_pData )
				{
					l_pTextbox->Text += l_strFileName;
				}
			 }
		public: System::Void DNCTFunction_textBox_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
			 {
				if (e->Data->GetDataPresent(DataFormats::FileDrop, false) == true)
				{
					e->Effect = DragDropEffects::Copy;
				}
			 }	
	};
	char*	DNCTWcharToChar(wchar_t *e_strWchar)
	{
		static char	l_sChar[DOT_NET_TEMP_SIZE];
		assert(e_strWchar&&"the WCHAR string is nullptr");
		WCHAR_TO_CHAR(e_strWchar,l_sChar);
		return l_sChar;
	}

	char*	DNCTWcharToChar(const wchar_t *e_strWchar)
	{
		static char	l_sChar[DOT_NET_TEMP_SIZE];
		assert(e_strWchar&&"the WCHAR string is nullptr");
		WCHAR_TO_CHAR(e_strWchar,l_sChar);
		return l_sChar;
	}

	wchar_t*	DNCTCharToWchar(char *e_strChar)
	{
		static wchar_t	l_sWchar[DOT_NET_TEMP_SIZE];
		assert(e_strChar&&"the WCHAR string is nullptr");
		CHAR_TO_WCHAR(e_strChar,l_sWchar);
		return l_sWchar;
	}

	wchar_t*	DNCTCharToWchar(const char *e_strChar)
	{
		static wchar_t	l_sWchar[DOT_NET_TEMP_SIZE];
		assert(e_strChar&&"the WCHAR string is nullptr");
		CHAR_TO_WCHAR(e_strChar,l_sWchar);
		return l_sWchar;
	}
	char*	GcStringToChar(System::String^e_str)
	{
		if( !e_str )
		{
			//WARNING_MSG("input gcstring string is empty");
			return 0;
		}
		static char	l_sStr[DOT_NET_TEMP_SIZE];
		GCSTRING_TO_CHAR(e_str,l_sStr);
		return l_sStr;
	}
	WCHAR*	GcStringToWchar(System::String^e_str)
	{
		if( !e_str )
		{
			//WARNING_MSG("input gcstring string is empty");
			return 0;
		}
		static WCHAR	l_sStr[DOT_NET_TEMP_SIZE];
		GCSTRING_TO_WCHAR(e_str,l_sStr);
		return l_sStr;
	}

	String^	WcharToGcstring(const WCHAR*e_str)
	{
		if( !e_str)
		{
			WARNING_MSG("input gcstring string is empty");
			return "";
		}
		if(!wcslen(e_str))
			return "";
		//return	String(DNCTWcharToChar(e_str)).ToString();
		return	String(e_str).ToString();
	}
	String^	ForceAddExtenName(String^e_strDest,const char*e_strExtensionName)
	{
		return ForceAddExtenName(e_strDest,(char*)e_strExtensionName);
	}

	String^	ForceAddExtenName(String^e_strDest,char*e_strExtensionName)
	{
		String^l_str;
		if( !e_strDest )
			return l_str;
		if(System::IO::Path::GetExtension(e_strDest)->Equals(String(e_strExtensionName).ToString()))
			return e_strDest;
		if( !e_strDest->Contains(".") )
		{
			e_strDest += (String(".").ToString() + String(e_strExtensionName).ToString());
		}//whatever it named as extension name,replace by png
		else
		{
			for(int i=e_strDest->Length-1;i!=0;--i)
			{
				if( e_strDest[i] == L'.' )
				{
					char*l_strName = DNCT::GcStringToChar(e_strDest);
					e_strDest = gcnew String(e_strDest->Substring(0,i+1)+String(e_strExtensionName).ToString());
					break;
				}
			}
		}
		return e_strDest;
	}


	std::vector<std::string>	GCStringToSTDStringVector(String^e_str)
	{
		cli::array<String^>^l_strArray = e_str->Split(L',');
		return GCArrayToSTDStringVector(l_strArray);
	}

	std::vector<std::string>	GCArrayToSTDStringVector(cli::array<String^>^e_str)
	{
		std::vector<std::string>	l_strVector;
		cli::array<String^>^l_strArray = e_str;
		for each(String^l_str in l_strArray)
		{
			std::string	l_str1 = GcStringToChar(l_str);
			l_strVector.push_back(l_str1);
		}
		return l_strVector;
	}

	std::vector<std::wstring >	GCStringToSTDWStringVector(String^e_str)
	{
		cli::array<String^>^l_strArray = e_str->Split(L',');
		return GCArrayToSTDWStringVector(l_strArray);
	}

	std::vector<std::wstring >	GCArrayToSTDWStringVector(cli::array<String^>^e_str)
	{
		std::vector<std::wstring>	l_strVector;
		cli::array<String^>^l_strArray = e_str;
		for each(String^l_str in l_strArray)
		{
			std::wstring	l_str1 = GcStringToWchar(l_str);
			l_strVector.push_back(l_str1);
		}
		return l_strVector;	
	}

	std::vector<int>	GetIntegerListByCommaDivide(String^e_str,int* e_piSize)
	{
		std::vector<int>	l_Result;
		if( *e_piSize )
			*e_piSize = 0;
		cli::array<String^>^l_strArray = e_str->Split(L',');
		for each(String^l_str in l_strArray)
		{
			int	l_iValue = System::Int32::Parse(l_str);
			l_Result.push_back(l_iValue);
			if( e_piSize )
				++*e_piSize;
		}
		return l_Result;
	}

	System::Object ^ HashTableGetValue(System::String^ e_p,System::Collections::Hashtable^ e_pHashTable)
	{
		assert(e_p);
		System::Object ^l_pObject;
		int	l_iKey = 0;
		if(e_pHashTable->ContainsKey(e_p))
		{
			for each (System::Object^ l_p in e_pHashTable->Keys)
			{
				l_iKey++;
				if(l_p->ToString()->Equals(e_p))		
					break;
			}
			for each (System::Object^ l_p in e_pHashTable->Values)
			{
				l_iKey--;
				if(!l_iKey)
					return l_p;
			}
		}
		return l_pObject;
	}

	System::Void	ReplaceValuInHashTable(System::String^e_pKey,System::Object^e_pNewValue,System::Collections::Hashtable^e_pHashTable)
	{
		if( !e_pHashTable[e_pKey] )
			assert(0&&"no such key");
		e_pHashTable->Remove(e_pKey);
		e_pHashTable->Add(e_pKey,e_pNewValue);
	}

	System::Windows::Forms::TextBox^	CreateDefaultTextBox(char*e_pName,int	e_iXPOs,int e_iYPos)
	{
		System::Windows::Forms::TextBox^	l_pTextBox = gcnew 	System::Windows::Forms::TextBox();
		l_pTextBox->Name = gcnew System::String(e_pName);
		l_pTextBox->Size = System::Drawing::Size(160,20);
		l_pTextBox->Location = System::Drawing::Point(e_iXPOs,e_iYPos);
		return l_pTextBox;
	}

	System::Windows::Forms::ComboBox^	CreateDefaultCombox(char*e_pName,System::Collections::ArrayList^ e_pStringForText,int	e_iPosX,int e_iPosY)
	{
		System::Windows::Forms::ComboBox^	l_pComboBox = gcnew System::Windows::Forms::ComboBox;
		l_pComboBox->Location = System::Drawing::Point(e_iPosX,e_iPosY);
		l_pComboBox->Name = TO_GCSTRING(e_pName);
		l_pComboBox->Size = System::Drawing::Size(100, 40);
		if(e_pStringForText)
		for each ( System::String^	l_pString in e_pStringForText)
			l_pComboBox->Items->Add( l_pString );
		return l_pComboBox;
	}

	float	CreateNumericFormAndGetValue(float e_fMax,float e_fIncreaseValue,int e_fDecimal)
	{
		GCFORM::Form ^l_pForm = gcnew GCFORM::Form();
		GCFORM::NumericUpDown^l_pNumeric = gcnew GCFORM::NumericUpDown;
		l_pNumeric->Maximum = System::Decimal(e_fMax);
		l_pNumeric->DecimalPlaces = e_fDecimal;
		l_pNumeric->Increment = System::Decimal(e_fIncreaseValue);
		l_pForm->Controls->Add(l_pNumeric);
		l_pForm->ShowDialog();
		return (float)l_pNumeric->Value;
	}

	System::Windows::Forms::ListBox^	CreateDefaultListBox(System::String^e_pString,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX,int e_iPosY)
	{
		System::Windows::Forms::ListBox^	l_pBox = gcnew System::Windows::Forms::ListBox();
		l_pBox->HorizontalScrollbar = true;
		l_pBox->ItemHeight = 12;
		int l_iPosX = 0;
		if(e_p)
			l_iPosX = e_p->Count*30;
		l_pBox->Location = System::Drawing::Point(e_iPosX+l_iPosX,e_iPosY);
		l_pBox->Name = e_pString;
		l_pBox->Size = System::Drawing::Size(150, 100);
		l_pBox->FormattingEnabled = true;
		l_pBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;	
		if(e_p)
			e_p->Add(l_pBox);
		return l_pBox;
	}


	GCFORM::ListBox^	GetActuallyListBoxFromHashTable(System::String^e_pString,System::Collections::Hashtable^ e_pHashTable,System::Windows::Forms::Control::ControlCollection^ e_p)
	{
		GCFORM::ListBox^	l_pListBox = (GCFORM::ListBox^)DNCT::HashTableGetValue(e_pString,e_pHashTable);
		if(!l_pListBox)
		{
			l_pListBox = CreateDefaultListBox(e_pString,e_p);
			e_pHashTable->Add(e_pString,l_pListBox);
			return l_pListBox;
		}
		return l_pListBox;
	}

	System::Windows::Forms::ListBox^	CreateDefaultListBox(char*e_pString,System::Windows::Forms::Control::ControlCollection^ e_p,int e_iPosX,int e_iPosY)
	{
		return CreateDefaultListBox(gcnew System::String(e_pString),e_p,e_iPosX,e_iPosY);
	}

	System::Windows::Forms::Form^	CreateTempFromToDecideSelect( char*e_pAcceptString,char*e_pChanelString,System::Collections::ArrayList^ e_pArrayList)
	{	
	// Create a new instance of the form.
	   System::Windows::Forms::Form^ XMLForm = gcnew System::Windows::Forms::Form;
	   // Create two buttons to use as the accept and cancel buttons.
	   System::Windows::Forms::Button^ button1 = gcnew System::Windows::Forms::Button ();
	   System::Windows::Forms::Button^ button2 = gcnew System::Windows::Forms::Button ();
	  
	  // Set the text of button1 to "OK".
	   button1->Text = gcnew System::String( e_pAcceptString );
	  // Set the position of the button on the form.
	  button1->Location = System::Drawing::Point(70,80);
	  // Set the text of button2 to "Cancel".
	  button2->Text = gcnew System::String( e_pChanelString );
	  // Set the position of the button based on the location of button1.
	  button2->Location =
		 System::Drawing::Point( button1->Left, button1->Height + button1->Top + 10 );
	  // Set the caption bar text of the form.   
	  XMLForm->Text = "Dialog Box";
	  // Display a help button on the form.
	  XMLForm->HelpButton = true;
	  
	  // Define the border style of the form to a dialog box.
	  XMLForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
	  // Set the MaximizeBox to false to remove the maximize box.
	  XMLForm->MaximizeBox = false;      
	  // Set the MinimizeBox to false to remove the minimize box.
	  XMLForm->MinimizeBox = false;
	  // Set the accept button of the form to button1.
	  XMLForm->AcceptButton = button1;
	  // Set the cancel button of the form to button2.
	  XMLForm->CancelButton = button2;
	  // Set the start position of the form to the center of the screen.
	  XMLForm->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	  
	  // Add button1 to the form.
	  XMLForm->Controls->Add( button1 );
	  // Add button2 to the form.
	  XMLForm->Controls->Add( button2 );
	  // Display the form as a modal dialog box.  
	  int i=0;
	  for each ( System::Windows::Forms::Control^	l_pControl in e_pArrayList)
	  {	  
		  XMLForm->Controls->Add( (System::Windows::Forms::Control^)e_pArrayList[i] );
		  i++;
	  }
	//      XMLForm->ShowDialog();
	  return XMLForm;

	}

	System::String^	OpenFileAndGetName(char*e_pFileFilter)
	{
		System::Windows::Forms::OpenFileDialog^	openFileDialog1 = gcnew System::Windows::Forms::OpenFileDialog();
		
		openFileDialog1->Multiselect = false;
		if(e_pFileFilter)
			openFileDialog1->Filter = TO_GCSTRING(e_pFileFilter);

		if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			return openFileDialog1->FileName;
		}
		return nullptr;
	}

	void	CreateDirectory(String^e_strDirectory)
	{
		if( !System::IO::Directory::Exists(e_strDirectory) )
			System::IO::Directory::CreateDirectory(e_strDirectory);
	}

	System::String^	SelectDirectory(const char*e_strSelectDirectory )
	{
		System::Windows::Forms::FolderBrowserDialog^l_p = gcnew System::Windows::Forms::FolderBrowserDialog();
		if( e_strSelectDirectory  )
		{
			l_p->SelectedPath = gcnew String(e_strSelectDirectory);
			SendKeys::Send ("{TAB}{TAB}{RIGHT}");  // <<-- Workaround
		}
		if(l_p->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			return l_p->SelectedPath+"\\";
		}
		return nullptr;
	}

	
	cli::array<String^>^OpenFileAndGetNames(char*e_pFileFilter)
	{
		System::Windows::Forms::OpenFileDialog^	openFileDialog1 = gcnew System::Windows::Forms::OpenFileDialog();
		openFileDialog1->Multiselect = true;
		if(e_pFileFilter)
			openFileDialog1->Filter = TO_GCSTRING(e_pFileFilter);

		if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			return openFileDialog1->FileNames;
		}
		return nullptr;
	}
	char*	ConbineFileDescribtionWithExtensionToFilter(char*e_strDescribtion,const char*e_strExtension,bool e_bWithAllFileFilter)
	{
		static char	l_sFilter[MAX_PATH];
		memset(l_sFilter,0,sizeof(char)*MAX_PATH);
		strcpy_s(l_sFilter,e_strDescribtion);
		strcpy_s(l_sFilter,ConvertExtensionToFilter(e_strExtension,e_bWithAllFileFilter));
		return l_sFilter;
	}

	char*	ConvertExtensionToFilter(const char*e_strExtension,bool e_bWithAllFileFilter)
	{
		String^l_str = "(*"+String(e_strExtension).ToString()+")|*"+String(e_strExtension).ToString();
		if( e_bWithAllFileFilter )
		{
			l_str += "|All files (*.*)|*.*";
		}
		return DNCT::GcStringToChar(l_str);
	}

	System::String^	SaveFileAndGetName(char*e_pFileFilter)
	{
		System::Windows::Forms::SaveFileDialog^	SaveFileDialog1 = gcnew System::Windows::Forms::SaveFileDialog();
		if(e_pFileFilter)
			SaveFileDialog1->Filter = TO_GCSTRING(e_pFileFilter);

		if(SaveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			return SaveFileDialog1->FileName;
		}
		return nullptr;
	}
//==================================================
//
//==================================================
	System::String^	OpenFileGetText(String^e_strFileName,String^e_strCodeName)
	{
		String^l_pFileNameString = e_strFileName;
		if( l_pFileNameString == nullptr )
		{
			l_pFileNameString = DNCT::OpenFileAndGetName();
		}
		if( System::IO::File::Exists(l_pFileNameString) )
		{
			System::IO::StreamReader^l_pReader = nullptr;
			if( e_strCodeName )
				l_pReader = gcnew System::IO::StreamReader(l_pFileNameString,System::Text::Encoding::GetEncoding(e_strCodeName));
			else
				l_pReader = gcnew System::IO::StreamReader(l_pFileNameString);
			String^l_str = l_pReader->ReadToEnd();
			l_pReader->Close();
			return l_str;
		}
		return nullptr;
	}

	System::String^	GetChanglineString()
	{
		wchar_t l_temp[3] = {13,10,'\0'};
		return TO_GCSTRING(l_temp);
	}


	bool	AssignDragEnterAndDropEventToTextboxWithFileName(GCFORM::TextBox^e_pTextBox,GCFORM::Control::ControlCollection^e_pControlCollection)
	{
		String^l_str = "cDNCTFunctionDragEnterAndDrop";
		cDNCTFunctionDragEnterAndDrop^l_pDNCTFunction = nullptr;
		auto l_pControl = e_pControlCollection->Find(l_str,false);
		e_pTextBox->AllowDrop = true;
		if( !l_pControl || l_pControl->Length == 0 )
		{
			l_pDNCTFunction = gcnew cDNCTFunctionDragEnterAndDrop();
			l_pDNCTFunction->Name = l_str;
			e_pControlCollection->Add(l_pDNCTFunction);
		}
		else
		{
			l_pDNCTFunction = (cDNCTFunctionDragEnterAndDrop^)l_pControl[0];
		}
		if( !l_pDNCTFunction )
			return false;
		e_pTextBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(l_pDNCTFunction, &cDNCTFunctionDragEnterAndDrop::DNCTFunction_textBox_DragDrop);
		e_pTextBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(l_pDNCTFunction, &cDNCTFunctionDragEnterAndDrop::DNCTFunction_textBox_DragEnter);
		return true;
	}

	cli::array<String^>^ DragDropEventWhileFileDrop(System::Windows::Forms::DragEventArgs^  e	)
	{
		cli::array<String^>^ itemList;
		if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
		{
			// Perform drag-and-drop, depending upon the effect.
			if ( e->Effect == DragDropEffects::Copy )
			{
				itemList = dynamic_cast<cli::array<String^>^>(e->Data->GetData( DataFormats::FileDrop ));
				return itemList;
			}
		}
		return itemList;
	}

	GCFORM::TreeNode^	DragDropEventWhileTreeNodeDrop(System::Windows::Forms::DragEventArgs^  e,bool e_bForceToFetch)
	{
		GCFORM::TreeNode^ l_pTreeNode;
		String^l_strType = "System.Windows.Forms.TreeNode";
		if ( e->Data->GetDataPresent( l_strType ) )
		{
			// Perform drag-and-drop, depending upon the effect.
			if ( e->Effect == DragDropEffects::Copy || e_bForceToFetch )
			{
				l_pTreeNode = dynamic_cast<TreeNode^>(e->Data->GetData( l_strType ));
				return l_pTreeNode;
			}
		}
		return l_pTreeNode;	
	}

	bool	SwapListBoxValue( int e_i1, int e_i2 ,GCFORM::ListBox^e_pListBox)
	{
		if(!e_pListBox||e_pListBox->Items->Count<e_i1+1||e_pListBox->Items->Count<e_i2+1||e_i2<0||e_i1<0)
			return false;
		System::String^l_str1 = e_pListBox->Items[e_i1]->ToString();
		System::String^l_str2 = e_pListBox->Items[e_i2]->ToString();
		e_pListBox->Items[e_i1] = l_str2;
		e_pListBox->Items[e_i2] = l_str1;
		return true;
	}

	bool	CheckListContainStringAndAdd(GCFORM::ListBox^e_pListBox,System::String^e_pString,bool e_bShowAlreadExistMessage)
	{
		if(!e_pListBox->Items->Contains(e_pString))
		{
			e_pListBox->Items->Add(e_pString);
			return false;
		}
		if( e_bShowAlreadExistMessage )
			WARNING_MSG("this name already exists!!");
		return true;
	}


	bool	ReplaceListContainString(GCFORM::ListBox^e_pListBox,System::String^e_pString)
	{
		int l_i = e_pListBox->SelectedIndex;
		if( e_pListBox->Items->Contains(e_pString)||l_i==-1 )
			return false;
		e_pListBox->Items->RemoveAt(l_i);
		e_pListBox->Items->Insert(l_i,e_pString);
		return true;
	}

	bool	ReplaceListBoxStringByIndex(GCFORM::ListBox^e_pListBox,int e_iIndex,System::String^e_pString)
	{
		int l_i = e_iIndex;
		if(e_pListBox->Items->Count >l_i)
		{
			e_pListBox->Items->RemoveAt(l_i);
			e_pListBox->Items->Insert(l_i,e_pString);
			return true;	
		}
		return false;
	}

	System::Void	StreamWriteFileFromString(System::String^ e_pString,System::String^e_pFileName,System::Text::Encoding^ e_pEncoding)
	{
		System::IO::FileStream ^sb;
		if(e_pFileName!="")
			sb = gcnew System::IO::FileStream(e_pFileName, System::IO::FileMode::Create);
		else
		{
			System::String^l_pStringFileName = DNCT::SaveFileAndGetName(0);
			if( !l_pStringFileName->Length )
				return;
			sb = gcnew System::IO::FileStream(l_pStringFileName, FileMode::Create);
		}
		System::IO::StreamWriter ^l_pStreamWriter = gcnew  System::IO::StreamWriter(sb,e_pEncoding);
		l_pStreamWriter->Write(e_pString);
		l_pStreamWriter->Close();
		delete l_pStreamWriter;
	}

	System::Drawing::Font^ GetFontFromFontDialog()
	{
		GCFORM::FontDialog^l_pFontDialog = gcnew GCFORM::FontDialog();
		DIALOG_OK(l_pFontDialog)
		{
			return l_pFontDialog->Font;
		}
		return nullptr;
	}
	//===============================================
	//
	//===============================================
	bool	IsNodesContainName(System::String^e_pString,GCFORM::TreeNodeCollection^e_pNodeCollection)
	{
		for each (GCFORM::TreeNode^l_pNode in e_pNodeCollection)
		{
			if( e_pString->Equals( l_pNode->Text ) )
				return true;
		}
		return false;
	}

	bool	TreeNodeAddChild(System::String^e_pString,GCFORM::TreeNode^e_pTreeNode)
	{
		if(!e_pTreeNode)
			return false;
		if(!IsNodesContainName(e_pString,e_pTreeNode->Nodes))
		{
			GCFORM::TreeNode^l_pNode = e_pTreeNode->Nodes->Add(e_pString);
			e_pTreeNode->Expand();
			return true;
		}
		return false;
	}

	bool	TreeViewAddRoot(System::String^e_pString,GCFORM::TreeView^e_pTreeView,bool e_bDoNotAcceptSameName)
	{
		if(e_bDoNotAcceptSameName)
		{
			if(!IsNodesContainName(e_pString,e_pTreeView->Nodes))
			{
				e_pTreeView->Nodes->Add(e_pString);
				return true;
			}
			else
				return false;
		}
		e_pTreeView->Nodes->Add(e_pString);
		return true;
	}

	void	SearchAllNodes(GCFORM::TreeNode^e_pNode,void*e_pVoid,TREE_NODE_CALL_BACK e_CallBack)
	{
		e_CallBack(e_pNode,e_pVoid);
		if (e_pNode->Nodes->Count)
		{
			SearchAllNodes(e_pNode->Nodes[0],e_pVoid,e_CallBack);
		}

		if (e_pNode->NextNode)
		{
			SearchAllNodes(e_pNode->NextNode,e_pVoid,e_CallBack);
		}
	}

	System::String^	GetFileNameWithoutFullPath(System::String^e_pString,bool e_bStripExtensionFileName)
	{
		System::String^l_pString;
		int l_iForStripExtensionFileName = 0;
		for(int i=e_pString->Length-1;i!=0;--i)
		{
			if( e_bStripExtensionFileName )
			{
				if( e_pString[i] == L'.' )
					l_iForStripExtensionFileName = e_pString->Length-i;
			}

			if( e_pString[i] == L'\\' )
			{
				l_pString = e_pString->Substring(i+1,e_pString->Length-i-1-l_iForStripExtensionFileName);
				return l_pString;
			}
		}		
		return l_pString;
	}


	System::String^	ChangeFileExtensionName(System::String^e_strSource,System::String^e_pExtensionName)
	{
		String^l_Str;
		for(int i= e_strSource->Length-1;i>0;--i)
		{
			if( e_strSource[i] == L'.' )
			{
				return e_strSource->Substring(0,i+1)+e_pExtensionName;
			}
		}
		return l_Str;	
	}

	System::String^	GetDirectoryWithoutFileName(System::String^e_pString)
	{
		String^l_Str;
		for(int i= e_pString->Length-1;i>0;--i)
		{
			if( e_pString[i] == L'\\' )
			{
				return e_pString->Substring(0,i+1);
			}
		}
		return l_Str;
	}

	int		GetSlashCount(System::String^e_pString)
	{
		int l_iCount = 0;
		for( int i = 0;i<e_pString->Length;i++ )
		{
			if( e_pString[i] == L'\\' )
				l_iCount++;
		}
		return l_iCount;
	}

	System::Void AddNode(System::Xml::XmlNode^ inXmlNode, TreeNode^ inTreeNode)
	{
         XmlNode^ xNode;
         TreeNode^ tNode;
         XmlNodeList^ nodeList;
         int i;

         // Loop through the XML nodes until the leaf is reached.
         // Add the nodes to the TreeView during the looping process.
         if (inXmlNode->HasChildNodes)
         {
            nodeList = inXmlNode->ChildNodes;
            for(i = 0; i<=nodeList->Count - 1; i++)
            {
               xNode = inXmlNode->ChildNodes[i];
			   inTreeNode->Nodes->Add(gcnew TreeNode(xNode->Name));
			   GCFORM::TreeNode^l_pNode = inTreeNode->Nodes[inTreeNode->Nodes->Count-1];
			   l_pNode->Text = xNode->Name;
               tNode = inTreeNode->Nodes[i];
               AddNode(xNode, tNode);
            }
         }
         else
         {
            // Here you need to pull the data from the XmlNode based on the
            // type of node, whether attribute values are required, and so forth.
            //inTreeNode->Text = (inXmlNode->OuterXml)->Trim();
			 inTreeNode->Text = (inXmlNode->Name)->Trim();

         }
      }

	XmlNode^ getXmlNode(TreeNode^ e_pTreeNode,XmlDocument^ e_pXmlDocument)
	{
		XmlNode^ l_pXmlNode;
		if( e_pTreeNode->Text->Equals(XML_COMMENT) )
		{
			l_pXmlNode = e_pXmlDocument->CreateComment(e_pTreeNode->Name);
		}
		else
		if( e_pTreeNode->Text->Equals(XML_TEXT) )
		{
			l_pXmlNode = e_pXmlDocument->CreateTextNode(e_pTreeNode->Name);
		}
		else
		{
			//it have name and text so it's attributes
			if( e_pTreeNode->Name->Length>=1 )
			{
				System::Xml::XmlAttribute^l_pAttributes = e_pXmlDocument->CreateAttribute(e_pTreeNode->Text);
				l_pAttributes->Value = e_pTreeNode->Name;
				//l_pAttributes->Name = e_pTreeNode->Text;
				l_pXmlNode = l_pAttributes;
			}
			else
			{
				l_pXmlNode = e_pXmlDocument->CreateNode(XmlNodeType::Element, e_pTreeNode->Text, "");
			}
		}
		if( e_pTreeNode->Nodes->Count )
		{
			System::Xml::XmlComment^l_pComment;
			System::Xml::XmlText^l_pText;
			System::Xml::XmlNode^l_pPerviousNode;
			for each (TreeNode^ t in e_pTreeNode->Nodes)
			{
				if( t->Nodes->Count )
				{
					l_pPerviousNode =  getXmlNode(t,e_pXmlDocument);
					l_pXmlNode->AppendChild(l_pPerviousNode);
				}
				else
				{
					if( t->Text->Equals(XML_COMMENT) )
					{
						l_pComment = e_pXmlDocument->CreateComment(t->Name);
						l_pXmlNode->InsertAfter(l_pComment,l_pPerviousNode);
					}
					if( t->Text->Equals(XML_TEXT) )
					{
						l_pText = e_pXmlDocument->CreateTextNode(t->Name);
						l_pXmlNode->InsertAfter(l_pText,l_pPerviousNode);
					}
					else
					{
						if( t->Name->Length > 0 )//make sure this is attribute
						{
							System::Xml::XmlAttribute^l_pAtt = e_pXmlDocument->CreateAttribute(t->Text);
							l_pAtt->Value = t->Name;
							l_pXmlNode->Attributes->Append(l_pAtt);
						}
						else
						{//it poss ible error if the user set wrong order(attribute after element)
							l_pPerviousNode =  getXmlNode(t,e_pXmlDocument);
							l_pXmlNode->AppendChild(l_pPerviousNode);
						}
					}
				}
			}
		}
		return l_pXmlNode;
	}

	void AddAttributesAndChildren(XmlNode^ inXmlNode, TreeNode^ inTreeNode)
	{
		XmlAttributeCollection^ atts = inXmlNode->Attributes;

        if (atts && atts->Count != 0)
		{
            //TreeNode^ tNode = gcnew TreeNode("(Attributes)");
            for each (XmlAttribute ^att in atts)
			{
				TreeNode^l_pTreeNode = gcnew TreeNode();
				l_pTreeNode->Name = att->Value;
				l_pTreeNode->Text = att->Name;
				inTreeNode->Nodes->Add(l_pTreeNode);
            }
        }

        for each (XmlNode^ xNode in inXmlNode->ChildNodes)
		{
            TreeNode^ tNode;
            switch (xNode->NodeType)
			{
			case XmlNodeType::Element :
                tNode = gcnew TreeNode(xNode->Name);
                AddAttributesAndChildren(xNode, tNode);
                break;
			case XmlNodeType::Text:
                tNode = gcnew TreeNode();
				tNode->ForeColor = System::Drawing::Color::RoyalBlue;
				tNode->Name = xNode->Value;
				tNode->Text = XML_TEXT;
                break;
            case XmlNodeType::Comment:
                tNode = gcnew TreeNode();
				tNode->ForeColor = System::Drawing::Color::Green;
				tNode->Name = xNode->Value;
				tNode->Text = XML_COMMENT;
                break;
            case XmlNodeType::ProcessingInstruction:
                tNode = gcnew TreeNode(xNode->Value);
                break;
            case XmlNodeType::CDATA:
                tNode = gcnew TreeNode(xNode->Value);
                break;				
            default :
				assert(0);
//                throw gcnew Exception("Unexpected NodeType: " + xNode->NodeType->ToString());
            }
            inTreeNode->Nodes->Add(tNode);
        }
    }

	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView)
	{
		System::String^l_pString = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		SaveTreeViewToFile(e_pTreeView,l_pString);		
	}

	System::Void	SaveTreeViewToFile(GCFORM::TreeView^e_pTreeView,String^e_strFileName)
	{
		System::String^l_pString = e_strFileName;
		if( l_pString )
		{
			XmlDocument^ d = gcnew XmlDocument();
			XmlNode^ n;
			if( e_pTreeView->Nodes->Count == 1 )
			{
			    n = d->CreateNode(XmlNodeType::Element,e_pTreeView->Nodes[0]->Text,"");
			    for each (TreeNode^ t in e_pTreeView->Nodes[0]->Nodes)
			    {
			        System::Xml::XmlNode^l_pNode = DNCT::getXmlNode(t, d);
					if( l_pNode )
					{
						String^l_strType = l_pNode->ToString();
						if(  l_strType->CompareTo("System.Xml.XmlAttribute") == 0 )
						{
							n->Attributes->Append((System::Xml::XmlAttribute^)l_pNode);
						}
						else
							n->AppendChild(l_pNode);
					}
			    }			    
			}
			else
			{
			    n = d->CreateNode(XmlNodeType::Element,"Root","");
			    for each (TreeNode^ t in e_pTreeView->Nodes)
			    {
			        System::Xml::XmlNode^l_pNode = DNCT::getXmlNode(t, d);
				    n->AppendChild(l_pNode);
			    }
			}
			d->AppendChild(n);
			TextWriter^ sw = gcnew StreamWriter(l_pString, false, System::Text::Encoding::Unicode);
			{
				d->Save(sw);
			}
			sw->Close();
			////d->Save(l_pString);
		}	
	}

	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode)
	{
		System::String^l_pString = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		SaveTreeNodeToFile(e_pTreeNode,l_pString);		
	}

	System::Void	SaveTreeNodeToFile(GCFORM::TreeNode^e_pTreeNode,String^e_strFileName)
	{
		if( e_strFileName->Length )
		{
			XmlDocument^ d = gcnew XmlDocument();
			XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root","");
			for each (TreeNode^ t in e_pTreeNode->Nodes)
			{
				n->AppendChild(DNCT::getXmlNode(t, d));
			}
			d->AppendChild(n);
			TextWriter^ sw = gcnew StreamWriter(e_strFileName, false, System::Text::Encoding::Unicode);
			{
				d->Save(sw);
			}
			sw->Close();
			//d->Save(e_strFileName);
		}	
	}

	System::Xml::XmlDocument^OpenXmlFile(String^e_strFileName,bool e_bUnidoe)
	{
		System::String^l_pStringForFileName;
		if( !e_strFileName )
			l_pStringForFileName = DNCT::OpenFileAndGetName("xml files (*.path)|*.path|xml files (*.xml)|*.xml|All files (*.*)|*.*");
		else
			l_pStringForFileName = e_strFileName;
		if( l_pStringForFileName )
		{
			if(!System::IO::File::Exists(l_pStringForFileName))
				return nullptr;
			System::Xml::XmlDocument^ xmldoc = gcnew System::Xml::XmlDocument();
			System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(l_pStringForFileName);
			System::String^l_pString = sr->ReadToEnd();
			try 
			{
				xmldoc->LoadXml(l_pString);
			}
			catch(XmlException^ xmlEx)
			{
				xmlEx->Message;
				sr->Close();
				delete sr;
				try
				{
					sr = gcnew  System::IO::StreamReader(l_pStringForFileName,System::Text::Encoding::Unicode);
					l_pString = sr->ReadToEnd();
					xmldoc->LoadXml(l_pString);
				}
				catch(Exception^ ex)
				{
					sr->Close();
					delete sr;
					WARNING_MSG(ex->Message);
					return nullptr;
				}
				if( !xmldoc->DocumentElement )
					WARNING_MSG(xmlEx->Message);
			}
			sr->Close();
			delete sr;
			return xmldoc;
		}
		return nullptr;
	}

	GCFORM::TreeNode^	OpenXmlFileToNode(String^e_strFileName,bool e_bUnidoe)
	{
		TreeNode^ tNode;
		System::String^l_pStringForFileName;
		if( !e_strFileName )
			l_pStringForFileName = DNCT::OpenFileAndGetName("xml files (*.path)|*.path|xml files (*.xml)|*.xml|All files (*.*)|*.*");
		else
			l_pStringForFileName = e_strFileName;
		if( l_pStringForFileName )
		{
			
			System::Xml::XmlDocument^ xmldoc = OpenXmlFile(e_strFileName,e_bUnidoe);
			if( xmldoc && xmldoc->DocumentElement )
			{
				tNode = gcnew GCFORM::TreeNode(xmldoc->DocumentElement->Name);
				String^l_strName = xmldoc->DocumentElement->Name;
				if( xmldoc->DocumentElement->Attributes->Count == 0 )//skip root start from its children
				{
					for each( System::Xml::XmlNode^l_pXmlNode in xmldoc->DocumentElement->ChildNodes)
					{
						GCFORM::TreeNode^l_pNode = gcnew GCFORM::TreeNode(l_pXmlNode->Name);
						DNCT::AddAttributesAndChildren(l_pXmlNode, l_pNode);
						tNode->Nodes->Add(l_pNode);
					}
				}
				else
					DNCT::AddAttributesAndChildren(xmldoc->DocumentElement, tNode);
				delete xmldoc;
			}
		}				
		return tNode;
	}

	POINT	ImageGetAlphaOffsetPosLeftTop(System::Drawing::Bitmap^e_pImage)
	{
		int	l_iWidth = e_pImage->Width;
		int	l_iHeight = e_pImage->Height;
		POINT	l_OffsetPos = { l_iWidth,l_iHeight };
		//scan X
		for( int i=0;i<l_iWidth;++i )
		{
			for( int j=0;j<l_iHeight;++j )
			{
				if( e_pImage->GetPixel(i,j).A != 0 )//none alpah
				{
					if( l_OffsetPos.x>i )
						l_OffsetPos.x = i;
					if( l_OffsetPos.y>j )
						l_OffsetPos.y = j;
				}
			}		
		}
		return l_OffsetPos;
	}
	POINT	ImageGetAlphaOffsetPosRightDown(System::Drawing::Bitmap^e_pImage)
	{
		int	l_iWidth = e_pImage->Width;
		int	l_iHeight = e_pImage->Height;
		POINT	l_OffsetPos = { 0,0 };
		//scan X
		for( int i=l_iWidth-1;i>-1;--i )
		{
			for( int j=l_iHeight-1;j>-1;--j )
			{
				if( e_pImage->GetPixel(i,j).A != 0 )//none alpah
				{
					if( l_OffsetPos.x<i )
						l_OffsetPos.x = i;
					if( l_OffsetPos.y<j )
						l_OffsetPos.y = j;
				}
			}		
		}
		return l_OffsetPos;	
	}
	String^SplitTextBySpecificCharacter(String^e_str,bool e_bGetFrontString,String^e_strCharacter)
	{
		if( e_str->Contains(e_strCharacter) )
		{
			int	l_iIndex = e_str->IndexOf(e_strCharacter)+1;
			String^l_strText;
			if( e_bGetFrontString )
			{
				l_strText = e_str->Substring(l_iIndex);
			}
			else
			{
				l_strText = e_str->Substring(0,l_iIndex-1);
			}
			return l_strText;
		}
		return e_str;
	}
	void	LanguageSwitch(GCFORM::Control::ControlCollection^e_pControls,String^e_strSplitCharacter,System::IntPtr e_pHwnd)
	{
		if( e_pControls->Owner )
			e_pControls->Owner->Text = SplitTextBySpecificCharacter(e_pControls->Owner->Text,g_bLanguageChinese,"/");
		System::Drawing::Graphics^l_pGraphics = System::Drawing::Graphics::FromHwnd(e_pHwnd);
		for(int i=0;i<e_pControls->Count;++i)
		{
			String^l_strType = e_pControls[i]->GetType()->ToString();
			if(l_strType->Equals("System.Windows.Forms.Label")||
				l_strType->Equals("System.Windows.Forms.Button")||
				l_strType->Equals("System.Windows.Forms.CheckBox"))
			{
				String^l_str = e_pControls[i]->Text;
				System::Drawing::SizeF l_iOriginalSize =l_pGraphics->MeasureString(l_str,e_pControls[i]->Font);
				if( l_str->Contains(e_strSplitCharacter) )
				{
					e_pControls[i]->Text = SplitTextBySpecificCharacter(e_pControls[i]->Text,g_bLanguageChinese,"/");
					System::Drawing::SizeF	l_AfterSizef = l_pGraphics->MeasureString(e_pControls[i]->Text,e_pControls[i]->Font);
					if( l_AfterSizef.Height != l_iOriginalSize.Height )
					{
						e_pControls[i]->Height = (int)(l_AfterSizef.Height+e_pControls[i]->Margin.Top+e_pControls[i]->Margin.Bottom);
					}
					if( l_strType->Equals("System.Windows.Forms.Label") )
					{
						//System::Drawing::Point	l_Point = e_pControls[i]->Location;
						//l_Point.X += (int)(l_iOriginalSize.Width-l_AfterSizef.Width);
						//e_pControls[i]->Location  = l_Point;
					}
				}
			}
		}	
	}
	void	CopyDirectory(String^ e_strSourceDirectoryName,String^e_strTargetDirectoryName)
	{
		try
		{
			// Create directory if needed
			System::IO::Directory::CreateDirectory(e_strTargetDirectoryName);
			// Treate all the file under current source directory
			cli::array<String^>^ l_strFiles = System::IO::Directory::GetFiles(e_strSourceDirectoryName);
			for each(String^ l_strFileName in l_strFiles)
			{
				System::IO::File::Copy(l_strFileName, e_strTargetDirectoryName +
					System::IO::Path::DirectorySeparatorChar +
					System::IO::Path::GetFileName(l_strFileName),true);
			}
			// now treat all subdirectory and their related files (called this routine recursively)
			cli::array<String^>^ l_strDirectories = System::IO::Directory::GetDirectories(e_strSourceDirectoryName);
			for each(String^ l_strDirectory in l_strDirectories)
			{
				CopyDirectory(l_strDirectory, e_strTargetDirectoryName +
				System::IO::Path::DirectorySeparatorChar +
				System::IO::Path::GetFileName(l_strDirectory));
			}
		}
		//catch (BTException ex0)
		//{
		//	// This is an exception thrown while recursively be called. Just throw	further
		//	throw ex0;
		//}
		catch (Exception^ ex)
		{
			String^l_str = "Unable to Copy Directory " + e_strSourceDirectoryName
			+
			" to " + e_strTargetDirectoryName;
			WARNING_MSG(l_str+ex->ToString());
		}
	}

	void	GetFilesNameByRecursivelyDirectory(String^ e_strSourceDirectoryName,System::Collections::ArrayList^e_pArrayList,String^e_strSpecificExtensionName)
	{
		cli::array<String^>^ l_strFiles = System::IO::Directory::GetFiles(e_strSourceDirectoryName);
		for each(String^ l_strFileName in l_strFiles)
		{
			if( e_strSpecificExtensionName )
			{
				if( System::String::Compare(e_strSpecificExtensionName,System::IO::Path::GetExtension(l_strFileName)) != 0 )
					continue;
			}
			e_pArrayList->Add(l_strFileName);
			// now treat all subdirectory and their related files (called this routine recursively)
		}
		cli::array<String^>^ l_strDirectories = System::IO::Directory::GetDirectories(e_strSourceDirectoryName);
		for each(String^ l_strDirectory in l_strDirectories)
			GetFilesNameByRecursivelyDirectory(l_strDirectory,e_pArrayList,e_strSpecificExtensionName);
	}
	//sample to use
	//String^l_strDirectoryName = DNCT::SelectDirectory();
	//if( l_strDirectoryName )
	//{
	//	System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList;
	//	System::Collections::ArrayList^l_pArrayList2 = gcnew System::Collections::ArrayList;
	//	GetFilesNameByRecursivelyDirectoryAndAssignNewFolderToIt(l_strDirectoryName,l_pArrayList,"Compress",l_pArrayList2);
	//	for(int i=0;i<l_pArrayList->Count;++i )
	//	{
	//		String^l_strOriginalName = (String^)l_pArrayList[i];
	//		String^l_strNewName = (String^)l_pArrayList2[i];
	//		DNCT::CreateDirectory(System::IO::Path::GetDirectoryName(l_strNewName));
	//		std::string	l_strFileName = DNCT::GcStringToChar(l_strOriginalName);
	//		std::string	l_strOutputFileName = DNCT::GcStringToChar(l_strNewName);
	//		FileCompressHuffman(l_strFileName.c_str(),l_strOutputFileName.c_str());
	//	}
	//}
	void	GetFilesNameByRecursivelyDirectoryAndAssignNewFolderToIt(String^ e_strSourceDirectoryName,System::Collections::ArrayList^e_pArrayList,String^e_strNewDirectoryName,System::Collections::ArrayList^e_pArrayList2)
	{
		String^l_strRoot = System::IO::Directory::GetParent(e_strSourceDirectoryName)->Parent->FullName;
		GetFilesNameByRecursivelyDirectory(e_strSourceDirectoryName,e_pArrayList);
		for each (String^l_strName in e_pArrayList )
		{
			String^l_pTargetDirectory = l_strRoot+"\\"+e_strNewDirectoryName+"\\";
			String^l_strCurrentFileDirectory = System::IO::Path::GetDirectoryName(l_strName);
			String^l_strSubString = "";
			if( l_strCurrentFileDirectory->Length+1 != e_strSourceDirectoryName->Length )
			{
				l_strSubString = l_strCurrentFileDirectory->Substring(e_strSourceDirectoryName->Length,l_strCurrentFileDirectory->Length-e_strSourceDirectoryName->Length);
				l_pTargetDirectory += l_strSubString;
				l_pTargetDirectory += "\\";
			}
			//DNCT::CreateDirectory(l_pTargetDirectory);
			String^l_strNewFileName = l_pTargetDirectory+System::IO::Path::GetFileName(l_strName);
			e_pArrayList2->Add(l_strNewFileName);
			//std::string	l_strFileNmae = DNCT::GcStringToChar(l_strName);
			//String^l_strOutputFileName = l_strDirectory+"\\"+DNCT::GetFileNameWithoutFullPath(l_strName);
			//std::string	l_strOutputFullFileName = DNCT::GcStringToChar(l_strOutputFileName);
			//FileCompressHuffman(l_strFileNmae.c_str(),l_strOutputFullFileName.c_str(),m_iBinaryHeaderID);
		}	
	}

	void	GetDiectoriesAndFilesNameByRecursivelyDirectory(String^ e_strSourceDirectoryName,System::Collections::ArrayList^e_pFileArrayList,System::Collections::ArrayList^e_pDirecoryArrayList)
	{
		cli::array<String^>^ l_strFiles = System::IO::Directory::GetFiles(e_strSourceDirectoryName);
		for each(String^ l_strFileName in l_strFiles)
		{
			e_pFileArrayList->Add(l_strFileName);
			// now treat all subdirectory and their related files (called this routine recursively)
		}
		e_pDirecoryArrayList->Add(e_strSourceDirectoryName);
		cli::array<String^>^ l_strDirectories = System::IO::Directory::GetDirectories(e_strSourceDirectoryName);
		for each(String^ l_strDirectory in l_strDirectories)
		{
			GetDiectoriesAndFilesNameByRecursivelyDirectory(l_strDirectory,e_pFileArrayList,e_pDirecoryArrayList);
		}
	}



	bool	FileToUnicode(String^e_strFileName,char*e_strCodingName,String^e_strNewFileName)
	{
		if( !System::IO::File::Exists(e_strFileName) )
			return false;
		System::Text::Encoding^l_pEncoding;
		if( e_strCodingName == nullptr )
		{
			System::IO::FileStream^fs = System::IO::File::Open(e_strFileName,System::IO::FileMode::Open,System::IO::FileAccess::Read);
			{
				if (fs->Length > 3)//判斷檔案長度需大於3
				{
					cli::array<Byte>^header = gcnew cli::array<Byte>(1024);
					fs->Read(header, 0, 4);
					//以下幾種編碼的判斷來源,可以參考文章後的參考.
					  if ((header[0] == 0xef && header[1] == 0xbb && header[2] == 0xbf))
						  l_pEncoding = System::Text::Encoding::UTF8;
					  else if ((header[0] == 0xfe && header[1] == 0xff))
						  l_pEncoding = System::Text::Encoding::BigEndianUnicode;
					  else if ((header[0] == 0xff && header[1] == 0xfe))
					  {
						  l_pEncoding = System::Text::Encoding::Unicode;//LittleEndianUnicode
					  }
					  else if ((header[0] == 0 && header[1] == 0 && header[2] == 0xfe && header[3] == 0xff) ||
						 (header[0] == 0xff && header[1] == 0xfe && header[2] == 0 && header[3] == 0))
					  {
						  //this is target file don't need to do anything!.
						  l_pEncoding = System::Text::Encoding::UTF32;
					  }
					  else
						  l_pEncoding = System::Text::Encoding::Default;
				}
				else
				{
					l_pEncoding = System::Text::Encoding::Default;
				}
				fs->Close();
				if(l_pEncoding == System::Text::Encoding::UTF32 || l_pEncoding == System::Text::Encoding::Unicode)
				{
					if( e_strNewFileName == nullptr )
						return true;
				}
			}
		}
		else
		{
			//System::Globalization::CultureInfo::CurrentCulture^l_pCulture = System::Globalization::CultureInfo::GetCultures(System::Globalization::CultureTypes::WindowsOnlyCultures)
			l_pEncoding = System::Text::Encoding::GetEncoding(String(e_strCodingName).ToString());
		}
		String^l_strFileName = e_strFileName;
		if( e_strNewFileName != nullptr )
			l_strFileName = e_strNewFileName;
		System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(e_strFileName,l_pEncoding);
		//System::IO::StreamReader ^sr = gcnew  System::IO::StreamReader(e_strFileName,System::Text::Encoding::ASCII);
		System::String^l_pString = sr->ReadToEnd();
		sr->Close();
		System::IO::StreamWriter^l_pWriter = gcnew System::IO::StreamWriter(l_strFileName,false,System::Text::Encoding::UTF32);
		try 
		{
			l_pWriter->Write(l_pString);
		}
		catch(System::Exception^l_pExp)
		{
			WARNING_MSG(l_pExp->ToString());
			OutputDebugString(L"\n");
			OutputDebugString(DNCT::GcStringToWchar(l_pExp->ToString()));
			OutputDebugString(L"\n");
		}
		l_pWriter->Close();
		return true;
	}

	bool	TempFileToUnicodeParseFunction(String^e_strFileName,std::function<bool(const char*)>e_ParseFunction,char*e_strencodingName,bool e_bAutoDeleteFile)
	{
		String^l_strTempFileName = e_strFileName+".tmp";
		bool l_bResult = false;
		if(FileToUnicode(e_strFileName,e_strencodingName,l_strTempFileName))
		{
			if( e_ParseFunction )
			{
				std::string l_strFileName = DNCT::GcStringToChar(l_strTempFileName);
				l_bResult = e_ParseFunction(l_strFileName.c_str());
				if( e_bAutoDeleteFile )
					System::IO::File::Delete(l_strTempFileName);
			}
		}
		return l_bResult;
	}

	String^	GetUseerNameAndTime()
	{
		return "Author:"+Environment::UserName+
			",Time:"+//System::DateTime::Now.Year.ToString()+
			//System::DateTime::Now.Month.ToString()+
			//System::DateTime::Now.ToString()+
			//System::DateTime::Now.ToString()+
			System::DateTime::Now.ToString();
	}

	bool	IsTextLegalForXMLElement(GCFORM::TextBox^e_pTextBox,bool e_bShowMsg)
	{
		if( e_pTextBox->Text->Length == 0 )
		 {
			 if( e_bShowMsg )
				System::Windows::Forms::MessageBox::Show("empty data","waring",System::Windows::Forms::MessageBoxButtons::OK);
			 return false;
		 }
		 if( e_pTextBox->Text->Contains(" !@#$%%^&*()\\//l;'`~") )
		 {
			if( e_bShowMsg )
				System::Windows::Forms::MessageBox::Show("symbols and blank is not prohibted","waring",System::Windows::Forms::MessageBoxButtons::OK);
			return false;		 
		 }
		 if(e_pTextBox->Text[0]>=L'0')
		 if(e_pTextBox->Text[0]<=(L'0'+9))
		 {
			if( e_bShowMsg )
				System::Windows::Forms::MessageBox::Show("first character can not be numerical","waring",System::Windows::Forms::MessageBoxButtons::OK);
			return false;
		 }
		 return true;
	}
	
	bool    IsTreeNodeContainText(GCFORM::TreeNodeCollection^e_pTreeNodeCollection,String^e_strText)
	{
	    for each(GCFORM::TreeNode^l_pNode in e_pTreeNodeCollection)
	    {
	        if(l_pNode->Text->Equals(e_strText))
	        return true;
	    }
	    return false;
	}
	
	
	System::Drawing::Image^ FixedSize2(System::Drawing::Image^imgPhoto,int e_iWidth, int e_iHeight)
	{
        int sourceWidth = imgPhoto->Width;
        int sourceHeight = imgPhoto->Height;
        int sourceX = 0;
        int sourceY = 0;

        int destX = 0;
        int destY = 0; 
        int destWidth  = e_iWidth;
        int destHeight = e_iHeight;

        System::Drawing::Bitmap^bmPhoto = gcnew System::Drawing::Bitmap(destWidth, destHeight, 
                                 PixelFormat::Format24bppRgb);
        bmPhoto->SetResolution(imgPhoto->HorizontalResolution, 
                                imgPhoto->VerticalResolution);

        System::Drawing::Graphics^grPhoto = System::Drawing::Graphics::FromImage(bmPhoto);
        grPhoto->InterpolationMode = InterpolationMode::HighQualityBicubic;

        grPhoto->DrawImage(imgPhoto, 
            System::Drawing::Rectangle(destX,destY,destWidth,destHeight),
            System::Drawing::Rectangle(sourceX,sourceY,sourceWidth,sourceHeight),
            GraphicsUnit::Pixel);
        return bmPhoto;	
	}
    System::Drawing::Image^ ScaleByPercent(System::Drawing::Image^imgPhoto,float e_fPercentX,float e_fPercentY)
    {
        int sourceWidth = imgPhoto->Width;
        int sourceHeight = imgPhoto->Height;
        int sourceX = 0;
        int sourceY = 0;

        int destX = 0;
        int destY = 0; 
        int destWidth  = (int)(sourceWidth * e_fPercentX);
        int destHeight = (int)(sourceHeight * e_fPercentY);

        System::Drawing::Bitmap^bmPhoto = gcnew System::Drawing::Bitmap(destWidth, destHeight, 
                                 PixelFormat::Format24bppRgb);
        bmPhoto->SetResolution(imgPhoto->HorizontalResolution, 
                                imgPhoto->VerticalResolution);

        System::Drawing::Graphics^grPhoto = System::Drawing::Graphics::FromImage(bmPhoto);
        grPhoto->InterpolationMode = InterpolationMode::HighQualityBicubic;

        grPhoto->DrawImage(imgPhoto, 
            System::Drawing::Rectangle(destX,destY,destWidth,destHeight),
            System::Drawing::Rectangle(sourceX,sourceY,sourceWidth,sourceHeight),
            GraphicsUnit::Pixel);
        return bmPhoto;
    }	
	
System::Drawing::Image^ FixedSize(System::Drawing::Image^imgPhoto, int Width, int Height)
    {
        int sourceWidth = imgPhoto->Width;
        int sourceHeight = imgPhoto->Height;
        int sourceX = 0;
        int sourceY = 0;
        int destX = 0;
        int destY = 0; 

        float nPercent = 0;
        float nPercentW = 0;
        float nPercentH = 0;

        nPercentW = ((float)Width/(float)sourceWidth);
        nPercentH = ((float)Height/(float)sourceHeight);
        if(nPercentH < nPercentW)
        {
            nPercent = nPercentH;
            destX = System::Convert::ToInt16((Width - 
                          (sourceWidth * nPercent))/2);
        }
        else
        {
            nPercent = nPercentW;
            destY = System::Convert::ToInt16((Height - 
                          (sourceHeight * nPercent))/2);
        }

        int destWidth  = (int)(sourceWidth * nPercent);
        int destHeight = (int)(sourceHeight * nPercent);

        System::Drawing::Bitmap^ bmPhoto = gcnew System::Drawing::Bitmap(Width, Height, 
                          System::Drawing::Imaging::PixelFormat::Format24bppRgb);
        bmPhoto->SetResolution(imgPhoto->HorizontalResolution, 
                         imgPhoto->VerticalResolution);

        System::Drawing::Graphics^ grPhoto = System::Drawing::Graphics::FromImage(bmPhoto);
        grPhoto->Clear(System::Drawing::Color::Red);
        grPhoto->InterpolationMode = 
                System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;

        grPhoto->DrawImage(imgPhoto, 
            System::Drawing::Rectangle(destX,destY,destWidth,destHeight),
            System::Drawing::Rectangle(sourceX,sourceY,sourceWidth,sourceHeight),
            System::Drawing::GraphicsUnit::Pixel);
        return bmPhoto;
    }	

bool	ParseXMALFileToHandl(GCFORM::Control^e_pControl,String^e_strFileName)
	{
		FileStream^l_pStream = File::OpenRead(e_strFileName);
		if( !l_pStream )
			return false;
		//System::Windows::Controls::Page^l_pPage = (System::Windows::Controls::Page^)System::Windows::Markup::XamlReader::Load(l_pStream);
		//l_pStream->Close();
		//System::Drawing::Size	l_Size = e_pControl->Size;
		//System::Windows::Interop::HwndSourceParameters^l_pSourceParams = gcnew System::Windows::Interop::HwndSourceParameters(e_pControl->Name);
		//l_pSourceParams->PositionX = l_pSourceParams->PositionY = 0;
		//l_pSourceParams->Height = l_Size.Height;
		//l_pSourceParams->Width = l_Size.Width;
		//l_pSourceParams->ParentWindow = e_pControl->Handle;
		//l_pSourceParams->WindowStyle = WS_VISIVLES|WS_CHILD|WS_CLIPCHILDREN;
		//System::Windows::Interop::HwndSource^l_pSource = gcnew System::Windows::Interop::HwndSource(l_pSourceParams);
		//if( ！l_pSource )
		//	return false
		//l_pSource->RootVisual = l_pPage;
		return true;
	}
	//http://stackoverflow.com/questions/1879395/how-to-generate-a-stream-from-a-string
	System::IO::Stream^ GenerateStreamFromString(String^e_str)
	{
		System::IO::MemoryStream^stream = gcnew System::IO::MemoryStream();
		System::IO::StreamWriter^writer = gcnew System::IO::StreamWriter(stream);
		writer->Write(e_str);
		writer->Flush();
		stream->Position = 0;
		return stream;
	}
//end DNCT
}