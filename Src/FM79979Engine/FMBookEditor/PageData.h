#ifndef _PAGE_DATA_H_
#define _PAGE_DATA_H_
#include "FMBookPageDat.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class   cPageData
{
public:
    cPageData(){}
    cPageData(cPageData^e_pPageData)
    {
        //m_Pos = e_pPageData->m_Pos;
        //m_Size = e_pPageData->m_Size;
        m_eEventObjectType = e_pPageData->m_eEventObjectType;
        //m_pTextBox = gcnew GCFORM::TextBox;
        m_pTextBox = e_pPageData->m_pTextBox;
        //m_pTextBox->Text = e_pPageData->m_pTextBox->Text;
    }   
    
    ~cPageData(){}
    
    //System::Drawing::Point  m_Pos;
    //System::Drawing::Point  m_Size;
    eEventObjectType   m_eEventObjectType;
    //eEOT_TEXT
    GCFORM::TextBox^m_pTextBox;
    ////eEOT_MPDI
    //cMultiPathDynamicImage*m_pMPDI;
    //cMPDIList*m_pMPDIList;
    ////eEOT_IMAGE
    //cPuzzleImage*m_pPI;
    //cPuzzleImageUnit*m_pPIUnit;
};


public ref class cPageDataList
{
public:
    cPageDataList(){ m_pPageDataList = gcnew System::Collections::ArrayList; }
    cPageDataList(cPageDataList^e_pPageDataList)
    {
        m_pPageDataList = gcnew System::Collections::ArrayList;
        for(int i=0;i<e_pPageDataList->m_pPageDataList->Count;++i)
        {
            cPageData^l_pPageData = (cPageData^)e_pPageDataList->m_pPageDataList[i];
            m_pPageDataList->Add(gcnew cPageData(l_pPageData));
        }
    }
    ~cPageDataList(){}
    System::Collections::ArrayList^m_pPageDataList;
    String^m_strName;
};


#endif