#include "stdafx.h"
#include "MPDINodeData.h"
namespace MPDINodeForm 
{

	String^	CombineData(int e_iIndex,float e_fTime,Vector3 e_vData,bool e_bInterger)
	{
		String^l_strDescription = GetAlignIndexString(e_iIndex);
		l_strDescription += " Time:";
		char l_strTemp[MAX_PATH];
		sprintf(l_strTemp,"%.3f\n",e_fTime);
		l_strDescription += String(l_strTemp).ToString();
		if( e_bInterger )
		{
			l_strDescription += "x:";
			l_strDescription += ((int)(e_vData.x)).ToString()+"\ty:";
			l_strDescription += ((int)(e_vData.y)).ToString()+"\tz:";
			l_strDescription += ((int)(e_vData.z)).ToString();		
		}
		else
		{
			l_strDescription += "x:";
			l_strDescription += e_vData.x.ToString()+"\ty:";
			l_strDescription += e_vData.y.ToString()+"\tz:";
			l_strDescription += e_vData.z.ToString();
		}
		return 	l_strDescription;
	}

	String^	CombineData(int e_iIndex,float e_fTime,Vector2 e_vData)
	{
		String^l_strDescription = GetAlignIndexString(e_iIndex);
		l_strDescription += " Time:";
		char l_strTemp[MAX_PATH];
		sprintf(l_strTemp,"%.3f\n",e_fTime);
		l_strDescription += String(l_strTemp).ToString();
		l_strDescription += "x:";
		l_strDescription += e_vData.x.ToString()+"\ty:";
		l_strDescription += e_vData.y.ToString();
		return 	l_strDescription;
	}

	String^	CombineData(int e_iIndex,float e_fTime,Vector4 e_vData)
	{
		String^l_strDescription = GetAlignIndexString(e_iIndex);
		l_strDescription += " Time:";
		char l_strTemp[MAX_PATH];
		sprintf(l_strTemp,"%.3f\n",e_fTime);
		l_strDescription += String(l_strTemp).ToString();
		l_strDescription += "x:";
		l_strDescription += e_vData.x.ToString()+"\ty:";
		l_strDescription += e_vData.y.ToString()+"\tz:";
		l_strDescription += e_vData.z.ToString()+"\tw:";
		l_strDescription += e_vData.w.ToString();
		return 	l_strDescription;
	}

	String^	CombineData(int e_iIndex,float e_fTime,cPuzzleImageUnit*e_pPIUnit)
	{
		String^l_strDescription = GetAlignIndexString(e_iIndex);
		l_strDescription += " Time:";
		char l_strTemp[MAX_PATH];
		sprintf(l_strTemp,"%.3f\n",e_fTime);
		l_strDescription += String(l_strTemp).ToString();
		l_strDescription += "x:";
		l_strDescription += DNCT::WcharToGcstring(e_pPIUnit->GetName());
		return 	l_strDescription;	
	}

	String^	ChangeIndex(int e_iIndex,String^e_strDescription)
	{
		int	l_iSkipLength = 3;
		String^l_strIndexToString = l_iSkipLength.ToString();
		l_iSkipLength -= (l_strIndexToString->Length-1)*2;
		String^l_strDescription = e_strDescription;
		String^l_strDescription2 = GetAlignIndexString(e_iIndex);
		l_strDescription2 += l_strDescription->Substring(l_iSkipLength,l_strDescription->Length-l_iSkipLength);
		if( e_strDescription->Length != l_strDescription2->Length)
		{
			int a=0;
		}
		return l_strDescription2;
	}

	float	GetProperTime(cTimeAndDataLinerUpdateInterface*e_pTimeAndDataLinerUpdateInterface)
	{
		float	l_fTime = 0.f;
		if( e_pTimeAndDataLinerUpdateInterface->Count() > 0 )
			l_fTime = *e_pTimeAndDataLinerUpdateInterface->GetTime(e_pTimeAndDataLinerUpdateInterface->Count()-1)+0.1f;
		return l_fTime;
	}
	void	AddPosToListbox(float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		InsertPosToListbox(e_pMPDINodeData->m_CurrentWorkingListbox->Items->Count,e_fTime,e_vPos,e_pMPDINodeData);
	}

	void	AddRotationToListbox(float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		AddPosToListbox(e_fTime,e_vPos,e_pMPDINodeData);
	}

	void	AddColorToListbox(float e_fTime,Vector4 e_vColor,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		InsertColorToListbox(e_pMPDINodeData->m_CurrentWorkingListbox->Items->Count,e_fTime,e_vColor,e_pMPDINodeData);
	}

	void	AddSizeToListbox(float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		InsertSizeToListbox(e_pMPDINodeData->m_CurrentWorkingListbox->Items->Count,e_fTime,e_vSize,e_pMPDINodeData);
	}

	void	AddImageToListbox(float e_fTime,cPuzzleImageUnit* e_pPIUnit,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		InsertImageToListbox(e_pMPDINodeData->m_CurrentWorkingListbox->Items->Count,e_fTime,e_pPIUnit,e_pMPDINodeData);
	}


	void	InsertPosToListbox(int e_iIndex,float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vPos,true);	
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Insert(e_iIndex,l_strDescription);	
	}

	void	InsertRotationToListbox(int e_iIndex,float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		InsertPosToListbox(e_iIndex,e_fTime,e_vPos,e_pMPDINodeData);
	}

	void	InsertColorToListbox(int e_iIndex,float e_fTime,Vector4 e_vColor,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vColor);
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Insert(e_iIndex,l_strDescription);	
	}

	void	InsertSizeToListbox(int e_iIndex,float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vSize);
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Insert(e_iIndex,l_strDescription);	
	}

	void	InsertImageToListbox(int e_iIndex,float e_fTime,cPuzzleImageUnit* e_vPIUnitImage,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vPIUnitImage);
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Insert(e_iIndex,l_strDescription);		
	}

	void	ChangeData(int e_iIndex,float e_fTime,void *e_fData,int e_iDataCount,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		switch(e_iDataCount)
		{
			case 2:
				ChangeSizeData(e_iIndex,e_fTime,*(Vector2*)e_fData,e_pMPDINodeData);
				break;
			case 3:
				ChangePosData(e_iIndex,e_fTime,*(Vector3*)e_fData,e_pMPDINodeData);
				break;
			case 4:
				ChangeColorData(e_iIndex,e_fTime,*(Vector4*)e_fData,e_pMPDINodeData);
				break;
		}
	}

	void	ChangePosData(int e_iIndex,float e_fTime,Vector3 e_vPos,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vPos,true);	
		e_pMPDINodeData->m_CurrentWorkingListbox->Items[e_iIndex] = l_strDescription;
	}

	void	ChangeColorData(int e_iIndex,float e_fTime,Vector4 e_vColor,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vColor);	
		e_pMPDINodeData->m_CurrentWorkingListbox->Items[e_iIndex] = l_strDescription;	
	}

	void	ChangeSizeData(int e_iIndex,float e_fTime,Vector2 e_vSize,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vSize);	
		e_pMPDINodeData->m_CurrentWorkingListbox->Items[e_iIndex] = l_strDescription;	
	}

	void	ChangeRotationData(int e_iIndex,float e_fTime,Vector3 e_vRotation,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vRotation,false);	
		e_pMPDINodeData->m_CurrentWorkingListbox->Items[e_iIndex] = l_strDescription;
	}

	void	ChangeImageData(int e_iIndex,float e_fTime,cPuzzleImageUnit* e_vPIUnitImage,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		String^l_strDescription = CombineData(e_iIndex,e_fTime,e_vPIUnitImage);
		e_pMPDINodeData->m_CurrentWorkingListbox->Items[e_iIndex] = l_strDescription;
	}

	String^	GetLastTimeString(cMPDINode*e_pNode)
	{
		if( e_pNode )
		{
			float	l_fEndTime = e_pNode->GetEndTime();
			int		l_iMin		= (int)(l_fEndTime/1000/60);
			int		l_iSecond	= (int)(l_fEndTime/1000-l_iMin*60);
			int		l_iMillesimalSecond = (int)UT::GetFloatModulus(l_fEndTime,1000)/1000;
			String^l_strResult = l_iMin.ToString()+":"+l_iSecond.ToString()+":"+l_iMillesimalSecond.ToString();
			return l_strResult;
		}
		return nullptr;
	}

	Vector3	GetPos(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		Vector3 l_vPos;
		e_pMPDINodeData->m_Pos->GetValue((float*)&l_vPos);
		return l_vPos;
	}
	Vector4	GetColor(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		Vector4 l_vColor;
		e_pMPDINodeData->m_Color->GetValue((float*)&l_vColor);
		return l_vColor;
	}
	Vector2	GetSize(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		Vector2 l_vSize;
		e_pMPDINodeData->m_Size->GetValue((float*)&l_vSize);
		return l_vSize;
	}
	Vector3	GetRotation(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		Vector3 l_vRotation;
		e_pMPDINodeData->m_Rotation->GetValue((float*)&l_vRotation);
		return l_vRotation;
	}

	cPuzzleImageUnit*	GetPIUnit(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		int	l_iSelectedPIIndex = e_pMPDINodeData->m_PI->m_PI_listBox->SelectedIndex;
		if( l_iSelectedPIIndex != -1 )
		{
			cPuzzleImage*l_pPI = cGameApp::m_spImageParser->GetPuzzleImage(l_iSelectedPIIndex);
			if( l_pPI )
			{
				return (cPuzzleImageUnit*)l_pPI->GetObject(e_pMPDINodeData->m_PI->m_PIUnit_listBox->SelectedIndex);
			}
		}
		return nullptr;
	}

	String^	GetAlignIndexString(int e_iCount)
	{
		String^l_strIndexString = e_iCount.ToString();
		int	l_iNumSpaceToAlign = 0;
		int	l_iLength = 3-e_iCount.ToString()->Length;
		for( int ii=0;ii<l_iLength;++ii )
		{
			l_strIndexString += "_";
		}
		return l_strIndexString;
	}

	void	ApplyAllPosDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Clear();
		cLinerDataProcessor<Vector3>*l_pData = e_pMPDINode->GetPosData();
		int	l_iCount = l_pData->Count();
		for( int i=0;i<l_iCount;++i )
		{
			AddPosToListbox(*l_pData->GetTime(i),*l_pData->GetData(i),e_pMPDINodeData);
		}
	}
	void	ApplyAllColorDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Clear();
		cLinerDataProcessor<Vector4>*l_pData = e_pMPDINode->GetColorData();
		int	l_iCount = l_pData->Count();
		for( int i=0;i<l_iCount;++i )
		{
				
			AddColorToListbox(*l_pData->GetTime(i),*l_pData->GetData(i),e_pMPDINodeData);
		}
	}
	void	ApplyAllSizeDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Clear();
		cLinerDataProcessor<Vector2>*l_pData = e_pMPDINode->GetSizeData();
		int	l_iCount = l_pData->Count();
		for( int i=0;i<l_iCount;++i )
		{
			AddSizeToListbox(*l_pData->GetTime(i),*l_pData->GetData(i),e_pMPDINodeData);
		}
	}
	void	ApplyAllRotationDataInfoToListBox(cMPDINode*e_pMPDINode,TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData)
	{
		e_pMPDINodeData->m_CurrentWorkingListbox->Items->Clear();
		cLinerDataProcessor<Vector3>*l_pData = e_pMPDINode->GetRotationData();
		int	l_iCount = l_pData->Count();
		for( int i=0;i<l_iCount;++i )
		{
			AddRotationToListbox(*l_pData->GetTime(i),*l_pData->GetData(i),e_pMPDINodeData);
		}
	}

	cTimeAndDataLinerUpdateInterface*	GetTimeAndDataLinerUpdateInterfaceFromTabControlSelectedIndex(TextureEditorWPF::MPDI_PathPointData^e_pMPDINodeData,cMPDINode*e_pMPDINode)
	{
		int	l_iIndex = e_pMPDINodeData->m_CurrentWorkingListbox->SelectedIndex;
		if( l_iIndex != -1 )
		{
			int	l_iTabControlSelevtedIndex = e_pMPDINodeData->m_TabControl->SelectedIndex;
			cTimeAndDataLinerUpdateInterface*l_pTimeAndDataLinerUpdateInterface = 0;
			switch( l_iTabControlSelevtedIndex )
			{
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_POS:
					l_pTimeAndDataLinerUpdateInterface = e_pMPDINode->GetPosData();
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_COLOR:
					l_pTimeAndDataLinerUpdateInterface = e_pMPDINode->GetColorData();
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_SIZE:
					l_pTimeAndDataLinerUpdateInterface = e_pMPDINode->GetSizeData();
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_ROTATION:
					l_pTimeAndDataLinerUpdateInterface = e_pMPDINode->GetRotationData();
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_IMAGE:
					l_pTimeAndDataLinerUpdateInterface = e_pMPDINode->GetImageData();
					break;
				default:
					return 0;
			}
			return l_pTimeAndDataLinerUpdateInterface;
		}
		return 0;
	}

	bool	AddPIFile(String^e_strFileNam)
	{
		String^l_strPIName = DNCT::GetFileNameWithoutFullPath(e_strFileNam,true);
		std::wstring	l_strWcharPIFileName = DNCT::GcStringToWchar(l_strPIName);
		if(!cGameApp::m_spImageParser->GetObject(l_strWcharPIFileName))
		{
			if(System::IO::Path::GetExtension(e_strFileNam)->Equals(".pi"))
			{
				std::wstring	l_strFullPIFileName = DNCT::GcStringToWchar(e_strFileNam);
				if(cGameApp::GetPuzzleImageByFileName(l_strFullPIFileName.c_str()))
				{
					return true;
				}
			}
		}
		return false;
	}
}