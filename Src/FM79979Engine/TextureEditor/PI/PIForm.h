#pragma once
#include "pack.h"
#include "BinPack.h"
#include "PuzzleImageUnitTriangulator.h"
#include "PIUtility.h"
#include "../../Core/GameplayUT/OpenGL/WindowsOpenGL.h"
#include "AtlasConvert.h"
cGlyphFontRender*g_pDebugFont = nullptr;
#define	LAST_USE_PI_FILE_NAME	"_lastpifile.txt"
namespace PI 
{

	//HWND GetConsoleHwnd(void)
 //  {
 //      #define MY_BUFSIZE 1024 // Buffer size for console window titles.
 //      HWND hwndFound;         // This is what is returned to the caller.
 //      WCHAR pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
 //                                          // WindowTitle.
 //      WCHAR pszOldWindowTitle[MY_BUFSIZE]; // Contains original
 //                                          // WindowTitle.

 //      // Fetch current window title.

 //      GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

 //      // Format a "unique" NewWindowTitle.

 //      wsprintf(pszNewWindowTitle,L"%d/%d",
 //                  GetTickCount(),
 //                  GetCurrentProcessId());

 //      // Change current window title.

 //      //SetConsoleTitle(pszNewWindowTitle);

 //      // Ensure window title has been updated.

 //      Sleep(40);

 //      // Look for NewWindowTitle.

 //      hwndFound=FindWindow(nullptr, L"");

 //      // Restore original window title.

 //      //SetConsoleTitle(pszOldWindowTitle);

 //      return(hwndFound);
 //  }

	//HWND FindMyTopMostWindow()
	//{
	//	DWORD dwProcID = GetCurrentProcessId();
	//	HWND hWnd = GetTopWindow(GetDesktopWindow());
	//	while(hWnd)
	//	{
	//		DWORD dwWndProcID = 0;
	//		GetWindowThreadProcessId(hWnd, &dwWndProcID);
	//		if(dwWndProcID == dwProcID)
	//			return hWnd;            
	//		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	//	}
	//	return nullptr;
	// }

	//HWND FindMyWindowByID(int e_iID)
	//{
	//	DWORD dwProcID = e_iID;
	//	HWND hWnd = GetTopWindow(GetDesktopWindow());
	//	while(hWnd)
	//	{
	//		DWORD dwWndProcID = 0;
	//		GetWindowThreadProcessId(hWnd, &dwWndProcID);
	//		if(dwWndProcID == dwProcID)
	//			return hWnd;            
	//		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	//	}
	//	return nullptr;
	// }

	//static HWND	hwndMain = 0;
	//BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam)
	//{
	//	HINSTANCE hinst=(HINSTANCE)GetModuleHandle(nullptr);
	//	HWND	hwndMain = 0;
	//	if((HINSTANCE)GetWindowLongPtr(hwnd, GWL_HINSTANCE)==hinst )
	//	{
	//		hwndMain=hwnd;
	//		return FALSE;
	//	}
	//	else
	//		return TRUE;
	//}

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for cPIEditor
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	//if ui designer cannt open,please comment the define
//#ifdef USER_CONTROL_ENABLE
//	public ref class cPIEditor : public System::Windows::Forms::UserControl
//	{
//	public:
//		cPIEditor(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm,String^e_strFileName)
//#else
	public ref class cPIEditor : public System::Windows::Forms::Form
	{
	public:
		cPIEditor(String^e_strFileName)
//#endif
		{
			InitializeComponent();
			cGameApp::CreateDefaultOpenGLRender();
			//now it only support power of two on windows,because I am lazy to fix cUIImage.
			g_bSupportNonPowerOfTwoTexture = true;
			this->menuStrip1->Visible = false;
			m_pvBGColor = new Vector4(0, 0, 0, 1);
			cNodeISAX l_NodeISAX;
			if (l_NodeISAX.ParseDataIntoXMLNode("Setup.xml"))
			{
				auto l_pRoot = l_NodeISAX.GetRootElement();
				PARSE_ELEMENT_START(l_pRoot)
					COMPARE_NAME("Chinese")
					{
						if (wcslen(l_strValue) == 4 || GetInt(l_strValue)>0)
							g_bLanguageChinese = true;
						else
							g_bLanguageChinese = false;
					}
					else
					COMPARE_NAME("BGColor")
					{
						*m_pvBGColor = GetVector4(l_strValue);
					}
				PARSE_NAME_VALUE_END
				//Chinese = "true" NonPowerOfTwoSupport = "true" BGColor = "0.5,0.5,0.5,1"
			}
			m_pPuzzleImageUnitTriangulatorManager = new cPuzzleImageUnitTriangulatorManager();
			m_pCurrentSelectedPuzzleImageUnitTriangulator = nullptr;
			m_pTimeAndFPS = new UT::sTimeAndFPS;
			m_pImageIndexOfAnimation = new cImageIndexOfAnimation(true);
			m_pImageIndexOfAnimationList = new cNamedTypedObjectVector<cImageIndexOfAnimation>;
			m_pImageomposerIRM = new cImageParser();
			cPuzzleImage::m_sbSortPIFileAsOriginal = true;
			m_pOrthogonalCamera = new cOrthogonalCamera();
			m_pOrthogonalCameraForTrianhulatorPIUnit = new cOrthogonalCamera();
			m_ImageTale = gcnew System::Collections::Hashtable;
			m_pParentHandle = 0;
			m_HdcMV = GetDC((HWND)this->splitContainer2->Panel1->Handle.ToPointer());
			m_HGLRCMV = InitOpenGL((HWND)this->splitContainer2->Panel1->Handle.ToPointer(),true,m_HdcMV);
			MouseControlMode_comboBox->SelectedIndex = 0;
#ifdef USER_CONTROL_ENABLE
			m_pForm = e_pForm;
			if( m_pForm )
			{
				if( m_pForm->Parent )
				{
					m_pParentHandle = (HWND)m_pForm->Parent->Handle.ToPointer();
				}
			}
			if( e_ptabControl )
			{
				m_pTabPage = gcnew GCFORM::TabPage();
				m_pTabPage->Text = "PuzzleImage/圖檔合併";
				m_pTabPage->AutoScroll = true;
				m_pTabPage->Controls->Add(this);
				e_ptabControl->TabPages->Add(m_pTabPage);
				m_pTabControl = e_ptabControl;
				e_ptabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::tabControl_SelectedIndexChanged);
			}
			else
			{
				m_pForm->Controls->Add(this);
			}
#endif
			splitContainer2->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseMove);
			splitContainer2->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseDown);
			splitContainer2->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseUp);
			splitContainer2->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseUp);
			splitContainer2->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::MyKeyUp);
			splitContainer2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &cPIEditor::MyKeyPress);
			splitContainer2->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::MyKeyDown);
			//
			LanguageSwitch(this->Controls,"/",this->Handle);
			LanguageSwitch(splitContainer1->Panel1->Controls,"/",this->Handle);
			m_pDebugFont = 0;
			if(System::IO::File::Exists("Font.png"))
			{
				m_pDebugFont = new cGlyphFontRender("Font");
				if( !m_pDebugFont->GetFontImage() )
					SAFE_DELETE(m_pDebugFont);
			}
			g_pDebugFont = m_pDebugFont;
			CreateShader(g_bCommonVSClientState,L"ICPathShader");
			CreateShader(g_bCommonVSNoTextureClientState,L"IC_NoTextureShader");
			CreateShader(g_bCommonVSNoTextureClientState, g_strDrawPointsVS, g_strCommonFSNoTexture, L"IC_DrawPointsShader");
			m_bAvoidDataAssignForPIUintChild = false;
			if( e_strFileName && e_strFileName->Length )
				OpenPIFile(e_strFileName);
			ToolStripMenuItem^l_pToolStripMenuItem = (ToolStripMenuItem^)this->menuStrip1->Items[0];
			this->Dock = GCFORM::DockStyle::Fill;
			TriangulatorMouseBehavior_comboBox->SelectedIndex = 0;
			//WARNING_MSG(openXMLToolStripMenuItem->Size.ToString());
			if(System::IO::File::Exists(LAST_USE_PI_FILE_NAME))
			{
				int l_iFileLength = 0;
				auto l_pFileContent = UT::GetFileContent(LAST_USE_PI_FILE_NAME, l_iFileLength);
				if (l_pFileContent)
				{
					//if (UT::IsFileExists(l_pFileContent))
					{
						if (!OpenPIFile(gcnew String(l_pFileContent)))
						{
							UT::MyRemoveFile(LAST_USE_PI_FILE_NAME);
						}
					}
				}
			}
			this->timer1->Enabled = true;
		}

	protected:
		~cPIEditor()
		{
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(cGameApp::m_spOpenGLRender);
			DeleteShader(L"ICPathShader");
			DeleteShader(L"IC_NoTextureShader");
			DeleteShader(L"IC_DrawPointsShader");
			delete m_ImageTale;
			SAFE_DELETE(m_pvBGColor);
			SAFE_DELETE(m_pDebugFont);
			SAFE_DELETE(m_pImageIndexOfAnimation);
			SAFE_DELETE(m_pImageIndexOfAnimationList);
			SAFE_DELETE(m_pImageomposerIRM);
			SAFE_DELETE(m_pTimeAndFPS);
			SAFE_DELETE(m_pOrthogonalCamera);
			SAFE_DELETE(m_pOrthogonalCameraForTrianhulatorPIUnit);
			SAFE_DELETE(m_pPuzzleImageUnitTriangulatorManager);
		}
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::Button^  ImageObjectDown_button;
	private: System::Windows::Forms::Button^  ImageObjectUP_button;
	private: System::Windows::Forms::Button^  AutoOrderPosition_button;
	private: System::Windows::Forms::Button^  DeleteImage_button;
	private: System::Windows::Forms::Button^  AddImage_button;
	private: System::Windows::Forms::Button^  AnimationImageDown_button;
	private: System::Windows::Forms::Button^  AnimationImageUp_button;
	private: System::Windows::Forms::Button^  DelAnimationImage_button;
	private: System::Windows::Forms::Button^  AddAnimationImage_button;
	private: System::Windows::Forms::Button^  ProperStripPixel_button;
	private: System::Windows::Forms::Button^  ToOriginalImage_button;
	private: System::Windows::Forms::Button^  StripAlpha_button;
	private: System::Windows::Forms::Button^  AddNewPIUnitImage_button;
	private: System::Windows::Forms::Button^  AnimationDatDel_button;
	private: System::Windows::Forms::Button^  AnimationDatAdd_button;
	private: System::Windows::Forms::Button^  DeleteTime_button;
	private: System::Windows::Forms::Button^  AddTime_button;
	private: System::Windows::Forms::Button^  LODToPoints_button;
	private: System::Windows::Forms::ListBox^  AllImage_listBox;
	private: System::Windows::Forms::ListBox^  AnimationData_listBox;
	private: System::Windows::Forms::ListBox^  AnimationDataList_listBox;
	private: System::Windows::Forms::ListBox^  MorphingAnimationTime_listBox;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  AnimationTime_label;
	private: System::Windows::Forms::Label^  MorphintTime_label;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  TotalPixel_label;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  ImageTriangulatorLOD_label;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  MouseControlMode_label;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::NumericUpDown^  ImagePosX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImagePosY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImageDistanceX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImageDistanceY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitStartX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitEndY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitEndX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitStartY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImageHeight_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImageWidth_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImageTriangulatorLOD_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  AnimationTime_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  RearrangeMorphingAnimationTime_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  MorphingAnimationTime_numericUpDown;
	private: System::Windows::Forms::CheckBox^  ImageSaveAsDDS_checkBox;
	private: System::Windows::Forms::CheckBox^  AutoAligment_checkBox;
	private: System::Windows::Forms::CheckBox^  BinaryExport_checkBox;
	private: System::Windows::Forms::CheckBox^  ShowTriangulaotrPoints_checkBox;
	private: System::Windows::Forms::CheckBox^  AllShowBoundry_checkBox;
	private: System::Windows::Forms::CheckBox^  ShowBoundary_checkBox;
	private: System::Windows::Forms::CheckBox^  DebugLine_checkBox;
	private: System::Windows::Forms::CheckBox^  EditAnimation_checkBox;
	private: System::Windows::Forms::CheckBox^  PlayMorphingAnimation_checkBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  generateAllImageWithPowerOfTwoToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  dockToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  floatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  childToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  floatToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addImagesByFolderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  generateTriangulatorImagesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openXMLToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  invertPuzzleImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  generatePowOf2ImageToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^  TriangulatorMouseBehavior_comboBox;
	private: System::Windows::Forms::ComboBox^  MouseControlMode_comboBox;
	private: System::Windows::Forms::TextBox^  ImageTriangulator_textBox;
	private: System::Windows::Forms::TextBox^  NewPIUnitName_textBox;
	private: System::Windows::Forms::TextBox^  AnimationDataName_textBox;
	private: System::Windows::Forms::TextBox^  ImageDetail_textBox;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  ImageAligment_tabPage;
	private: System::Windows::Forms::TabPage^  ImageTriangulator_tabPage;
	private: System::Windows::Forms::TabPage^  SequenceAnimation_tabPage;
	private: System::Windows::Forms::TrackBar^  MorphingAnimation_trackBar;
private: System::Windows::Forms::GroupBox^ Morphing_groupBox;

	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	//my
	private: GCFORM::TabPage^m_pTabPage;					//for attach from.
	private: GCFORM::TabControl^m_pTabControl;				//to determin is tabpage clicked.
	private:cPuzzleImageUnitTriangulatorManager*		m_pPuzzleImageUnitTriangulatorManager;
	private:cPuzzleImageUnitTriangulator *				m_pCurrentSelectedPuzzleImageUnitTriangulator;
	private: GCFORM::Form^								m_pForm;
	public:HWND											m_pParentHandle;
	String^												m_strParentProcessID;
	bool												m_bAvoidDataAssignForPIUintChild;
	HGLRC												m_HGLRCMV;
	HDC													m_HdcMV;
	cGlyphFontRender*									m_pDebugFont;
	cImageParser*										m_pImageomposerIRM;
	int													m_iCurrentSelectedObjectIndex;
	cImageIndexOfAnimation*								m_pImageIndexOfAnimation;
	cNamedTypedObjectVector<cImageIndexOfAnimation>*	m_pImageIndexOfAnimationList;
	cOrthogonalCamera*									m_pOrthogonalCamera;
	cOrthogonalCamera*									m_pOrthogonalCameraForTrianhulatorPIUnit;
	UT::sTimeAndFPS*									m_pTimeAndFPS;
	Vector4	*m_pvBGColor;
private: System::Windows::Forms::CheckBox^  MorphingAddKeyTimeUsePreKeyData_checkBox;
private: System::Windows::Forms::Button^  RearrangeMorphingTime_button;
private: System::Windows::Forms::Button^  ChangeMorphingTime_button;
private: System::Windows::Forms::Button^  InsertMorphingTime_button;
private: System::Windows::Forms::GroupBox^ MorphingCenterOffset_groupBox;
private: System::Windows::Forms::Button^ OffsetCenter_button;
private: System::Windows::Forms::Label^ CenterOffsetY_label;
private: System::Windows::Forms::NumericUpDown^ CenterOffsetPosY_numericUpDown;
private: System::Windows::Forms::Label^ CenterOffsetPosX_label;
private: System::Windows::Forms::NumericUpDown^ CenterOffsetPosX_numericUpDown;
private: System::Windows::Forms::ToolStripMenuItem^ generateSelectedPIUnitXMLToolStripMenuItem;
private: System::Windows::Forms::TabPage^ DividImageTool_tabPage;
private: System::Windows::Forms::Label^ label17;
private: System::Windows::Forms::TextBox^ DividImagePrefixName_textBox;
private: System::Windows::Forms::Button^ DividImageTool_button;




private: System::Windows::Forms::Label^ label18;
private: System::Windows::Forms::NumericUpDown^ DivideToolColumn_numericUpDown;
private: System::Windows::Forms::Label^ label19;
private: System::Windows::Forms::NumericUpDown^ DivideToolRow_numericUpDown;
private: System::Windows::Forms::TabPage^ AtlasFileConvert_tabPage;
private: System::Windows::Forms::Label^ AltasScale_label;
private: System::Windows::Forms::DomainUpDown^ ASltasScale_domainUpDown;
private: System::Windows::Forms::CheckBox^ ReplaceOriginalFile_checkBox;
private: System::Windows::Forms::Button^ ConvertAltas_button;
private: System::Windows::Forms::ListBox^ AltasFiles_listBox;
private: System::Windows::Forms::Button^ SelectDirecotry_button;
public:
public:
	private: System::Collections::Hashtable^			m_ImageTale;	//key:string,value:System::Drawing::Bitmap.,if m_pImageomposerIRM's child(UIImage) has owner,then m_ImageTale do not has its data
	private: System::Void								SavePuzzleFile(String^e_strFileName,bool e_bBinary);
	private: cPuzzleImage*								OpenPuzzleFile(String^e_strFileName);
	public:  bool										OpenPIFile(String^e_strFileName);
	private: bool										ParsePuzzleImage(cPuzzleImage*e_pPuzzleImage,String^e_strFileName);
	private: System::Void								GeneratePowOf2Image(bool e_bPowerOfTwo);
	private: System::Void								MouseCollideForPickUpObject( System::Windows::Forms::MouseEventArgs^  e,System::Windows::Forms::Panel^e_pPanel);
	private: System::Void								AssignAnimationData();
	private: int										GetMouseWheelDelta(System::Windows::Forms::MouseEventArgs^  e);
	private: String^									GetTotalPixelExisted();	//all UIImage pixel size
			 String^									m_strCurrentFileName;
			 cUIImage*									GetNewUIImageByBitMap(System::Drawing::Bitmap^e_pBitMap,const WCHAR*e_strName);
			 void										AttachObjectPosAdjust();
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->TotalPixel_label = (gcnew System::Windows::Forms::Label());
			this->AllShowBoundry_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImageObjectDown_button = (gcnew System::Windows::Forms::Button());
			this->ImageObjectUP_button = (gcnew System::Windows::Forms::Button());
			this->ShowBoundary_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImageDetail_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->ImageHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ImageWidth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AutoOrderPosition_button = (gcnew System::Windows::Forms::Button());
			this->DeleteImage_button = (gcnew System::Windows::Forms::Button());
			this->AddImage_button = (gcnew System::Windows::Forms::Button());
			this->AllImage_listBox = (gcnew System::Windows::Forms::ListBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openXMLToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->invertPuzzleImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->generatePowOf2ImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->generateAllImageWithPowerOfTwoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->generateTriangulatorImagesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->generateSelectedPIUnitXMLToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dockToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->floatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->childToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->floatToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addImagesByFolderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->ToOriginalImage_button = (gcnew System::Windows::Forms::Button());
			this->ProperStripPixel_button = (gcnew System::Windows::Forms::Button());
			this->StripAlpha_button = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->ImageAligment_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->MouseControlMode_label = (gcnew System::Windows::Forms::Label());
			this->MouseControlMode_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->ShowTriangulaotrPoints_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->AutoAligment_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->BinaryExport_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImageSaveAsDDS_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->NewPIUnitName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->AddNewPIUnitImage_button = (gcnew System::Windows::Forms::Button());
			this->NewPIUnitStartX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->NewPIUnitEndY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->NewPIUnitEndX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->NewPIUnitStartY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->ImageDistanceY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->ImageDistanceX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DebugLine_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImagePosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ImagePosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SequenceAnimation_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->AnimationTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AnimationData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AnimationDataList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AnimationDatDel_button = (gcnew System::Windows::Forms::Button());
			this->AddAnimationImage_button = (gcnew System::Windows::Forms::Button());
			this->AnimationDatAdd_button = (gcnew System::Windows::Forms::Button());
			this->DelAnimationImage_button = (gcnew System::Windows::Forms::Button());
			this->AnimationDataName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->AnimationImageUp_button = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->AnimationImageDown_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ImageTriangulator_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->MorphingCenterOffset_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->OffsetCenter_button = (gcnew System::Windows::Forms::Button());
			this->CenterOffsetY_label = (gcnew System::Windows::Forms::Label());
			this->CenterOffsetPosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CenterOffsetPosX_label = (gcnew System::Windows::Forms::Label());
			this->CenterOffsetPosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->InsertMorphingTime_button = (gcnew System::Windows::Forms::Button());
			this->RearrangeMorphingTime_button = (gcnew System::Windows::Forms::Button());
			this->ChangeMorphingTime_button = (gcnew System::Windows::Forms::Button());
			this->MorphingAddKeyTimeUsePreKeyData_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->EditAnimation_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->PlayMorphingAnimation_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->MorphintTime_label = (gcnew System::Windows::Forms::Label());
			this->MorphingAnimation_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->Morphing_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->TriangulatorMouseBehavior_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->ImageTriangulatorLOD_label = (gcnew System::Windows::Forms::Label());
			this->ImageTriangulatorLOD_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->LODToPoints_button = (gcnew System::Windows::Forms::Button());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->AnimationTime_label = (gcnew System::Windows::Forms::Label());
			this->RearrangeMorphingAnimationTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DeleteTime_button = (gcnew System::Windows::Forms::Button());
			this->AddTime_button = (gcnew System::Windows::Forms::Button());
			this->MorphingAnimationTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->MorphingAnimationTime_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ImageTriangulator_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DividImageTool_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->DivideToolColumn_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->DivideToolRow_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->DividImagePrefixName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DividImageTool_button = (gcnew System::Windows::Forms::Button());
			this->AtlasFileConvert_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->AltasScale_label = (gcnew System::Windows::Forms::Label());
			this->ASltasScale_domainUpDown = (gcnew System::Windows::Forms::DomainUpDown());
			this->ReplaceOriginalFile_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ConvertAltas_button = (gcnew System::Windows::Forms::Button());
			this->AltasFiles_listBox = (gcnew System::Windows::Forms::ListBox());
			this->SelectDirecotry_button = (gcnew System::Windows::Forms::Button());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageHeight_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageWidth_numericUpDown))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer3))->BeginInit();
			this->splitContainer3->Panel1->SuspendLayout();
			this->splitContainer3->Panel2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->ImageAligment_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitStartX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitEndY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitEndX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitStartY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageDistanceY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageDistanceX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImagePosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImagePosX_numericUpDown))->BeginInit();
			this->SequenceAnimation_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AnimationTime_numericUpDown))->BeginInit();
			this->ImageTriangulator_tabPage->SuspendLayout();
			this->MorphingCenterOffset_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CenterOffsetPosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CenterOffsetPosX_numericUpDown))->BeginInit();
			this->flowLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MorphingAnimation_trackBar))->BeginInit();
			this->Morphing_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageTriangulatorLOD_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->RearrangeMorphingAnimationTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MorphingAnimationTime_numericUpDown))->BeginInit();
			this->DividImageTool_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DivideToolColumn_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DivideToolRow_numericUpDown))->BeginInit();
			this->AtlasFileConvert_tabPage->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer2))->BeginInit();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &cPIEditor::timer1_Tick);
			// 
			// TotalPixel_label
			// 
			this->TotalPixel_label->AutoSize = true;
			this->TotalPixel_label->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->TotalPixel_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->TotalPixel_label->Location = System::Drawing::Point(12, 312);
			this->TotalPixel_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->TotalPixel_label->Name = L"TotalPixel_label";
			this->TotalPixel_label->Size = System::Drawing::Size(185, 25);
			this->TotalPixel_label->TabIndex = 46;
			this->TotalPixel_label->Text = L"TotalPixel:640000";
			// 
			// AllShowBoundry_checkBox
			// 
			this->AllShowBoundry_checkBox->AutoSize = true;
			this->AllShowBoundry_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AllShowBoundry_checkBox->Checked = true;
			this->AllShowBoundry_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->AllShowBoundry_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllShowBoundry_checkBox->Location = System::Drawing::Point(12, 87);
			this->AllShowBoundry_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->AllShowBoundry_checkBox->Name = L"AllShowBoundry_checkBox";
			this->AllShowBoundry_checkBox->Size = System::Drawing::Size(366, 29);
			this->AllShowBoundry_checkBox->TabIndex = 45;
			this->AllShowBoundry_checkBox->Text = L"AllShowBoundary/顯示所有圖片框";
			this->AllShowBoundry_checkBox->UseVisualStyleBackColor = false;
			// 
			// ImageObjectDown_button
			// 
			this->ImageObjectDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageObjectDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageObjectDown_button->Location = System::Drawing::Point(596, 212);
			this->ImageObjectDown_button->Margin = System::Windows::Forms::Padding(6);
			this->ImageObjectDown_button->Name = L"ImageObjectDown_button";
			this->ImageObjectDown_button->Size = System::Drawing::Size(42, 85);
			this->ImageObjectDown_button->TabIndex = 42;
			this->ImageObjectDown_button->Text = L"v\r\nv";
			this->ImageObjectDown_button->UseVisualStyleBackColor = false;
			this->ImageObjectDown_button->Click += gcnew System::EventHandler(this, &cPIEditor::button2_Click);
			// 
			// ImageObjectUP_button
			// 
			this->ImageObjectUP_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageObjectUP_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageObjectUP_button->Location = System::Drawing::Point(596, 54);
			this->ImageObjectUP_button->Margin = System::Windows::Forms::Padding(6);
			this->ImageObjectUP_button->Name = L"ImageObjectUP_button";
			this->ImageObjectUP_button->Size = System::Drawing::Size(42, 85);
			this->ImageObjectUP_button->TabIndex = 41;
			this->ImageObjectUP_button->Text = L"^\r\n^";
			this->ImageObjectUP_button->UseVisualStyleBackColor = false;
			this->ImageObjectUP_button->Click += gcnew System::EventHandler(this, &cPIEditor::button2_Click);
			// 
			// ShowBoundary_checkBox
			// 
			this->ShowBoundary_checkBox->AutoSize = true;
			this->ShowBoundary_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ShowBoundary_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ShowBoundary_checkBox->Location = System::Drawing::Point(12, 35);
			this->ShowBoundary_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->ShowBoundary_checkBox->Name = L"ShowBoundary_checkBox";
			this->ShowBoundary_checkBox->Size = System::Drawing::Size(342, 29);
			this->ShowBoundary_checkBox->TabIndex = 40;
			this->ShowBoundary_checkBox->Text = L"ShowBoundary/顯示目前圖片框";
			this->ShowBoundary_checkBox->UseVisualStyleBackColor = false;
			// 
			// ImageDetail_textBox
			// 
			this->ImageDetail_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageDetail_textBox->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->ImageDetail_textBox->Enabled = false;
			this->ImageDetail_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->ImageDetail_textBox->Location = System::Drawing::Point(6, 497);
			this->ImageDetail_textBox->Margin = System::Windows::Forms::Padding(6);
			this->ImageDetail_textBox->MinimumSize = System::Drawing::Size(4, 56);
			this->ImageDetail_textBox->Multiline = true;
			this->ImageDetail_textBox->Name = L"ImageDetail_textBox";
			this->ImageDetail_textBox->Size = System::Drawing::Size(2690, 56);
			this->ImageDetail_textBox->TabIndex = 39;
			this->ImageDetail_textBox->Text = L"79979";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label4->Location = System::Drawing::Point(522, 142);
			this->label4->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(185, 25);
			this->label4->TabIndex = 38;
			this->label4->Text = L"Height/產生圖片高";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label5->Location = System::Drawing::Point(528, 42);
			this->label5->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(178, 25);
			this->label5->TabIndex = 37;
			this->label5->Text = L"Width/產生圖片寬";
			// 
			// ImageHeight_numericUpDown
			// 
			this->ImageHeight_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageHeight_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageHeight_numericUpDown->Location = System::Drawing::Point(528, 173);
			this->ImageHeight_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->ImageHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImageHeight_numericUpDown->Name = L"ImageHeight_numericUpDown";
			this->ImageHeight_numericUpDown->Size = System::Drawing::Size(124, 31);
			this->ImageHeight_numericUpDown->TabIndex = 36;
			this->ImageHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 512, 0, 0, 0 });
			this->ImageHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImageWidth_numericUpDown_ValueChanged);
			// 
			// ImageWidth_numericUpDown
			// 
			this->ImageWidth_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageWidth_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageWidth_numericUpDown->Location = System::Drawing::Point(528, 81);
			this->ImageWidth_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->ImageWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImageWidth_numericUpDown->Name = L"ImageWidth_numericUpDown";
			this->ImageWidth_numericUpDown->Size = System::Drawing::Size(124, 31);
			this->ImageWidth_numericUpDown->TabIndex = 35;
			this->ImageWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1024, 0, 0, 0 });
			this->ImageWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImageWidth_numericUpDown_ValueChanged);
			// 
			// AutoOrderPosition_button
			// 
			this->AutoOrderPosition_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AutoOrderPosition_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AutoOrderPosition_button->Location = System::Drawing::Point(528, 229);
			this->AutoOrderPosition_button->Margin = System::Windows::Forms::Padding(6);
			this->AutoOrderPosition_button->Name = L"AutoOrderPosition_button";
			this->AutoOrderPosition_button->Size = System::Drawing::Size(278, 75);
			this->AutoOrderPosition_button->TabIndex = 34;
			this->AutoOrderPosition_button->Text = L"ImagePositionAutoAssign/圖片自動校正位置";
			this->AutoOrderPosition_button->UseVisualStyleBackColor = false;
			this->AutoOrderPosition_button->Click += gcnew System::EventHandler(this, &cPIEditor::AutoOrderPosition_button_Click);
			// 
			// DeleteImage_button
			// 
			this->DeleteImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DeleteImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteImage_button->Location = System::Drawing::Point(28, 112);
			this->DeleteImage_button->Margin = System::Windows::Forms::Padding(6);
			this->DeleteImage_button->Name = L"DeleteImage_button";
			this->DeleteImage_button->Size = System::Drawing::Size(200, 52);
			this->DeleteImage_button->TabIndex = 29;
			this->DeleteImage_button->Text = L"DeleteImage/刪除圖片";
			this->DeleteImage_button->UseVisualStyleBackColor = false;
			this->DeleteImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::DeleteImage_button_Click);
			// 
			// AddImage_button
			// 
			this->AddImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddImage_button->Location = System::Drawing::Point(28, 48);
			this->AddImage_button->Margin = System::Windows::Forms::Padding(6);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(198, 58);
			this->AddImage_button->TabIndex = 28;
			this->AddImage_button->Text = L"AddImage/增加圖片";
			this->AddImage_button->UseVisualStyleBackColor = false;
			this->AddImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::DeleteImage_button_Click);
			// 
			// AllImage_listBox
			// 
			this->AllImage_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AllImage_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllImage_listBox->FormattingEnabled = true;
			this->AllImage_listBox->ItemHeight = 25;
			this->AllImage_listBox->Location = System::Drawing::Point(236, 27);
			this->AllImage_listBox->Margin = System::Windows::Forms::Padding(6);
			this->AllImage_listBox->MinimumSize = System::Drawing::Size(4, 164);
			this->AllImage_listBox->Name = L"AllImage_listBox";
			this->AllImage_listBox->ScrollAlwaysVisible = true;
			this->AllImage_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllImage_listBox->Size = System::Drawing::Size(344, 304);
			this->AllImage_listBox->TabIndex = 27;
			this->AllImage_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::AllImage_listBox_SelectedIndexChanged);
			this->AllImage_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::AllImage_listBox_KeyUp_1);
			this->AllImage_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::AllImage_listBox_MouseDoubleClick);
			// 
			// menuStrip1
			// 
			this->menuStrip1->AccessibleRole = System::Windows::Forms::AccessibleRole::Grip;
			this->menuStrip1->AllowItemReorder = true;
			this->menuStrip1->AllowMerge = false;
			this->menuStrip1->AutoSize = false;
			this->menuStrip1->GripMargin = System::Windows::Forms::Padding(2, 2, 0, 2);
			this->menuStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Visible;
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(32, 32);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->fileToolStripMenuItem,
					this->dockToolStripMenuItem, this->floatToolStripMenuItem1, this->toolToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->MdiWindowListItem = this->fileToolStripMenuItem;
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->menuStrip1->Size = System::Drawing::Size(3374, 50);
			this->menuStrip1->Stretch = false;
			this->menuStrip1->TabIndex = 26;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {
				this->openXMLToolStripMenuItem,
					this->saveToolStripMenuItem, this->invertPuzzleImageToolStripMenuItem, this->generatePowOf2ImageToolStripMenuItem, this->generateAllImageWithPowerOfTwoToolStripMenuItem,
					this->generateTriangulatorImagesToolStripMenuItem, this->generateSelectedPIUnitXMLToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(71, 46);
			this->fileToolStripMenuItem->Text = L"File";
			this->fileToolStripMenuItem->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::fileToolStripMenuItem_MouseDown);
			// 
			// openXMLToolStripMenuItem
			// 
			this->openXMLToolStripMenuItem->Name = L"openXMLToolStripMenuItem";
			this->openXMLToolStripMenuItem->Size = System::Drawing::Size(576, 44);
			this->openXMLToolStripMenuItem->Text = L"Open";
			this->openXMLToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::openXMLToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(576, 44);
			this->saveToolStripMenuItem->Text = L"GeneratePuzzleImage/產生單張圖片";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::Test_button_Click);
			// 
			// invertPuzzleImageToolStripMenuItem
			// 
			this->invertPuzzleImageToolStripMenuItem->Name = L"invertPuzzleImageToolStripMenuItem";
			this->invertPuzzleImageToolStripMenuItem->Size = System::Drawing::Size(576, 44);
			this->invertPuzzleImageToolStripMenuItem->Text = L"InvertPuzzleImage/將資料反轉成張圖片";
			this->invertPuzzleImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::InvertPuzzleImage_button_Click);
			// 
			// generatePowOf2ImageToolStripMenuItem
			// 
			this->generatePowOf2ImageToolStripMenuItem->Name = L"generatePowOf2ImageToolStripMenuItem";
			this->generatePowOf2ImageToolStripMenuItem->Size = System::Drawing::Size(576, 44);
			this->generatePowOf2ImageToolStripMenuItem->Text = L"Generate All Image/產生所有圖片";
			this->generatePowOf2ImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::button1_Click);
			// 
			// generateAllImageWithPowerOfTwoToolStripMenuItem
			// 
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Name = L"generateAllImageWithPowerOfTwoToolStripMenuItem";
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Size = System::Drawing::Size(576, 44);
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Text = L"Generate All Image With PowerOfTwo";
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::button1_Click);
			// 
			// generateTriangulatorImagesToolStripMenuItem
			// 
			this->generateTriangulatorImagesToolStripMenuItem->Name = L"generateTriangulatorImagesToolStripMenuItem";
			this->generateTriangulatorImagesToolStripMenuItem->Size = System::Drawing::Size(576, 44);
			this->generateTriangulatorImagesToolStripMenuItem->Text = L"Generate Triangulator Images";
			this->generateTriangulatorImagesToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::generateTriangulatorImagesToolStripMenuItem_Click);
			// 
			// generateSelectedPIUnitXMLToolStripMenuItem
			// 
			this->generateSelectedPIUnitXMLToolStripMenuItem->Name = L"generateSelectedPIUnitXMLToolStripMenuItem";
			this->generateSelectedPIUnitXMLToolStripMenuItem->Size = System::Drawing::Size(576, 44);
			this->generateSelectedPIUnitXMLToolStripMenuItem->Text = L"GenerateSelectedPIUnitXML";
			this->generateSelectedPIUnitXMLToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::generateSelectedPIUnitXMLToolStripMenuItem_Click);
			// 
			// dockToolStripMenuItem
			// 
			this->dockToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->floatToolStripMenuItem,
					this->childToolStripMenuItem
			});
			this->dockToolStripMenuItem->Name = L"dockToolStripMenuItem";
			this->dockToolStripMenuItem->Size = System::Drawing::Size(88, 46);
			this->dockToolStripMenuItem->Text = L"Dock";
			// 
			// floatToolStripMenuItem
			// 
			this->floatToolStripMenuItem->Name = L"floatToolStripMenuItem";
			this->floatToolStripMenuItem->Size = System::Drawing::Size(202, 44);
			this->floatToolStripMenuItem->Text = L"Float";
			this->floatToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// childToolStripMenuItem
			// 
			this->childToolStripMenuItem->Name = L"childToolStripMenuItem";
			this->childToolStripMenuItem->Size = System::Drawing::Size(202, 44);
			this->childToolStripMenuItem->Text = L"Child";
			this->childToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// floatToolStripMenuItem1
			// 
			this->floatToolStripMenuItem1->Name = L"floatToolStripMenuItem1";
			this->floatToolStripMenuItem1->Size = System::Drawing::Size(86, 46);
			this->floatToolStripMenuItem1->Text = L"Float";
			this->floatToolStripMenuItem1->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// toolToolStripMenuItem
			// 
			this->toolToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->addImagesByFolderToolStripMenuItem });
			this->toolToolStripMenuItem->Name = L"toolToolStripMenuItem";
			this->toolToolStripMenuItem->Size = System::Drawing::Size(79, 46);
			this->toolToolStripMenuItem->Text = L"Tool";
			// 
			// addImagesByFolderToolStripMenuItem
			// 
			this->addImagesByFolderToolStripMenuItem->Name = L"addImagesByFolderToolStripMenuItem";
			this->addImagesByFolderToolStripMenuItem->Size = System::Drawing::Size(380, 44);
			this->addImagesByFolderToolStripMenuItem->Text = L"Add Images By Folder";
			this->addImagesByFolderToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::addImagesByFolderToolStripMenuItem_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 50);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(4);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->splitContainer1->Panel1->Controls->Add(this->splitContainer3);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(3374, 1010);
			this->splitContainer1->SplitterDistance = 610;
			this->splitContainer1->SplitterWidth = 6;
			this->splitContainer1->TabIndex = 49;
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Margin = System::Windows::Forms::Padding(6);
			this->splitContainer3->Name = L"splitContainer3";
			// 
			// splitContainer3.Panel1
			// 
			this->splitContainer3->Panel1->Controls->Add(this->AllImage_listBox);
			this->splitContainer3->Panel1->Controls->Add(this->AddImage_button);
			this->splitContainer3->Panel1->Controls->Add(this->ToOriginalImage_button);
			this->splitContainer3->Panel1->Controls->Add(this->DeleteImage_button);
			this->splitContainer3->Panel1->Controls->Add(this->ImageObjectUP_button);
			this->splitContainer3->Panel1->Controls->Add(this->ProperStripPixel_button);
			this->splitContainer3->Panel1->Controls->Add(this->StripAlpha_button);
			this->splitContainer3->Panel1->Controls->Add(this->ImageObjectDown_button);
			// 
			// splitContainer3.Panel2
			// 
			this->splitContainer3->Panel2->Controls->Add(this->tabControl1);
			this->splitContainer3->Size = System::Drawing::Size(3370, 606);
			this->splitContainer3->SplitterDistance = 644;
			this->splitContainer3->SplitterWidth = 8;
			this->splitContainer3->TabIndex = 67;
			// 
			// ToOriginalImage_button
			// 
			this->ToOriginalImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ToOriginalImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ToOriginalImage_button->Location = System::Drawing::Point(28, 229);
			this->ToOriginalImage_button->Margin = System::Windows::Forms::Padding(6);
			this->ToOriginalImage_button->Name = L"ToOriginalImage_button";
			this->ToOriginalImage_button->Size = System::Drawing::Size(200, 52);
			this->ToOriginalImage_button->TabIndex = 66;
			this->ToOriginalImage_button->Text = L"ToOriginalImag/回復原狀";
			this->ToOriginalImage_button->UseVisualStyleBackColor = false;
			this->ToOriginalImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::StripAlpha_button_Click);
			// 
			// ProperStripPixel_button
			// 
			this->ProperStripPixel_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ProperStripPixel_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ProperStripPixel_button->Location = System::Drawing::Point(28, 287);
			this->ProperStripPixel_button->Margin = System::Windows::Forms::Padding(6);
			this->ProperStripPixel_button->Name = L"ProperStripPixel_button";
			this->ProperStripPixel_button->Size = System::Drawing::Size(200, 52);
			this->ProperStripPixel_button->TabIndex = 64;
			this->ProperStripPixel_button->Text = L"ProperStripPixel/找出最大可用圖片";
			this->ProperStripPixel_button->UseVisualStyleBackColor = false;
			this->ProperStripPixel_button->Click += gcnew System::EventHandler(this, &cPIEditor::ProperStripPixel_button_Click);
			// 
			// StripAlpha_button
			// 
			this->StripAlpha_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->StripAlpha_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->StripAlpha_button->Location = System::Drawing::Point(28, 171);
			this->StripAlpha_button->Margin = System::Windows::Forms::Padding(6);
			this->StripAlpha_button->Name = L"StripAlpha_button";
			this->StripAlpha_button->Size = System::Drawing::Size(200, 52);
			this->StripAlpha_button->TabIndex = 65;
			this->StripAlpha_button->Text = L"StripAlpha/砍掉alpha";
			this->StripAlpha_button->UseVisualStyleBackColor = false;
			this->StripAlpha_button->Click += gcnew System::EventHandler(this, &cPIEditor::StripAlpha_button_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->ImageAligment_tabPage);
			this->tabControl1->Controls->Add(this->SequenceAnimation_tabPage);
			this->tabControl1->Controls->Add(this->ImageTriangulator_tabPage);
			this->tabControl1->Controls->Add(this->DividImageTool_tabPage);
			this->tabControl1->Controls->Add(this->AtlasFileConvert_tabPage);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Margin = System::Windows::Forms::Padding(6);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(2718, 606);
			this->tabControl1->TabIndex = 0;
			this->tabControl1->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::tabControl1_SelectedIndexChanged);
			// 
			// ImageAligment_tabPage
			// 
			this->ImageAligment_tabPage->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ImageAligment_tabPage->Controls->Add(this->MouseControlMode_label);
			this->ImageAligment_tabPage->Controls->Add(this->MouseControlMode_comboBox);
			this->ImageAligment_tabPage->Controls->Add(this->ShowTriangulaotrPoints_checkBox);
			this->ImageAligment_tabPage->Controls->Add(this->label8);
			this->ImageAligment_tabPage->Controls->Add(this->label4);
			this->ImageAligment_tabPage->Controls->Add(this->AutoAligment_checkBox);
			this->ImageAligment_tabPage->Controls->Add(this->label5);
			this->ImageAligment_tabPage->Controls->Add(this->BinaryExport_checkBox);
			this->ImageAligment_tabPage->Controls->Add(this->ImageDetail_textBox);
			this->ImageAligment_tabPage->Controls->Add(this->ShowBoundary_checkBox);
			this->ImageAligment_tabPage->Controls->Add(this->ImageSaveAsDDS_checkBox);
			this->ImageAligment_tabPage->Controls->Add(this->ImageHeight_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->label12);
			this->ImageAligment_tabPage->Controls->Add(this->ImageWidth_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->NewPIUnitName_textBox);
			this->ImageAligment_tabPage->Controls->Add(this->AutoOrderPosition_button);
			this->ImageAligment_tabPage->Controls->Add(this->AddNewPIUnitImage_button);
			this->ImageAligment_tabPage->Controls->Add(this->AllShowBoundry_checkBox);
			this->ImageAligment_tabPage->Controls->Add(this->NewPIUnitStartX_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->TotalPixel_label);
			this->ImageAligment_tabPage->Controls->Add(this->NewPIUnitEndY_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->NewPIUnitEndX_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->NewPIUnitStartY_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->label11);
			this->ImageAligment_tabPage->Controls->Add(this->label10);
			this->ImageAligment_tabPage->Controls->Add(this->label9);
			this->ImageAligment_tabPage->Controls->Add(this->label7);
			this->ImageAligment_tabPage->Controls->Add(this->ImageDistanceY_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->label6);
			this->ImageAligment_tabPage->Controls->Add(this->ImageDistanceX_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->DebugLine_checkBox);
			this->ImageAligment_tabPage->Controls->Add(this->ImagePosY_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->ImagePosX_numericUpDown);
			this->ImageAligment_tabPage->Controls->Add(this->label3);
			this->ImageAligment_tabPage->Location = System::Drawing::Point(8, 39);
			this->ImageAligment_tabPage->Margin = System::Windows::Forms::Padding(6);
			this->ImageAligment_tabPage->Name = L"ImageAligment_tabPage";
			this->ImageAligment_tabPage->Padding = System::Windows::Forms::Padding(6);
			this->ImageAligment_tabPage->Size = System::Drawing::Size(2702, 559);
			this->ImageAligment_tabPage->TabIndex = 0;
			this->ImageAligment_tabPage->Text = L"ImageAligment";
			// 
			// MouseControlMode_label
			// 
			this->MouseControlMode_label->AutoSize = true;
			this->MouseControlMode_label->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->MouseControlMode_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->MouseControlMode_label->Location = System::Drawing::Point(1666, 117);
			this->MouseControlMode_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->MouseControlMode_label->Name = L"MouseControlMode_label";
			this->MouseControlMode_label->Size = System::Drawing::Size(290, 25);
			this->MouseControlMode_label->TabIndex = 88;
			this->MouseControlMode_label->Text = L"MouseControlMode/操作模式";
			// 
			// MouseControlMode_comboBox
			// 
			this->MouseControlMode_comboBox->FormattingEnabled = true;
			this->MouseControlMode_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"ObjectClick", L"SelectNewObject" });
			this->MouseControlMode_comboBox->Location = System::Drawing::Point(1674, 154);
			this->MouseControlMode_comboBox->Margin = System::Windows::Forms::Padding(6);
			this->MouseControlMode_comboBox->Name = L"MouseControlMode_comboBox";
			this->MouseControlMode_comboBox->Size = System::Drawing::Size(238, 33);
			this->MouseControlMode_comboBox->TabIndex = 87;
			// 
			// ShowTriangulaotrPoints_checkBox
			// 
			this->ShowTriangulaotrPoints_checkBox->AutoSize = true;
			this->ShowTriangulaotrPoints_checkBox->Location = System::Drawing::Point(212, 275);
			this->ShowTriangulaotrPoints_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->ShowTriangulaotrPoints_checkBox->Name = L"ShowTriangulaotrPoints_checkBox";
			this->ShowTriangulaotrPoints_checkBox->Size = System::Drawing::Size(272, 29);
			this->ShowTriangulaotrPoints_checkBox->TabIndex = 86;
			this->ShowTriangulaotrPoints_checkBox->Text = L"ShowTriangulaotrPoints";
			this->ShowTriangulaotrPoints_checkBox->UseVisualStyleBackColor = true;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label8->Location = System::Drawing::Point(1164, 12);
			this->label8->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(379, 25);
			this->label8->TabIndex = 72;
			this->label8->Text = L"NewPIUnitStartX/新拼圖單元開始位置X";
			// 
			// AutoAligment_checkBox
			// 
			this->AutoAligment_checkBox->AutoSize = true;
			this->AutoAligment_checkBox->Checked = true;
			this->AutoAligment_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->AutoAligment_checkBox->Location = System::Drawing::Point(12, 137);
			this->AutoAligment_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->AutoAligment_checkBox->Name = L"AutoAligment_checkBox";
			this->AutoAligment_checkBox->Size = System::Drawing::Size(261, 29);
			this->AutoAligment_checkBox->TabIndex = 85;
			this->AutoAligment_checkBox->Text = L"AutoAligment/自動排列";
			this->AutoAligment_checkBox->UseVisualStyleBackColor = true;
			// 
			// BinaryExport_checkBox
			// 
			this->BinaryExport_checkBox->AutoSize = true;
			this->BinaryExport_checkBox->Location = System::Drawing::Point(12, 177);
			this->BinaryExport_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->BinaryExport_checkBox->Name = L"BinaryExport_checkBox";
			this->BinaryExport_checkBox->Size = System::Drawing::Size(167, 29);
			this->BinaryExport_checkBox->TabIndex = 84;
			this->BinaryExport_checkBox->Text = L"BinaryExport";
			this->BinaryExport_checkBox->UseVisualStyleBackColor = true;
			// 
			// ImageSaveAsDDS_checkBox
			// 
			this->ImageSaveAsDDS_checkBox->AutoSize = true;
			this->ImageSaveAsDDS_checkBox->Location = System::Drawing::Point(12, 225);
			this->ImageSaveAsDDS_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->ImageSaveAsDDS_checkBox->Name = L"ImageSaveAsDDS_checkBox";
			this->ImageSaveAsDDS_checkBox->Size = System::Drawing::Size(220, 29);
			this->ImageSaveAsDDS_checkBox->TabIndex = 83;
			this->ImageSaveAsDDS_checkBox->Text = L"ImageSaveAsDDS";
			this->ImageSaveAsDDS_checkBox->UseVisualStyleBackColor = true;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label12->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label12->Location = System::Drawing::Point(1590, 12);
			this->label12->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(320, 25);
			this->label12->TabIndex = 82;
			this->label12->Text = L"NewPIUnitName/新圖片單元名稱";
			// 
			// NewPIUnitName_textBox
			// 
			this->NewPIUnitName_textBox->Location = System::Drawing::Point(1674, 46);
			this->NewPIUnitName_textBox->Margin = System::Windows::Forms::Padding(6);
			this->NewPIUnitName_textBox->Name = L"NewPIUnitName_textBox";
			this->NewPIUnitName_textBox->Size = System::Drawing::Size(190, 31);
			this->NewPIUnitName_textBox->TabIndex = 81;
			// 
			// AddNewPIUnitImage_button
			// 
			this->AddNewPIUnitImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddNewPIUnitImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddNewPIUnitImage_button->Location = System::Drawing::Point(1670, 262);
			this->AddNewPIUnitImage_button->Margin = System::Windows::Forms::Padding(6);
			this->AddNewPIUnitImage_button->Name = L"AddNewPIUnitImage_button";
			this->AddNewPIUnitImage_button->Size = System::Drawing::Size(198, 73);
			this->AddNewPIUnitImage_button->TabIndex = 80;
			this->AddNewPIUnitImage_button->Text = L"AddNewPIUnitImage/增加新拼圖單元";
			this->AddNewPIUnitImage_button->UseVisualStyleBackColor = false;
			this->AddNewPIUnitImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::AddNewPIUnitImage_button_Click);
			// 
			// NewPIUnitStartX_numericUpDown
			// 
			this->NewPIUnitStartX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitStartX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitStartX_numericUpDown->Location = System::Drawing::Point(1254, 42);
			this->NewPIUnitStartX_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->NewPIUnitStartX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitStartX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->NewPIUnitStartX_numericUpDown->Name = L"NewPIUnitStartX_numericUpDown";
			this->NewPIUnitStartX_numericUpDown->Size = System::Drawing::Size(170, 31);
			this->NewPIUnitStartX_numericUpDown->TabIndex = 79;
			this->NewPIUnitStartX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitEndY_numericUpDown
			// 
			this->NewPIUnitEndY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitEndY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitEndY_numericUpDown->Location = System::Drawing::Point(1250, 287);
			this->NewPIUnitEndY_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->NewPIUnitEndY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitEndY_numericUpDown->Name = L"NewPIUnitEndY_numericUpDown";
			this->NewPIUnitEndY_numericUpDown->Size = System::Drawing::Size(170, 31);
			this->NewPIUnitEndY_numericUpDown->TabIndex = 78;
			this->NewPIUnitEndY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitEndX_numericUpDown
			// 
			this->NewPIUnitEndX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitEndX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitEndX_numericUpDown->Location = System::Drawing::Point(1254, 204);
			this->NewPIUnitEndX_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->NewPIUnitEndX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitEndX_numericUpDown->Name = L"NewPIUnitEndX_numericUpDown";
			this->NewPIUnitEndX_numericUpDown->Size = System::Drawing::Size(170, 31);
			this->NewPIUnitEndX_numericUpDown->TabIndex = 77;
			this->NewPIUnitEndX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitStartY_numericUpDown
			// 
			this->NewPIUnitStartY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitStartY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitStartY_numericUpDown->Location = System::Drawing::Point(1254, 127);
			this->NewPIUnitStartY_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->NewPIUnitStartY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitStartY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->NewPIUnitStartY_numericUpDown->Name = L"NewPIUnitStartY_numericUpDown";
			this->NewPIUnitStartY_numericUpDown->Size = System::Drawing::Size(170, 31);
			this->NewPIUnitStartY_numericUpDown->TabIndex = 76;
			this->NewPIUnitStartY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label11->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label11->Location = System::Drawing::Point(1250, 254);
			this->label11->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(374, 25);
			this->label11->TabIndex = 75;
			this->label11->Text = L"NewPIUnitEndY/新拼圖單元結束位置Y";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label10->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label10->Location = System::Drawing::Point(1250, 171);
			this->label10->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(372, 25);
			this->label10->TabIndex = 74;
			this->label10->Text = L"NewPIUnitEndX/新拼圖單元結束位置X";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label9->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label9->Location = System::Drawing::Point(1250, 90);
			this->label9->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(381, 25);
			this->label9->TabIndex = 73;
			this->label9->Text = L"NewPIUnitStartY/新拼圖單元開始位置Y";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label7->Location = System::Drawing::Point(840, 244);
			this->label7->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(274, 25);
			this->label7->TabIndex = 71;
			this->label7->Text = L"ImageDistanceY/圖片間距Y";
			// 
			// ImageDistanceY_numericUpDown
			// 
			this->ImageDistanceY_numericUpDown->Location = System::Drawing::Point(842, 275);
			this->ImageDistanceY_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->ImageDistanceY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->ImageDistanceY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->ImageDistanceY_numericUpDown->Name = L"ImageDistanceY_numericUpDown";
			this->ImageDistanceY_numericUpDown->Size = System::Drawing::Size(122, 31);
			this->ImageDistanceY_numericUpDown->TabIndex = 70;
			this->ImageDistanceY_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label6->Location = System::Drawing::Point(836, 162);
			this->label6->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(272, 25);
			this->label6->TabIndex = 69;
			this->label6->Text = L"ImageDistanceX/圖片間距X";
			// 
			// ImageDistanceX_numericUpDown
			// 
			this->ImageDistanceX_numericUpDown->Location = System::Drawing::Point(846, 194);
			this->ImageDistanceX_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->ImageDistanceX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->ImageDistanceX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->ImageDistanceX_numericUpDown->Name = L"ImageDistanceX_numericUpDown";
			this->ImageDistanceX_numericUpDown->Size = System::Drawing::Size(122, 31);
			this->ImageDistanceX_numericUpDown->TabIndex = 68;
			this->ImageDistanceX_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// DebugLine_checkBox
			// 
			this->DebugLine_checkBox->AutoSize = true;
			this->DebugLine_checkBox->Location = System::Drawing::Point(12, 275);
			this->DebugLine_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->DebugLine_checkBox->Name = L"DebugLine_checkBox";
			this->DebugLine_checkBox->Size = System::Drawing::Size(148, 29);
			this->DebugLine_checkBox->TabIndex = 67;
			this->DebugLine_checkBox->Text = L"DebugLine";
			this->DebugLine_checkBox->UseVisualStyleBackColor = true;
			// 
			// ImagePosY_numericUpDown
			// 
			this->ImagePosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImagePosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImagePosY_numericUpDown->Location = System::Drawing::Point(840, 112);
			this->ImagePosY_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->ImagePosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImagePosY_numericUpDown->Name = L"ImagePosY_numericUpDown";
			this->ImagePosY_numericUpDown->Size = System::Drawing::Size(170, 31);
			this->ImagePosY_numericUpDown->TabIndex = 61;
			this->ImagePosY_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 512, 0, 0, 0 });
			this->ImagePosY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImagePosX_numericUpDown_ValueChanged);
			// 
			// ImagePosX_numericUpDown
			// 
			this->ImagePosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImagePosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImagePosX_numericUpDown->Location = System::Drawing::Point(840, 54);
			this->ImagePosX_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->ImagePosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImagePosX_numericUpDown->Name = L"ImagePosX_numericUpDown";
			this->ImagePosX_numericUpDown->Size = System::Drawing::Size(170, 31);
			this->ImagePosX_numericUpDown->TabIndex = 60;
			this->ImagePosX_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1024, 0, 0, 0 });
			this->ImagePosX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImagePosX_numericUpDown_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(842, 8);
			this->label3->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(55, 25);
			this->label3->TabIndex = 62;
			this->label3->Text = L"Pos:";
			// 
			// SequenceAnimation_tabPage
			// 
			this->SequenceAnimation_tabPage->BackColor = System::Drawing::SystemColors::ControlDark;
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationTime_numericUpDown);
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationData_listBox);
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationDataList_listBox);
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationDatDel_button);
			this->SequenceAnimation_tabPage->Controls->Add(this->AddAnimationImage_button);
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationDatAdd_button);
			this->SequenceAnimation_tabPage->Controls->Add(this->DelAnimationImage_button);
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationDataName_textBox);
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationImageUp_button);
			this->SequenceAnimation_tabPage->Controls->Add(this->label2);
			this->SequenceAnimation_tabPage->Controls->Add(this->AnimationImageDown_button);
			this->SequenceAnimation_tabPage->Controls->Add(this->label1);
			this->SequenceAnimation_tabPage->Location = System::Drawing::Point(8, 39);
			this->SequenceAnimation_tabPage->Margin = System::Windows::Forms::Padding(6);
			this->SequenceAnimation_tabPage->Name = L"SequenceAnimation_tabPage";
			this->SequenceAnimation_tabPage->Size = System::Drawing::Size(2702, 559);
			this->SequenceAnimation_tabPage->TabIndex = 2;
			this->SequenceAnimation_tabPage->Text = L"SequenceAnimation";
			// 
			// AnimationTime_numericUpDown
			// 
			this->AnimationTime_numericUpDown->DecimalPlaces = 3;
			this->AnimationTime_numericUpDown->Location = System::Drawing::Point(106, 340);
			this->AnimationTime_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->AnimationTime_numericUpDown->Name = L"AnimationTime_numericUpDown";
			this->AnimationTime_numericUpDown->Size = System::Drawing::Size(240, 31);
			this->AnimationTime_numericUpDown->TabIndex = 60;
			// 
			// AnimationData_listBox
			// 
			this->AnimationData_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AnimationData_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationData_listBox->FormattingEnabled = true;
			this->AnimationData_listBox->ItemHeight = 25;
			this->AnimationData_listBox->Location = System::Drawing::Point(106, 71);
			this->AnimationData_listBox->Margin = System::Windows::Forms::Padding(4);
			this->AnimationData_listBox->Name = L"AnimationData_listBox";
			this->AnimationData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AnimationData_listBox->Size = System::Drawing::Size(264, 254);
			this->AnimationData_listBox->TabIndex = 49;
			this->AnimationData_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::AnimationData_listBox_SelectedIndexChanged);
			// 
			// AnimationDataList_listBox
			// 
			this->AnimationDataList_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AnimationDataList_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDataList_listBox->FormattingEnabled = true;
			this->AnimationDataList_listBox->ItemHeight = 25;
			this->AnimationDataList_listBox->Location = System::Drawing::Point(538, 71);
			this->AnimationDataList_listBox->Margin = System::Windows::Forms::Padding(4);
			this->AnimationDataList_listBox->Name = L"AnimationDataList_listBox";
			this->AnimationDataList_listBox->Size = System::Drawing::Size(300, 254);
			this->AnimationDataList_listBox->TabIndex = 54;
			this->AnimationDataList_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::AnimationData_listBox_KeyUp);
			this->AnimationDataList_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::AnimationData_listBox_MouseDoubleClick);
			// 
			// AnimationDatDel_button
			// 
			this->AnimationDatDel_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationDatDel_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDatDel_button->Location = System::Drawing::Point(412, 248);
			this->AnimationDatDel_button->Margin = System::Windows::Forms::Padding(4);
			this->AnimationDatDel_button->Name = L"AnimationDatDel_button";
			this->AnimationDatDel_button->Size = System::Drawing::Size(66, 42);
			this->AnimationDatDel_button->TabIndex = 59;
			this->AnimationDatDel_button->Text = L"Del";
			this->AnimationDatDel_button->UseVisualStyleBackColor = false;
			this->AnimationDatDel_button->Click += gcnew System::EventHandler(this, &cPIEditor::AnimationDatAdd_button_Click);
			// 
			// AddAnimationImage_button
			// 
			this->AddAnimationImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddAnimationImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddAnimationImage_button->Location = System::Drawing::Point(26, 33);
			this->AddAnimationImage_button->Margin = System::Windows::Forms::Padding(4);
			this->AddAnimationImage_button->Name = L"AddAnimationImage_button";
			this->AddAnimationImage_button->Size = System::Drawing::Size(66, 52);
			this->AddAnimationImage_button->TabIndex = 50;
			this->AddAnimationImage_button->Text = L"Add";
			this->AddAnimationImage_button->UseVisualStyleBackColor = false;
			this->AddAnimationImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::AddAnimationImage_button_Click);
			// 
			// AnimationDatAdd_button
			// 
			this->AnimationDatAdd_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationDatAdd_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDatAdd_button->Location = System::Drawing::Point(412, 140);
			this->AnimationDatAdd_button->Margin = System::Windows::Forms::Padding(4);
			this->AnimationDatAdd_button->Name = L"AnimationDatAdd_button";
			this->AnimationDatAdd_button->Size = System::Drawing::Size(66, 52);
			this->AnimationDatAdd_button->TabIndex = 58;
			this->AnimationDatAdd_button->Text = L"Add";
			this->AnimationDatAdd_button->UseVisualStyleBackColor = false;
			this->AnimationDatAdd_button->Click += gcnew System::EventHandler(this, &cPIEditor::AnimationDatAdd_button_Click);
			// 
			// DelAnimationImage_button
			// 
			this->DelAnimationImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelAnimationImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelAnimationImage_button->Location = System::Drawing::Point(26, 102);
			this->DelAnimationImage_button->Margin = System::Windows::Forms::Padding(4);
			this->DelAnimationImage_button->Name = L"DelAnimationImage_button";
			this->DelAnimationImage_button->Size = System::Drawing::Size(66, 42);
			this->DelAnimationImage_button->TabIndex = 51;
			this->DelAnimationImage_button->Text = L"Del";
			this->DelAnimationImage_button->UseVisualStyleBackColor = false;
			this->DelAnimationImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::AddAnimationImage_button_Click);
			// 
			// AnimationDataName_textBox
			// 
			this->AnimationDataName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationDataName_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDataName_textBox->Location = System::Drawing::Point(380, 77);
			this->AnimationDataName_textBox->Margin = System::Windows::Forms::Padding(4);
			this->AnimationDataName_textBox->Name = L"AnimationDataName_textBox";
			this->AnimationDataName_textBox->Size = System::Drawing::Size(148, 31);
			this->AnimationDataName_textBox->TabIndex = 57;
			// 
			// AnimationImageUp_button
			// 
			this->AnimationImageUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationImageUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationImageUp_button->Location = System::Drawing::Point(26, 162);
			this->AnimationImageUp_button->Margin = System::Windows::Forms::Padding(6);
			this->AnimationImageUp_button->Name = L"AnimationImageUp_button";
			this->AnimationImageUp_button->Size = System::Drawing::Size(64, 75);
			this->AnimationImageUp_button->TabIndex = 52;
			this->AnimationImageUp_button->Text = L"^\r\n^";
			this->AnimationImageUp_button->UseVisualStyleBackColor = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label2->Location = System::Drawing::Point(534, 40);
			this->label2->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(180, 25);
			this->label2->TabIndex = 56;
			this->label2->Text = L"AnimationData:ist";
			// 
			// AnimationImageDown_button
			// 
			this->AnimationImageDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationImageDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationImageDown_button->Location = System::Drawing::Point(26, 248);
			this->AnimationImageDown_button->Margin = System::Windows::Forms::Padding(6);
			this->AnimationImageDown_button->Name = L"AnimationImageDown_button";
			this->AnimationImageDown_button->Size = System::Drawing::Size(64, 71);
			this->AnimationImageDown_button->TabIndex = 53;
			this->AnimationImageDown_button->Text = L"v\r\nv";
			this->AnimationImageDown_button->UseVisualStyleBackColor = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label1->Location = System::Drawing::Point(102, 40);
			this->label1->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(152, 25);
			this->label1->TabIndex = 55;
			this->label1->Text = L"AnimationData";
			// 
			// ImageTriangulator_tabPage
			// 
			this->ImageTriangulator_tabPage->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ImageTriangulator_tabPage->Controls->Add(this->MorphingCenterOffset_groupBox);
			this->ImageTriangulator_tabPage->Controls->Add(this->InsertMorphingTime_button);
			this->ImageTriangulator_tabPage->Controls->Add(this->RearrangeMorphingTime_button);
			this->ImageTriangulator_tabPage->Controls->Add(this->ChangeMorphingTime_button);
			this->ImageTriangulator_tabPage->Controls->Add(this->MorphingAddKeyTimeUsePreKeyData_checkBox);
			this->ImageTriangulator_tabPage->Controls->Add(this->EditAnimation_checkBox);
			this->ImageTriangulator_tabPage->Controls->Add(this->flowLayoutPanel1);
			this->ImageTriangulator_tabPage->Controls->Add(this->Morphing_groupBox);
			this->ImageTriangulator_tabPage->Controls->Add(this->label14);
			this->ImageTriangulator_tabPage->Controls->Add(this->AnimationTime_label);
			this->ImageTriangulator_tabPage->Controls->Add(this->RearrangeMorphingAnimationTime_numericUpDown);
			this->ImageTriangulator_tabPage->Controls->Add(this->DeleteTime_button);
			this->ImageTriangulator_tabPage->Controls->Add(this->AddTime_button);
			this->ImageTriangulator_tabPage->Controls->Add(this->MorphingAnimationTime_numericUpDown);
			this->ImageTriangulator_tabPage->Controls->Add(this->MorphingAnimationTime_listBox);
			this->ImageTriangulator_tabPage->Controls->Add(this->ImageTriangulator_textBox);
			this->ImageTriangulator_tabPage->Location = System::Drawing::Point(8, 39);
			this->ImageTriangulator_tabPage->Margin = System::Windows::Forms::Padding(6);
			this->ImageTriangulator_tabPage->Name = L"ImageTriangulator_tabPage";
			this->ImageTriangulator_tabPage->Padding = System::Windows::Forms::Padding(6);
			this->ImageTriangulator_tabPage->Size = System::Drawing::Size(2702, 559);
			this->ImageTriangulator_tabPage->TabIndex = 1;
			this->ImageTriangulator_tabPage->Text = L"ImageTriangulator";
			// 
			// MorphingCenterOffset_groupBox
			// 
			this->MorphingCenterOffset_groupBox->Controls->Add(this->OffsetCenter_button);
			this->MorphingCenterOffset_groupBox->Controls->Add(this->CenterOffsetY_label);
			this->MorphingCenterOffset_groupBox->Controls->Add(this->CenterOffsetPosY_numericUpDown);
			this->MorphingCenterOffset_groupBox->Controls->Add(this->CenterOffsetPosX_label);
			this->MorphingCenterOffset_groupBox->Controls->Add(this->CenterOffsetPosX_numericUpDown);
			this->MorphingCenterOffset_groupBox->Location = System::Drawing::Point(1584, 267);
			this->MorphingCenterOffset_groupBox->Margin = System::Windows::Forms::Padding(6);
			this->MorphingCenterOffset_groupBox->Name = L"MorphingCenterOffset_groupBox";
			this->MorphingCenterOffset_groupBox->Padding = System::Windows::Forms::Padding(6);
			this->MorphingCenterOffset_groupBox->Size = System::Drawing::Size(484, 192);
			this->MorphingCenterOffset_groupBox->TabIndex = 113;
			this->MorphingCenterOffset_groupBox->TabStop = false;
			this->MorphingCenterOffset_groupBox->Text = L"MorphingCenterOffset_";
			// 
			// OffsetCenter_button
			// 
			this->OffsetCenter_button->Location = System::Drawing::Point(242, 44);
			this->OffsetCenter_button->Margin = System::Windows::Forms::Padding(6);
			this->OffsetCenter_button->Name = L"OffsetCenter_button";
			this->OffsetCenter_button->Size = System::Drawing::Size(196, 90);
			this->OffsetCenter_button->TabIndex = 109;
			this->OffsetCenter_button->Text = L"Center";
			this->OffsetCenter_button->UseVisualStyleBackColor = true;
			this->OffsetCenter_button->Click += gcnew System::EventHandler(this, &cPIEditor::OffsetCenter_button_Click);
			// 
			// CenterOffsetY_label
			// 
			this->CenterOffsetY_label->AutoSize = true;
			this->CenterOffsetY_label->Location = System::Drawing::Point(12, 108);
			this->CenterOffsetY_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->CenterOffsetY_label->Name = L"CenterOffsetY_label";
			this->CenterOffsetY_label->Size = System::Drawing::Size(27, 25);
			this->CenterOffsetY_label->TabIndex = 108;
			this->CenterOffsetY_label->Text = L"Y";
			// 
			// CenterOffsetPosY_numericUpDown
			// 
			this->CenterOffsetPosY_numericUpDown->Location = System::Drawing::Point(48, 94);
			this->CenterOffsetPosY_numericUpDown->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->CenterOffsetPosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 99999, 0, 0, 0 });
			this->CenterOffsetPosY_numericUpDown->Name = L"CenterOffsetPosY_numericUpDown";
			this->CenterOffsetPosY_numericUpDown->Size = System::Drawing::Size(184, 31);
			this->CenterOffsetPosY_numericUpDown->TabIndex = 107;
			this->CenterOffsetPosY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::CenterOffsetPosX_numericUpDown_ValueChanged);
			// 
			// CenterOffsetPosX_label
			// 
			this->CenterOffsetPosX_label->AutoSize = true;
			this->CenterOffsetPosX_label->Location = System::Drawing::Point(12, 37);
			this->CenterOffsetPosX_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->CenterOffsetPosX_label->Name = L"CenterOffsetPosX_label";
			this->CenterOffsetPosX_label->Size = System::Drawing::Size(26, 25);
			this->CenterOffsetPosX_label->TabIndex = 106;
			this->CenterOffsetPosX_label->Text = L"X";
			// 
			// CenterOffsetPosX_numericUpDown
			// 
			this->CenterOffsetPosX_numericUpDown->Location = System::Drawing::Point(48, 33);
			this->CenterOffsetPosX_numericUpDown->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->CenterOffsetPosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 99999, 0, 0, 0 });
			this->CenterOffsetPosX_numericUpDown->Name = L"CenterOffsetPosX_numericUpDown";
			this->CenterOffsetPosX_numericUpDown->Size = System::Drawing::Size(184, 31);
			this->CenterOffsetPosX_numericUpDown->TabIndex = 105;
			this->CenterOffsetPosX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::CenterOffsetPosX_numericUpDown_ValueChanged);
			// 
			// InsertMorphingTime_button
			// 
			this->InsertMorphingTime_button->Location = System::Drawing::Point(1328, 360);
			this->InsertMorphingTime_button->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->InsertMorphingTime_button->Name = L"InsertMorphingTime_button";
			this->InsertMorphingTime_button->Size = System::Drawing::Size(212, 56);
			this->InsertMorphingTime_button->TabIndex = 112;
			this->InsertMorphingTime_button->Text = L"Insert Time";
			this->InsertMorphingTime_button->UseVisualStyleBackColor = true;
			this->InsertMorphingTime_button->Click += gcnew System::EventHandler(this, &cPIEditor::InsertMorphingTime_button_Click);
			// 
			// RearrangeMorphingTime_button
			// 
			this->RearrangeMorphingTime_button->Location = System::Drawing::Point(1584, 202);
			this->RearrangeMorphingTime_button->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->RearrangeMorphingTime_button->Name = L"RearrangeMorphingTime_button";
			this->RearrangeMorphingTime_button->Size = System::Drawing::Size(212, 56);
			this->RearrangeMorphingTime_button->TabIndex = 111;
			this->RearrangeMorphingTime_button->Text = L"RearrangeMorphingTime";
			this->RearrangeMorphingTime_button->UseVisualStyleBackColor = true;
			this->RearrangeMorphingTime_button->Click += gcnew System::EventHandler(this, &cPIEditor::RearrangeMorphingTime_button_Click);
			// 
			// ChangeMorphingTime_button
			// 
			this->ChangeMorphingTime_button->Location = System::Drawing::Point(1326, 298);
			this->ChangeMorphingTime_button->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->ChangeMorphingTime_button->Name = L"ChangeMorphingTime_button";
			this->ChangeMorphingTime_button->Size = System::Drawing::Size(212, 56);
			this->ChangeMorphingTime_button->TabIndex = 110;
			this->ChangeMorphingTime_button->Text = L"Change Time";
			this->ChangeMorphingTime_button->UseVisualStyleBackColor = true;
			this->ChangeMorphingTime_button->Click += gcnew System::EventHandler(this, &cPIEditor::ChangeMorphingTime_button_Click);
			// 
			// MorphingAddKeyTimeUsePreKeyData_checkBox
			// 
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->AutoSize = true;
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->Location = System::Drawing::Point(1326, 67);
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->Name = L"MorphingAddKeyTimeUsePreKeyData_checkBox";
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->Size = System::Drawing::Size(197, 29);
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->TabIndex = 109;
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->Text = L"UsePreKeyData";
			this->MorphingAddKeyTimeUsePreKeyData_checkBox->UseVisualStyleBackColor = true;
			// 
			// EditAnimation_checkBox
			// 
			this->EditAnimation_checkBox->AutoSize = true;
			this->EditAnimation_checkBox->Location = System::Drawing::Point(12, 433);
			this->EditAnimation_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->EditAnimation_checkBox->Name = L"EditAnimation_checkBox";
			this->EditAnimation_checkBox->Size = System::Drawing::Size(176, 29);
			this->EditAnimation_checkBox->TabIndex = 106;
			this->EditAnimation_checkBox->Text = L"EditAnimation";
			this->EditAnimation_checkBox->UseVisualStyleBackColor = true;
			this->EditAnimation_checkBox->CheckedChanged += gcnew System::EventHandler(this, &cPIEditor::EditAnimation_checkBox_CheckedChanged);
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->PlayMorphingAnimation_checkBox);
			this->flowLayoutPanel1->Controls->Add(this->MorphintTime_label);
			this->flowLayoutPanel1->Controls->Add(this->MorphingAnimation_trackBar);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->flowLayoutPanel1->Location = System::Drawing::Point(6, 507);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(6);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(2690, 46);
			this->flowLayoutPanel1->TabIndex = 108;
			// 
			// PlayMorphingAnimation_checkBox
			// 
			this->PlayMorphingAnimation_checkBox->AutoSize = true;
			this->PlayMorphingAnimation_checkBox->Enabled = false;
			this->PlayMorphingAnimation_checkBox->Location = System::Drawing::Point(6, 6);
			this->PlayMorphingAnimation_checkBox->Margin = System::Windows::Forms::Padding(6);
			this->PlayMorphingAnimation_checkBox->Name = L"PlayMorphingAnimation_checkBox";
			this->PlayMorphingAnimation_checkBox->Size = System::Drawing::Size(86, 29);
			this->PlayMorphingAnimation_checkBox->TabIndex = 107;
			this->PlayMorphingAnimation_checkBox->Text = L"Play";
			this->PlayMorphingAnimation_checkBox->UseVisualStyleBackColor = true;
			// 
			// MorphintTime_label
			// 
			this->MorphintTime_label->AutoSize = true;
			this->MorphintTime_label->Location = System::Drawing::Point(104, 0);
			this->MorphintTime_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->MorphintTime_label->Name = L"MorphintTime_label";
			this->MorphintTime_label->Size = System::Drawing::Size(24, 25);
			this->MorphintTime_label->TabIndex = 97;
			this->MorphintTime_label->Text = L"0";
			// 
			// MorphingAnimation_trackBar
			// 
			this->MorphingAnimation_trackBar->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MorphingAnimation_trackBar->Location = System::Drawing::Point(138, 2);
			this->MorphingAnimation_trackBar->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->MorphingAnimation_trackBar->Name = L"MorphingAnimation_trackBar";
			this->MorphingAnimation_trackBar->Size = System::Drawing::Size(2408, 37);
			this->MorphingAnimation_trackBar->TabIndex = 97;
			this->MorphingAnimation_trackBar->Scroll += gcnew System::EventHandler(this, &cPIEditor::MorphingAnimation_trackBar_Scroll);
			this->MorphingAnimation_trackBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MorphingAnimation_trackBar_MouseDown);
			// 
			// Morphing_groupBox
			// 
			this->Morphing_groupBox->Controls->Add(this->label13);
			this->Morphing_groupBox->Controls->Add(this->TriangulatorMouseBehavior_comboBox);
			this->Morphing_groupBox->Controls->Add(this->ImageTriangulatorLOD_label);
			this->Morphing_groupBox->Controls->Add(this->ImageTriangulatorLOD_numericUpDown);
			this->Morphing_groupBox->Controls->Add(this->LODToPoints_button);
			this->Morphing_groupBox->Location = System::Drawing::Point(16, 67);
			this->Morphing_groupBox->Margin = System::Windows::Forms::Padding(6);
			this->Morphing_groupBox->Name = L"Morphing_groupBox";
			this->Morphing_groupBox->Padding = System::Windows::Forms::Padding(6);
			this->Morphing_groupBox->Size = System::Drawing::Size(792, 227);
			this->Morphing_groupBox->TabIndex = 107;
			this->Morphing_groupBox->TabStop = false;
			this->Morphing_groupBox->Text = L"MorphingLOD";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(12, 48);
			this->label13->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(162, 25);
			this->label13->TabIndex = 89;
			this->label13->Text = L"MouseBehavior";
			// 
			// TriangulatorMouseBehavior_comboBox
			// 
			this->TriangulatorMouseBehavior_comboBox->FormattingEnabled = true;
			this->TriangulatorMouseBehavior_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Add", L"Move", L"Delete" });
			this->TriangulatorMouseBehavior_comboBox->Location = System::Drawing::Point(280, 48);
			this->TriangulatorMouseBehavior_comboBox->Margin = System::Windows::Forms::Padding(6);
			this->TriangulatorMouseBehavior_comboBox->Name = L"TriangulatorMouseBehavior_comboBox";
			this->TriangulatorMouseBehavior_comboBox->Size = System::Drawing::Size(238, 33);
			this->TriangulatorMouseBehavior_comboBox->TabIndex = 88;
			this->TriangulatorMouseBehavior_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::TriangulatorMouseBehavior_comboBox_SelectedIndexChanged);
			// 
			// ImageTriangulatorLOD_label
			// 
			this->ImageTriangulatorLOD_label->AutoSize = true;
			this->ImageTriangulatorLOD_label->Location = System::Drawing::Point(12, 162);
			this->ImageTriangulatorLOD_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->ImageTriangulatorLOD_label->Name = L"ImageTriangulatorLOD_label";
			this->ImageTriangulatorLOD_label->Size = System::Drawing::Size(228, 25);
			this->ImageTriangulatorLOD_label->TabIndex = 90;
			this->ImageTriangulatorLOD_label->Text = L"ImageTriangulatorLOD";
			// 
			// ImageTriangulatorLOD_numericUpDown
			// 
			this->ImageTriangulatorLOD_numericUpDown->Location = System::Drawing::Point(280, 160);
			this->ImageTriangulatorLOD_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->ImageTriangulatorLOD_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->ImageTriangulatorLOD_numericUpDown->Name = L"ImageTriangulatorLOD_numericUpDown";
			this->ImageTriangulatorLOD_numericUpDown->Size = System::Drawing::Size(240, 31);
			this->ImageTriangulatorLOD_numericUpDown->TabIndex = 91;
			this->ImageTriangulatorLOD_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->ImageTriangulatorLOD_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImageTriangulatorLOD_numericUpDown_ValueChanged);
			// 
			// LODToPoints_button
			// 
			this->LODToPoints_button->Location = System::Drawing::Point(562, 48);
			this->LODToPoints_button->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->LODToPoints_button->Name = L"LODToPoints_button";
			this->LODToPoints_button->Size = System::Drawing::Size(212, 144);
			this->LODToPoints_button->TabIndex = 96;
			this->LODToPoints_button->Text = L"LODToPoints";
			this->LODToPoints_button->UseVisualStyleBackColor = true;
			this->LODToPoints_button->Click += gcnew System::EventHandler(this, &cPIEditor::LODToPoints_button_Click);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(1590, 104);
			this->label14->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(255, 25);
			this->label14->TabIndex = 104;
			this->label14->Text = L"RearrangeAnimationTime";
			// 
			// AnimationTime_label
			// 
			this->AnimationTime_label->AutoSize = true;
			this->AnimationTime_label->Location = System::Drawing::Point(1322, 108);
			this->AnimationTime_label->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->AnimationTime_label->Name = L"AnimationTime_label";
			this->AnimationTime_label->Size = System::Drawing::Size(154, 25);
			this->AnimationTime_label->TabIndex = 103;
			this->AnimationTime_label->Text = L"AnimationTime";
			// 
			// RearrangeMorphingAnimationTime_numericUpDown
			// 
			this->RearrangeMorphingAnimationTime_numericUpDown->DecimalPlaces = 3;
			this->RearrangeMorphingAnimationTime_numericUpDown->Location = System::Drawing::Point(1596, 135);
			this->RearrangeMorphingAnimationTime_numericUpDown->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->RearrangeMorphingAnimationTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
				99999,
					0, 0, 0
			});
			this->RearrangeMorphingAnimationTime_numericUpDown->Name = L"RearrangeMorphingAnimationTime_numericUpDown";
			this->RearrangeMorphingAnimationTime_numericUpDown->Size = System::Drawing::Size(184, 31);
			this->RearrangeMorphingAnimationTime_numericUpDown->TabIndex = 102;
			// 
			// DeleteTime_button
			// 
			this->DeleteTime_button->Location = System::Drawing::Point(1326, 421);
			this->DeleteTime_button->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->DeleteTime_button->Name = L"DeleteTime_button";
			this->DeleteTime_button->Size = System::Drawing::Size(212, 56);
			this->DeleteTime_button->TabIndex = 101;
			this->DeleteTime_button->Text = L"DeleteTime";
			this->DeleteTime_button->UseVisualStyleBackColor = true;
			this->DeleteTime_button->Click += gcnew System::EventHandler(this, &cPIEditor::DeleteTime_button_Click);
			// 
			// AddTime_button
			// 
			this->AddTime_button->Location = System::Drawing::Point(1326, 217);
			this->AddTime_button->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->AddTime_button->Name = L"AddTime_button";
			this->AddTime_button->Size = System::Drawing::Size(212, 56);
			this->AddTime_button->TabIndex = 100;
			this->AddTime_button->Text = L"AddTime";
			this->AddTime_button->UseVisualStyleBackColor = true;
			this->AddTime_button->Click += gcnew System::EventHandler(this, &cPIEditor::AddTime_button_Click);
			// 
			// MorphingAnimationTime_numericUpDown
			// 
			this->MorphingAnimationTime_numericUpDown->DecimalPlaces = 3;
			this->MorphingAnimationTime_numericUpDown->Location = System::Drawing::Point(1328, 135);
			this->MorphingAnimationTime_numericUpDown->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->MorphingAnimationTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 99999, 0, 0, 0 });
			this->MorphingAnimationTime_numericUpDown->Name = L"MorphingAnimationTime_numericUpDown";
			this->MorphingAnimationTime_numericUpDown->Size = System::Drawing::Size(184, 31);
			this->MorphingAnimationTime_numericUpDown->TabIndex = 99;
			// 
			// MorphingAnimationTime_listBox
			// 
			this->MorphingAnimationTime_listBox->FormattingEnabled = true;
			this->MorphingAnimationTime_listBox->ItemHeight = 25;
			this->MorphingAnimationTime_listBox->Location = System::Drawing::Point(932, 100);
			this->MorphingAnimationTime_listBox->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->MorphingAnimationTime_listBox->Name = L"MorphingAnimationTime_listBox";
			this->MorphingAnimationTime_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->MorphingAnimationTime_listBox->Size = System::Drawing::Size(320, 354);
			this->MorphingAnimationTime_listBox->TabIndex = 98;
			this->MorphingAnimationTime_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::MorphingAnimationTime_listBox_SelectedIndexChanged);
			// 
			// ImageTriangulator_textBox
			// 
			this->ImageTriangulator_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageTriangulator_textBox->Dock = System::Windows::Forms::DockStyle::Top;
			this->ImageTriangulator_textBox->Enabled = false;
			this->ImageTriangulator_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->ImageTriangulator_textBox->Location = System::Drawing::Point(6, 6);
			this->ImageTriangulator_textBox->Margin = System::Windows::Forms::Padding(6);
			this->ImageTriangulator_textBox->MinimumSize = System::Drawing::Size(4, 56);
			this->ImageTriangulator_textBox->Multiline = true;
			this->ImageTriangulator_textBox->Name = L"ImageTriangulator_textBox";
			this->ImageTriangulator_textBox->Size = System::Drawing::Size(2690, 56);
			this->ImageTriangulator_textBox->TabIndex = 95;
			// 
			// DividImageTool_tabPage
			// 
			this->DividImageTool_tabPage->BackColor = System::Drawing::Color::DarkGray;
			this->DividImageTool_tabPage->Controls->Add(this->label18);
			this->DividImageTool_tabPage->Controls->Add(this->DivideToolColumn_numericUpDown);
			this->DividImageTool_tabPage->Controls->Add(this->label19);
			this->DividImageTool_tabPage->Controls->Add(this->DivideToolRow_numericUpDown);
			this->DividImageTool_tabPage->Controls->Add(this->label17);
			this->DividImageTool_tabPage->Controls->Add(this->DividImagePrefixName_textBox);
			this->DividImageTool_tabPage->Controls->Add(this->DividImageTool_button);
			this->DividImageTool_tabPage->Location = System::Drawing::Point(8, 39);
			this->DividImageTool_tabPage->Name = L"DividImageTool_tabPage";
			this->DividImageTool_tabPage->Size = System::Drawing::Size(2702, 559);
			this->DividImageTool_tabPage->TabIndex = 3;
			this->DividImageTool_tabPage->Text = L"DividImageTool";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label18->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label18->Location = System::Drawing::Point(14, 108);
			this->label18->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(112, 25);
			this->label18->TabIndex = 82;
			this->label18->Text = L"Column/列";
			// 
			// DivideToolColumn_numericUpDown
			// 
			this->DivideToolColumn_numericUpDown->Location = System::Drawing::Point(16, 139);
			this->DivideToolColumn_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->DivideToolColumn_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->DivideToolColumn_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->DivideToolColumn_numericUpDown->Name = L"DivideToolColumn_numericUpDown";
			this->DivideToolColumn_numericUpDown->Size = System::Drawing::Size(122, 31);
			this->DivideToolColumn_numericUpDown->TabIndex = 81;
			this->DivideToolColumn_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label19->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label19->Location = System::Drawing::Point(10, 26);
			this->label19->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(81, 25);
			this->label19->TabIndex = 80;
			this->label19->Text = L"Row/行";
			// 
			// DivideToolRow_numericUpDown
			// 
			this->DivideToolRow_numericUpDown->Location = System::Drawing::Point(20, 58);
			this->DivideToolRow_numericUpDown->Margin = System::Windows::Forms::Padding(6);
			this->DivideToolRow_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->DivideToolRow_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->DivideToolRow_numericUpDown->Name = L"DivideToolRow_numericUpDown";
			this->DivideToolRow_numericUpDown->Size = System::Drawing::Size(122, 31);
			this->DivideToolRow_numericUpDown->TabIndex = 79;
			this->DivideToolRow_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label17->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label17->Location = System::Drawing::Point(7, 182);
			this->label17->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(215, 25);
			this->label17->TabIndex = 78;
			this->label17->Text = L"ImagePrefix/預設名稱";
			// 
			// DividImagePrefixName_textBox
			// 
			this->DividImagePrefixName_textBox->Location = System::Drawing::Point(11, 211);
			this->DividImagePrefixName_textBox->Name = L"DividImagePrefixName_textBox";
			this->DividImagePrefixName_textBox->Size = System::Drawing::Size(185, 31);
			this->DividImagePrefixName_textBox->TabIndex = 77;
			// 
			// DividImageTool_button
			// 
			this->DividImageTool_button->Location = System::Drawing::Point(15, 266);
			this->DividImageTool_button->Name = L"DividImageTool_button";
			this->DividImageTool_button->Size = System::Drawing::Size(144, 76);
			this->DividImageTool_button->TabIndex = 76;
			this->DividImageTool_button->Text = L"Do divison";
			this->DividImageTool_button->UseVisualStyleBackColor = true;
			this->DividImageTool_button->Click += gcnew System::EventHandler(this, &cPIEditor::DividImageTool_button_Click);
			// 
			// AtlasFileConvert_tabPage
			// 
			this->AtlasFileConvert_tabPage->Controls->Add(this->AltasScale_label);
			this->AtlasFileConvert_tabPage->Controls->Add(this->ASltasScale_domainUpDown);
			this->AtlasFileConvert_tabPage->Controls->Add(this->ReplaceOriginalFile_checkBox);
			this->AtlasFileConvert_tabPage->Controls->Add(this->ConvertAltas_button);
			this->AtlasFileConvert_tabPage->Controls->Add(this->AltasFiles_listBox);
			this->AtlasFileConvert_tabPage->Controls->Add(this->SelectDirecotry_button);
			this->AtlasFileConvert_tabPage->Location = System::Drawing::Point(8, 39);
			this->AtlasFileConvert_tabPage->Name = L"AtlasFileConvert_tabPage";
			this->AtlasFileConvert_tabPage->Size = System::Drawing::Size(2702, 559);
			this->AtlasFileConvert_tabPage->TabIndex = 4;
			this->AtlasFileConvert_tabPage->Text = L"AtlasFileConvert";
			this->AtlasFileConvert_tabPage->UseVisualStyleBackColor = true;
			// 
			// AltasScale_label
			// 
			this->AltasScale_label->AutoSize = true;
			this->AltasScale_label->Location = System::Drawing::Point(985, 386);
			this->AltasScale_label->Name = L"AltasScale_label";
			this->AltasScale_label->Size = System::Drawing::Size(162, 25);
			this->AltasScale_label->TabIndex = 5;
			this->AltasScale_label->Text = L"AltasScale/比例";
			// 
			// ASltasScale_domainUpDown
			// 
			this->ASltasScale_domainUpDown->Location = System::Drawing::Point(990, 414);
			this->ASltasScale_domainUpDown->Name = L"ASltasScale_domainUpDown";
			this->ASltasScale_domainUpDown->Size = System::Drawing::Size(201, 31);
			this->ASltasScale_domainUpDown->TabIndex = 4;
			this->ASltasScale_domainUpDown->Text = L"50";
			// 
			// ReplaceOriginalFile_checkBox
			// 
			this->ReplaceOriginalFile_checkBox->AutoSize = true;
			this->ReplaceOriginalFile_checkBox->Location = System::Drawing::Point(990, 470);
			this->ReplaceOriginalFile_checkBox->Name = L"ReplaceOriginalFile_checkBox";
			this->ReplaceOriginalFile_checkBox->Size = System::Drawing::Size(364, 29);
			this->ReplaceOriginalFile_checkBox->TabIndex = 3;
			this->ReplaceOriginalFile_checkBox->Text = L"ReplaceOriginalFile/覆蓋原本檔案";
			this->ReplaceOriginalFile_checkBox->UseVisualStyleBackColor = true;
			// 
			// ConvertAltas_button
			// 
			this->ConvertAltas_button->Location = System::Drawing::Point(990, 193);
			this->ConvertAltas_button->Name = L"ConvertAltas_button";
			this->ConvertAltas_button->Size = System::Drawing::Size(269, 121);
			this->ConvertAltas_button->TabIndex = 2;
			this->ConvertAltas_button->Text = L"Convert/轉換";
			this->ConvertAltas_button->UseVisualStyleBackColor = true;
			this->ConvertAltas_button->Click += gcnew System::EventHandler(this, &cPIEditor::ConvertAltas_button_Click);
			// 
			// AltasFiles_listBox
			// 
			this->AltasFiles_listBox->AllowDrop = true;
			this->AltasFiles_listBox->FormattingEnabled = true;
			this->AltasFiles_listBox->HorizontalScrollbar = true;
			this->AltasFiles_listBox->ItemHeight = 25;
			this->AltasFiles_listBox->Location = System::Drawing::Point(23, 29);
			this->AltasFiles_listBox->Name = L"AltasFiles_listBox";
			this->AltasFiles_listBox->Size = System::Drawing::Size(913, 504);
			this->AltasFiles_listBox->TabIndex = 1;
			this->AltasFiles_listBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &cPIEditor::AltasFiles_listBox_DragDrop);
			this->AltasFiles_listBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &cPIEditor::AltasFiles_listBox_DragEnter);
			// 
			// SelectDirecotry_button
			// 
			this->SelectDirecotry_button->Location = System::Drawing::Point(990, 39);
			this->SelectDirecotry_button->Name = L"SelectDirecotry_button";
			this->SelectDirecotry_button->Size = System::Drawing::Size(269, 121);
			this->SelectDirecotry_button->TabIndex = 0;
			this->SelectDirecotry_button->Text = L"Select Directory/選資料夾";
			this->SelectDirecotry_button->UseVisualStyleBackColor = true;
			this->SelectDirecotry_button->Click += gcnew System::EventHandler(this, &cPIEditor::SelectDirecotry_button_Click);
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(4);
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Resize += gcnew System::EventHandler(this, &cPIEditor::splitContainer2_Panel1_Resize);
			this->splitContainer2->Panel2Collapsed = true;
			this->splitContainer2->Size = System::Drawing::Size(3370, 390);
			this->splitContainer2->SplitterDistance = 356;
			this->splitContainer2->SplitterWidth = 6;
			this->splitContainer2->TabIndex = 0;
			// 
			// cPIEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(3374, 1060);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(6);
			this->Name = L"cPIEditor";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageHeight_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageWidth_numericUpDown))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer3->Panel1->ResumeLayout(false);
			this->splitContainer3->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer3))->EndInit();
			this->splitContainer3->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->ImageAligment_tabPage->ResumeLayout(false);
			this->ImageAligment_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitStartX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitEndY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitEndX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->NewPIUnitStartY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageDistanceY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageDistanceX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImagePosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImagePosX_numericUpDown))->EndInit();
			this->SequenceAnimation_tabPage->ResumeLayout(false);
			this->SequenceAnimation_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->AnimationTime_numericUpDown))->EndInit();
			this->ImageTriangulator_tabPage->ResumeLayout(false);
			this->ImageTriangulator_tabPage->PerformLayout();
			this->MorphingCenterOffset_groupBox->ResumeLayout(false);
			this->MorphingCenterOffset_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CenterOffsetPosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CenterOffsetPosX_numericUpDown))->EndInit();
			this->flowLayoutPanel1->ResumeLayout(false);
			this->flowLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MorphingAnimation_trackBar))->EndInit();
			this->Morphing_groupBox->ResumeLayout(false);
			this->Morphing_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageTriangulatorLOD_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->RearrangeMorphingAnimationTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MorphingAnimationTime_numericUpDown))->EndInit();
			this->DividImageTool_tabPage->ResumeLayout(false);
			this->DividImageTool_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DivideToolColumn_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->DivideToolRow_numericUpDown))->EndInit();
			this->AtlasFileConvert_tabPage->ResumeLayout(false);
			this->AtlasFileConvert_tabPage->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer2))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void AllImage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void DeleteImage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AutoOrderPosition_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AllImage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e);
	//unform
	private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void MyMouseHover(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
	private: System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void MyKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void Test_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ImageWidth_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void InvertPuzzleImage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void TO96PdiPngFile_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void openXMLToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddAnimationImage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AnimationDatAdd_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ImagePosX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AllImage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void AllImage_listBox_KeyUp_1(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void AnimationData_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void AnimationData_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void ProperStripPixel_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void StripAlpha_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void splitContainer2_Panel1_Resize(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddNewPIUnitImage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void NewPIUnitStartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void fileToolStripMenuItem_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void addImagesByFolderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void TriangulatorMouseBehavior_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ImageTriangulatorLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AnimationData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void tabControl1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void generateTriangulatorImagesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MorphingAnimationTime_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddTime_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void DeleteTime_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void EditAnimation_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void LODToPoints_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MorphingAnimation_trackBar_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MorphingAnimation_trackBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void RearrangeMorphingTime_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ChangeMorphingTime_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void InsertMorphingTime_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void CenterOffsetPosX_numericUpDown_ValueChanged(System::Object^ sender, System::EventArgs^ e);
	private: System::Void OffsetCenter_button_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void generateSelectedPIUnitXMLToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void DividImageTool_button_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void SelectDirecotry_button_Click(System::Object^ sender, System::EventArgs^ e)
	{
		auto l_strDirectory = DNCT::SelectDirectory();
		if (l_strDirectory)
		{
			AltasFiles_listBox->Items->Clear();
			System::Collections::ArrayList^ l_pArrayList = gcnew System::Collections::ArrayList();
			GetFilesNameByRecursivelyDirectory(l_strDirectory, l_pArrayList);
			char* l_strAllSupportFormat[] = {
			   ".PNG",".JPG",".atlas"
			};
			for each (String ^ l_strName in l_pArrayList)
			{
				String^ l_strExtensionName = System::IO::Path::GetExtension(l_strName);
				auto l_strFileExtensionName = DNCT::GcStringToChar(l_strExtensionName);
				for (int i = 0; i < 3; ++i)
				{
					if (!_stricmp(l_strAllSupportFormat[i], l_strFileExtensionName.c_str()))
					{
						AltasFiles_listBox->Items->Add(l_strName);
					}
				}
			}
		}
	}
	private: System::Void AltasFiles_listBox_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
	{
		cli::array<String^>^ l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
		for each (String ^ l_str in l_strFileNames)
		{
			AltasFiles_listBox->Items->Add(l_str);
		}
	}
	private: System::Void AltasFiles_listBox_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
	{
		if (e->Data->GetDataPresent(DataFormats::FileDrop))
		{
			e->Effect = DragDropEffects::Copy;
		}
		else
		{
			e->Effect = DragDropEffects::None;
		}
	}

	private: System::Void ConvertAltas_button_Click(System::Object^ sender, System::EventArgs^ e)
	{
		bool l_bReplaceFile = this->ReplaceOriginalFile_checkBox->Checked;
		System::String^l_strDirectory = nullptr;
		if (!l_bReplaceFile)
		{
			l_strDirectory = DNCT::SelectDirectory();
			if (!l_strDirectory)
			{
				return;
			}
		}
		float l_fSacle = System::Int32::Parse(this->ASltasScale_domainUpDown->Text)/100.f;
		for (int i = 0; i < AltasFiles_listBox->Items->Count; ++i)
		{
			auto l_strFileName = AltasFiles_listBox->Items[i]->ToString();
			String^ l_strExtensionName = System::IO::Path::GetExtension(l_strFileName);
			auto l_strFileExtensionName = DNCT::GcStringToChar(l_strExtensionName);
			std::string l_strOutputDirectory;
			std::string l_strTargetFileName = DNCT::GcStringToChar(l_strFileName);
			sAtlasData l_AtlasData;
			if (l_strDirectory)
			{
				l_strOutputDirectory = DNCT::GcStringToChar(l_strDirectory);
			}
			else
			{
				auto l_strTargetDirectory = System::IO::Path::GetDirectoryName(l_strFileName);
				l_strOutputDirectory = DNCT::GcStringToChar(l_strTargetDirectory);
			}
			std::string l_strOutputFileName = l_strOutputDirectory+"/";
			l_strOutputFileName += GetFileNameWithoutFullPath(DNCT::GcStringToChar(l_strFileName),false);
			if (!_stricmp(l_strFileExtensionName.c_str(), ".png"))
			{
				Image^l_pConvertedImage = DNCT::ResizeImage(l_strFileName, l_fSacle);
				if (l_pConvertedImage)
				{
					l_pConvertedImage->Save(gcnew String(l_strOutputFileName.c_str()));
				}
			}
			else
			if (!_stricmp(l_strFileExtensionName.c_str(), ".atlas"))
			{
				sAtlasData l_AtlasData;
				auto l_bRsult = l_AtlasData.DoScaleThenExport(l_strTargetFileName.c_str(), l_strOutputFileName.c_str(), l_fSacle);
				WARNING_MSG("convert "+ l_bRsult?"ok":"failed");
			}
		}
		
	}
};
	static GCFORM::Form^CallForm(System::String^e_strFileName);
//end namespace
}