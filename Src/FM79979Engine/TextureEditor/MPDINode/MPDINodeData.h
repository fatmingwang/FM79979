#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


#pragma unmanaged
#include <functional>
#pragma managed

#include "LinerDataModifer.h"
namespace MPDINodeForm
{

	/// <summary>
	/// Summary for MPDINodeData
	/// </summary>
	public ref class MPDINodeData : public System::Object
	{
	public:
		MPDINodeData(TextureEditorWPF::MPDI::MPDINodeForm^);

	protected:
		~MPDINodeData();
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		//my
	public:
		cPuzzleImageUnit*						m_pSelectedPIUnit;
		cMPDINode*								m_pMPDINode;
		TextureEditorWPF::MPDI_PathPointData^	m_pMPDI_PathPointData;
		bool									m_bListboxValueChanged;
		bool									IsLegalApplyData();
		cLinerDataModifer*						m_pLinerDataModifer;
		void									Run(Vector3 e_vMousePos);
		void									SetMPDINode(cMPDINode*e_pMPDINode);
		private:HDC								m_HdcMV;
		//
		private: System::Void StartTimeChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void SelectedTimeChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void CurrentDataTotalTimeChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AllDataTotalTimeChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MPDINodeData_Load();
		private: System::Void ColorFadeOut(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ColorFadeIn(System::Object^  sender, System::EventArgs^  e);
		private: System::Void PathBehaviorComboxSelecteChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void LODCountChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AddPositionButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AddColorButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AddSizeButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AddRotationButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AddAnimationImageButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DeleteButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void InsertButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void LinerUpdateClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void LoopClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ListBoxSelectionChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void TabControlSelectionChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void PosValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ColorChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void SizeChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void RotationChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AddPIButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DelPIButtonClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void PIListBoxDrop(System::Object^  sender, System::EventArgs^  e);
		private: System::Void PIChangeSelection(System::Object^  sender, System::EventArgs^  e);
		private: System::Void PIUnitChangeSelection(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ImageAnchorChanged(System::Object^  sender, System::EventArgs^  e);
		//
		private: System::Void RefreshTabcontrolSelectedIndex();
//end class MPDINodeData
	};
	//common function
	String^	CombineData(int e_iIndex,float e_fTime,Vector3 e_vData,bool e_bInterger);
	String^	CombineData(int e_iIndex,float e_fTime,Vector2 e_vData);
	String^	CombineData(int e_iIndex,float e_fTime,Vector4 e_vData);
	String^	CombineData(int e_iIndex,float e_fTime,cPuzzleImageUnit*e_pPIUnit);
	String^	ChangeIndex(int e_iIndex,String^e_strDescription);
	float	GetProperTime(cTimeAndDataLinerUpdateInterface*e_pTimeAndDataLinerUpdateInterface);
	void	AddPosToListbox(float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	AddRotationToListbox(float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	AddColorToListbox(float e_fTime,Vector4 e_vColor,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	AddSizeToListbox(float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	AddImageToListbox(float e_fTime,cPuzzleImageUnit* e_pPIUnit,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	//void	AddImageToListbox(float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	InsertPosToListbox(int e_iIndex,float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	InsertRotationToListbox(int e_iIndex,float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	InsertColorToListbox(int e_iIndex,float e_fTime,Vector4 e_vColor,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	InsertSizeToListbox(int e_iIndex,float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	InsertImageToListbox(int e_iIndex,float e_fTime,cPuzzleImageUnit* e_vPIUnitImage,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	//void	InsertImageToListbox(int e_iIndex,float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ChangeData(int e_iIndex,float e_fTime,void *e_fData,int e_iDataCount,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ChangePosData(int e_iIndex,float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ChangeColorData(int e_iIndex,float e_fTime,Vector4 e_vColor,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ChangeSizeData(int e_iIndex,float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ChangeRotationData(int e_iIndex,float e_fTime,Vector3 e_vRotation,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ChangeImageData(int e_iIndex,float e_fTime,cPuzzleImageUnit* e_vPIUnitImage,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	String^	GetLastTimeString(cMPDINode*e_pNode);

	Vector3	GetPos(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	Vector4	GetColor(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	Vector2	GetSize(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	Vector3	GetRotation(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	cPuzzleImageUnit*	GetPIUnit(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	String^	GetAlignIndexString(int e_iCount);
	void	ApplyAllPosDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ApplyAllColorDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ApplyAllSizeDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	void	ApplyAllRotationDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData);
	cTimeAndDataLinerUpdateInterface*	GetTimeAndDataLinerUpdateInterfaceFromTabControlSelectedIndex(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData,cMPDINode*e_pMPDINode);
	template <class T>	bool	ValueChange(
		MPDINodeForm::MPDINodeData^e_pMPDINodeData,
		cLinerDataProcessor<T>*e_pDataVector,
		T	e_Data,int e_iDataCount)
	{
		if(!e_pMPDINodeData->IsLegalApplyData())
			return false;
		System::Collections::ArrayList^ l_SelectedItem = gcnew System::Collections::ArrayList;
		if( e_pMPDINodeData->m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems->Count != 0)
		{
			T l_Data = e_Data;
			for(int i=0;i<e_pMPDINodeData->m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems->Count;++i)
			{
				int	l_iSelectedIndex = e_pMPDINodeData->m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items->IndexOf(e_pMPDINodeData->m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems[i]);
				e_pDataVector->ChangeData(l_iSelectedIndex,l_Data);
				l_SelectedItem->Add(l_iSelectedIndex);
				ChangeData(l_iSelectedIndex,*e_pDataVector->GetTime(l_iSelectedIndex),(float*)&l_Data,e_iDataCount,e_pMPDINodeData->m_pMPDI_PathPointData);
			}
		}
		for each ( int	l_iIndex in l_SelectedItem)
		{
			e_pMPDINodeData->m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems->Add(e_pMPDINodeData->m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items[l_iIndex]);
		}
		return true;
	}
	bool	AddPIFile(String^e_strFileNam);
//end namespace MPDINodeForm
}
