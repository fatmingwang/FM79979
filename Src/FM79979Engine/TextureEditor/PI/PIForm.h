#pragma once
#include "pack.h"
#include "BinPack.h"
#include "PuzzleImageUnitTriangulator.h"
#include "PIUtility.h"
#include "../../Core/GameplayUT/OpenGL/WindowsOpenGL.h"
cGlyphFontRender*g_pDebugFont = nullptr;
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
			pictureBox2->AutoSize = true;
			pictureBox1->AutoSize = true;
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
			m_bAvoidDataAssignForPIUintChild = false;
			if( e_strFileName && e_strFileName->Length )
				OpenPIFile(e_strFileName);
			ToolStripMenuItem^l_pToolStripMenuItem = (ToolStripMenuItem^)this->menuStrip1->Items[0];
			this->Dock = GCFORM::DockStyle::Fill;
			TriangulatorMouseBehavior_comboBox->SelectedIndex = 0;
			//WARNING_MSG(openXMLToolStripMenuItem->Size.ToString());
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
	private: System::Windows::Forms::Label^  TotalPixel_label;
	private: System::Windows::Forms::CheckBox^  AllShowBoundry_checkBox;
	private: System::Windows::Forms::Button^  ImageObjectDown_button;
	private: System::Windows::Forms::Button^  ImageObjectUP_button;
	private: System::Windows::Forms::CheckBox^  ShowBoundary_checkBox;
	private: System::Windows::Forms::TextBox^  ImageDetail_textBox;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  ImageHeight_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImageWidth_numericUpDown;
	private: System::Windows::Forms::Button^  AutoOrderPosition_button;
	private: System::Windows::Forms::Button^  DeleteImage_button;
	private: System::Windows::Forms::Button^  AddImage_button;
	private: System::Windows::Forms::ListBox^  AllImage_listBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openXMLToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  invertPuzzleImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  generatePowOf2ImageToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::Button^  AnimationImageDown_button;
	private: System::Windows::Forms::Button^  AnimationImageUp_button;
	private: System::Windows::Forms::Button^  DelAnimationImage_button;
	private: System::Windows::Forms::Button^  AddAnimationImage_button;
	private: System::Windows::Forms::ListBox^  AnimationData_listBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListBox^  AnimationDataList_listBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  AnimationDatDel_button;
	private: System::Windows::Forms::Button^  AnimationDatAdd_button;
	private: System::Windows::Forms::TextBox^  AnimationDataName_textBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::ToolStripMenuItem^  generateAllImageWithPowerOfTwoToolStripMenuItem;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  ImagePosX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImagePosY_numericUpDown;
    private: System::Windows::Forms::Button^  ProperStripPixel_button;
    private: System::Windows::Forms::Button^  ToOriginalImage_button;
    private: System::Windows::Forms::Button^  StripAlpha_button;
    private: System::Windows::Forms::CheckBox^  DebugLine_checkBox;
    private: System::Windows::Forms::NumericUpDown^  ImageDistanceX_numericUpDown;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::NumericUpDown^  ImageDistanceY_numericUpDown;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitStartX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitEndY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitEndX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitStartY_numericUpDown;
	private: System::Windows::Forms::Button^  AddNewPIUnitImage_button;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  NewPIUnitName_textBox;
	private: System::Windows::Forms::CheckBox^  ImageSaveAsDDS_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  dockToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  floatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  childToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  floatToolStripMenuItem1;
	private: System::Windows::Forms::CheckBox^  BinaryExport_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  toolToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addImagesByFolderToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  AutoAligment_checkBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  ImageAligment_tabPage;
	private: System::Windows::Forms::TabPage^  ImageTriangulator_tabPage;
	private: System::Windows::Forms::NumericUpDown^  ImageTriangulatorLOD_numericUpDown;
	private: System::Windows::Forms::Label^  ImageTriangulatorLOD_label;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::ComboBox^  TriangulatorMouseBehavior_comboBox;
	private: System::Windows::Forms::TabPage^  SequenceAnimation_tabPage;
	private: GCFORM::TabPage^m_pTabPage;					//for attach from.
	private: GCFORM::TabControl^m_pTabControl;				//to determin is tabpage clicked.
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::CheckBox^  ShowTriangulaotrPoints_checkBox;
	private: System::Windows::Forms::NumericUpDown^  AnimationTime_numericUpDown;
	//my
	private:cPuzzleImageUnitTriangulatorManager*m_pPuzzleImageUnitTriangulatorManager;
	private:cPuzzleImageUnitTriangulator * m_pCurrentSelectedPuzzleImageUnitTriangulator;
	private: GCFORM::Form^	m_pForm;
	public:HWND				m_pParentHandle;
	String^					m_strParentProcessID;
	bool					m_bAvoidDataAssignForPIUintChild;
	HGLRC					m_HGLRCMV;
	HDC						m_HdcMV;
	cGlyphFontRender*m_pDebugFont;
	cImageParser*								m_pImageomposerIRM;
	int											m_iCurrentSelectedObjectIndex;
	cImageIndexOfAnimation*						m_pImageIndexOfAnimation;
	cNamedTypedObjectVector<cImageIndexOfAnimation>*	m_pImageIndexOfAnimationList;
	cOrthogonalCamera*							m_pOrthogonalCamera;
	cOrthogonalCamera*							m_pOrthogonalCameraForTrianhulatorPIUnit;
	UT::sTimeAndFPS*							m_pTimeAndFPS;
	Vector4	*m_pvBGColor;
private: System::Windows::Forms::Label^  MouseControlMode_label;
private: System::Windows::Forms::ComboBox^  MouseControlMode_comboBox;
public:

	private: System::Collections::Hashtable^m_ImageTale;	//key:string,value:System::Drawing::Bitmap.,if m_pImageomposerIRM's child(UIImage) has owner,then m_ImageTale do not has its data
	private: System::Void	SavePuzzleFile(String^e_strFileName,bool e_bBinary);
	private: cPuzzleImage*	OpenPuzzleFile(String^e_strFileName);
	public:  void			OpenPIFile(String^e_strFileName);
	private: bool			ParsePuzzleImage(cPuzzleImage*e_pPuzzleImage,String^e_strFileName);
	private: System::Void	GeneratePowOf2Image(bool e_bPowerOfTwo);
	private: System::Void	MouseCollideForPickUpObject( System::Windows::Forms::MouseEventArgs^  e,System::Windows::Forms::Panel^e_pPanel);
	private: System::Void	AssignAnimationData();
	private: int			GetMouseWheelDelta(System::Windows::Forms::MouseEventArgs^  e);
	private: String^		GetTotalPixelExisted();	//all UIImage pixel size
			 String^		m_strCurrentFileName;
			 cUIImage*	GetNewUIImageByBitMap(System::Drawing::Bitmap^e_pBitMap,const WCHAR*e_strName);
			 void		AttachObjectPosAdjust();
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
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->ImageTriangulatorLOD_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ImageTriangulatorLOD_label = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->TriangulatorMouseBehavior_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->MouseControlMode_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->MouseControlMode_label = (gcnew System::Windows::Forms::Label());
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageTriangulatorLOD_numericUpDown))->BeginInit();
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
			this->TotalPixel_label->Location = System::Drawing::Point(6, 150);
			this->TotalPixel_label->Name = L"TotalPixel_label";
			this->TotalPixel_label->Size = System::Drawing::Size(91, 12);
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
			this->AllShowBoundry_checkBox->Location = System::Drawing::Point(6, 42);
			this->AllShowBoundry_checkBox->Name = L"AllShowBoundry_checkBox";
			this->AllShowBoundry_checkBox->Size = System::Drawing::Size(198, 16);
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
			this->ImageObjectDown_button->Location = System::Drawing::Point(298, 102);
			this->ImageObjectDown_button->Name = L"ImageObjectDown_button";
			this->ImageObjectDown_button->Size = System::Drawing::Size(21, 41);
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
			this->ImageObjectUP_button->Location = System::Drawing::Point(298, 26);
			this->ImageObjectUP_button->Name = L"ImageObjectUP_button";
			this->ImageObjectUP_button->Size = System::Drawing::Size(21, 41);
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
			this->ShowBoundary_checkBox->Location = System::Drawing::Point(6, 17);
			this->ShowBoundary_checkBox->Name = L"ShowBoundary_checkBox";
			this->ShowBoundary_checkBox->Size = System::Drawing::Size(184, 16);
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
			this->ImageDetail_textBox->Location = System::Drawing::Point(3, 172);
			this->ImageDetail_textBox->MinimumSize = System::Drawing::Size(4, 29);
			this->ImageDetail_textBox->Multiline = true;
			this->ImageDetail_textBox->Name = L"ImageDetail_textBox";
			this->ImageDetail_textBox->Size = System::Drawing::Size(1340, 29);
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
			this->label4->Location = System::Drawing::Point(261, 68);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(99, 12);
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
			this->label5->Location = System::Drawing::Point(264, 20);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(97, 12);
			this->label5->TabIndex = 37;
			this->label5->Text = L"Width/產生圖片寬";
			// 
			// ImageHeight_numericUpDown
			// 
			this->ImageHeight_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageHeight_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageHeight_numericUpDown->Location = System::Drawing::Point(264, 83);
			this->ImageHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImageHeight_numericUpDown->Name = L"ImageHeight_numericUpDown";
			this->ImageHeight_numericUpDown->Size = System::Drawing::Size(62, 22);
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
			this->ImageWidth_numericUpDown->Location = System::Drawing::Point(264, 39);
			this->ImageWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImageWidth_numericUpDown->Name = L"ImageWidth_numericUpDown";
			this->ImageWidth_numericUpDown->Size = System::Drawing::Size(62, 22);
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
			this->AutoOrderPosition_button->Location = System::Drawing::Point(264, 110);
			this->AutoOrderPosition_button->Name = L"AutoOrderPosition_button";
			this->AutoOrderPosition_button->Size = System::Drawing::Size(139, 36);
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
			this->DeleteImage_button->Location = System::Drawing::Point(14, 54);
			this->DeleteImage_button->Name = L"DeleteImage_button";
			this->DeleteImage_button->Size = System::Drawing::Size(100, 25);
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
			this->AddImage_button->Location = System::Drawing::Point(14, 23);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(99, 28);
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
			this->AllImage_listBox->ItemHeight = 12;
			this->AllImage_listBox->Location = System::Drawing::Point(118, 13);
			this->AllImage_listBox->MinimumSize = System::Drawing::Size(4, 81);
			this->AllImage_listBox->Name = L"AllImage_listBox";
			this->AllImage_listBox->ScrollAlwaysVisible = true;
			this->AllImage_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllImage_listBox->Size = System::Drawing::Size(174, 148);
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
			this->menuStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Visible;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->fileToolStripMenuItem,
					this->dockToolStripMenuItem, this->floatToolStripMenuItem1, this->toolToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->MdiWindowListItem = this->fileToolStripMenuItem;
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->menuStrip1->Size = System::Drawing::Size(1687, 24);
			this->menuStrip1->Stretch = false;
			this->menuStrip1->TabIndex = 26;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->openXMLToolStripMenuItem,
					this->saveToolStripMenuItem, this->invertPuzzleImageToolStripMenuItem, this->generatePowOf2ImageToolStripMenuItem, this->generateAllImageWithPowerOfTwoToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			this->fileToolStripMenuItem->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::fileToolStripMenuItem_MouseDown);
			// 
			// openXMLToolStripMenuItem
			// 
			this->openXMLToolStripMenuItem->Name = L"openXMLToolStripMenuItem";
			this->openXMLToolStripMenuItem->Size = System::Drawing::Size(292, 22);
			this->openXMLToolStripMenuItem->Text = L"Open";
			this->openXMLToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::openXMLToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(292, 22);
			this->saveToolStripMenuItem->Text = L"GeneratePuzzleImage/產生單張圖片";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::Test_button_Click);
			// 
			// invertPuzzleImageToolStripMenuItem
			// 
			this->invertPuzzleImageToolStripMenuItem->Name = L"invertPuzzleImageToolStripMenuItem";
			this->invertPuzzleImageToolStripMenuItem->Size = System::Drawing::Size(292, 22);
			this->invertPuzzleImageToolStripMenuItem->Text = L"InvertPuzzleImage/將資料反轉成張圖片";
			this->invertPuzzleImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::InvertPuzzleImage_button_Click);
			// 
			// generatePowOf2ImageToolStripMenuItem
			// 
			this->generatePowOf2ImageToolStripMenuItem->Name = L"generatePowOf2ImageToolStripMenuItem";
			this->generatePowOf2ImageToolStripMenuItem->Size = System::Drawing::Size(292, 22);
			this->generatePowOf2ImageToolStripMenuItem->Text = L"Generate All Image/產生所有圖片";
			this->generatePowOf2ImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::button1_Click);
			// 
			// generateAllImageWithPowerOfTwoToolStripMenuItem
			// 
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Name = L"generateAllImageWithPowerOfTwoToolStripMenuItem";
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Size = System::Drawing::Size(292, 22);
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Text = L"Generate All Image With PowerOfTwo";
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::button1_Click);
			// 
			// dockToolStripMenuItem
			// 
			this->dockToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->floatToolStripMenuItem,
					this->childToolStripMenuItem
			});
			this->dockToolStripMenuItem->Name = L"dockToolStripMenuItem";
			this->dockToolStripMenuItem->Size = System::Drawing::Size(46, 20);
			this->dockToolStripMenuItem->Text = L"Dock";
			// 
			// floatToolStripMenuItem
			// 
			this->floatToolStripMenuItem->Name = L"floatToolStripMenuItem";
			this->floatToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->floatToolStripMenuItem->Text = L"Float";
			this->floatToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// childToolStripMenuItem
			// 
			this->childToolStripMenuItem->Name = L"childToolStripMenuItem";
			this->childToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->childToolStripMenuItem->Text = L"Child";
			this->childToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// floatToolStripMenuItem1
			// 
			this->floatToolStripMenuItem1->Name = L"floatToolStripMenuItem1";
			this->floatToolStripMenuItem1->Size = System::Drawing::Size(45, 20);
			this->floatToolStripMenuItem1->Text = L"Float";
			this->floatToolStripMenuItem1->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// toolToolStripMenuItem
			// 
			this->toolToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->addImagesByFolderToolStripMenuItem });
			this->toolToolStripMenuItem->Name = L"toolToolStripMenuItem";
			this->toolToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->toolToolStripMenuItem->Text = L"Tool";
			// 
			// addImagesByFolderToolStripMenuItem
			// 
			this->addImagesByFolderToolStripMenuItem->Name = L"addImagesByFolderToolStripMenuItem";
			this->addImagesByFolderToolStripMenuItem->Size = System::Drawing::Size(189, 22);
			this->addImagesByFolderToolStripMenuItem->Text = L"Add Images By Folder";
			this->addImagesByFolderToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::addImagesByFolderToolStripMenuItem_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
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
			this->splitContainer1->Size = System::Drawing::Size(1687, 948);
			this->splitContainer1->SplitterDistance = 234;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 49;
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
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
			this->splitContainer3->Size = System::Drawing::Size(1683, 230);
			this->splitContainer3->SplitterDistance = 325;
			this->splitContainer3->TabIndex = 67;
			// 
			// ToOriginalImage_button
			// 
			this->ToOriginalImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ToOriginalImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ToOriginalImage_button->Location = System::Drawing::Point(14, 110);
			this->ToOriginalImage_button->Name = L"ToOriginalImage_button";
			this->ToOriginalImage_button->Size = System::Drawing::Size(100, 25);
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
			this->ProperStripPixel_button->Location = System::Drawing::Point(14, 138);
			this->ProperStripPixel_button->Name = L"ProperStripPixel_button";
			this->ProperStripPixel_button->Size = System::Drawing::Size(100, 25);
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
			this->StripAlpha_button->Location = System::Drawing::Point(14, 82);
			this->StripAlpha_button->Name = L"StripAlpha_button";
			this->StripAlpha_button->Size = System::Drawing::Size(100, 25);
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
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1354, 230);
			this->tabControl1->TabIndex = 0;
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
			this->ImageAligment_tabPage->Location = System::Drawing::Point(4, 22);
			this->ImageAligment_tabPage->Name = L"ImageAligment_tabPage";
			this->ImageAligment_tabPage->Padding = System::Windows::Forms::Padding(3);
			this->ImageAligment_tabPage->Size = System::Drawing::Size(1346, 204);
			this->ImageAligment_tabPage->TabIndex = 0;
			this->ImageAligment_tabPage->Text = L"ImageAligment";
			// 
			// ShowTriangulaotrPoints_checkBox
			// 
			this->ShowTriangulaotrPoints_checkBox->AutoSize = true;
			this->ShowTriangulaotrPoints_checkBox->Location = System::Drawing::Point(106, 132);
			this->ShowTriangulaotrPoints_checkBox->Name = L"ShowTriangulaotrPoints_checkBox";
			this->ShowTriangulaotrPoints_checkBox->Size = System::Drawing::Size(136, 16);
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
			this->label8->Location = System::Drawing::Point(582, 6);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(204, 12);
			this->label8->TabIndex = 72;
			this->label8->Text = L"NewPIUnitStartX/新拼圖單元開始位置X";
			// 
			// AutoAligment_checkBox
			// 
			this->AutoAligment_checkBox->AutoSize = true;
			this->AutoAligment_checkBox->Location = System::Drawing::Point(6, 66);
			this->AutoAligment_checkBox->Name = L"AutoAligment_checkBox";
			this->AutoAligment_checkBox->Size = System::Drawing::Size(141, 16);
			this->AutoAligment_checkBox->TabIndex = 85;
			this->AutoAligment_checkBox->Text = L"AutoAligment/自動排列";
			this->AutoAligment_checkBox->UseVisualStyleBackColor = true;
			// 
			// BinaryExport_checkBox
			// 
			this->BinaryExport_checkBox->AutoSize = true;
			this->BinaryExport_checkBox->Location = System::Drawing::Point(6, 85);
			this->BinaryExport_checkBox->Name = L"BinaryExport_checkBox";
			this->BinaryExport_checkBox->Size = System::Drawing::Size(88, 16);
			this->BinaryExport_checkBox->TabIndex = 84;
			this->BinaryExport_checkBox->Text = L"BinaryExport";
			this->BinaryExport_checkBox->UseVisualStyleBackColor = true;
			// 
			// ImageSaveAsDDS_checkBox
			// 
			this->ImageSaveAsDDS_checkBox->AutoSize = true;
			this->ImageSaveAsDDS_checkBox->Location = System::Drawing::Point(6, 108);
			this->ImageSaveAsDDS_checkBox->Name = L"ImageSaveAsDDS_checkBox";
			this->ImageSaveAsDDS_checkBox->Size = System::Drawing::Size(109, 16);
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
			this->label12->Location = System::Drawing::Point(795, 6);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(170, 12);
			this->label12->TabIndex = 82;
			this->label12->Text = L"NewPIUnitName/新圖片單元名稱";
			// 
			// NewPIUnitName_textBox
			// 
			this->NewPIUnitName_textBox->Location = System::Drawing::Point(837, 22);
			this->NewPIUnitName_textBox->Name = L"NewPIUnitName_textBox";
			this->NewPIUnitName_textBox->Size = System::Drawing::Size(97, 22);
			this->NewPIUnitName_textBox->TabIndex = 81;
			// 
			// AddNewPIUnitImage_button
			// 
			this->AddNewPIUnitImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddNewPIUnitImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddNewPIUnitImage_button->Location = System::Drawing::Point(835, 126);
			this->AddNewPIUnitImage_button->Name = L"AddNewPIUnitImage_button";
			this->AddNewPIUnitImage_button->Size = System::Drawing::Size(99, 35);
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
			this->NewPIUnitStartX_numericUpDown->Location = System::Drawing::Point(627, 20);
			this->NewPIUnitStartX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitStartX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->NewPIUnitStartX_numericUpDown->Name = L"NewPIUnitStartX_numericUpDown";
			this->NewPIUnitStartX_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->NewPIUnitStartX_numericUpDown->TabIndex = 79;
			this->NewPIUnitStartX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitEndY_numericUpDown
			// 
			this->NewPIUnitEndY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitEndY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitEndY_numericUpDown->Location = System::Drawing::Point(625, 138);
			this->NewPIUnitEndY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitEndY_numericUpDown->Name = L"NewPIUnitEndY_numericUpDown";
			this->NewPIUnitEndY_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->NewPIUnitEndY_numericUpDown->TabIndex = 78;
			this->NewPIUnitEndY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitEndX_numericUpDown
			// 
			this->NewPIUnitEndX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitEndX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitEndX_numericUpDown->Location = System::Drawing::Point(627, 98);
			this->NewPIUnitEndX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitEndX_numericUpDown->Name = L"NewPIUnitEndX_numericUpDown";
			this->NewPIUnitEndX_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->NewPIUnitEndX_numericUpDown->TabIndex = 77;
			this->NewPIUnitEndX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitStartY_numericUpDown
			// 
			this->NewPIUnitStartY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitStartY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitStartY_numericUpDown->Location = System::Drawing::Point(627, 61);
			this->NewPIUnitStartY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->NewPIUnitStartY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, System::Int32::MinValue });
			this->NewPIUnitStartY_numericUpDown->Name = L"NewPIUnitStartY_numericUpDown";
			this->NewPIUnitStartY_numericUpDown->Size = System::Drawing::Size(85, 22);
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
			this->label11->Location = System::Drawing::Point(625, 122);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(202, 12);
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
			this->label10->Location = System::Drawing::Point(625, 82);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(202, 12);
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
			this->label9->Location = System::Drawing::Point(625, 43);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(204, 12);
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
			this->label7->Location = System::Drawing::Point(420, 117);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(140, 12);
			this->label7->TabIndex = 71;
			this->label7->Text = L"ImageDistanceY/圖片間距Y";
			// 
			// ImageDistanceY_numericUpDown
			// 
			this->ImageDistanceY_numericUpDown->Location = System::Drawing::Point(421, 132);
			this->ImageDistanceY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->ImageDistanceY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->ImageDistanceY_numericUpDown->Name = L"ImageDistanceY_numericUpDown";
			this->ImageDistanceY_numericUpDown->Size = System::Drawing::Size(61, 22);
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
			this->label6->Location = System::Drawing::Point(418, 78);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(140, 12);
			this->label6->TabIndex = 69;
			this->label6->Text = L"ImageDistanceX/圖片間距X";
			// 
			// ImageDistanceX_numericUpDown
			// 
			this->ImageDistanceX_numericUpDown->Location = System::Drawing::Point(423, 93);
			this->ImageDistanceX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->ImageDistanceX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->ImageDistanceX_numericUpDown->Name = L"ImageDistanceX_numericUpDown";
			this->ImageDistanceX_numericUpDown->Size = System::Drawing::Size(61, 22);
			this->ImageDistanceX_numericUpDown->TabIndex = 68;
			this->ImageDistanceX_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// DebugLine_checkBox
			// 
			this->DebugLine_checkBox->AutoSize = true;
			this->DebugLine_checkBox->Location = System::Drawing::Point(6, 132);
			this->DebugLine_checkBox->Name = L"DebugLine_checkBox";
			this->DebugLine_checkBox->Size = System::Drawing::Size(76, 16);
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
			this->ImagePosY_numericUpDown->Location = System::Drawing::Point(420, 54);
			this->ImagePosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImagePosY_numericUpDown->Name = L"ImagePosY_numericUpDown";
			this->ImagePosY_numericUpDown->Size = System::Drawing::Size(85, 22);
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
			this->ImagePosX_numericUpDown->Location = System::Drawing::Point(420, 26);
			this->ImagePosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000000, 0, 0, 0 });
			this->ImagePosX_numericUpDown->Name = L"ImagePosX_numericUpDown";
			this->ImagePosX_numericUpDown->Size = System::Drawing::Size(85, 22);
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
			this->label3->Location = System::Drawing::Point(421, 4);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(24, 12);
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
			this->SequenceAnimation_tabPage->Location = System::Drawing::Point(4, 22);
			this->SequenceAnimation_tabPage->Name = L"SequenceAnimation_tabPage";
			this->SequenceAnimation_tabPage->Size = System::Drawing::Size(1346, 204);
			this->SequenceAnimation_tabPage->TabIndex = 2;
			this->SequenceAnimation_tabPage->Text = L"SequenceAnimation";
			// 
			// AnimationTime_numericUpDown
			// 
			this->AnimationTime_numericUpDown->DecimalPlaces = 3;
			this->AnimationTime_numericUpDown->Location = System::Drawing::Point(53, 163);
			this->AnimationTime_numericUpDown->Name = L"AnimationTime_numericUpDown";
			this->AnimationTime_numericUpDown->Size = System::Drawing::Size(120, 22);
			this->AnimationTime_numericUpDown->TabIndex = 60;
			// 
			// AnimationData_listBox
			// 
			this->AnimationData_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AnimationData_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationData_listBox->FormattingEnabled = true;
			this->AnimationData_listBox->ItemHeight = 12;
			this->AnimationData_listBox->Location = System::Drawing::Point(53, 34);
			this->AnimationData_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AnimationData_listBox->Name = L"AnimationData_listBox";
			this->AnimationData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AnimationData_listBox->Size = System::Drawing::Size(134, 124);
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
			this->AnimationDataList_listBox->ItemHeight = 12;
			this->AnimationDataList_listBox->Location = System::Drawing::Point(269, 34);
			this->AnimationDataList_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AnimationDataList_listBox->Name = L"AnimationDataList_listBox";
			this->AnimationDataList_listBox->Size = System::Drawing::Size(152, 124);
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
			this->AnimationDatDel_button->Location = System::Drawing::Point(206, 119);
			this->AnimationDatDel_button->Margin = System::Windows::Forms::Padding(2);
			this->AnimationDatDel_button->Name = L"AnimationDatDel_button";
			this->AnimationDatDel_button->Size = System::Drawing::Size(33, 20);
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
			this->AddAnimationImage_button->Location = System::Drawing::Point(13, 16);
			this->AddAnimationImage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddAnimationImage_button->Name = L"AddAnimationImage_button";
			this->AddAnimationImage_button->Size = System::Drawing::Size(33, 25);
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
			this->AnimationDatAdd_button->Location = System::Drawing::Point(206, 67);
			this->AnimationDatAdd_button->Margin = System::Windows::Forms::Padding(2);
			this->AnimationDatAdd_button->Name = L"AnimationDatAdd_button";
			this->AnimationDatAdd_button->Size = System::Drawing::Size(33, 25);
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
			this->DelAnimationImage_button->Location = System::Drawing::Point(13, 49);
			this->DelAnimationImage_button->Margin = System::Windows::Forms::Padding(2);
			this->DelAnimationImage_button->Name = L"DelAnimationImage_button";
			this->DelAnimationImage_button->Size = System::Drawing::Size(33, 20);
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
			this->AnimationDataName_textBox->Location = System::Drawing::Point(190, 37);
			this->AnimationDataName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->AnimationDataName_textBox->Name = L"AnimationDataName_textBox";
			this->AnimationDataName_textBox->Size = System::Drawing::Size(76, 22);
			this->AnimationDataName_textBox->TabIndex = 57;
			// 
			// AnimationImageUp_button
			// 
			this->AnimationImageUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationImageUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationImageUp_button->Location = System::Drawing::Point(13, 78);
			this->AnimationImageUp_button->Name = L"AnimationImageUp_button";
			this->AnimationImageUp_button->Size = System::Drawing::Size(32, 36);
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
			this->label2->Location = System::Drawing::Point(267, 19);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(88, 12);
			this->label2->TabIndex = 56;
			this->label2->Text = L"AnimationData:ist";
			// 
			// AnimationImageDown_button
			// 
			this->AnimationImageDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationImageDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationImageDown_button->Location = System::Drawing::Point(13, 119);
			this->AnimationImageDown_button->Name = L"AnimationImageDown_button";
			this->AnimationImageDown_button->Size = System::Drawing::Size(32, 34);
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
			this->label1->Location = System::Drawing::Point(51, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(75, 12);
			this->label1->TabIndex = 55;
			this->label1->Text = L"AnimationData";
			// 
			// ImageTriangulator_tabPage
			// 
			this->ImageTriangulator_tabPage->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ImageTriangulator_tabPage->Controls->Add(this->pictureBox2);
			this->ImageTriangulator_tabPage->Controls->Add(this->pictureBox1);
			this->ImageTriangulator_tabPage->Controls->Add(this->button1);
			this->ImageTriangulator_tabPage->Controls->Add(this->ImageTriangulatorLOD_numericUpDown);
			this->ImageTriangulator_tabPage->Controls->Add(this->ImageTriangulatorLOD_label);
			this->ImageTriangulator_tabPage->Controls->Add(this->label13);
			this->ImageTriangulator_tabPage->Controls->Add(this->TriangulatorMouseBehavior_comboBox);
			this->ImageTriangulator_tabPage->Location = System::Drawing::Point(4, 22);
			this->ImageTriangulator_tabPage->Name = L"ImageTriangulator_tabPage";
			this->ImageTriangulator_tabPage->Padding = System::Windows::Forms::Padding(3);
			this->ImageTriangulator_tabPage->Size = System::Drawing::Size(1346, 204);
			this->ImageTriangulator_tabPage->TabIndex = 1;
			this->ImageTriangulator_tabPage->Text = L"ImageTriangulator";
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(1077, 21);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(234, 152);
			this->pictureBox2->TabIndex = 94;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(777, 21);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(234, 152);
			this->pictureBox1->TabIndex = 93;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(674, 91);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 92;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &cPIEditor::button1_Click_1);
			// 
			// ImageTriangulatorLOD_numericUpDown
			// 
			this->ImageTriangulatorLOD_numericUpDown->Location = System::Drawing::Point(140, 101);
			this->ImageTriangulatorLOD_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->ImageTriangulatorLOD_numericUpDown->Name = L"ImageTriangulatorLOD_numericUpDown";
			this->ImageTriangulatorLOD_numericUpDown->Size = System::Drawing::Size(120, 22);
			this->ImageTriangulatorLOD_numericUpDown->TabIndex = 91;
			this->ImageTriangulatorLOD_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->ImageTriangulatorLOD_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImageTriangulatorLOD_numericUpDown_ValueChanged);
			// 
			// ImageTriangulatorLOD_label
			// 
			this->ImageTriangulatorLOD_label->AutoSize = true;
			this->ImageTriangulatorLOD_label->Location = System::Drawing::Point(6, 102);
			this->ImageTriangulatorLOD_label->Name = L"ImageTriangulatorLOD_label";
			this->ImageTriangulatorLOD_label->Size = System::Drawing::Size(115, 12);
			this->ImageTriangulatorLOD_label->TabIndex = 90;
			this->ImageTriangulatorLOD_label->Text = L"ImageTriangulatorLOD";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(6, 47);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(79, 12);
			this->label13->TabIndex = 89;
			this->label13->Text = L"MouseBehavior";
			// 
			// TriangulatorMouseBehavior_comboBox
			// 
			this->TriangulatorMouseBehavior_comboBox->FormattingEnabled = true;
			this->TriangulatorMouseBehavior_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Add", L"Move", L"Delete" });
			this->TriangulatorMouseBehavior_comboBox->Location = System::Drawing::Point(140, 47);
			this->TriangulatorMouseBehavior_comboBox->Name = L"TriangulatorMouseBehavior_comboBox";
			this->TriangulatorMouseBehavior_comboBox->Size = System::Drawing::Size(121, 20);
			this->TriangulatorMouseBehavior_comboBox->TabIndex = 88;
			this->TriangulatorMouseBehavior_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::TriangulatorMouseBehavior_comboBox_SelectedIndexChanged);
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Resize += gcnew System::EventHandler(this, &cPIEditor::splitContainer2_Panel1_Resize);
			this->splitContainer2->Panel2Collapsed = true;
			this->splitContainer2->Size = System::Drawing::Size(1683, 707);
			this->splitContainer2->SplitterDistance = 356;
			this->splitContainer2->SplitterWidth = 3;
			this->splitContainer2->TabIndex = 0;
			// 
			// MouseControlMode_comboBox
			// 
			this->MouseControlMode_comboBox->FormattingEnabled = true;
			this->MouseControlMode_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"ObjectClick", L"SelectNewObject" });
			this->MouseControlMode_comboBox->Location = System::Drawing::Point(837, 74);
			this->MouseControlMode_comboBox->Name = L"MouseControlMode_comboBox";
			this->MouseControlMode_comboBox->Size = System::Drawing::Size(121, 20);
			this->MouseControlMode_comboBox->TabIndex = 87;
			// 
			// MouseControlMode_label
			// 
			this->MouseControlMode_label->AutoSize = true;
			this->MouseControlMode_label->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->MouseControlMode_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->MouseControlMode_label->Location = System::Drawing::Point(833, 56);
			this->MouseControlMode_label->Name = L"MouseControlMode_label";
			this->MouseControlMode_label->Size = System::Drawing::Size(150, 12);
			this->MouseControlMode_label->TabIndex = 88;
			this->MouseControlMode_label->Text = L"MouseControlMode/操作模式";
			// 
			// cPIEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1687, 972);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ImageTriangulatorLOD_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer2))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void AllImage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if( AllImage_listBox->SelectedIndex != -1 )
				 {
					 if(e->KeyCode == Keys::Delete)
					 {
						if( m_pImageomposerIRM->Count() )
						{
							m_pImageomposerIRM->RemoveObject(AllImage_listBox->SelectedIndex);
							m_ImageTale->Remove(AllImage_listBox->SelectedItem->ToString());
							AllImage_listBox->Items->RemoveAt(AllImage_listBox->SelectedIndex);
							ImageDetail_textBox->Text = GetTotalPixelExisted();
						}
					 }
				 }
			 }
	private: System::Void DeleteImage_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( sender == DeleteImage_button )
				 {
					 this->timer1->Enabled = false;
					 std::list<int>	l_iErseIndexList;
					 System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
					 l_MyList->AddRange(AllImage_listBox->SelectedItems);
					 for each (System::Object^l_pObject in l_MyList)
						l_iErseIndexList.push_back(AllImage_listBox->Items->IndexOf(l_pObject));
					 if( l_MyList->Count )
						m_iCurrentSelectedObjectIndex = -1;
					 for each (System::Object^l_pObject in l_MyList)
					 {
						int	l_iIndex = AllImage_listBox->Items->IndexOf(l_pObject);
						AllImage_listBox->Items->Remove(l_pObject);
						NamedTypedObject*l_pNamedTypedObject = m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_pObject->ToString()));
						if( l_pNamedTypedObject->GetOwner() )
							m_ImageTale->Remove(l_pObject->ToString());

						for(int i = 0;i<m_pImageomposerIRM->Count();++i)
						{
							cUIImage*l_pUIImage2 = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
							if( l_pUIImage2->m_pEditorAttachParent == l_pNamedTypedObject )
							{
								int	l_iIndex = AllImage_listBox->Items->IndexOf(DNCT::WcharToGcstring(l_pUIImage2->GetName()));
								AllImage_listBox->Items->RemoveAt(l_iIndex);
								m_pImageomposerIRM->RemoveObject(l_pUIImage2->GetName());
								--i;
							}
						}
						if (m_pPuzzleImageUnitTriangulatorManager)
							m_pPuzzleImageUnitTriangulatorManager->RemoveObject(dynamic_cast<cUIImage*>(l_pNamedTypedObject));
						m_pImageomposerIRM->RemoveObject(DNCT::GcStringToWchar(l_pObject->ToString()));
					 }
					 if( m_pImageomposerIRM->Count() != AllImage_listBox->Items->Count )
						 UT::ErrorMsg(L"delete image error occur!!!",L"call fatming");
					 this->timer1->Enabled = true;
				 }
				 else
				 {
					cli::array<String^>^l_pImageNameList;
					if( addImagesByFolderToolStripMenuItem->Checked )
					{
						System::String^l_strDirectoryName = DNCT::SelectDirectory();
						if( l_strDirectoryName )
						{
							System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList;
							GetFilesNameByRecursivelyDirectory(l_strDirectoryName,l_pArrayList,".png");
							if( l_pArrayList->Count )
							{
								l_pImageNameList = gcnew cli::array<String^>(l_pArrayList->Count);
								l_pArrayList->CopyTo(l_pImageNameList);
							}
						}
					}
					else
					{
						l_pImageNameList = DNCT::OpenFileAndGetNames("image Files(*.png;*.bmp;*.jpg;*.dds)|*.png;*.bmp;*.jpg;*.dds|All files (*.*)|*.*");
					}
					if( !l_pImageNameList )
						return;
					this->timer1->Enabled = false;
					for each(String^l_strFileName in l_pImageNameList)
					{
						String^l_strFileNameWithoutDirectory = DNCT::GetFileNameWithoutFullPath(l_strFileName,true);
						bool	l_bAlpha = false;
						if( l_strFileName->Contains(".png")||l_strFileName->Contains(".PNG") )
							l_bAlpha = true;					
						int l_iOriginalIndex = -1;
                        if(DNCT::CheckListContainStringAndAdd(AllImage_listBox,l_strFileNameWithoutDirectory))
                        {
					        WARING_YES_NO_TO_NO(l_strFileNameWithoutDirectory+"object name already exists,would u like to replace?"+DNCT::GetChanglineString()+"該物件名稱已存在,要覆蓋嗎?")
					            continue;
					        //remove old data
					        m_ImageTale->Remove(l_strFileNameWithoutDirectory);
					        l_iOriginalIndex = m_pImageomposerIRM->GetObjectIndexByName(DNCT::GcStringToWchar(l_strFileNameWithoutDirectory).c_str());
					        cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_strFileNameWithoutDirectory)));
					        SAFE_DELETE(l_pUIImage);
					    }
						try
						{
							//System::Diagnostics::Stopwatch^ sw = gcnew System::Diagnostics::Stopwatch();
							//sw->Start();
							System::Drawing::Bitmap^l_pImage;
							cUIImage*l_pUIImage = 0;
							bool	l_bUsingDDS = false;
							if( l_bUsingDDS )
							{
								l_pImage = DNCT::OpenImageFile(l_strFileName,&l_pUIImage);
							}
							else
							{
								l_pImage = DNCT::OpenImageFile(l_strFileName);
							}



							m_ImageTale[l_strFileNameWithoutDirectory] = l_pImage;
							std::wstring l_strWcharFileName = DNCT::GcStringToWchar(l_strFileName);
							std::wstring l_strwstringFileName = UT::GetFileNameWithoutFullPath(l_strWcharFileName.c_str());
							if( !l_pUIImage )
								l_pUIImage = GetNewUIImageByBitMap(l_pImage,l_strwstringFileName.c_str());

							//POINT	l_WidthHeight = { *l_pUIImage->GetWidth()-1,*l_pUIImage->GetHeight()-1 };
							//l_pUIImage->SetRightDownStripOffPos(l_WidthHeight);
							//sw->Stop();
							if( l_iOriginalIndex != -1 )
						        (*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
						    else
							    m_pImageomposerIRM->AddObject(l_pUIImage);
						}
						catch(System::Exception^l_pException)
						{
							WARNING_MSG("doesn't support such file"+l_strFileName+DNCT::GetChanglineString()+l_pException->ToString());
							AllImage_listBox->Items->Remove(l_strFileNameWithoutDirectory);
						}
					}
					this->timer1->Enabled = true;
				 }
				 ImageDetail_textBox->Text = GetTotalPixelExisted();
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(this->menuStrip1->Visible == false )
					this->menuStrip1->Visible = true;
				 if( !this->Visible )
				 {
					 ::Threading::Thread::Sleep(100);
				 }
				//wglMakeCurrent(nullptr,nullptr);
				wglMakeCurrent( m_HdcMV,m_HGLRCMV );
				static	float	l_sfTime = 0;
				l_sfTime += 0.01f;
				if( l_sfTime >1.f )
					l_sfTime = 0.f;
				DEFAULT_SHADER = L"ICPathShader";
				NO_TEXTURE_SHADER = L"IC_NoTextureShader";
				UseShaderProgram();
				m_pTimeAndFPS->Update();
				float	l_fElpaseTime = m_pTimeAndFPS->fElpaseTime;
				glViewport(0,0,splitContainer2->Panel1->Width,splitContainer2->Panel1->Height);
				glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
				glClearColor( m_pvBGColor->x,m_pvBGColor->y,m_pvBGColor->z,m_pvBGColor->w );
				glClearDepth(1.0f);									// Depth Buffer Setup
				cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)splitContainer2->Panel1->Width;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)splitContainer2->Panel1->Height;
				if (tabControl1->SelectedIndex == 2)
				{
					m_pOrthogonalCameraForTrianhulatorPIUnit->Render();
					m_pOrthogonalCameraForTrianhulatorPIUnit->DrawGrid(0, 0, Vector4(0.3f, 0.7f, 0.3f, 0.7f));
					if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
					{
						m_pCurrentSelectedPuzzleImageUnitTriangulator->Render();
					}
				}
				else
				{
					cGameApp::ApplyViewPort();
					m_pOrthogonalCamera->Render();
					m_pOrthogonalCamera->DrawGrid(0, 0, Vector4(0.3f, 0.7f, 0.3f, 0.7f));
					glEnable(GL_ALPHA_TEST);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					//glEnable(GL_TEXTURE_2D);
					for (int i = 0; i < m_pImageomposerIRM->Count(); ++i)
					{
						cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
						if (!l_p->m_pEditorAttachParent)
							l_p->Render();
					}
					glDisable(GL_TEXTURE_2D);
					int	l_iRectWidth = 0;
					int	l_iRectHeight = 0;
					if (AllShowBoundry_checkBox->Checked)
					{
						int	l_iNum = m_pImageomposerIRM->Count();
						for (int i = 0; i < m_pImageomposerIRM->Count(); ++i)
						{
							cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
							//Vector4	l_vColor(0.5, 0.5, 0.5, 1.0);
							Vector4	l_vColor(1.5, 1.5, 1.5, 1.0);
							//Vector4	l_vColor(0, 0, 0, 1);
							//l_vColor.x = 1.f / ((i % 3) + 1);
							//l_vColor.y = 0.5f;
							//l_vColor.z = 1.f / ((l_iNum % (i + 1)) + 1);
							//l_vColor.w = 1;
							Vector3	l_vPos = l_p->GetPos();
							//+1 for offset start at 0,0
							l_iRectWidth = l_p->GetRightDownStripOffPos().x - l_p->GetOffsetPos()->x + 1;
							l_iRectHeight = l_p->GetRightDownStripOffPos().y - l_p->GetOffsetPos()->y + 1;
							l_vPos.x += l_p->GetOffsetPos()->x;
							l_vPos.y += l_p->GetOffsetPos()->y;
							POINT	l_Pos = { (int)l_vPos.x,(int)l_vPos.y };
							//+1 for 0,0 start offset
							RenderRectangle(l_Pos, l_iRectWidth, l_iRectHeight, l_vColor, 0, 1);
							//l_p->DebugRender();
						}
					}
					if (ShowTriangulaotrPoints_checkBox->Checked && m_pPuzzleImageUnitTriangulatorManager)
					{
						this->m_pPuzzleImageUnitTriangulatorManager->RenderPointsShapeLine();
					}
					if (AllImage_listBox->Items->Count&&m_iCurrentSelectedObjectIndex != -1)
					{
						if (ShowBoundary_checkBox->Checked)
						{
							static Vector4 l_vColor(0, 0, 0, 1);
							l_vColor.x = l_sfTime;
							l_vColor.y -= l_sfTime;
							l_vColor.z = l_sfTime;
							l_vColor.w = l_sfTime;
							if (l_vColor.x >= 1.f)
								l_vColor.x = 1.f;
							if (l_vColor.y <= 0)
								l_vColor.y = 0;
							if (m_iCurrentSelectedObjectIndex >= m_pImageomposerIRM->Count())
								m_iCurrentSelectedObjectIndex = m_pImageomposerIRM->Count() - 1;
							if (m_pImageomposerIRM->Count() > -1)
							{
								cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
								Vector4	l_vOriginalColor = *l_p->GetColor();
								l_p->SetColor(l_vColor);
								l_p->Render();
								l_p->SetColor(l_vOriginalColor);
								Vector3	l_vPos = l_p->GetPos();
								//+1 for offset start at 0,0
								l_iRectWidth = l_p->GetRightDownStripOffPos().x - l_p->GetOffsetPos()->x + 1;
								l_iRectHeight = l_p->GetRightDownStripOffPos().y - l_p->GetOffsetPos()->y + 1;
								l_vPos.x += l_p->GetOffsetPos()->x;
								l_vPos.y += l_p->GetOffsetPos()->y;
								POINT	l_Pos = { (int)l_vPos.x,(int)l_vPos.y };
								//+1 for 0,0 start offset
								RenderRectangle(l_Pos, l_iRectWidth, l_iRectHeight, l_vColor, 0, 1);
							}
						}
					}
					if (tabControl1->SelectedIndex != 2)
					{
						//for final image size
						POINT	l_Pos = { 0,0 };
						RenderRectangle(l_Pos, (int)ImageWidth_numericUpDown->Value, (int)ImageHeight_numericUpDown->Value, Vector4(1.f, 0.3f, 0.3f, 0.3f));
					}
					if (this->MouseControlMode_comboBox->SelectedIndex == 1)
					{
						m_pOrthogonalCamera->SetDrawSelectFrame(true);
						m_pOrthogonalCamera->DrawSelectFrame();
					}
					else
						m_pOrthogonalCamera->SetDrawSelectFrame(true);
					if (m_pDebugFont)
					{
						UseShaderProgram();
						glEnable2D(1280, 720);
						POINT	ptCursor = { (int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y };
						std::wstring l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString() + "," + ptCursor.y.ToString());
						m_pDebugFont->RenderFont(0, 0, l_strMousePos.c_str());
						glDisable(GL_TEXTURE_2D);
					}
					glDisable(GL_ALPHA_TEST);
				}
				//GLRender::glDisable2D();
				glFlush();
				SwapBuffers (m_HdcMV);
			 }
	private: System::Void AutoOrderPosition_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				int	l_iPixelXGap = (int)ImageDistanceX_numericUpDown->Value;
				int	l_iPixelYGap = (int)ImageDistanceY_numericUpDown->Value;
				if(!AutoAligment_checkBox->Checked)
				{
				//old method
					int	l_iCurrentX = 0;
					int	l_iCurrentY = 0;
					int	l_iMaxX = (int)ImageWidth_numericUpDown->Value;
					//the max height in current row
					int	l_iYLastBigPos = 0;
					if( !m_pImageomposerIRM->Count() )
						return;
					//so to fix UV problem,1 pixle problem
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(0));
					POINT	l_Pos = {-l_pUIImage->GetOffsetPos()->x,-l_pUIImage->GetOffsetPos()->y};
					l_pUIImage->SetPos(l_Pos);
					//+1 for start offset(0,0)
					l_iCurrentX = l_pUIImage->GetRightDownStripOffPos().x-l_pUIImage->GetOffsetPos()->x+1;
					l_iYLastBigPos = l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1;
					//if(OnePixelDistance_checkBox->Checked)
					{
						l_iYLastBigPos += l_iPixelYGap;
						l_iCurrentX += l_iPixelXGap;
					}
					for( int i=1;i<m_pImageomposerIRM->Count();++i )
					{
						l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
						if( l_pUIImage->m_pEditorAttachParent )
							continue;
						//this length has pixels,offset+width-rightdown pos,add 1 for avoid same image at same pixel
						//+1 for offset start at 0,0
						int	l_iXLengthForPixels = l_pUIImage->GetRightDownStripOffPos().x-l_pUIImage->GetOffsetPos()->x+1;
						//if(OnePixelDistance_checkBox->Checked)
							l_iXLengthForPixels += l_iPixelXGap;
						l_iCurrentX += (l_iXLengthForPixels);
						//the y pixel we do not need
						//+1 for offset start at 0,0
						int	l_iGarbagePixelY = l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1;
						l_iGarbagePixelY += l_iPixelXGap;
						if( l_iCurrentX>l_iMaxX)
						{
							//for next start not object position
							l_iCurrentX = l_iXLengthForPixels;//-l_pUIImage->GetOffsetPos()->x;
							l_iCurrentY += l_iYLastBigPos;
							//+1 for offset start at 0,0
							l_iYLastBigPos = l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1;
							//if(OnePixelDistance_checkBox->Checked)
								l_iYLastBigPos += l_iPixelYGap;
							//set object position x to offset pos,because it's changing to next line
							//
							l_Pos.x = -l_pUIImage->GetOffsetPos()->x;
							l_Pos.y = l_iCurrentY-l_pUIImage->GetOffsetPos()->y;
							l_pUIImage->SetPos(l_Pos);
							continue;
						}
						if( l_iYLastBigPos<l_iGarbagePixelY )
							l_iYLastBigPos = l_iGarbagePixelY;
						//although we know the position we wanted,we still have to minus the offset position to position
						l_Pos.x = l_iCurrentX-(l_iXLengthForPixels)-l_pUIImage->GetOffsetPos()->x;
						l_Pos.y = l_iCurrentY-l_pUIImage->GetOffsetPos()->y;
						l_pUIImage->SetPos(l_Pos);
					}
				}
				else
				{
					int l_iImageCount = m_pImageomposerIRM->Count();
					  // Create some 'content' to work on.
					BinPack2D::ContentAccumulator<cUIImage*> inputContent;
					for(int i=0;i<l_iImageCount;i++)
					{
						auto l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
						if (l_pUIImage->m_pEditorAttachParent)
							continue;
						//inputContent += BinPack2D::Content<cUIImage*>(l_pUIImage, BinPack2D::Coord(), BinPack2D::Size(l_pUIImage->GetWidth()+l_iPixelXGap,l_pUIImage->GetHeight()+l_iPixelYGap), false );
						POINT l_LeftUpPos = *l_pUIImage->GetOffsetPos();
						POINT l_RightDownPos = l_pUIImage->GetRightDownStripOffPos();
						POINT l_Size = {l_RightDownPos.x-l_LeftUpPos.x,l_RightDownPos.y-l_LeftUpPos.y};
						inputContent += BinPack2D::Content<cUIImage*>(l_pUIImage, BinPack2D::Coord(), BinPack2D::Size(l_Size.x+l_iPixelXGap,l_Size.y+l_iPixelYGap), false );
					}
  
					// Sort the input content by size... usually packs better.
					inputContent.Sort();
  
					// Create some bins! ( 2 bins, 128x128 in this example )
					BinPack2D::CanvasArray<cUIImage*> canvasArray = BinPack2D::UniformCanvasArrayBuilder<cUIImage*>((int)ImageWidth_numericUpDown->Value,(int)ImageHeight_numericUpDown->Value,2).Build();
    
					// A place to store content that didnt fit into the canvas array.
					BinPack2D::ContentAccumulator<cUIImage*> remainder;
  
					// try to pack content into the bins.
					bool success = canvasArray.Place( inputContent, remainder );
  
					// A place to store packed content.
					BinPack2D::ContentAccumulator<cUIImage*> outputContent;
  
					// Read all placed content.
					canvasArray.CollectContent( outputContent );
  
					// parse output.
					typedef BinPack2D::Content<cUIImage*>::Vector::iterator binpack2d_iterator;
					//printf("PLACED:\n");
					if( remainder.Get().size() == 0)
					{
						for( binpack2d_iterator itor = outputContent.Get().begin(); itor != outputContent.Get().end(); itor++ )
						{
   
							const BinPack2D::Content<cUIImage*> &content = *itor;
							// retreive your data.
							auto myContent = content.content;
							myContent->SetPos(Vector3((float)content.coord.x-myContent->GetOffsetPos()->x, (float)content.coord.y-myContent->GetOffsetPos()->y,0));
  
							//printf("\t%9s of size %3dx%3d at position %3d,%3d,%2d rotated=%s\n",
							//   myContent.str.c_str(), 
							//   content.size.w, 
							//   content.size.h, 
							//   content.coord.x, 
							//   content.coord.y, 
							//   content.coord.z, 
							//   (content.rotated ? "yes":" no"));
						}
						if(IsObjectOverlap(m_pImageomposerIRM))
						{
							WARNING_MSG("texture is too small");
						}
					}
					else
					{
						WARNING_MSG("texture is too small");
						return;
					}
					 // printf("NOT PLACED:\n");
					 // for( binpack2d_iterator itor = remainder.Get().begin(); itor != remainder.Get().end(); itor++ ) {
   
						//const BinPack2D::Content<NamedTypedObject*> &content = *itor;
    
						//const NamedTypedObject* &myContent = content.content;
  
						//printf("\t%9s of size %3dx%3d\n",
						//   myContent.str.c_str(), 
						//   content.size.w, 
						//   content.size.h);
					 // }


					//if( !m_pImageomposerIRM->Count() )
					//	return;
					//rect_xywhf* l_pRects = new rect_xywhf[m_pImageomposerIRM->Count()];
					//if( m_pImageomposerIRM->Count() >= 1000 )
					//{
					//	WARNING_MSG("too many images,please call fatming to make image over 10000");
					//	return;
					//}
					//rect_xywhf *l_ptr_rects[1000];
					//for(int i = 0; i < m_pImageomposerIRM->Count(); ++i) {
					//	auto l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					//	l_pRects[i] = rect_xywhf(0, 0, l_pUIImage->GetWidth()+l_iPixelXGap,l_pUIImage->GetHeight()+l_iPixelYGap,i);
					//	l_ptr_rects[i] = l_pRects+i;
					//}

					//vector<bin> bins;
					//int l_MaxSize = max((int)ImageHeight_numericUpDown->Value,(int)ImageWidth_numericUpDown->Value);
					//for(int i=0;i<100;++i)
					//{
					//	int l_iMax = 100*i;
					//	if(pack(l_ptr_rects, m_pImageomposerIRM->Count(), l_iMax, bins))
					//	{
					//		if( bins.size() == 1 )
					//		{
					//			for( int j=0;j<(int)bins[0].rects.size();++j )
					//			{
					//				int l_iIndex = bins[0].rects[j]->iImageIndex;
					//				auto l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iIndex));
					//				l_pUIImage->SetPos(Vector3(bins[0].rects[j]->x-l_pUIImage->GetOffsetPos()->x,bins[0].rects[j]->y-l_pUIImage->GetOffsetPos()->y,0));
					//				cGameApp::OutputDebugInfoString(UT::ComposeMsgByFormat("%d:firsy:%d,%d,second:",l_iIndex,bins[0].rects[j]->w,bins[0].rects[j]->h,l_pUIImage->GetWidth(),l_pUIImage->GetHeight()));
					//				int a=0;
					//			}
					//			break;
					//		}
					//		else
					//		{
					//			bins.clear();
					//			//WARNING_MSG("please adjust texture size");
					//		}
					//	}
					//}
					//SAFE_DELETE(l_pRects);
				}
				AttachObjectPosAdjust();
			 }
	private: System::Void AllImage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( AllImage_listBox->SelectedIndex != -1 )
				 {
					m_iCurrentSelectedObjectIndex = AllImage_listBox->SelectedIndex;
					pictureBox2->Image = (Image^)m_ImageTale[AllImage_listBox->Items[AllImage_listBox->SelectedIndex]->ToString()];
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
					if (l_pUIImage && m_pPuzzleImageUnitTriangulatorManager)
					{
						m_pCurrentSelectedPuzzleImageUnitTriangulator = m_pPuzzleImageUnitTriangulatorManager->GetObject(l_pUIImage);
						if (m_pCurrentSelectedPuzzleImageUnitTriangulator && TriangulatorMouseBehavior_comboBox->SelectedIndex != -1)
						{
							m_pCurrentSelectedPuzzleImageUnitTriangulator->SetPointsToTriangulatorType((ePointsToTriangulatorType)TriangulatorMouseBehavior_comboBox->SelectedIndex);
							ImageTriangulatorLOD_numericUpDown->Value = m_pCurrentSelectedPuzzleImageUnitTriangulator->GetLOD();
						}
					}
					NewPIUnitName_textBox->Text = DNCT::WcharToGcstring(l_pUIImage->GetName());
					//+1 for offset start at 0,0
					POINT	l_ImageSize = l_pUIImage->GetImageRealSize();
					ImageDetail_textBox->Text = "X:"+l_pUIImage->GetPos().x.ToString()+"Y:"+l_pUIImage->GetPos().y.ToString()+DNCT::GetChanglineString()+
						"Size:"+l_ImageSize.x.ToString()+","+l_ImageSize.y.ToString()+DNCT::GetChanglineString()+
						"OffsetPos:"+l_pUIImage->GetOffsetPos()->x.ToString()+","+l_pUIImage->GetOffsetPos()->y.ToString()+
						GetTotalPixelExisted();
					if( l_pUIImage->m_pEditorAttachParent )
					{
						ImageDetail_textBox->Text += DNCT::GetChanglineString()+"AttachedPIUnit!!!";
						m_bAvoidDataAssignForPIUintChild = true;
						NewPIUnitStartX_numericUpDown->Value = (System::Decimal)l_pUIImage->GetPos().x;
						NewPIUnitStartY_numericUpDown->Value = (System::Decimal)l_pUIImage->GetPos().y;
						NewPIUnitEndX_numericUpDown->Value = (System::Decimal)(l_pUIImage->GetPos().x+l_pUIImage->GetRightDownStripOffPos().x);
						NewPIUnitEndY_numericUpDown->Value = (System::Decimal)(l_pUIImage->GetPos().y+l_pUIImage->GetRightDownStripOffPos().y);
						m_bAvoidDataAssignForPIUintChild = false;
					}
				 }
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 int	l_iSelectIndex = AllImage_listBox->SelectedIndex;
				 if( l_iSelectIndex == -1 )
					return;
				 std::list<int>	l_iAllSelectedIndex;
				 bool	l_bContainTop = false;
				 bool	l_bContainDown = false;
				 for each(int l_iIndex in AllImage_listBox->SelectedIndices)
					l_iAllSelectedIndex.push_back(l_iIndex);
				 l_iAllSelectedIndex.sort();
				 AllImage_listBox->SelectedIndices->Clear();
				 if( sender == ImageObjectDown_button )
				 {
					while( l_iAllSelectedIndex.size() )
					{
						int	l_iIndex = l_iAllSelectedIndex.back();
						l_iAllSelectedIndex.pop_back();
						 //it's bottom
						if( l_iIndex >= AllImage_listBox->Items->Count-1 )
						{
							WARNING_MSG("buttom one can't be down any more");
							goto ADJUST_IMAG_POS;
						}
						cUIImage*	l_pUIImage = m_pImageomposerIRM->GetUIImage(l_iIndex);
						m_pImageomposerIRM->RemoveObjectWithoutDelete( l_iIndex );
						m_pImageomposerIRM->GetList()->insert(m_pImageomposerIRM->GetList()->begin()+(l_iIndex+1),l_pUIImage);
						DNCT::SwapListBoxValue(l_iIndex,l_iIndex+1,AllImage_listBox);
						AllImage_listBox->SelectedIndices->Add(l_iIndex+1);
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
							goto ADJUST_IMAG_POS;
						}
						cUIImage*	l_pUIImage = m_pImageomposerIRM->GetUIImage(l_iIndex);
						m_pImageomposerIRM->RemoveObjectWithoutDelete( l_iIndex );
						m_pImageomposerIRM->GetList()->insert(m_pImageomposerIRM->GetList()->begin()+(l_iIndex-1),l_pUIImage);
						DNCT::SwapListBoxValue(l_iIndex,l_iIndex-1,AllImage_listBox);
						AllImage_listBox->SelectedIndices->Add(l_iIndex-1);
					}
				 }
				 ADJUST_IMAG_POS:
				 AutoOrderPosition_button_Click(sender,e);
				 if( AllImage_listBox->SelectedIndex != -1 )
				 {
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(AllImage_listBox->SelectedIndex));
					POINT	l_ImageSize = {l_pUIImage->GetRightDownStripOffPos().x-l_pUIImage->GetOffsetPos()->x+1,
						l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1};
					Vector3	l_vImagePos = l_pUIImage->GetPos();
					POINT	l_ImagePos = {(int)l_vImagePos.x,(int)l_vImagePos.y};
					ImageDetail_textBox->Text = "X:"+l_ImagePos.x.ToString()+"Y:"+l_ImagePos.y.ToString()+DNCT::GetChanglineString()+
						"Size:"+l_ImageSize.x.ToString()+","+l_ImageSize.y.ToString()+DNCT::GetChanglineString()+
						"OffsetPos:"+l_pUIImage->GetOffsetPos()->x.ToString()+","+l_pUIImage->GetOffsetPos()->y.ToString()+
						GetTotalPixelExisted();
				 }
			 }
	//unform
	private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if(this->Visible)
				{
					timer1->Enabled = true;
					wglMakeCurrent( m_HdcMV,m_HGLRCMV );
					cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0.f;
					cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0.f;
					cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)splitContainer2->Panel1->Width;
					cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)splitContainer2->Panel1->Height;
					cGameApp::ApplyViewPort();
				}
				else
				{
					timer1->Enabled = false;

				}
			 }
	private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if (!timer1->Enabled)
					return;
				GCFORM::MouseButtons l_MouseButton = e->Button;
				auto l_iDelte = GetMouseWheelDelta(e);
				if (tabControl1->SelectedIndex == 2)
				{
					POINT	ptCursor = { (int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().x,(int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().y };
					m_pOrthogonalCameraForTrianhulatorPIUnit->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
						, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
					if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
					{
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MouseMove(ptCursor.x, ptCursor.y);
					}
				}
				else
				{
					m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
						, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
					if (this->MouseControlMode_comboBox->SelectedIndex == 0)
					{
						POINT	ptCursor = { (int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y };
						MouseCollideForPickUpObject(e, splitContainer2->Panel1);
						ptCursor.x = e->X; ptCursor.y = e->Y;
						//HWND	l_Hwnd = WindowFromPoint(ptCursor);
						if (this->m_pPuzzleImageUnitTriangulatorManager)
						{
							m_pPuzzleImageUnitTriangulatorManager->MouseMove(ptCursor.x, ptCursor.y);
						}
					}
				}
			 }
	private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if (!timer1->Enabled)
					return;
				splitContainer2->Panel1->Focus();
				GCFORM::MouseButtons l_MouseButton = e->Button;
				auto l_iDelte = GetMouseWheelDelta(e);
				if (tabControl1->SelectedIndex == 2)
				{
					m_pOrthogonalCameraForTrianhulatorPIUnit->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
						, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
					POINT	ptCursor = { (int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().x,(int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().y };
					if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
					{
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MouseDown(ptCursor.x, ptCursor.y);
					}
				}
				else
				{
					m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
						, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
					if (this->MouseControlMode_comboBox->SelectedIndex == 0)
					{
						if (this->Visible)
							MouseCollideForPickUpObject(e, splitContainer2->Panel1);
					}
				}
			 }
	private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if (!timer1->Enabled)
					return;
				GCFORM::MouseButtons l_MouseButton = e->Button;
				auto l_iDelte = GetMouseWheelDelta(e);
				if (tabControl1->SelectedIndex == 2)
				{
					m_pOrthogonalCameraForTrianhulatorPIUnit->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
						, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
					POINT	ptCursor = { (int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().x,(int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().y };
					if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
					{
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MouseUp(ptCursor.x, ptCursor.y);
					}
				}
				else
				{
					m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
						, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
					if (this->MouseControlMode_comboBox->SelectedIndex == 0)
					{
						if (this->Visible)
							MouseCollideForPickUpObject(e, splitContainer2->Panel1);
					}
				}
			 }
	private: System::Void MyMouseHover(System::Object^  sender, System::EventArgs^  e) {
				if(!timer1->Enabled)
					return;
			 }
	private: System::Void MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
				if(!timer1->Enabled)
					return;
			 }
	private: System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				if( e->KeyCode == Keys::Add  )
				{
					m_pOrthogonalCamera->SetScale(m_pOrthogonalCamera->GetScale()-0.1f);
				}
				else
				if( e->KeyCode == Keys::Subtract )
				{
					m_pOrthogonalCamera->SetScale(m_pOrthogonalCamera->GetScale()+0.1f);
				}
				if( m_iCurrentSelectedObjectIndex!=-1 )
				{
					cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
					Vector3	l_Pos = l_p->GetPos();
					if(e->KeyCode == Keys::Right)
						l_Pos.x+=1;
					if(e->KeyCode == Keys::Left)
						l_Pos.x-=1;
					if(e->KeyCode == Keys::Up)
						l_Pos.y-=1; 
					if(e->KeyCode == Keys::Down)
						l_Pos.y+=1;
					l_p->SetPos(l_Pos);
					AttachObjectPosAdjust();
				}
				//if(!timer1->Enabled)
				//	return;
			 }
	private: System::Void MyKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				if(e->KeyCode == Keys::R)
					m_pOrthogonalCamera->Reset();
				//if(e->KeyCode == Keys::C)
				//{
				//	char l_strFileName[2048];
				//   SYSTEMTIME l_SYSTEMTIME;
				//   GetSystemTime(&l_SYSTEMTIME);
				//   sprintf(l_strFileName,"%d%d%d%d%d%d.bmp",l_SYSTEMTIME.wYear,l_SYSTEMTIME.wMonth,l_SYSTEMTIME.wDay,l_SYSTEMTIME.wHour,l_SYSTEMTIME.wMinute,l_SYSTEMTIME.wSecond);
				//   SaveCurrentBufferToImage(l_strFileName,(int)DRAW_PANEL_RESOLUTION_WIDTH,(int)DRAW_PANEL_RESOLUTION_HEIGHT);
				//}
				if(!timer1->Enabled)
					return;
				if( m_iCurrentSelectedObjectIndex!=-1 )
				{
					cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
					Vector3	l_Pos = l_p->GetPos();
					if(e->KeyCode == Keys::Right)
						l_Pos.x+=1;
					if(e->KeyCode == Keys::Left)
						l_Pos.x-=1;
					if(e->KeyCode == Keys::Up)
						l_Pos.y-=1; 
					if(e->KeyCode == Keys::Down)
						l_Pos.y+=1;
					l_p->SetPos(l_Pos);
				}
			 }
private: System::Void Test_button_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			String^l_FileName = DNCT::SaveFileAndGetName("(*.pi)|*.pi");
			if( l_FileName )
			{
				SavePuzzleFile(l_FileName,false);
				if( BinaryExport_checkBox->Checked )
				{
					SavePuzzleFile(l_FileName,true);
				}
			}
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 GeneratePowOf2Image(sender==generateAllImageWithPowerOfTwoToolStripMenuItem?true:false);
		 }
private: System::Void ImageWidth_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {

			 TotalPixel_label->Text = (ImageHeight_numericUpDown->Value*ImageWidth_numericUpDown->Value).ToString();
		 }
private: System::Void InvertPuzzleImage_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 cli::array< String^ >^l_pNameList = DNCT::OpenFileAndGetNames("pi files (*.pi)|*.pi|All files (*.*)|*.*");
			 if( !l_pNameList )
				 return;
			 String^l_strDirectory = DNCT::SelectDirectory();
			 if( !l_strDirectory )
				 return;
			 if( l_pNameList )
			 for each (String ^l_fileName in l_pNameList)
			 {
				 cNodeISAX	l_cNodeISAX;
				 TiXmlElement*l_pRootTiXmlElement = 0;
				 bool	l_b = l_cNodeISAX.ParseDataIntoXMLNode(DNCT::GcStringToChar(l_fileName).c_str());
				 if( l_b )
				 {
					const WCHAR*	l_strFileName = 0;
					 //find the tag we needed
					l_pRootTiXmlElement	= l_cNodeISAX.GetXmlElementByNameFromRoot(L"PuzzleImage");
					while( l_pRootTiXmlElement )
					{
						l_strFileName = l_pRootTiXmlElement->Attribute(L"ImageName");
						System::Drawing::Bitmap^l_pBitmapSource;
						String^l_strPGileName = DNCT::GetDirectoryWithoutFileName(l_fileName)+String(l_strFileName).ToString();
						try
						{
							l_pBitmapSource = gcnew System::Drawing::Bitmap(l_strPGileName);
						}
						catch(System::Exception^l_pExecption)
						{
							l_pExecption->ToString();
							WARNING_MSG(l_strPGileName+" not existed");
							continue;
						}
						int	l_iCount = _wtoi(l_pRootTiXmlElement->Attribute(L"Count"));
						TiXmlElement*l_pPuzzleImageUintTiXmlElement = l_pRootTiXmlElement->FirstChildElement();
						while(l_pPuzzleImageUintTiXmlElement)
						{
							TiXmlElement*l_pCurrentElement  = l_pPuzzleImageUintTiXmlElement;
							TiXmlAttribute*l_pTiXmlAttribute = l_pCurrentElement->FirstAttribute();
							float	l_fUV[4];
							POINT	l_Size;
							POINT	l_OffsetPos;
							POINT	l_ShowPosInPI;
							POINT	l_OriginalSize;
							//new attribute so make sure the data has this
							bool	l_bContainShowPosInPI = false;
							const WCHAR*	l_strPuzzleImageName = 0;
							while( l_pTiXmlAttribute )
							{
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"UV") )
								{
									GetUV(l_pTiXmlAttribute->Value(),l_fUV);
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"Size") )
								{
									l_Size = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"OffsetPos") )
								{
									l_OffsetPos = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"Name") )
								{
									l_strPuzzleImageName = l_pTiXmlAttribute->Value();
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"ShowPosInPI") )
								{
									l_bContainShowPosInPI = true;
									l_ShowPosInPI = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"OriginalSize") )
								{
									l_OriginalSize = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								{
									assert(0);
								}
								l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
							}
							//fill all alpha
							System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap( l_OriginalSize.x,l_OriginalSize.y );
							System::Drawing::Color l_NewColor = System::Drawing::Color::Transparent;
							System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( l_NewColor );
							System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
							l_pGr->FillRectangle(l_pDrawBrush,0,0,l_pBitMap->Width,l_pBitMap->Height);
							System::Drawing::Rectangle	l_rect = System::Drawing::Rectangle((int)(l_fUV[0]*l_pBitmapSource->Width),(int)(l_fUV[1]*l_pBitmapSource->Height),l_Size.x,l_Size.y);
							if( l_bContainShowPosInPI )
							{
								l_rect = System::Drawing::Rectangle(l_ShowPosInPI.x,l_ShowPosInPI.y,l_Size.x,l_Size.y);
							}
							if( 0 )
							{
								////copy Src
								//int	bufferSizeInPixels = l_pBitMap->Width*l_pBitMap->Height;
								//char*l_pbuff = new char[bufferSizeInPixels*l_iSrcChannel];
								//BitmapData^l_pData =
								//l_pBitMap->LockBits(System::Drawing::Rectangle(0, 0, l_pBitMap->Width, l_pBitMap->Height),
								//ImageLockMode::WriteOnly,l_pBitmapForSave->PixelFormat);
								//memcpy(l_pbuff,l_pData->Scan0.ToPointer(),bufferSizeInPixels*l_iSrcChannel);
								//l_pBitmapForSave->UnlockBits(l_pData);
								////past to dest
								//l_pData = l_pBitMap->LockBits(System::Drawing::Rectangle(0, 0, l_pBitMap->Width, l_pBitMap->Height),ImageLockMode::WriteOnly,l_pBitmapForSave->PixelFormat);
								//char*l_strScrData = (char*)l_pData->Scan0.ToPointer();

								//int	l_iSrcRenderStartPosX = l_pUIImage->GetOffsetPos()->x;
								//int	l_iSrcRenderStartPosY = l_pUIImage->GetOffsetPos()->y;
								//+1 for offset start at 0,0
								//int	l_iSrcRenderEndPosX = l_pUIImage->GetRightDownStripOffPos().x+1;
								//int	l_iSrcRenderEndPosY = l_pUIImage->GetRightDownStripOffPos().y+1;
								//int	l_iDestRenderPosX = StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos()->x+l_pUIImage->GetOffsetPos()->x);
								//int	l_iDestRenderPosY = StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos()->y+l_pUIImage->GetOffsetPos()->y);
								//int	l_iWorkPixelX = l_iSrcRenderEndPosX-l_iSrcRenderStartPosX;
								//int	l_iWorkPixelY = l_iSrcRenderEndPosY-l_iSrcRenderStartPosY;
								//int	l_iIndex = 0;
								//for( int l_iStartPixelY=l_iSrcRenderStartPosY;l_iStartPixelY<l_iSrcRenderEndPosY;++l_iStartPixelY )
								//{
								//	int	l_iYIndex = ((l_iDestRenderPosY+l_iIndex)*l_iSrcChannel*l_pBitMap->Width);
								//	int	l_iXIndex = l_iSrcChannel*l_iDestRenderPosX;
								//	int	l_iStartCopyIndex = l_iXIndex+l_iYIndex;
								//	int	l_iCopyIntoIndex = (l_iStartPixelY*l_iSrcChannel*l_pBitmapForSave->Width)+(l_iSrcChannel*l_iSrcRenderStartPosX);
								//	memcpy(&l_strScrData[l_iStartCopyIndex],&l_pbuff[l_iCopyIntoIndex],l_iWorkPixelX*l_iSrcChannel);
								//	++l_iIndex;
								//}
								//l_pBitMap->UnlockBits(l_pData);
								//delete l_pbuff;
							}
							else
								l_pGr->DrawImage(l_pBitmapSource,l_OffsetPos.x,l_OffsetPos.y,l_rect,System::Drawing::GraphicsUnit::Pixel);
							l_pBitMap->Save(l_strDirectory+String(l_strPuzzleImageName).ToString()+".png");
							l_pPuzzleImageUintTiXmlElement = l_pPuzzleImageUintTiXmlElement->NextSiblingElement();
						}
						if( l_pRootTiXmlElement->NextSibling() )
							l_pRootTiXmlElement	= GetXmlElementByNameFromElement(L"PuzzleImage",l_pRootTiXmlElement->NextSiblingElement());
						else
							l_pRootTiXmlElement = 0;
					}
				 }
			 }
		 }
private: System::Void TO96PdiPngFile_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			String^l_strDirectory = DNCT::SelectDirectory();
			if( !l_strDirectory )
				return;
			cli::array<String^>^l_AllFileName = DNCT::OpenFileAndGetNames();
			for each(String^l_strName in l_AllFileName)
			{
				System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap(l_strName);
				l_pBitMap->Save(l_strDirectory+DNCT::GetFileNameWithoutFullPath(l_strName));
			}
			//System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( System::Drawing::Color::Transparent );
			//for( int i=0;i<m_pImageomposerIRM->Count();++i )
			//{
			//	cUIImage*l_pUIImage = (cUIImage*)((*m_pImageomposerIRM)[i]);
			//	System::Drawing::Bitmap^l_pBitmapForSave = (System::Drawing::Bitmap^)m_ImageTale[DNCT::WcharToGcstring((*m_pImageomposerIRM)[i]->GetName())];
			//	l_pBitMap->Save(l_strDirectory+DNCT::WcharToGcstring(l_pUIImage->GetName())+".png");
			//}
		}
private: System::Void openXMLToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::OpenFileAndGetName("PuzzleImage(*.pi;*.pib)|*.pi;*.pib");
			 //String^l_strFileName = DNCT::OpenFileAndGetName("txt files (*.txt)|*.txt|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				 OpenPIFile(l_strFileName);
			 }
		 }
private: System::Void AddAnimationImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == AddAnimationImage_button )
			 {
				 if( AllImage_listBox->SelectedIndices->Count == 0)
					 return;
				 while(AllImage_listBox->SelectedIndices->Count != 0 )
				 {
					int	l_iIndex = AllImage_listBox->SelectedIndices[0];
					float l_fTime = (float)AnimationTime_numericUpDown->Value;
					m_pImageIndexOfAnimation->AddNameObject(DNCT::GcStringToWchar(AllImage_listBox->SelectedItems[0]->ToString()).c_str(),AllImage_listBox->SelectedIndices[0], l_fTime);
					AnimationData_listBox->Items->Add(AllImage_listBox->SelectedItems[0]->ToString());
					AllImage_listBox->SelectedItems->Remove(AllImage_listBox->SelectedItems[0]);
				 }
			 }
			 else
			 if( sender == DelAnimationImage_button )
			 {
				 if( AnimationData_listBox->SelectedIndices->Count == 0 )
					 return;
				 while(this->AnimationData_listBox->SelectedIndices->Count)
				 {
					int	l_iIndex = AnimationData_listBox->SelectedIndices[0];
					m_pImageIndexOfAnimation->RemoveNameObject(l_iIndex);
					AnimationData_listBox->Items->RemoveAt(l_iIndex);
				  }
			 }
		 }
private: System::Void AnimationDatAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == AnimationDatAdd_button )
			 {
				if( AnimationDataName_textBox->Text->Length )
				{
				    if( m_pImageIndexOfAnimation->Count() )
				    {
					    if( !AnimationDataList_listBox->Items->Contains(AnimationDataName_textBox->Text) )
					    {
						    AnimationDataList_listBox->Items->Add(AnimationDataName_textBox->Text);
						    AnimationData_listBox->Items->Clear();
						    cImageIndexOfAnimation*l_pImageIndexOfAnimation = new cImageIndexOfAnimation(m_pImageIndexOfAnimation);
						    m_pImageIndexOfAnimation->Clear();
						    l_pImageIndexOfAnimation->SetName(DNCT::GcStringToWchar(AnimationDataName_textBox->Text));
						    m_pImageIndexOfAnimationList->AddObject(l_pImageIndexOfAnimation);
					    }
					}
				}
			 }
			 else
			 if( sender == AnimationDatDel_button )
			 {
				 while( AnimationDataList_listBox->SelectedItems->Count )
				 {
					 m_pImageIndexOfAnimationList->RemoveObject(DNCT::GcStringToWchar(AnimationDataList_listBox->SelectedItems[0]->ToString()));
					 AnimationDataList_listBox->Items->Remove(AnimationDataList_listBox->SelectedItems[0]);
				 }
			 }
		 }

private: System::Void ImagePosX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			int	l_iSelectedIndex = AllImage_listBox->SelectedIndex;
			if( l_iSelectedIndex != -1 )
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[l_iSelectedIndex]);
				l_pUIImage->SetPos(Vector3((float)ImagePosX_numericUpDown->Value,(float)ImagePosY_numericUpDown->Value,0.f));
			}
		 }
private: System::Void AllImage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if( AllImage_listBox->SelectedIndex != -1)
			 {
				 label3->Text = "Pos:"+AllImage_listBox->Text;
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[AllImage_listBox->SelectedIndex]);
				Vector3	l_vPos = l_pUIImage->GetPos();
				ImagePosX_numericUpDown->Value = (int)l_vPos.x;
				ImagePosY_numericUpDown->Value = (int)l_vPos.y;
			 }
		 }
private: System::Void AllImage_listBox_KeyUp_1(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if(e->KeyCode == Keys::Enter)
			 {
				 System::Windows::Forms::MouseEventArgs^  e2;
				AllImage_listBox_MouseDoubleClick(sender,e2);
			 }
		 }
private: System::Void AnimationData_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
        {
            AssignAnimationData();
        }
private: System::Void AnimationData_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
        {
            if( e->KeyCode == GCFORM::Keys::Enter )
                AssignAnimationData();
        }
private: System::Void ProperStripPixel_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
			POINT	l_MinOffset = {79979,79979};
			POINT	l_MaxRightDown = {0,0};
			POINT	l_OffsetPos;
			POINT	l_RightDown;
			for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
				l_OffsetPos = *l_pUIImage->GetOffsetPos();
				l_RightDown = l_pUIImage->GetRightDownStripOffPos();
				//+1 for offset start at 0,0
				l_RightDown.x +=1;
				l_RightDown.y +=1;

				l_MinOffset.x = min(l_MinOffset.x,l_OffsetPos.x);
				l_MinOffset.y = min(l_MinOffset.y,l_OffsetPos.y);
				l_MaxRightDown.x = max(l_MaxRightDown.x,l_RightDown.x);
				l_MaxRightDown.y = max(l_MaxRightDown.y,l_RightDown.y);
			}
			for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
				l_pUIImage->SetOffsetPos(l_MinOffset);
				l_pUIImage->SetRightDownStripOffPos(l_MaxRightDown);
			}
        }
private: System::Void StripAlpha_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( ToOriginalImage_button == sender )
			 {
				 for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
				 {
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
					POINT	l_WidthHeight = { l_pUIImage->GetWidth()-1,l_pUIImage->GetHeight()-1 };
					POINT	l_Offset = {0,0};
					l_pUIImage->SetRightDownStripOffPos(l_WidthHeight);
					l_pUIImage->SetOffsetPos(l_Offset);
				 }
				 AllImage_listBox_SelectedIndexChanged(sender,e);
			 }
			 else
			 if( StripAlpha_button == sender )
			 {
				 for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
				 {
					System::Drawing::Bitmap^l_pImage = (System::Drawing::Bitmap^)m_ImageTale[AllImage_listBox->Items[l_iSelectedIndex]->ToString()];
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
					l_pUIImage->SetOffsetPos(DNCT::ImageGetAlphaOffsetPosLeftTop(l_pImage));
					l_pUIImage->SetRightDownStripOffPos(DNCT::ImageGetAlphaOffsetPosRightDown(l_pImage));
					POINT	l_ImageRealSize = l_pUIImage->GetImageRealSize();
					POINT	l_RightDownStripOffPos = l_pUIImage->GetRightDownStripOffPos();
					POINT	l_OffsetPos = *l_pUIImage->GetOffsetPos();
					//because MPDI is setposition by center
					//if( l_ImageRealSize.x%2 )
					//{
					//	if( l_RightDownStripOffPos.x+1 < l_pUIImage->GetOriginalImageSize().x )
					//		l_RightDownStripOffPos.x += 1;
					//	else
					//	if( l_OffsetPos.x != 0 )
					//		l_OffsetPos.x -= 1;
					//}
					//if( l_ImageRealSize.y%2)
					//{
					//	if( l_RightDownStripOffPos.y+1 < l_pUIImage->GetOriginalImageSize().y )
					//		l_RightDownStripOffPos.y += 1;
					//	else
					//	if( l_OffsetPos.y != 0 )
					//		l_OffsetPos.y -= 1;
					//}
					l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
					l_pUIImage->SetOffsetPos(l_OffsetPos);
				 }
				 AllImage_listBox_SelectedIndexChanged(sender,e);
			 }
		 }

private: System::Void splitContainer2_Panel1_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			if( !m_pOrthogonalCamera )
				 return;
			POINT l_ViewportSize = { splitContainer2->Panel1->Width,splitContainer2->Panel1->Height };
			Vector2	l_vOriginalResolution = m_pOrthogonalCamera->GetResolution();
			Vector2	l_vOriginalLeftUpPos = m_pOrthogonalCamera->GetCameraPos()-(l_vOriginalResolution/2.f);
			m_pOrthogonalCamera->SetResolution(Vector2((float)l_ViewportSize.x,(float)l_ViewportSize.y));
			m_pOrthogonalCamera->SetCameraPos(l_vOriginalLeftUpPos+m_pOrthogonalCamera->GetResolution()/2.f);
			m_pOrthogonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2((float)splitContainer2->Panel1->Size.Width,(float)splitContainer2->Panel1->Size.Height));
		 }

private: System::Void AddNewPIUnitImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( NewPIUnitStartX_numericUpDown->Value != NewPIUnitEndX_numericUpDown->Value &&
				 NewPIUnitStartY_numericUpDown->Value != NewPIUnitEndY_numericUpDown->Value &&
				   NewPIUnitEndX_numericUpDown->Value >  NewPIUnitStartX_numericUpDown->Value && 
				   NewPIUnitEndY_numericUpDown->Value >  NewPIUnitStartY_numericUpDown->Value)
			 {
				 if( NewPIUnitName_textBox->Text->Length )
				 {
					 this->timer1->Enabled = false;
					 int	l_iOriginalIndex = -1;
					 POINT	l_OffsetPos = {0,0};
					 POINT	l_RightDownStripOffPos = {(int)NewPIUnitEndX_numericUpDown->Value-(int)NewPIUnitStartX_numericUpDown->Value,(int)NewPIUnitEndY_numericUpDown->Value-(int)NewPIUnitStartY_numericUpDown->Value};
					 Vector3	l_vPos((float)NewPIUnitStartX_numericUpDown->Value,(float)NewPIUnitStartY_numericUpDown->Value,0.f);
					 RECT	l_rc = {(int)l_vPos.x,(int)l_vPos.y,(int)l_vPos.x+l_RightDownStripOffPos.x,(int)l_vPos.y+l_RightDownStripOffPos.y};
					 int	l_iCollideIndex = -1;
					 for(int i = 0;i<m_pImageomposerIRM->Count();++i)
					 {
						cUIImage*l_pUIImage2 = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
						Vector3	l_vPos2 = l_pUIImage2->GetPos();
						l_vPos2.x += l_pUIImage2->GetOffsetPos()->x;
						l_vPos2.y += l_pUIImage2->GetOffsetPos()->y;
						POINT	l_RightDownStripOffPos2 = l_pUIImage2->GetRightDownStripOffPos();
						RECT	l_rc2 = {(int)l_vPos2.x,(int)l_vPos2.y,(int)l_vPos2.x+l_RightDownStripOffPos2.x,(int)l_vPos2.y+l_RightDownStripOffPos2.y};
						if( UT::RectCollideRect(l_rc2,l_rc) )
						{
							if( l_iCollideIndex != -1 )
							{
								if(!l_pUIImage2->m_pEditorAttachParent)
								{
									WARING_YES_NO_TO_NO("over 2 image would u like to do this anyway?")
									{
										goto LEAVE;
									}
									break;
								}
							}
							//ensure it is not a piunit child
							if(!l_pUIImage2->m_pEditorAttachParent)
								l_iCollideIndex = i;
						}
					 }
					 if( l_iCollideIndex == -1 )
					 {
						WARNING_MSG("no parent,please reset pos");
						goto LEAVE;
					 }
					 if(AllImage_listBox->Items->Contains(NewPIUnitName_textBox->Text))
					 {
						WARING_YES_NO_TO_NO("replace exist object?")
						{
							goto LEAVE;
						}
						l_iOriginalIndex = AllImage_listBox->Items->IndexOf(NewPIUnitName_textBox->Text);
						AllImage_listBox->SelectedIndex = l_iOriginalIndex;
						DeleteImage_button_Click(DeleteImage_button,e);
					 }
					 cUIImage*l_pAttachUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iCollideIndex));
					 cUIImage*l_pUIImage = new cUIImage(l_pAttachUIImage);
					 l_pUIImage->m_vEditorAttachParentRelativePos = l_vPos-l_pAttachUIImage->GetPos();
					 l_pUIImage->SetPos(l_vPos);
					 l_pUIImage->SetName(DNCT::GcStringToWchar(NewPIUnitName_textBox->Text));
					 l_pUIImage->SetOffsetPos(l_OffsetPos);
					 l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
					 l_RightDownStripOffPos.x += 1;
					 l_RightDownStripOffPos.y += 1;
					 l_pUIImage->SetOriginalImageSize(l_RightDownStripOffPos);
					 if( l_iOriginalIndex != -1 )
					 {
				        (*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
						AllImage_listBox->Items[l_iOriginalIndex] = NewPIUnitName_textBox->Text;
					 }
				     else
					 {
					    m_pImageomposerIRM->AddObject(l_pUIImage);
						AllImage_listBox->Items->Add(NewPIUnitName_textBox->Text);
					 }
					 LEAVE:
					 this->timer1->Enabled = true;
				 }
				 else
				 {
					WARNING_MSG("please input new PI unit name");
				 }
				 
			 }
			 else
			 {
				WARNING_MSG("data error");
			 }
		 }
	private: System::Void NewPIUnitStartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( AllImage_listBox->SelectedIndex == -1 || m_bAvoidDataAssignForPIUintChild )
			 {
				return;
			 }
			 cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(AllImage_listBox->SelectedIndex));
			 if( !l_pUIImage->m_pEditorAttachParent )
				 return;
			 POINT	l_RightDownStripOffPos = l_pUIImage->GetRightDownStripOffPos();
			 Vector3	l_vPos = l_pUIImage->GetPos();
			 l_RightDownStripOffPos.x = (int)(NewPIUnitEndX_numericUpDown->Value-NewPIUnitStartX_numericUpDown->Value);	
			 l_RightDownStripOffPos.y = (int)(NewPIUnitEndY_numericUpDown->Value-NewPIUnitStartY_numericUpDown->Value);
			 l_vPos.x = (float)NewPIUnitStartX_numericUpDown->Value;
			 l_vPos.y = (float)NewPIUnitStartY_numericUpDown->Value;
			 l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
			 l_pUIImage->SetPos(l_vPos);
			 l_RightDownStripOffPos.x += 1;
			 l_RightDownStripOffPos.y += 1;
			 l_pUIImage->SetOriginalImageSize(l_RightDownStripOffPos);
		 }
	private: System::Void floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 static	bool	l_bDock = false;
			 HWND	l_pHwnd = (HWND)this->Handle.ToPointer();
			 //HWND	l_pHwnd = FindMyTopMostWindow();
			 if(!this->m_pParentHandle)
			 {
				 //HWND	l_pThisHwnd = GetConsoleHwnd();
				 //m_pParentHandle = GetParent(l_pThisHwnd);
				 //DWORD pid = GetCurrentProcessId();
				 //HANDLE hProcess2 = OpenProcess(0, FALSE, pid);
				 //HANDLE hProcess2 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
				 //m_pParentHandle = GetParent((HWND)hProcess2);
				 //m_pParentHandle = GetParent((HWND)GetCurrentProcess());
				 //m_pParentHandle = (HWND)this->Parent->Handle.ToPointer();
				 //HWND	l_Hwnd2 = FindMyTopMostWindow();
				 //m_pParentHandle = GetParent(l_Hwnd2);

				 //m_pParentHandle = FindMyWindowByID(System::Int32::Parse(this->m_strParentProcessID));
				 //EnumWindows(EnumWindowProc, 0);
				 //m_pParentHandle = hwndMain;
				 m_pParentHandle = GetParent(l_pHwnd);
				 if( m_pParentHandle )
				 {
					WARNING_MSG("find");
				 }
				 else
					 WARNING_MSG("not find");
			 }
			 if( l_bDock )
			 {
				SetParent(l_pHwnd,m_pParentHandle);
				l_bDock = false;
				WARNING_MSG("Dock");
			 }
			 else
			 {
				 SetParent(l_pHwnd,0);
				 l_bDock = true;
				 WARNING_MSG("Float");
			 }
		 }
	private: System::Void fileToolStripMenuItem_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
			 {
					openXMLToolStripMenuItem->Owner->Show();
			 }
	private: System::Void addImagesByFolderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 addImagesByFolderToolStripMenuItem->Checked = !addImagesByFolderToolStripMenuItem->Checked;
			 }
	private: System::Void TriangulatorMouseBehavior_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator && TriangulatorMouseBehavior_comboBox->SelectedIndex != -1)
		{
			m_pCurrentSelectedPuzzleImageUnitTriangulator->SetPointsToTriangulatorType((ePointsToTriangulatorType)TriangulatorMouseBehavior_comboBox->SelectedIndex);
		}
	}

	private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			std::vector<Vector2>*l_pVector = m_pCurrentSelectedPuzzleImageUnitTriangulator->GetTriangulatorPointsVector();
			if (l_pVector->size() > 3)
			{
				
				cUIImage*l_pUIImage = m_pCurrentSelectedPuzzleImageUnitTriangulator->GetTargetImage();
				System::Drawing::Bitmap^l_pBitMap = (System::Drawing::Bitmap^)m_ImageTale[AllImage_listBox->Items[AllImage_listBox->SelectedIndex]->ToString()];
				List<System::Drawing::Point>^l_pPointList = Vector2ToListPoint(l_pVector);
				Image^l_pImage = (Image^)l_pBitMap;
				Bitmap^l_pFinalImage = GetSelectedArea(l_pImage, Color::Transparent, l_pPointList);
				Bitmap^l_pOriginalImage = (Bitmap^)m_ImageTale[AllImage_listBox->Items[AllImage_listBox->SelectedIndex]->ToString()];
				//pictureBox1->Image = l_pFinalImage;
				//pictureBox2->Image = (Image^)l_pOriginalImage;
				//l_pFinalImage->Save("New.png");
				//l_pOriginalImage->Save("Original.png");
			}
		}
	}
	private: System::Void ImageTriangulatorLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			if(m_pCurrentSelectedPuzzleImageUnitTriangulator->GetLOD() != (int)ImageTriangulatorLOD_numericUpDown->Value)
				m_pCurrentSelectedPuzzleImageUnitTriangulator->SetLOD((int)ImageTriangulatorLOD_numericUpDown->Value, false);
		}
	}
	private: System::Void AnimationData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (AnimationData_listBox->SelectedIndex != -1 && m_pImageIndexOfAnimation)
		{
			AnimationTime_numericUpDown->Value = (System::Decimal)m_pImageIndexOfAnimation->m_ImageAnimationDataList[AnimationData_listBox->SelectedIndex].fTimeGap;
		}
	}
};
	static GCFORM::Form^CallForm(System::String^e_strFileName);
//end namespace
}