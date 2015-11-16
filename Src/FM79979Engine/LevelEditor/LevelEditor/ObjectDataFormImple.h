#pragma once

#include"ObjectDataForm.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
namespace LevelEditor
{
	public ref class cObjectDataFormImple
	{
		private: System::Windows::Forms::SplitContainer^  splitContainer1;
		private: System::Windows::Forms::SplitContainer^  splitContainer2;
		private: System::Windows::Forms::MenuStrip^  menuStrip1;
		private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  openTemplateToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem1;
		private: System::Windows::Forms::ToolStripMenuItem^  assistanceToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  showGridToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  autoAllignGridToolStripMenuItem;
		private: System::Windows::Forms::TreeView^  XMLDataForQuickApply_treeView;
		//private: System::ComponentModel::IContainer^  components;
		private: System::Windows::Forms::ContextMenuStrip^  UserDefineData__contextMenuStrip;
		private: System::Windows::Forms::ToolStripMenuItem^  refreshToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  addNewNodeFromFileToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  deleteNodeToolStripMenuItem;
		private: System::Windows::Forms::Label^  label1;
		private: XMLDataEditor::XMLForm^  ObjectTemplate_xmlNodeEditComponent;
		private: System::Windows::Forms::ListBox^  AllStage_listBox;
		private: System::Windows::Forms::TextBox^  StageName_textBox;
		private: System::Windows::Forms::Button^  AddStage_button;
		private: System::Windows::Forms::Button^  DelStage_button;
		private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
		private: System::Windows::Forms::Button^  ReRditStagebutton;
		private: System::Windows::Forms::ToolStripMenuItem^  camerResetToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  showResolutionBorderToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
		private: System::Windows::Forms::Label^  label10;
		private: System::Windows::Forms::Label^  label4;
		private: System::Windows::Forms::NumericUpDown^  StartY_numericUpDown;
		private: System::Windows::Forms::NumericUpDown^  StartX_numericUpDown;
		private: System::Windows::Forms::Button^  AddToLayer_button;
		private: System::Windows::Forms::Button^  LayerDown_button;
		private: System::Windows::Forms::Button^  LayerUp_button;
		private: System::Windows::Forms::ListBox^  Layer_listBox;
		private: System::Windows::Forms::Button^  DeleteLayer_button;
		private: System::Windows::Forms::ToolStripMenuItem^  autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  cameraResetToolStripMenuItem;
		private: System::Windows::Forms::ListBox^  LayerObjectData_listBox;
		private: System::Windows::Forms::Button^  DeleteObject_button;
		private: System::Windows::Forms::ToolStripMenuItem^  showWarningMsgToolStripMenuItem;
		private: System::Windows::Forms::GroupBox^  groupBox1;
		private: System::Windows::Forms::Label^  label3;
		private: System::Windows::Forms::CheckBox^  AllowSamepositionObject_checkBox;
		private: System::Windows::Forms::NumericUpDown^  GridHeight_numericUpDown;
		private: System::Windows::Forms::Label^  label6;
		private: System::Windows::Forms::NumericUpDown^  GridWidth_numericUpDown;
		private: System::Windows::Forms::Label^  label7;
		private: System::Windows::Forms::Label^  label2;
		private: System::Windows::Forms::NumericUpDown^  MapHeight_numericUpDown;
		private: System::Windows::Forms::NumericUpDown^  MapWidth_numericUpDown;
		private: System::Windows::Forms::Button^  AddObjectToStage_button;
		private: System::Windows::Forms::Label^  label5;
		private: System::Windows::Forms::Label^  label15;
		private: System::Windows::Forms::Button^  DelTemplate_button;
		private: System::Windows::Forms::Label^  label13;
		private: System::Windows::Forms::ListBox^  Template_listBox;
		private: System::Windows::Forms::Button^  AddTemplate_button;
		private: System::Windows::Forms::NumericUpDown^  AddObjectY_numericUpDown;
		private: System::Windows::Forms::Button^  DelImage_button;
		private: System::Windows::Forms::NumericUpDown^  AddObjectX_numericUpDown;
		private: System::Windows::Forms::Button^  AddImage_button;
		private: System::Windows::Forms::TextBox^  TemplateName_textBox;
		private: System::Windows::Forms::Button^  Clear_button;
		private: System::Windows::Forms::Button^  ReEditTemplate_button;
		private: System::Windows::Forms::Label^  label17;
		private: System::Windows::Forms::TextBox^  LayerName_textBox;
		private: System::Windows::Forms::TreeView^  ImageList_treeView;
		private: System::Windows::Forms::ContextMenuStrip^  UserDefineData_contextMenuStrip;
		private: System::Windows::Forms::ToolStripMenuItem^  addToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  deleteToolStripMenuItem;
		private: System::Windows::Forms::ContextMenuStrip^  LayerData_contextMenuStrip;
		private: System::Windows::Forms::ToolStripMenuItem^  selectAllThisTypeTemplateToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  refreshUserDataFromToolStripMenuItem;
		private: System::Windows::Forms::Label^  label14;
		private: System::Windows::Forms::NumericUpDown^  AddObjectZ_numericUpDown;
		private: System::Windows::Forms::Timer^  timer1;
		private: System::Windows::Forms::SplitContainer^  Main_splitContainer;
		private: System::Windows::Forms::CheckBox^  PlaceObjectByImageCenter_checkBox;
		private: System::Windows::Forms::GroupBox^  StageData_groupBox;
		private: System::Windows::Forms::GroupBox^  TemplateList_groupBox;
		private: System::Windows::Forms::GroupBox^  XMLData_groupBox;
		private: System::Windows::Forms::TabControl^  tabControl1;
		private: System::Windows::Forms::TabPage^  tabPage1;
		private: System::Windows::Forms::TabPage^  tabPage2;
		private: System::Windows::Forms::FlowLayoutPanel^  EventData_flowLayoutPanel;
		private: System::Windows::Forms::CheckBox^  LayerVisible_checkBox;
		private: System::Windows::Forms::ComboBox^  MouseMode_comboBox;
		private: System::Windows::Forms::Label^  MouseMode_label;
		private: System::Windows::Forms::GroupBox^  LayerList_groupBox;
		private: System::Windows::Forms::GroupBox^  Object_groupBox;
		private: System::Windows::Forms::ToolStripMenuItem^  assignToObjectEventDataToolStripMenuItem;
		private: System::Windows::Forms::Button^  NoImage_button;
		private: XMLDataEditor::XMLForm^			LevelLayerGridEventDataNode_xmlNodeEditComponent;
		private: System::Windows::Forms::CheckBox^  InGameRender_checkBox;
		//my
		GCFORM::Form^m_pForm;
		//String,TreeNodes
		System::Collections::Hashtable^m_TemplateNameAndAttributeNode;
		cLevelEditorApp*m_pLevelEditorApp;
		void				AddEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData,GCFORM::TreeNode^e_pNode);
		void				RemoveEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData);
		//object's unique ID with a treenode
		System::Collections::Hashtable^m_UniqueUserDefineDataForMap_Cell_ObjectData;
		void	SaveTreeNodeFile(String^e_strFileName,System::Collections::Hashtable^e_pHashTable,bool e_bUserDefineData);
		//if e_bEraseContiously is true,we won't add object if blank is empty
		private:System::Void MyMouseDoubleClick(System::Object^  sender, System::EventArgs^  e);
		private:System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		private:System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		private:System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		System::Void Render();
		//template data click
		System::Void TemplateDataInvertorReEdit();
		//for user data
		//System::Void RemoveUserDefineNode(cLayer*e_pMapLayer);
		//System::Void RemoveUserDefineNode(cLevelLayerList*e_pMap_CellData);
		//System::Void RemoveUserDefineNode(cLevelLayerGridData*e_pMap_Cell_ObjectData);
		//while stage data reedited is called,to check the user data is re generate
		//System::Void CopyUserDataToNewData(cLevelLayerList*e_pSrc,cLevelLayerList*e_pDest);
		void	OpenUserDefineData(String^e_strFileName);
		void	OpenQuickApplyXmlFile(String^e_strFileName);
//=====================================
//
//
		public: cObjectDataFormImple(ObjectDataForm^e_pObjectDataForm);
		public: ~cObjectDataFormImple();
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void treeView1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
	private: System::Void treeView1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
	private: System::Void AssignData();
	private: System::Void AssignTemplateData_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void addNewNodeFromFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
			 void		  RemoveSelectedNode();
	private: System::Void deleteNodeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void treeView1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void AddImage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddTemplate_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddStage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void GridHeight_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void showGridToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Row_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Template_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Clear_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void backgroundImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ReEditTemplate_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void StartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddToLayer_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AllStage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void LayerDown_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AllStage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void cameraResetToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddObjectToStage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void DeleteObject_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
	private: System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void ImageList_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);
	private: System::Void Layer_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ImageList_treeView_BeforeSelect(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e);
	private: System::Void AllStage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void SyncUserDefineDataByFile_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void XMLDataForQuickApply_treeView_ItemDrag(System::Object^  sender, System::Windows::Forms::ItemDragEventArgs^  e) ;
	private: System::Void assignToObjectEventDataToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) ;
	private: System::Void ObjectData_treeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
	private: System::Void LevelLayerGridEventDataNode_xmlNodeEditComponentTreeViewDeleteOrCreateFunction(System::Object^sender,System::EventArgs^e);
	private: System::Void ImageList_treeView_MouseLeave(System::Object^  sender, System::EventArgs^  e) ;
	private: System::Void NoImage_button_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void LayerObjectData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void InGameRender_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Main_splitContainer_Panel1_SizeChanged(System::Object^  sender, System::EventArgs^  e);
	};
//end namespace LevelEditor
}