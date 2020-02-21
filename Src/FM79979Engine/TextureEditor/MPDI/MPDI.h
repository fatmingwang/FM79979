#pragma once
#include "../../Core/GameplayUT/Render/VideoPlayer/avi_utils.h"
#include "../../Core/GameplayUT/Render/MPDI/MPDIForEditor.h"
#include "MPDIThread.h"
#include "DotMPDICamera.h"
//#pragma comment(lib, "Vfw32.lib")
#include "vfw.h"
#include "MPDIUI.h"
namespace MPDI 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	public ref class MPDIEditor : public System::Windows::Forms::Form
	//public ref class MPDIEditor : public System::Windows::Forms::UserControl
	{
	public:
		//MPDIEditor(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm,String^e_strFileName);
		MPDIEditor(String^e_strFileName);

	protected:
		~MPDIEditor();
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  AnimationInvert_button;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  AnimationName_textBox;
	private: System::Windows::Forms::ListBox^  WholeAnimationList_listBox;
	private: System::Windows::Forms::Button^  DeleteListObject_button;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::Button^  AddAnimationToList_button;
	private: System::Windows::Forms::ListBox^  ShowImageOnScreen_listBox;
	private: System::Windows::Forms::Button^  AddShowImageOnScreenObject_button;
	private: System::Windows::Forms::Button^  DelShowImageOnScreenObject_button;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::ToolStripMenuItem^  editModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addPointModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  fixPointModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  moveWholePathToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openImageFileToolStripMenuItemToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  moveAllPathToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::ToolStripMenuItem^  rotatePathToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  rotateAllPathToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  AllPath_listBox;
	private: System::Windows::Forms::ToolStripMenuItem^  openPathFileToolStripMenuItem;
	private: System::Windows::Forms::Button^  DelPathIntoAllPath_button;
	private: System::Windows::Forms::Button^  AddPathIntoAllPath_button;
	private: System::Windows::Forms::Button^  UsingIntoCurrentPath_button;
	private: System::Windows::Forms::ToolStripMenuItem^  savePathFileToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  EventPointName_textBox;
	private: System::Windows::Forms::ComboBox^  HintPointType_comboBox;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Button^  DelEventPoint_button;
	private: System::Windows::Forms::Button^  AddEventPOint_button;
	private: System::Windows::Forms::ToolStripMenuItem^  assistantModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showImageBoderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showSelectedImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveGamePathToolStripMenuItem;
	private: System::Windows::Forms::Button^  MergeMPDIData_button;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAviFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showPath_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showErrorMsh_ToolStripMenuItem;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::ComboBox^  GroupRotationType_comboBox;
	private: System::Windows::Forms::NumericUpDown^  GroupPosY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupPosZ_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupPosX_numericUpDown;
	private: System::Windows::Forms::ToolStripMenuItem^  showPanelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showPIImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AddPI_toolStripMenuItem;
	private: System::Windows::Forms::Button^  ApplyGroupDataChange_button;
	private: System::Windows::Forms::NumericUpDown^  GroupScale_numericUpDown;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::NumericUpDown^  GroupRotationZ_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupRotationY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupRotationX_numericUpDown;
	private: System::Windows::Forms::ComboBox^  GroupPositionMoveType_comboBox;
	private: System::Windows::Forms::Label^  label29;
	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::ToolStripMenuItem^  ScaleWithPIAlphaStripPixel_toolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  GroupImageRotate_checkBox;
	private: System::Windows::Forms::Button^  WholeAnimationListUp_button;
	private: System::Windows::Forms::Button^  WholeAnimationListDown_lbutton;
	private: System::Windows::Forms::ToolStripMenuItem^  saveImageToolStripMenuItem;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel2;
	private: System::Windows::Forms::ToolStripMenuItem^  renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  addPointDataWithImageByPathNameToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  hockToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  floatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  childToolStripMenuItem;
	private: System::Windows::Forms::Panel^  GroupData_collpaseForm;
	private: System::Windows::Forms::Panel^  AnimationData_collpaseForm;
	private: System::Windows::Forms::GroupBox^  Path_groupBox;
	private: System::Windows::Forms::GroupBox^  ImageHintPoint_groupBox;
	private: System::Windows::Forms::GroupBox^  HintPoint_groupBox;
	private: System::Windows::Forms::ListBox^  AllEventPointHint_listBox;
	private: System::Windows::Forms::GroupBox^  Video_groupBox;
	private: System::Windows::Forms::ToolStripMenuItem^  FadeIn_toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  FadeOut_toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  flashSelectedImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mergeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraFitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showBGToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showGridToolStripMenuItem;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationZ_numericUpDown;
	private: System::Windows::Forms::Label^  CameraRotation_label3;
	private: System::Windows::Forms::Label^  CameraRotationCenter_label;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationCenterY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationCenterX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationCenterZ_numericUpDown;
	private: System::Windows::Forms::TabControl^  Edit_tabControl;
	private: System::Windows::Forms::TabPage^  Path_tabPage;
	private: System::Windows::Forms::TabPage^  Camera_tabPage;
	private: System::Windows::Forms::Button^  EventFindFile_button;
	public: System::Windows::Forms::CheckBox^  StayAtLastFrame_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  resolutionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x1080ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x720ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x480ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x1280ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x1024ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x1334ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x2208ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x2048ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x2732ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x900ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x1920ToolStripMenuItem;
	private: XMLDataEditor::XMLForm^  MPDIExtra_xmlNodeEditComponent;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel3;
	private: System::Windows::Forms::TrackBar^  CurrentProgress_trackBar;
	public: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::CheckBox^  MPDIWorking_checkBox;
	private: System::Windows::Forms::CheckBox^  CameraWorking_checkBox;
	public: System::Windows::Forms::CheckBox^  AllPlay_checkBox;
	public: System::Windows::Forms::CheckBox^  AllCurveLoop_checkBox;
	private: System::Windows::Forms::Panel^  BGColor_panel;
private: System::Windows::Forms::ToolStripMenuItem^  toGifToolStripMenuItem;
private:

	//my
	CoreWPF::Common::ColorPicker^m_pBGColorPicker;
	private: System::Void ColorChanged(System::Object^  sender, System::EventArgs^  e);
	MPDIUI^m_pMPDIUI;
	cDotNEtMPDICamera^m_pMPDICamera;
	public: void	Active(){wglMakeCurrent( m_HdcMV,m_HGLRCMV ); this->timer1->Enabled = true; }
	//main form text
	public: GCFORM::Form^	m_pForm;
	public:HWND					m_pParentHandle;
	//for attach from.
	private:GCFORM::TabPage^m_pTabPage;
	//to determin is tabpage clicked.
	private:GCFORM::TabControl^m_pTabControl;
	//each mpdi data could define a in game data it will store as another extra file.
	//String,GCFORM::TreeNode.from MPDIExtra_xmlNodeEditComponent
	private:System::Collections::Hashtable^m_pMPDIExtraData;
	private:HGLRC			m_HGLRCMV;
	private:HDC				m_HdcMV;
	private:cPuzzleImage*	m_pCurrentWorkingPI;//for multi pi,get the current selected
	private:String^			m_strCurrentFileName;
	private:void			UIInitialize();
	private:void			MPDIRender();
	private:void			AssignValueChange(System::Object^  sender);
	private:void			ImageApply();
	private:void			AddIntoList();							//add new m_pMultiPathDynamicImageAWP->m_pCurrentCurve into m_pMPDIList
	private:float			m_fTimeForCurrentProgess;				//for m_pMultiPathDynamicImageAWP
	private:bool			m_bAnimationPauseByTrackValue;
	//sometimes we do not want to change value,so set this as flag to prevent
	private:bool			m_bPreventChangeValue;
	//while invert MPDI data botton is click,we do not expect it change the path data time
	private:bool			m_bPreventChangeAllTimeValueSetup;
	//
	private:bool			m_bMouseEnterPathNodeListbox;
	private:cGlyphFontRender*m_pDebugFont;
	private:System::Void	MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void	MyMouseHover(System::Object^  sender, System::EventArgs^  e);
	private:System::Void	MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void	MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:void			PathModifyFunction(System::Object^  sender, System::EventArgs^  e);
	private:void			PathMoveModifyMode(System::Object^  sender, System::EventArgs^  e);
	private:void			PathSelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);//because the code is too long,so.....-_-
	private:void			DelPoint(System::Object^  sender, System::EventArgs^  e);
	private:void			PlayCheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private:void			AddPath(System::Object^  sender, System::EventArgs^  e);
	private:void			AddPath(String^e_strPathName);
	private:void			SaveAndOpen_MPDI(System::Object^  sender, System::EventArgs^  e);
	private:void			ReferencePathModify(System::Object^  sender, System::EventArgs^  e);
	private:System::Void	EventPoint(System::Object^  sender, System::EventArgs^  e);
	private:System::Void	AddGameUI(System::Object^  sender);
	//how many time opened
	cAnimationParser*				m_pImagePlayManagerAWP;
	cCurveManager*					m_pReferenceCurveManagerAWP;
	cBaseImage*						m_pBKImageAWP;
	cMPDIList*						m_pMPDIList;
	cImageParser*					m_pImageResourceManagerAWP;
	cMPDIForEditor*					m_pMultiPathDynamicImageAWP;
	UT::sTimeCounter*				m_pTimeCounterForAutoSaveFile;
	cOrthogonalCamera				*m_pOrthogonalCamera;
	float							m_fAlpha;
	public:bool						m_bDataChanged;
	//just avoid the numerialBoxto reset data ecome loop
	//so stupid here but who care
	int								m_iCurrentSelectedPathListIndexAWP;
	int								m_iCurrentSelectedPathNodeIndexAWP;
	//just for image pos info let artist easier to adjust position
	cCueToStartCurveWithTime*		m_pInterlUseForShowImagePos;
	GLenum							m_sfactor;
	GLenum							m_dfactor;
	UT::sTimeAndFPS*				m_pTimeAndFPS;
	cGameApp*						m_pGameApp;
	bool							m_bTimerInRun;
	//
	cMPDIThread^	m_pMPDIThread;
	Vector2*		m_pvResolution;
	Vector4*		m_pvBGColor;
	String^			m_strCurrentSaveFileName;
	public:void		OpenMPDIFile(String^e_strFileName);
	public:void		Save(String^e_strFileName);
	public:void		Merge(String^e_strFileName);
	public:void		AddPIFile(String^e_strFileName);
	public:void		OpenPIFile(String^e_strFileName);
    void			CurrentProgressValueChange(int e_iSelection);
	public:System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->FadeIn_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FadeOut_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addPointDataWithImageByPathNameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->backgroundImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openImageFileToolStripMenuItemToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddPI_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openPathFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->savePathFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveGamePathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAviFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mergeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addPointModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fixPointModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->moveWholePathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->moveAllPathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->rotatePathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->rotateAllPathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->assistantModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showImageBoderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showSelectedImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPath_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showErrorMsh_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPanelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPIImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->flashSelectedImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraFitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showBGToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hockToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->floatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->childToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resolutionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x480ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x1280ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x720ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x900ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x1334ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x1024ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x1080ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x2208ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x2048ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x2732ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x1920ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->Edit_tabControl = (gcnew System::Windows::Forms::TabControl());
			this->Path_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->Camera_tabPage = (gcnew System::Windows::Forms::TabPage());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->flowLayoutPanel3 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->AllPlay_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AllCurveLoop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->StayAtLastFrame_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->CurrentProgress_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->MPDIWorking_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->CameraWorking_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->BGColor_panel = (gcnew System::Windows::Forms::Panel());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->AnimationData_collpaseForm = (gcnew System::Windows::Forms::Panel());
			this->WholeAnimationList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->WholeAnimationListUp_button = (gcnew System::Windows::Forms::Button());
			this->MergeMPDIData_button = (gcnew System::Windows::Forms::Button());
			this->WholeAnimationListDown_lbutton = (gcnew System::Windows::Forms::Button());
			this->AnimationName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DeleteListObject_button = (gcnew System::Windows::Forms::Button());
			this->AnimationInvert_button = (gcnew System::Windows::Forms::Button());
			this->AddAnimationToList_button = (gcnew System::Windows::Forms::Button());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->GroupData_collpaseForm = (gcnew System::Windows::Forms::Panel());
			this->CameraRotationCenter_label = (gcnew System::Windows::Forms::Label());
			this->CameraRotationCenterY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationCenterX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationCenterZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->GroupScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotation_label3 = (gcnew System::Windows::Forms::Label());
			this->CameraRotationY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupRotationX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupPosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->GroupPosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->GroupPosZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupImageRotate_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->ApplyGroupDataChange_button = (gcnew System::Windows::Forms::Button());
			this->GroupPositionMoveType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->GroupRotationZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->GroupRotationY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupRotationType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->flowLayoutPanel2 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->Path_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->AddPathIntoAllPath_button = (gcnew System::Windows::Forms::Button());
			this->AllPath_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelPathIntoAllPath_button = (gcnew System::Windows::Forms::Button());
			this->UsingIntoCurrentPath_button = (gcnew System::Windows::Forms::Button());
			this->ImageHintPoint_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->ShowImageOnScreen_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelShowImageOnScreenObject_button = (gcnew System::Windows::Forms::Button());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->AddShowImageOnScreenObject_button = (gcnew System::Windows::Forms::Button());
			this->HintPoint_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->EventFindFile_button = (gcnew System::Windows::Forms::Button());
			this->AllEventPointHint_listBox = (gcnew System::Windows::Forms::ListBox());
			this->HintPointType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->AddEventPOint_button = (gcnew System::Windows::Forms::Button());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->EventPointName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DelEventPoint_button = (gcnew System::Windows::Forms::Button());
			this->Video_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->toGifToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->Edit_tabControl->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer3))->BeginInit();
			this->splitContainer3->Panel2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->flowLayoutPanel3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CurrentProgress_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer2))->BeginInit();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->AnimationData_collpaseForm->SuspendLayout();
			this->GroupData_collpaseForm->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationCenterY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationCenterX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationCenterZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupScale_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupRotationX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupPosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupPosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupPosZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupRotationZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupRotationY_numericUpDown))->BeginInit();
			this->tabPage2->SuspendLayout();
			this->flowLayoutPanel2->SuspendLayout();
			this->Path_groupBox->SuspendLayout();
			this->ImageHintPoint_groupBox->SuspendLayout();
			this->HintPoint_groupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// FadeIn_toolStripMenuItem
			// 
			this->FadeIn_toolStripMenuItem->Name = L"FadeIn_toolStripMenuItem";
			this->FadeIn_toolStripMenuItem->Size = System::Drawing::Size(125, 22);
			this->FadeIn_toolStripMenuItem->Text = L"FadeIn";
			this->FadeIn_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::FadeIn_toolStripMenuItem_Click);
			// 
			// FadeOut_toolStripMenuItem
			// 
			this->FadeOut_toolStripMenuItem->Name = L"FadeOut_toolStripMenuItem";
			this->FadeOut_toolStripMenuItem->Size = System::Drawing::Size(125, 22);
			this->FadeOut_toolStripMenuItem->Text = L"FadeOut";
			this->FadeOut_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::FadeIn_toolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(291, 22);
			this->toolStripMenuItem1->Text = L"Add Group Path";
			this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &MPDIEditor::toolStripMenuItem1_Click);
			// 
			// addPointDataWithImageByPathNameToolStripMenuItem
			// 
			this->addPointDataWithImageByPathNameToolStripMenuItem->Name = L"addPointDataWithImageByPathNameToolStripMenuItem";
			this->addPointDataWithImageByPathNameToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->addPointDataWithImageByPathNameToolStripMenuItem->Text = L"AddPointDataWithImageByPathName";
			this->addPointDataWithImageByPathNameToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointDataWithImageByPathNameToolStripMenuItem_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &MPDIEditor::timer1_Tick);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->backgroundImageToolStripMenuItem,
					this->editModeToolStripMenuItem, this->assistantModeToolStripMenuItem, this->hockToolStripMenuItem, this->resolutionToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1904, 24);
			this->menuStrip1->TabIndex = 120;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// backgroundImageToolStripMenuItem
			// 
			this->backgroundImageToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {
				this->openToolStripMenuItem,
					this->saveToolStripMenuItem, this->openImageFileToolStripMenuItemToolStripMenuItem, this->AddPI_toolStripMenuItem, this->toolStripMenuItem2,
					this->openPathFileToolStripMenuItem, this->savePathFileToolStripMenuItem, this->saveGamePathToolStripMenuItem, this->saveAviFileToolStripMenuItem,
					this->saveImageToolStripMenuItem, this->mergeToolStripMenuItem, this->toGifToolStripMenuItem
			});
			this->backgroundImageToolStripMenuItem->Name = L"backgroundImageToolStripMenuItem";
			this->backgroundImageToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->backgroundImageToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openToolStripMenuItem_Click);
			// 
			// openImageFileToolStripMenuItemToolStripMenuItem
			// 
			this->openImageFileToolStripMenuItemToolStripMenuItem->Name = L"openImageFileToolStripMenuItemToolStripMenuItem";
			this->openImageFileToolStripMenuItemToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->openImageFileToolStripMenuItemToolStripMenuItem->Text = L"Open PI/打開PI檔";
			this->openImageFileToolStripMenuItemToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::OpenImageFile_button_Click);
			// 
			// AddPI_toolStripMenuItem
			// 
			this->AddPI_toolStripMenuItem->Name = L"AddPI_toolStripMenuItem";
			this->AddPI_toolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->AddPI_toolStripMenuItem->Text = L"AddPI/增加PI檔";
			this->AddPI_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::OpenImageFile_button_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(236, 22);
			this->toolStripMenuItem2->Text = L"BackgroundImage/背景圖片";
			this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &MPDIEditor::BKImage_button_Click);
			// 
			// openPathFileToolStripMenuItem
			// 
			this->openPathFileToolStripMenuItem->Name = L"openPathFileToolStripMenuItem";
			this->openPathFileToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->openPathFileToolStripMenuItem->Text = L"OpenPathFile";
			this->openPathFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openPathFileToolStripMenuItem_Click);
			// 
			// savePathFileToolStripMenuItem
			// 
			this->savePathFileToolStripMenuItem->Name = L"savePathFileToolStripMenuItem";
			this->savePathFileToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->savePathFileToolStripMenuItem->Text = L"SavePathFile";
			this->savePathFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openPathFileToolStripMenuItem_Click);
			// 
			// saveGamePathToolStripMenuItem
			// 
			this->saveGamePathToolStripMenuItem->Name = L"saveGamePathToolStripMenuItem";
			this->saveGamePathToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->saveGamePathToolStripMenuItem->Text = L"SaveGamePath/儲存遊戲路徑";
			this->saveGamePathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openPathFileToolStripMenuItem_Click);
			// 
			// saveAviFileToolStripMenuItem
			// 
			this->saveAviFileToolStripMenuItem->Name = L"saveAviFileToolStripMenuItem";
			this->saveAviFileToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->saveAviFileToolStripMenuItem->Text = L"SaveAviFile";
			this->saveAviFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::saveAviFileToolStripMenuItem_Click);
			// 
			// saveImageToolStripMenuItem
			// 
			this->saveImageToolStripMenuItem->Name = L"saveImageToolStripMenuItem";
			this->saveImageToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->saveImageToolStripMenuItem->Text = L"SaveImage";
			this->saveImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::saveImageToolStripMenuItem_Click);
			// 
			// mergeToolStripMenuItem
			// 
			this->mergeToolStripMenuItem->Name = L"mergeToolStripMenuItem";
			this->mergeToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->mergeToolStripMenuItem->Text = L"Merge";
			this->mergeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::mergeToolStripMenuItem_Click);
			// 
			// editModeToolStripMenuItem
			// 
			this->editModeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->addPointModeToolStripMenuItem,
					this->fixPointModeToolStripMenuItem, this->moveWholePathToolStripMenuItem, this->moveAllPathToolStripMenuItem, this->rotatePathToolStripMenuItem,
					this->rotateAllPathToolStripMenuItem
			});
			this->editModeToolStripMenuItem->Name = L"editModeToolStripMenuItem";
			this->editModeToolStripMenuItem->Size = System::Drawing::Size(127, 20);
			this->editModeToolStripMenuItem->Text = L"EditMode/編輯模式";
			// 
			// addPointModeToolStripMenuItem
			// 
			this->addPointModeToolStripMenuItem->Checked = true;
			this->addPointModeToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->addPointModeToolStripMenuItem->Name = L"addPointModeToolStripMenuItem";
			this->addPointModeToolStripMenuItem->Size = System::Drawing::Size(245, 22);
			this->addPointModeToolStripMenuItem->Text = L"AddPointMode/增加";
			this->addPointModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// fixPointModeToolStripMenuItem
			// 
			this->fixPointModeToolStripMenuItem->Name = L"fixPointModeToolStripMenuItem";
			this->fixPointModeToolStripMenuItem->Size = System::Drawing::Size(245, 22);
			this->fixPointModeToolStripMenuItem->Text = L"FixPointMode/修改";
			this->fixPointModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// moveWholePathToolStripMenuItem
			// 
			this->moveWholePathToolStripMenuItem->Name = L"moveWholePathToolStripMenuItem";
			this->moveWholePathToolStripMenuItem->Size = System::Drawing::Size(245, 22);
			this->moveWholePathToolStripMenuItem->Text = L"MoveWholePath/移動整條路徑";
			this->moveWholePathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// moveAllPathToolStripMenuItem
			// 
			this->moveAllPathToolStripMenuItem->Name = L"moveAllPathToolStripMenuItem";
			this->moveAllPathToolStripMenuItem->Size = System::Drawing::Size(245, 22);
			this->moveAllPathToolStripMenuItem->Text = L"MoveAllPath/移動所有路徑";
			this->moveAllPathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// rotatePathToolStripMenuItem
			// 
			this->rotatePathToolStripMenuItem->Name = L"rotatePathToolStripMenuItem";
			this->rotatePathToolStripMenuItem->Size = System::Drawing::Size(245, 22);
			this->rotatePathToolStripMenuItem->Text = L"RotatePath/旋轉路徑";
			this->rotatePathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// rotateAllPathToolStripMenuItem
			// 
			this->rotateAllPathToolStripMenuItem->Name = L"rotateAllPathToolStripMenuItem";
			this->rotateAllPathToolStripMenuItem->Size = System::Drawing::Size(245, 22);
			this->rotateAllPathToolStripMenuItem->Text = L"RotateAllPath/旋轉所有路徑";
			this->rotateAllPathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// assistantModeToolStripMenuItem
			// 
			this->assistantModeToolStripMenuItem->Checked = true;
			this->assistantModeToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->assistantModeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {
				this->showImageBoderToolStripMenuItem,
					this->showSelectedImageToolStripMenuItem, this->showPath_ToolStripMenuItem, this->showErrorMsh_ToolStripMenuItem, this->showPanelToolStripMenuItem,
					this->showPIImageToolStripMenuItem, this->ScaleWithPIAlphaStripPixel_toolStripMenuItem, this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem,
					this->flashSelectedImageToolStripMenuItem, this->cameraFitToolStripMenuItem, this->showBGToolStripMenuItem, this->showGridToolStripMenuItem
			});
			this->assistantModeToolStripMenuItem->Name = L"assistantModeToolStripMenuItem";
			this->assistantModeToolStripMenuItem->Size = System::Drawing::Size(154, 20);
			this->assistantModeToolStripMenuItem->Text = L"AssistantMode/輔助模式";
			// 
			// showImageBoderToolStripMenuItem
			// 
			this->showImageBoderToolStripMenuItem->Name = L"showImageBoderToolStripMenuItem";
			this->showImageBoderToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showImageBoderToolStripMenuItem->Text = L"ShowImageBorder/顯示圖片邊框";
			this->showImageBoderToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// showSelectedImageToolStripMenuItem
			// 
			this->showSelectedImageToolStripMenuItem->Name = L"showSelectedImageToolStripMenuItem";
			this->showSelectedImageToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showSelectedImageToolStripMenuItem->Text = L"ShowSelectedImage/顯示選擇圖片";
			this->showSelectedImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// showPath_ToolStripMenuItem
			// 
			this->showPath_ToolStripMenuItem->Checked = true;
			this->showPath_ToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showPath_ToolStripMenuItem->Name = L"showPath_ToolStripMenuItem";
			this->showPath_ToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showPath_ToolStripMenuItem->Text = L"ShowPath/顯示線段";
			this->showPath_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// showErrorMsh_ToolStripMenuItem
			// 
			this->showErrorMsh_ToolStripMenuItem->Checked = true;
			this->showErrorMsh_ToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showErrorMsh_ToolStripMenuItem->Name = L"showErrorMsh_ToolStripMenuItem";
			this->showErrorMsh_ToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showErrorMsh_ToolStripMenuItem->Text = L"ShowErrorMsh/顯示錯誤訊息";
			// 
			// showPanelToolStripMenuItem
			// 
			this->showPanelToolStripMenuItem->Name = L"showPanelToolStripMenuItem";
			this->showPanelToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showPanelToolStripMenuItem->Text = L"ShowPanel/顯示面板";
			this->showPanelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// showPIImageToolStripMenuItem
			// 
			this->showPIImageToolStripMenuItem->Checked = true;
			this->showPIImageToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showPIImageToolStripMenuItem->Name = L"showPIImageToolStripMenuItem";
			this->showPIImageToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showPIImageToolStripMenuItem->Text = L"Show PI Image/顯示pi圖片";
			this->showPIImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// ScaleWithPIAlphaStripPixel_toolStripMenuItem
			// 
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Name = L"ScaleWithPIAlphaStripPixel_toolStripMenuItem";
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Text = L"ScaleWithPIAlphaStripPixelProble/縮放pi檔案中去除的pizel問題";
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::ScaleWithPIAlphaStripPixel_toolStripMenuItem_Click);
			// 
			// renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem
			// 
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Name = L"renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem";
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Text = L"RenewImageSize(ignore all animation with size)";
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem_Click);
			// 
			// flashSelectedImageToolStripMenuItem
			// 
			this->flashSelectedImageToolStripMenuItem->Name = L"flashSelectedImageToolStripMenuItem";
			this->flashSelectedImageToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->flashSelectedImageToolStripMenuItem->Text = L"FlashSelectedImage";
			this->flashSelectedImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// cameraFitToolStripMenuItem
			// 
			this->cameraFitToolStripMenuItem->Checked = true;
			this->cameraFitToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cameraFitToolStripMenuItem->Name = L"cameraFitToolStripMenuItem";
			this->cameraFitToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->cameraFitToolStripMenuItem->Text = L"CameraFit";
			this->cameraFitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// showBGToolStripMenuItem
			// 
			this->showBGToolStripMenuItem->Checked = true;
			this->showBGToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showBGToolStripMenuItem->Name = L"showBGToolStripMenuItem";
			this->showBGToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showBGToolStripMenuItem->Text = L"ShowBG/顯示背景圖片";
			this->showBGToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// showGridToolStripMenuItem
			// 
			this->showGridToolStripMenuItem->Checked = true;
			this->showGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showGridToolStripMenuItem->Name = L"showGridToolStripMenuItem";
			this->showGridToolStripMenuItem->Size = System::Drawing::Size(419, 22);
			this->showGridToolStripMenuItem->Text = L"ShowGrid";
			this->showGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// hockToolStripMenuItem
			// 
			this->hockToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->floatToolStripMenuItem,
					this->childToolStripMenuItem
			});
			this->hockToolStripMenuItem->Enabled = false;
			this->hockToolStripMenuItem->Name = L"hockToolStripMenuItem";
			this->hockToolStripMenuItem->Size = System::Drawing::Size(46, 20);
			this->hockToolStripMenuItem->Text = L"Dock";
			// 
			// floatToolStripMenuItem
			// 
			this->floatToolStripMenuItem->Name = L"floatToolStripMenuItem";
			this->floatToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->floatToolStripMenuItem->Text = L"Float";
			this->floatToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::floatToolStripMenuItem_Click);
			// 
			// childToolStripMenuItem
			// 
			this->childToolStripMenuItem->Name = L"childToolStripMenuItem";
			this->childToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->childToolStripMenuItem->Text = L"Child";
			this->childToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::floatToolStripMenuItem_Click);
			// 
			// resolutionToolStripMenuItem
			// 
			this->resolutionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {
				this->x480ToolStripMenuItem,
					this->x1280ToolStripMenuItem, this->x720ToolStripMenuItem, this->x900ToolStripMenuItem, this->x1334ToolStripMenuItem, this->x1024ToolStripMenuItem,
					this->x1080ToolStripMenuItem, this->x2208ToolStripMenuItem, this->x2048ToolStripMenuItem, this->x2732ToolStripMenuItem, this->x1920ToolStripMenuItem
			});
			this->resolutionToolStripMenuItem->Name = L"resolutionToolStripMenuItem";
			this->resolutionToolStripMenuItem->Size = System::Drawing::Size(75, 20);
			this->resolutionToolStripMenuItem->Text = L"Resolution";
			// 
			// x480ToolStripMenuItem
			// 
			this->x480ToolStripMenuItem->Name = L"x480ToolStripMenuItem";
			this->x480ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x480ToolStripMenuItem->Text = L"640x480";
			this->x480ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x1280ToolStripMenuItem
			// 
			this->x1280ToolStripMenuItem->Checked = true;
			this->x1280ToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->x1280ToolStripMenuItem->Name = L"x1280ToolStripMenuItem";
			this->x1280ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x1280ToolStripMenuItem->Text = L"720x1280";
			this->x1280ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x720ToolStripMenuItem
			// 
			this->x720ToolStripMenuItem->Name = L"x720ToolStripMenuItem";
			this->x720ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x720ToolStripMenuItem->Text = L"1280x720";
			this->x720ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x900ToolStripMenuItem
			// 
			this->x900ToolStripMenuItem->Name = L"x900ToolStripMenuItem";
			this->x900ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x900ToolStripMenuItem->Text = L"1440x900";
			this->x900ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x1334ToolStripMenuItem
			// 
			this->x1334ToolStripMenuItem->Name = L"x1334ToolStripMenuItem";
			this->x1334ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x1334ToolStripMenuItem->Text = L"1334x750";
			this->x1334ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x1024ToolStripMenuItem
			// 
			this->x1024ToolStripMenuItem->Name = L"x1024ToolStripMenuItem";
			this->x1024ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x1024ToolStripMenuItem->Text = L"1600x1024";
			this->x1024ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x1080ToolStripMenuItem
			// 
			this->x1080ToolStripMenuItem->Name = L"x1080ToolStripMenuItem";
			this->x1080ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x1080ToolStripMenuItem->Text = L"1920x1080";
			this->x1080ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x2208ToolStripMenuItem
			// 
			this->x2208ToolStripMenuItem->Name = L"x2208ToolStripMenuItem";
			this->x2208ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x2208ToolStripMenuItem->Text = L"2208x1242";
			this->x2208ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x2048ToolStripMenuItem
			// 
			this->x2048ToolStripMenuItem->Name = L"x2048ToolStripMenuItem";
			this->x2048ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x2048ToolStripMenuItem->Text = L"2048x1536";
			this->x2048ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x2732ToolStripMenuItem
			// 
			this->x2732ToolStripMenuItem->Name = L"x2732ToolStripMenuItem";
			this->x2732ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x2732ToolStripMenuItem->Text = L"2732x2048";
			this->x2732ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// x1920ToolStripMenuItem
			// 
			this->x1920ToolStripMenuItem->Name = L"x1920ToolStripMenuItem";
			this->x1920ToolStripMenuItem->Size = System::Drawing::Size(128, 22);
			this->x1920ToolStripMenuItem->Text = L"1080x1920";
			this->x1920ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::x1080ToolStripMenuItem_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->AutoScroll = true;
			this->splitContainer1->Panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->splitContainer1->Panel1->Controls->Add(this->Edit_tabControl);
			this->splitContainer1->Panel1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer3);
			this->splitContainer1->Size = System::Drawing::Size(1613, 941);
			this->splitContainer1->SplitterDistance = 304;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 145;
			// 
			// Edit_tabControl
			// 
			this->Edit_tabControl->Controls->Add(this->Path_tabPage);
			this->Edit_tabControl->Controls->Add(this->Camera_tabPage);
			this->Edit_tabControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Edit_tabControl->Location = System::Drawing::Point(0, 0);
			this->Edit_tabControl->Name = L"Edit_tabControl";
			this->Edit_tabControl->SelectedIndex = 0;
			this->Edit_tabControl->Size = System::Drawing::Size(1609, 300);
			this->Edit_tabControl->TabIndex = 0;
			// 
			// Path_tabPage
			// 
			this->Path_tabPage->AutoScroll = true;
			this->Path_tabPage->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Path_tabPage->Location = System::Drawing::Point(4, 22);
			this->Path_tabPage->Name = L"Path_tabPage";
			this->Path_tabPage->Padding = System::Windows::Forms::Padding(3);
			this->Path_tabPage->Size = System::Drawing::Size(1601, 274);
			this->Path_tabPage->TabIndex = 0;
			this->Path_tabPage->Text = L"Path";
			// 
			// Camera_tabPage
			// 
			this->Camera_tabPage->Location = System::Drawing::Point(4, 22);
			this->Camera_tabPage->Name = L"Camera_tabPage";
			this->Camera_tabPage->Padding = System::Windows::Forms::Padding(3);
			this->Camera_tabPage->Size = System::Drawing::Size(1597, 274);
			this->Camera_tabPage->TabIndex = 1;
			this->Camera_tabPage->Text = L"Camera";
			this->Camera_tabPage->UseVisualStyleBackColor = true;
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer3->Name = L"splitContainer3";
			this->splitContainer3->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer3.Panel1
			// 
			this->splitContainer3->Panel1->Resize += gcnew System::EventHandler(this, &MPDIEditor::splitContainer3_Panel1_Resize);
			// 
			// splitContainer3.Panel2
			// 
			this->splitContainer3->Panel2->Controls->Add(this->flowLayoutPanel3);
			this->splitContainer3->Size = System::Drawing::Size(1609, 630);
			this->splitContainer3->SplitterDistance = 583;
			this->splitContainer3->SplitterWidth = 3;
			this->splitContainer3->TabIndex = 0;
			this->splitContainer3->Resize += gcnew System::EventHandler(this, &MPDIEditor::splitContainer3_Resize);
			// 
			// flowLayoutPanel3
			// 
			this->flowLayoutPanel3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->flowLayoutPanel3->Controls->Add(this->AllPlay_checkBox);
			this->flowLayoutPanel3->Controls->Add(this->AllCurveLoop_checkBox);
			this->flowLayoutPanel3->Controls->Add(this->StayAtLastFrame_checkBox);
			this->flowLayoutPanel3->Controls->Add(this->label9);
			this->flowLayoutPanel3->Controls->Add(this->CurrentProgress_trackBar);
			this->flowLayoutPanel3->Controls->Add(this->MPDIWorking_checkBox);
			this->flowLayoutPanel3->Controls->Add(this->CameraWorking_checkBox);
			this->flowLayoutPanel3->Controls->Add(this->BGColor_panel);
			this->flowLayoutPanel3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->flowLayoutPanel3->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel3->Name = L"flowLayoutPanel3";
			this->flowLayoutPanel3->Size = System::Drawing::Size(1609, 44);
			this->flowLayoutPanel3->TabIndex = 0;
			// 
			// AllPlay_checkBox
			// 
			this->AllPlay_checkBox->AutoSize = true;
			this->AllPlay_checkBox->Enabled = false;
			this->AllPlay_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->AllPlay_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPlay_checkBox->Location = System::Drawing::Point(3, 3);
			this->AllPlay_checkBox->Name = L"AllPlay_checkBox";
			this->AllPlay_checkBox->Size = System::Drawing::Size(103, 15);
			this->AllPlay_checkBox->TabIndex = 183;
			this->AllPlay_checkBox->Text = L"AllPlay/全部撥放";
			this->AllPlay_checkBox->UseVisualStyleBackColor = true;
			// 
			// AllCurveLoop_checkBox
			// 
			this->AllCurveLoop_checkBox->AutoSize = true;
			this->AllCurveLoop_checkBox->Enabled = false;
			this->AllCurveLoop_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->AllCurveLoop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllCurveLoop_checkBox->Location = System::Drawing::Point(112, 3);
			this->AllCurveLoop_checkBox->Name = L"AllCurveLoop_checkBox";
			this->AllCurveLoop_checkBox->Size = System::Drawing::Size(148, 15);
			this->AllCurveLoop_checkBox->TabIndex = 184;
			this->AllCurveLoop_checkBox->Text = L"AllCurveLoop/全部路徑loop";
			this->AllCurveLoop_checkBox->UseVisualStyleBackColor = true;
			// 
			// StayAtLastFrame_checkBox
			// 
			this->StayAtLastFrame_checkBox->AutoSize = true;
			this->StayAtLastFrame_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->StayAtLastFrame_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->StayAtLastFrame_checkBox->Location = System::Drawing::Point(266, 3);
			this->StayAtLastFrame_checkBox->Name = L"StayAtLastFrame_checkBox";
			this->StayAtLastFrame_checkBox->Size = System::Drawing::Size(167, 15);
			this->StayAtLastFrame_checkBox->TabIndex = 194;
			this->StayAtLastFrame_checkBox->Text = L"StayAtLastFrame/停在最後一格";
			this->StayAtLastFrame_checkBox->UseVisualStyleBackColor = true;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->ForeColor = System::Drawing::Color::White;
			this->label9->Location = System::Drawing::Point(439, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(91, 12);
			this->label9->TabIndex = 174;
			this->label9->Text = L"Progess/目前進度";
			// 
			// CurrentProgress_trackBar
			// 
			this->CurrentProgress_trackBar->Location = System::Drawing::Point(536, 3);
			this->CurrentProgress_trackBar->Name = L"CurrentProgress_trackBar";
			this->CurrentProgress_trackBar->Size = System::Drawing::Size(453, 45);
			this->CurrentProgress_trackBar->TabIndex = 0;
			// 
			// MPDIWorking_checkBox
			// 
			this->MPDIWorking_checkBox->AutoSize = true;
			this->MPDIWorking_checkBox->Checked = true;
			this->MPDIWorking_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->MPDIWorking_checkBox->Location = System::Drawing::Point(995, 3);
			this->MPDIWorking_checkBox->Name = L"MPDIWorking_checkBox";
			this->MPDIWorking_checkBox->Size = System::Drawing::Size(97, 16);
			this->MPDIWorking_checkBox->TabIndex = 175;
			this->MPDIWorking_checkBox->Text = L"MPDI Working";
			this->MPDIWorking_checkBox->UseVisualStyleBackColor = true;
			// 
			// CameraWorking_checkBox
			// 
			this->CameraWorking_checkBox->AutoSize = true;
			this->CameraWorking_checkBox->Checked = true;
			this->CameraWorking_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CameraWorking_checkBox->Location = System::Drawing::Point(1098, 3);
			this->CameraWorking_checkBox->Name = L"CameraWorking_checkBox";
			this->CameraWorking_checkBox->Size = System::Drawing::Size(105, 16);
			this->CameraWorking_checkBox->TabIndex = 176;
			this->CameraWorking_checkBox->Text = L"Camera Working";
			this->CameraWorking_checkBox->UseVisualStyleBackColor = true;
			// 
			// BGColor_panel
			// 
			this->BGColor_panel->Location = System::Drawing::Point(1209, 3);
			this->BGColor_panel->Name = L"BGColor_panel";
			this->BGColor_panel->Size = System::Drawing::Size(85, 22);
			this->BGColor_panel->TabIndex = 185;
			// 
			// splitContainer2
			// 
			this->splitContainer2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 24);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->tabControl1);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->splitContainer1);
			this->splitContainer2->Size = System::Drawing::Size(1904, 941);
			this->splitContainer2->SplitterDistance = 288;
			this->splitContainer2->SplitterWidth = 3;
			this->splitContainer2->TabIndex = 146;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Multiline = true;
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(284, 937);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->AutoScroll = true;
			this->tabPage1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)),
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->tabPage1->Controls->Add(this->flowLayoutPanel1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(276, 911);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"EditAnimationData/編輯動畫資料";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->AutoScroll = true;
			this->flowLayoutPanel1->Controls->Add(this->AnimationData_collpaseForm);
			this->flowLayoutPanel1->Controls->Add(this->GroupData_collpaseForm);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->flowLayoutPanel1->Location = System::Drawing::Point(3, 3);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(270, 905);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// AnimationData_collpaseForm
			// 
			this->AnimationData_collpaseForm->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->AnimationData_collpaseForm->Controls->Add(this->WholeAnimationList_listBox);
			this->AnimationData_collpaseForm->Controls->Add(this->WholeAnimationListUp_button);
			this->AnimationData_collpaseForm->Controls->Add(this->MergeMPDIData_button);
			this->AnimationData_collpaseForm->Controls->Add(this->WholeAnimationListDown_lbutton);
			this->AnimationData_collpaseForm->Controls->Add(this->AnimationName_textBox);
			this->AnimationData_collpaseForm->Controls->Add(this->DeleteListObject_button);
			this->AnimationData_collpaseForm->Controls->Add(this->AnimationInvert_button);
			this->AnimationData_collpaseForm->Controls->Add(this->AddAnimationToList_button);
			this->AnimationData_collpaseForm->Controls->Add(this->label15);
			this->AnimationData_collpaseForm->Location = System::Drawing::Point(3, 3);
			this->AnimationData_collpaseForm->Name = L"AnimationData_collpaseForm";
			this->AnimationData_collpaseForm->Size = System::Drawing::Size(264, 363);
			this->AnimationData_collpaseForm->TabIndex = 1;
			// 
			// WholeAnimationList_listBox
			// 
			this->WholeAnimationList_listBox->AllowDrop = true;
			this->WholeAnimationList_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->WholeAnimationList_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeAnimationList_listBox->FormattingEnabled = true;
			this->WholeAnimationList_listBox->ItemHeight = 12;
			this->WholeAnimationList_listBox->Location = System::Drawing::Point(4, 64);
			this->WholeAnimationList_listBox->Margin = System::Windows::Forms::Padding(4);
			this->WholeAnimationList_listBox->MinimumSize = System::Drawing::Size(4, 85);
			this->WholeAnimationList_listBox->Name = L"WholeAnimationList_listBox";
			this->WholeAnimationList_listBox->ScrollAlwaysVisible = true;
			this->WholeAnimationList_listBox->Size = System::Drawing::Size(222, 292);
			this->WholeAnimationList_listBox->TabIndex = 114;
			this->WholeAnimationList_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationList_listBox_SelectedIndexChanged);
			this->WholeAnimationList_listBox->DoubleClick += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationList_listBox_DoubleClick);
			this->WholeAnimationList_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::WholeAnimationList_listBox_KeyUp);
			// 
			// WholeAnimationListUp_button
			// 
			this->WholeAnimationListUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholeAnimationListUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeAnimationListUp_button->Location = System::Drawing::Point(232, 106);
			this->WholeAnimationListUp_button->Name = L"WholeAnimationListUp_button";
			this->WholeAnimationListUp_button->Size = System::Drawing::Size(22, 39);
			this->WholeAnimationListUp_button->TabIndex = 139;
			this->WholeAnimationListUp_button->Text = L"^\r\n^";
			this->WholeAnimationListUp_button->UseVisualStyleBackColor = false;
			this->WholeAnimationListUp_button->Click += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationListUp_button_Click);
			// 
			// MergeMPDIData_button
			// 
			this->MergeMPDIData_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->MergeMPDIData_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->MergeMPDIData_button->Location = System::Drawing::Point(192, 2);
			this->MergeMPDIData_button->Margin = System::Windows::Forms::Padding(4);
			this->MergeMPDIData_button->Name = L"MergeMPDIData_button";
			this->MergeMPDIData_button->Size = System::Drawing::Size(67, 25);
			this->MergeMPDIData_button->TabIndex = 118;
			this->MergeMPDIData_button->Text = L"Merge/混合";
			this->MergeMPDIData_button->UseVisualStyleBackColor = false;
			this->MergeMPDIData_button->Click += gcnew System::EventHandler(this, &MPDIEditor::MergeMPDIData_button_Click);
			// 
			// WholeAnimationListDown_lbutton
			// 
			this->WholeAnimationListDown_lbutton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholeAnimationListDown_lbutton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeAnimationListDown_lbutton->Location = System::Drawing::Point(233, 221);
			this->WholeAnimationListDown_lbutton->Name = L"WholeAnimationListDown_lbutton";
			this->WholeAnimationListDown_lbutton->Size = System::Drawing::Size(22, 50);
			this->WholeAnimationListDown_lbutton->TabIndex = 138;
			this->WholeAnimationListDown_lbutton->Text = L"v\r\nv";
			this->WholeAnimationListDown_lbutton->UseVisualStyleBackColor = false;
			this->WholeAnimationListDown_lbutton->Click += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationListUp_button_Click);
			// 
			// AnimationName_textBox
			// 
			this->AnimationName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationName_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationName_textBox->Location = System::Drawing::Point(84, 4);
			this->AnimationName_textBox->Margin = System::Windows::Forms::Padding(4);
			this->AnimationName_textBox->Name = L"AnimationName_textBox";
			this->AnimationName_textBox->Size = System::Drawing::Size(105, 22);
			this->AnimationName_textBox->TabIndex = 115;
			// 
			// DeleteListObject_button
			// 
			this->DeleteListObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DeleteListObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteListObject_button->Location = System::Drawing::Point(81, 32);
			this->DeleteListObject_button->Margin = System::Windows::Forms::Padding(4);
			this->DeleteListObject_button->Name = L"DeleteListObject_button";
			this->DeleteListObject_button->Size = System::Drawing::Size(109, 24);
			this->DeleteListObject_button->TabIndex = 113;
			this->DeleteListObject_button->Text = L"DeleteListObject/刪除物件";
			this->DeleteListObject_button->UseVisualStyleBackColor = false;
			this->DeleteListObject_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddAnimationToList_button_Click);
			// 
			// AnimationInvert_button
			// 
			this->AnimationInvert_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationInvert_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationInvert_button->Location = System::Drawing::Point(196, 31);
			this->AnimationInvert_button->Margin = System::Windows::Forms::Padding(4);
			this->AnimationInvert_button->Name = L"AnimationInvert_button";
			this->AnimationInvert_button->Size = System::Drawing::Size(69, 25);
			this->AnimationInvert_button->TabIndex = 117;
			this->AnimationInvert_button->Text = L"ReEdit/重新編輯";
			this->AnimationInvert_button->UseVisualStyleBackColor = false;
			this->AnimationInvert_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddAnimationToList_button_Click);
			// 
			// AddAnimationToList_button
			// 
			this->AddAnimationToList_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddAnimationToList_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddAnimationToList_button->Location = System::Drawing::Point(3, 32);
			this->AddAnimationToList_button->Margin = System::Windows::Forms::Padding(4);
			this->AddAnimationToList_button->Name = L"AddAnimationToList_button";
			this->AddAnimationToList_button->Size = System::Drawing::Size(75, 24);
			this->AddAnimationToList_button->TabIndex = 112;
			this->AddAnimationToList_button->Text = L"AddToList/增加到輸出檔案列表";
			this->AddAnimationToList_button->UseVisualStyleBackColor = false;
			this->AddAnimationToList_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddAnimationToList_button_Click);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label15->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label15->Location = System::Drawing::Point(2, 6);
			this->label15->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(120, 12);
			this->label15->TabIndex = 116;
			this->label15->Text = L"AnimationName/動畫名";
			// 
			// GroupData_collpaseForm
			// 
			this->GroupData_collpaseForm->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenter_label);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenterY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenterX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenterZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label26);
			this->GroupData_collpaseForm->Controls->Add(this->GroupScale_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotation_label3);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPosY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label29);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPosX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label28);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPosZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupImageRotate_checkBox);
			this->GroupData_collpaseForm->Controls->Add(this->label24);
			this->GroupData_collpaseForm->Controls->Add(this->label27);
			this->GroupData_collpaseForm->Controls->Add(this->ApplyGroupDataChange_button);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPositionMoveType_comboBox);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label23);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationType_comboBox);
			this->GroupData_collpaseForm->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->GroupData_collpaseForm->Location = System::Drawing::Point(3, 372);
			this->GroupData_collpaseForm->Name = L"GroupData_collpaseForm";
			this->GroupData_collpaseForm->Size = System::Drawing::Size(265, 199);
			this->GroupData_collpaseForm->TabIndex = 0;
			// 
			// CameraRotationCenter_label
			// 
			this->CameraRotationCenter_label->AutoSize = true;
			this->CameraRotationCenter_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenter_label->Location = System::Drawing::Point(193, 51);
			this->CameraRotationCenter_label->Name = L"CameraRotationCenter_label";
			this->CameraRotationCenter_label->Size = System::Drawing::Size(76, 12);
			this->CameraRotationCenter_label->TabIndex = 37;
			this->CameraRotationCenter_label->Text = L"RotationCenter";
			// 
			// CameraRotationCenterY_numericUpDown
			// 
			this->CameraRotationCenterY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationCenterY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenterY_numericUpDown->Location = System::Drawing::Point(206, 98);
			this->CameraRotationCenterY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->CameraRotationCenterY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->CameraRotationCenterY_numericUpDown->Name = L"CameraRotationCenterY_numericUpDown";
			this->CameraRotationCenterY_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->CameraRotationCenterY_numericUpDown->TabIndex = 36;
			this->CameraRotationCenterY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationCenterX_numericUpDown
			// 
			this->CameraRotationCenterX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationCenterX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenterX_numericUpDown->Location = System::Drawing::Point(207, 70);
			this->CameraRotationCenterX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->CameraRotationCenterX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->CameraRotationCenterX_numericUpDown->Name = L"CameraRotationCenterX_numericUpDown";
			this->CameraRotationCenterX_numericUpDown->Size = System::Drawing::Size(57, 22);
			this->CameraRotationCenterX_numericUpDown->TabIndex = 34;
			this->CameraRotationCenterX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationCenterZ_numericUpDown
			// 
			this->CameraRotationCenterZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationCenterZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenterZ_numericUpDown->Location = System::Drawing::Point(206, 126);
			this->CameraRotationCenterZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->CameraRotationCenterZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->CameraRotationCenterZ_numericUpDown->Name = L"CameraRotationCenterZ_numericUpDown";
			this->CameraRotationCenterZ_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->CameraRotationCenterZ_numericUpDown->TabIndex = 35;
			this->CameraRotationCenterZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label26->Location = System::Drawing::Point(8, 12);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(140, 12);
			this->label26->TabIndex = 21;
			this->label26->Text = L"Perentage Scale/百分比大小";
			// 
			// GroupScale_numericUpDown
			// 
			this->GroupScale_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupScale_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupScale_numericUpDown->Location = System::Drawing::Point(142, 9);
			this->GroupScale_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->GroupScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->GroupScale_numericUpDown->Name = L"GroupScale_numericUpDown";
			this->GroupScale_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->GroupScale_numericUpDown->TabIndex = 25;
			this->GroupScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			// 
			// CameraRotation_label3
			// 
			this->CameraRotation_label3->AutoSize = true;
			this->CameraRotation_label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotation_label3->Location = System::Drawing::Point(141, 50);
			this->CameraRotation_label3->Name = L"CameraRotation_label3";
			this->CameraRotation_label3->Size = System::Drawing::Size(45, 12);
			this->CameraRotation_label3->TabIndex = 33;
			this->CameraRotation_label3->Text = L"Rotation";
			// 
			// CameraRotationY_numericUpDown
			// 
			this->CameraRotationY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationY_numericUpDown->Location = System::Drawing::Point(143, 96);
			this->CameraRotationY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->CameraRotationY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->CameraRotationY_numericUpDown->Name = L"CameraRotationY_numericUpDown";
			this->CameraRotationY_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->CameraRotationY_numericUpDown->TabIndex = 32;
			this->CameraRotationY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationX_numericUpDown
			// 
			this->CameraRotationX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationX_numericUpDown->Location = System::Drawing::Point(144, 68);
			this->CameraRotationX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->CameraRotationX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->CameraRotationX_numericUpDown->Name = L"CameraRotationX_numericUpDown";
			this->CameraRotationX_numericUpDown->Size = System::Drawing::Size(57, 22);
			this->CameraRotationX_numericUpDown->TabIndex = 30;
			this->CameraRotationX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationZ_numericUpDown
			// 
			this->CameraRotationZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationZ_numericUpDown->Location = System::Drawing::Point(143, 124);
			this->CameraRotationZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->CameraRotationZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->CameraRotationZ_numericUpDown->Name = L"CameraRotationZ_numericUpDown";
			this->CameraRotationZ_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->CameraRotationZ_numericUpDown->TabIndex = 31;
			this->CameraRotationZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// GroupRotationX_numericUpDown
			// 
			this->GroupRotationX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationX_numericUpDown->Location = System::Drawing::Point(10, 67);
			this->GroupRotationX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 360, 0, 0, 0 });
			this->GroupRotationX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 360, 0, 0, System::Int32::MinValue });
			this->GroupRotationX_numericUpDown->Name = L"GroupRotationX_numericUpDown";
			this->GroupRotationX_numericUpDown->Size = System::Drawing::Size(57, 22);
			this->GroupRotationX_numericUpDown->TabIndex = 18;
			// 
			// GroupPosY_numericUpDown
			// 
			this->GroupPosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPosY_numericUpDown->Location = System::Drawing::Point(74, 95);
			this->GroupPosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->GroupPosY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->GroupPosY_numericUpDown->Name = L"GroupPosY_numericUpDown";
			this->GroupPosY_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->GroupPosY_numericUpDown->TabIndex = 7;
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(-1, 123);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(10, 12);
			this->label29->TabIndex = 28;
			this->label29->Text = L"z";
			// 
			// GroupPosX_numericUpDown
			// 
			this->GroupPosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPosX_numericUpDown->Location = System::Drawing::Point(74, 67);
			this->GroupPosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->GroupPosX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->GroupPosX_numericUpDown->Name = L"GroupPosX_numericUpDown";
			this->GroupPosX_numericUpDown->Size = System::Drawing::Size(57, 22);
			this->GroupPosX_numericUpDown->TabIndex = 5;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(-1, 95);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(11, 12);
			this->label28->TabIndex = 27;
			this->label28->Text = L"y";
			// 
			// GroupPosZ_numericUpDown
			// 
			this->GroupPosZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPosZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPosZ_numericUpDown->Location = System::Drawing::Point(74, 123);
			this->GroupPosZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, 0 });
			this->GroupPosZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 79979, 0, 0, System::Int32::MinValue });
			this->GroupPosZ_numericUpDown->Name = L"GroupPosZ_numericUpDown";
			this->GroupPosZ_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->GroupPosZ_numericUpDown->TabIndex = 6;
			// 
			// GroupImageRotate_checkBox
			// 
			this->GroupImageRotate_checkBox->AutoSize = true;
			this->GroupImageRotate_checkBox->Checked = true;
			this->GroupImageRotate_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->GroupImageRotate_checkBox->Location = System::Drawing::Point(10, 148);
			this->GroupImageRotate_checkBox->Name = L"GroupImageRotate_checkBox";
			this->GroupImageRotate_checkBox->Size = System::Drawing::Size(113, 16);
			this->GroupImageRotate_checkBox->TabIndex = 29;
			this->GroupImageRotate_checkBox->Text = L"GroupImageRotate";
			this->GroupImageRotate_checkBox->UseVisualStyleBackColor = true;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label24->Location = System::Drawing::Point(70, 30);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(69, 12);
			this->label24->TabIndex = 11;
			this->label24->Text = L"Position/位置";
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(-1, 67);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(11, 12);
			this->label27->TabIndex = 26;
			this->label27->Text = L"x";
			// 
			// ApplyGroupDataChange_button
			// 
			this->ApplyGroupDataChange_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ApplyGroupDataChange_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ApplyGroupDataChange_button->Location = System::Drawing::Point(-4, 165);
			this->ApplyGroupDataChange_button->Name = L"ApplyGroupDataChange_button";
			this->ApplyGroupDataChange_button->Size = System::Drawing::Size(240, 23);
			this->ApplyGroupDataChange_button->TabIndex = 13;
			this->ApplyGroupDataChange_button->Text = L"ApplyGroupDataChange/群組資料變更應用";
			this->ApplyGroupDataChange_button->UseVisualStyleBackColor = false;
			this->ApplyGroupDataChange_button->Click += gcnew System::EventHandler(this, &MPDIEditor::ApplyGroupDataChange_button_Click);
			// 
			// GroupPositionMoveType_comboBox
			// 
			this->GroupPositionMoveType_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPositionMoveType_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPositionMoveType_comboBox->FormattingEnabled = true;
			this->GroupPositionMoveType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"None", L"Relative", L"World" });
			this->GroupPositionMoveType_comboBox->Location = System::Drawing::Point(74, 43);
			this->GroupPositionMoveType_comboBox->Name = L"GroupPositionMoveType_comboBox";
			this->GroupPositionMoveType_comboBox->Size = System::Drawing::Size(62, 20);
			this->GroupPositionMoveType_comboBox->TabIndex = 17;
			// 
			// GroupRotationZ_numericUpDown
			// 
			this->GroupRotationZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationZ_numericUpDown->Location = System::Drawing::Point(10, 123);
			this->GroupRotationZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 360, 0, 0, 0 });
			this->GroupRotationZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 360, 0, 0, System::Int32::MinValue });
			this->GroupRotationZ_numericUpDown->Name = L"GroupRotationZ_numericUpDown";
			this->GroupRotationZ_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->GroupRotationZ_numericUpDown->TabIndex = 19;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label23->Location = System::Drawing::Point(8, 30);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(72, 12);
			this->label23->TabIndex = 9;
			this->label23->Text = L"Rotation/旋轉";
			// 
			// GroupRotationY_numericUpDown
			// 
			this->GroupRotationY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationY_numericUpDown->Location = System::Drawing::Point(10, 95);
			this->GroupRotationY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 360, 0, 0, 0 });
			this->GroupRotationY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 360, 0, 0, System::Int32::MinValue });
			this->GroupRotationY_numericUpDown->Name = L"GroupRotationY_numericUpDown";
			this->GroupRotationY_numericUpDown->Size = System::Drawing::Size(58, 22);
			this->GroupRotationY_numericUpDown->TabIndex = 20;
			// 
			// GroupRotationType_comboBox
			// 
			this->GroupRotationType_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationType_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationType_comboBox->FormattingEnabled = true;
			this->GroupRotationType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"None", L"Self", L"World" });
			this->GroupRotationType_comboBox->Location = System::Drawing::Point(8, 43);
			this->GroupRotationType_comboBox->Name = L"GroupRotationType_comboBox";
			this->GroupRotationType_comboBox->Size = System::Drawing::Size(62, 20);
			this->GroupRotationType_comboBox->TabIndex = 8;
			// 
			// tabPage2
			// 
			this->tabPage2->AutoScroll = true;
			this->tabPage2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)),
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->tabPage2->Controls->Add(this->flowLayoutPanel2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(276, 911);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Utility/雜項功能";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// flowLayoutPanel2
			// 
			this->flowLayoutPanel2->AutoScroll = true;
			this->flowLayoutPanel2->Controls->Add(this->Path_groupBox);
			this->flowLayoutPanel2->Controls->Add(this->ImageHintPoint_groupBox);
			this->flowLayoutPanel2->Controls->Add(this->HintPoint_groupBox);
			this->flowLayoutPanel2->Controls->Add(this->Video_groupBox);
			this->flowLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel2->Location = System::Drawing::Point(3, 3);
			this->flowLayoutPanel2->Name = L"flowLayoutPanel2";
			this->flowLayoutPanel2->Size = System::Drawing::Size(270, 905);
			this->flowLayoutPanel2->TabIndex = 0;
			// 
			// Path_groupBox
			// 
			this->Path_groupBox->Controls->Add(this->AddPathIntoAllPath_button);
			this->Path_groupBox->Controls->Add(this->AllPath_listBox);
			this->Path_groupBox->Controls->Add(this->DelPathIntoAllPath_button);
			this->Path_groupBox->Controls->Add(this->UsingIntoCurrentPath_button);
			this->Path_groupBox->Location = System::Drawing::Point(2, 2);
			this->Path_groupBox->Margin = System::Windows::Forms::Padding(2);
			this->Path_groupBox->Name = L"Path_groupBox";
			this->Path_groupBox->Padding = System::Windows::Forms::Padding(2);
			this->Path_groupBox->Size = System::Drawing::Size(268, 157);
			this->Path_groupBox->TabIndex = 0;
			this->Path_groupBox->TabStop = false;
			this->Path_groupBox->Text = L"Path";
			// 
			// AddPathIntoAllPath_button
			// 
			this->AddPathIntoAllPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddPathIntoAllPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddPathIntoAllPath_button->Location = System::Drawing::Point(14, 16);
			this->AddPathIntoAllPath_button->Margin = System::Windows::Forms::Padding(2);
			this->AddPathIntoAllPath_button->Name = L"AddPathIntoAllPath_button";
			this->AddPathIntoAllPath_button->Size = System::Drawing::Size(57, 21);
			this->AddPathIntoAllPath_button->TabIndex = 1;
			this->AddPathIntoAllPath_button->Text = L"Add/增加";
			this->AddPathIntoAllPath_button->UseVisualStyleBackColor = false;
			this->AddPathIntoAllPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPathIntoAllPath_button_Click);
			// 
			// AllPath_listBox
			// 
			this->AllPath_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)),
				static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AllPath_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPath_listBox->FormattingEnabled = true;
			this->AllPath_listBox->ItemHeight = 12;
			this->AllPath_listBox->Location = System::Drawing::Point(12, 43);
			this->AllPath_listBox->Margin = System::Windows::Forms::Padding(0);
			this->AllPath_listBox->Name = L"AllPath_listBox";
			this->AllPath_listBox->Size = System::Drawing::Size(241, 100);
			this->AllPath_listBox->TabIndex = 0;
			// 
			// DelPathIntoAllPath_button
			// 
			this->DelPathIntoAllPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPathIntoAllPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPathIntoAllPath_button->Location = System::Drawing::Point(107, 16);
			this->DelPathIntoAllPath_button->Margin = System::Windows::Forms::Padding(2);
			this->DelPathIntoAllPath_button->Name = L"DelPathIntoAllPath_button";
			this->DelPathIntoAllPath_button->Size = System::Drawing::Size(57, 20);
			this->DelPathIntoAllPath_button->TabIndex = 2;
			this->DelPathIntoAllPath_button->Text = L"Del/刪除";
			this->DelPathIntoAllPath_button->UseVisualStyleBackColor = false;
			this->DelPathIntoAllPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPathIntoAllPath_button_Click);
			// 
			// UsingIntoCurrentPath_button
			// 
			this->UsingIntoCurrentPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->UsingIntoCurrentPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->UsingIntoCurrentPath_button->Location = System::Drawing::Point(183, 16);
			this->UsingIntoCurrentPath_button->Margin = System::Windows::Forms::Padding(2);
			this->UsingIntoCurrentPath_button->Name = L"UsingIntoCurrentPath_button";
			this->UsingIntoCurrentPath_button->Size = System::Drawing::Size(53, 21);
			this->UsingIntoCurrentPath_button->TabIndex = 3;
			this->UsingIntoCurrentPath_button->Text = L"Using/使用";
			this->UsingIntoCurrentPath_button->UseVisualStyleBackColor = false;
			this->UsingIntoCurrentPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPathIntoAllPath_button_Click);
			// 
			// ImageHintPoint_groupBox
			// 
			this->ImageHintPoint_groupBox->Controls->Add(this->ShowImageOnScreen_listBox);
			this->ImageHintPoint_groupBox->Controls->Add(this->DelShowImageOnScreenObject_button);
			this->ImageHintPoint_groupBox->Controls->Add(this->label16);
			this->ImageHintPoint_groupBox->Controls->Add(this->AddShowImageOnScreenObject_button);
			this->ImageHintPoint_groupBox->Location = System::Drawing::Point(2, 163);
			this->ImageHintPoint_groupBox->Margin = System::Windows::Forms::Padding(2);
			this->ImageHintPoint_groupBox->Name = L"ImageHintPoint_groupBox";
			this->ImageHintPoint_groupBox->Padding = System::Windows::Forms::Padding(2);
			this->ImageHintPoint_groupBox->Size = System::Drawing::Size(268, 128);
			this->ImageHintPoint_groupBox->TabIndex = 1;
			this->ImageHintPoint_groupBox->TabStop = false;
			this->ImageHintPoint_groupBox->Text = L"ImageHintPoint";
			// 
			// ShowImageOnScreen_listBox
			// 
			this->ShowImageOnScreen_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ShowImageOnScreen_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ShowImageOnScreen_listBox->FormattingEnabled = true;
			this->ShowImageOnScreen_listBox->ItemHeight = 12;
			this->ShowImageOnScreen_listBox->Location = System::Drawing::Point(96, 17);
			this->ShowImageOnScreen_listBox->Margin = System::Windows::Forms::Padding(2);
			this->ShowImageOnScreen_listBox->MinimumSize = System::Drawing::Size(4, 75);
			this->ShowImageOnScreen_listBox->Name = L"ShowImageOnScreen_listBox";
			this->ShowImageOnScreen_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->ShowImageOnScreen_listBox->Size = System::Drawing::Size(141, 100);
			this->ShowImageOnScreen_listBox->TabIndex = 131;
			// 
			// DelShowImageOnScreenObject_button
			// 
			this->DelShowImageOnScreenObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelShowImageOnScreenObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelShowImageOnScreenObject_button->Location = System::Drawing::Point(25, 76);
			this->DelShowImageOnScreenObject_button->Margin = System::Windows::Forms::Padding(2);
			this->DelShowImageOnScreenObject_button->Name = L"DelShowImageOnScreenObject_button";
			this->DelShowImageOnScreenObject_button->Size = System::Drawing::Size(38, 17);
			this->DelShowImageOnScreenObject_button->TabIndex = 132;
			this->DelShowImageOnScreenObject_button->Text = L"Del/刪除";
			this->DelShowImageOnScreenObject_button->UseVisualStyleBackColor = false;
			this->DelShowImageOnScreenObject_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddShowImageOnScreenObject_button_Click);
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label16->ForeColor = System::Drawing::Color::White;
			this->label16->Location = System::Drawing::Point(22, 26);
			this->label16->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(59, 11);
			this->label16->TabIndex = 135;
			this->label16->Text = L"Hint/提示點";
			// 
			// AddShowImageOnScreenObject_button
			// 
			this->AddShowImageOnScreenObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddShowImageOnScreenObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddShowImageOnScreenObject_button->Location = System::Drawing::Point(25, 42);
			this->AddShowImageOnScreenObject_button->Margin = System::Windows::Forms::Padding(2);
			this->AddShowImageOnScreenObject_button->Name = L"AddShowImageOnScreenObject_button";
			this->AddShowImageOnScreenObject_button->Size = System::Drawing::Size(38, 18);
			this->AddShowImageOnScreenObject_button->TabIndex = 133;
			this->AddShowImageOnScreenObject_button->Text = L"Add/增加";
			this->AddShowImageOnScreenObject_button->UseVisualStyleBackColor = false;
			this->AddShowImageOnScreenObject_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddShowImageOnScreenObject_button_Click);
			// 
			// HintPoint_groupBox
			// 
			this->HintPoint_groupBox->Controls->Add(this->EventFindFile_button);
			this->HintPoint_groupBox->Controls->Add(this->AllEventPointHint_listBox);
			this->HintPoint_groupBox->Controls->Add(this->HintPointType_comboBox);
			this->HintPoint_groupBox->Controls->Add(this->AddEventPOint_button);
			this->HintPoint_groupBox->Controls->Add(this->label17);
			this->HintPoint_groupBox->Controls->Add(this->EventPointName_textBox);
			this->HintPoint_groupBox->Controls->Add(this->DelEventPoint_button);
			this->HintPoint_groupBox->Location = System::Drawing::Point(2, 295);
			this->HintPoint_groupBox->Margin = System::Windows::Forms::Padding(2);
			this->HintPoint_groupBox->Name = L"HintPoint_groupBox";
			this->HintPoint_groupBox->Padding = System::Windows::Forms::Padding(2);
			this->HintPoint_groupBox->Size = System::Drawing::Size(268, 204);
			this->HintPoint_groupBox->TabIndex = 6;
			this->HintPoint_groupBox->TabStop = false;
			this->HintPoint_groupBox->Text = L"HintPoint";
			// 
			// EventFindFile_button
			// 
			this->EventFindFile_button->Location = System::Drawing::Point(234, 30);
			this->EventFindFile_button->Name = L"EventFindFile_button";
			this->EventFindFile_button->Size = System::Drawing::Size(19, 18);
			this->EventFindFile_button->TabIndex = 7;
			this->EventFindFile_button->Text = L"...";
			this->EventFindFile_button->UseVisualStyleBackColor = true;
			this->EventFindFile_button->Click += gcnew System::EventHandler(this, &MPDIEditor::EventFindFile_button_Click);
			// 
			// AllEventPointHint_listBox
			// 
			this->AllEventPointHint_listBox->FormattingEnabled = true;
			this->AllEventPointHint_listBox->ItemHeight = 12;
			this->AllEventPointHint_listBox->Location = System::Drawing::Point(21, 90);
			this->AllEventPointHint_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllEventPointHint_listBox->Name = L"AllEventPointHint_listBox";
			this->AllEventPointHint_listBox->Size = System::Drawing::Size(216, 100);
			this->AllEventPointHint_listBox->TabIndex = 6;
			// 
			// HintPointType_comboBox
			// 
			this->HintPointType_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->HintPointType_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->HintPointType_comboBox->FormattingEnabled = true;
			this->HintPointType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"None", L"Sound", L"Particle" });
			this->HintPointType_comboBox->Location = System::Drawing::Point(59, 28);
			this->HintPointType_comboBox->Margin = System::Windows::Forms::Padding(2);
			this->HintPointType_comboBox->Name = L"HintPointType_comboBox";
			this->HintPointType_comboBox->Size = System::Drawing::Size(86, 20);
			this->HintPointType_comboBox->TabIndex = 0;
			// 
			// AddEventPOint_button
			// 
			this->AddEventPOint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddEventPOint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddEventPOint_button->Location = System::Drawing::Point(41, 52);
			this->AddEventPOint_button->Margin = System::Windows::Forms::Padding(2);
			this->AddEventPOint_button->Name = L"AddEventPOint_button";
			this->AddEventPOint_button->Size = System::Drawing::Size(56, 19);
			this->AddEventPOint_button->TabIndex = 4;
			this->AddEventPOint_button->Text = L"Add/增加";
			this->AddEventPOint_button->UseVisualStyleBackColor = false;
			this->AddEventPOint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddEventPOint_button_Click);
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label17->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label17->Location = System::Drawing::Point(10, 30);
			this->label17->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(56, 12);
			this->label17->TabIndex = 2;
			this->label17->Text = L"Type/類別";
			// 
			// EventPointName_textBox
			// 
			this->EventPointName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->EventPointName_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EventPointName_textBox->Location = System::Drawing::Point(149, 28);
			this->EventPointName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->EventPointName_textBox->Name = L"EventPointName_textBox";
			this->EventPointName_textBox->Size = System::Drawing::Size(83, 22);
			this->EventPointName_textBox->TabIndex = 1;
			// 
			// DelEventPoint_button
			// 
			this->DelEventPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelEventPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)),
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelEventPoint_button->Location = System::Drawing::Point(140, 52);
			this->DelEventPoint_button->Margin = System::Windows::Forms::Padding(2);
			this->DelEventPoint_button->Name = L"DelEventPoint_button";
			this->DelEventPoint_button->Size = System::Drawing::Size(56, 19);
			this->DelEventPoint_button->TabIndex = 5;
			this->DelEventPoint_button->Text = L"Del/刪除";
			this->DelEventPoint_button->UseVisualStyleBackColor = false;
			this->DelEventPoint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddEventPOint_button_Click);
			// 
			// Video_groupBox
			// 
			this->Video_groupBox->Location = System::Drawing::Point(3, 504);
			this->Video_groupBox->Name = L"Video_groupBox";
			this->Video_groupBox->Size = System::Drawing::Size(271, 303);
			this->Video_groupBox->TabIndex = 7;
			this->Video_groupBox->TabStop = false;
			this->Video_groupBox->Text = L"Video";
			// 
			// toGifToolStripMenuItem
			// 
			this->toGifToolStripMenuItem->Name = L"toGifToolStripMenuItem";
			this->toGifToolStripMenuItem->Size = System::Drawing::Size(236, 22);
			this->toGifToolStripMenuItem->Text = L"ToGif";
			this->toGifToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::saveGIFFileToolStripMenuItem_Click);
			// 
			// MPDIEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ClientSize = System::Drawing::Size(1904, 965);
			this->Controls->Add(this->splitContainer2);
			this->Controls->Add(this->menuStrip1);
			this->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->KeyPreview = true;
			this->Name = L"MPDIEditor";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::MyKeyUp);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->Edit_tabControl->ResumeLayout(false);
			this->splitContainer3->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer3))->EndInit();
			this->splitContainer3->ResumeLayout(false);
			this->flowLayoutPanel3->ResumeLayout(false);
			this->flowLayoutPanel3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CurrentProgress_trackBar))->EndInit();
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer2))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->AnimationData_collpaseForm->ResumeLayout(false);
			this->AnimationData_collpaseForm->PerformLayout();
			this->GroupData_collpaseForm->ResumeLayout(false);
			this->GroupData_collpaseForm->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationCenterY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationCenterX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationCenterZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupScale_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CameraRotationZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupRotationX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupPosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupPosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupPosZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupRotationZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->GroupRotationY_numericUpDown))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->flowLayoutPanel2->ResumeLayout(false);
			this->Path_groupBox->ResumeLayout(false);
			this->ImageHintPoint_groupBox->ResumeLayout(false);
			this->ImageHintPoint_groupBox->PerformLayout();
			this->HintPoint_groupBox->ResumeLayout(false);
			this->HintPoint_groupBox->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void OpenImageFile_button_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void BKImage_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddPath_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholePath_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
private: System::Void PathLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void DelPoint_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AssignImage_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AlphaColor_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void Play_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void TotalPlayTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddAnimationToList_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CurrentProgress_trackBar_Scroll(System::Object^  sender, System::EventArgs^  e);
private: System::Void ApplyLODToPoints_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void BehaviorUpdateActive_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddShowImageOnScreenObject_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholePuzzleImageUnit_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void addPointModeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void PathPiorityUp_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void Mirror_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationList_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e);
private: System::Void EnableColorBlending_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationList_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void SrcBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void DestBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void openPathFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddPathIntoAllPath_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddEventPOint_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void RearrangeWholePathTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void showImageBoderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void MergeMPDIData_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CurrentProgress_trackBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void saveAviFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void saveGIFFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);		 
private: System::Void showPath_ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void DelPI_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AllPI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void ApplyGroupDataChange_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void ScaleWithPIAlphaStripPixel_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void SelectFirstAndLast_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationListUp_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void saveImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholePath_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void SelectAllPoints_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void splitContainer3_Panel1_Resize(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddSliderBarUI_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void addPointDataWithImageByPathNameToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void FadeIn_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void mergeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void UseViewPort_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void ImageAnchor_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CameraRotationX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void EventFindFile_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void splitContainer3_Resize(System::Object^  sender, System::EventArgs^  e) ;
private: System::Void x1080ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
};
//end namespace
}