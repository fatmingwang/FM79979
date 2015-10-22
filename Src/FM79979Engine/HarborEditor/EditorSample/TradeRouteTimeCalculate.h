#pragma once

#include "TradeRoutes.h"
#include "RegionMapChange.h"
#include "MapPointInfo.h"
#include "RegionMapInfo.h"
#include "TradeRouteFinder.h"

#include "TradeRoutes.h"
#include "muParser.h"
#include "PortNameXML.h"

#include "../../Core/Math/MathFormulaXML.h"
extern char*g_strSelectDirectory;


namespace EditorSample {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for TradeRouteTimeCalculate
	/// </summary>
	public ref class TradeRouteTimeCalculate : public System::Windows::Forms::Form
	{
	public:
		TradeRouteTimeCalculate(void)
		{
			//cMathFormulaXML	l_cMathFormulaXML;
			//l_cMathFormulaXML.ParseWithMyParse("C:/Users/USER/Desktop/Equation2.xml");
			//l_cMathFormulaXML.Export("C:/Users/USER/Desktop/Equation2.xml");
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			m_pTradeRouteTimeCalculateUI = gcnew HarborUI::TradeRouteTimeCalculateUI();
			System::Windows::Forms::Integration::ElementHost^l_pElementHots = gcnew System::Windows::Forms::Integration::ElementHost;
			l_pElementHots->Dock = DockStyle::Fill;
			l_pElementHots->Child = m_pTradeRouteTimeCalculateUI;
			Controls->Add(l_pElementHots);
			//m_pTradeRouteTimeCalculateUI->f_
			m_pRegionMapChange = new cRegionMapChange();

			m_pTradeRouteTimeCalculateUI->f_AllRoute						+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_AllRoute);
			m_pTradeRouteTimeCalculateUI->f_Apply							+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_Apply);
			m_pTradeRouteTimeCalculateUI->f_Excel							+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_Excel);
			m_pTradeRouteTimeCalculateUI->f_FileOpen						+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_FileOpen);
			m_pTradeRouteTimeCalculateUI->f_FileSave						+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_FileSave);
			m_pTradeRouteTimeCalculateUI->f_Output							+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_Output);
			m_pTradeRouteTimeCalculateUI->f_MapNameListbox_SelectionChanged +=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_MapNameListbox_SelectionChanged);
			m_pTradeRouteTimeCalculateUI->f_ExcelToXML						+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_ExcelToXML);
			m_pTradeRouteTimeCalculateUI->f_ApplyEquation					+=	gcnew System::EventHandler(this, &TradeRouteTimeCalculate::f_ApplyEquation);
			

			m_pTradeRouteTimeCalculateUI->SetFunction();
			//m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text = HarborUI::MyExcel::ExcelToXML("C:/Users/USER/Desktop/77.xlsx");
			m_pdbDistance = nullptr;
			m_pbSpeed = nullptr;
			m_pMathFormulaXML = nullptr;
			m_pMathFormulaXML = new cMathFormulaXML();
			if( m_pMathFormulaXML )
			{
				std::wstring l_strEquation = DNCT::GcStringToWchar(m_pTradeRouteTimeCalculateUI->m_MUMathParserTextbox->Text);
				m_pMathFormulaXML->SetExpr(l_strEquation);
				double l_pbSpeed = System::Double::Parse(m_pTradeRouteTimeCalculateUI->m_SpeedTextbox->Text);
				m_pdbDistance = m_pMathFormulaXML->AddDefineVariable(L"Dis",1);
				m_pbSpeed = m_pMathFormulaXML->AddDefineVariable(L"Speed",l_pbSpeed);
			}
			//m_pMathFormulaXML->GetParser()->DefineVar(L"Dis",m_pdbDistance);
			//m_pbSpeed = m_pMathFormulaXML->GetParser()->DefineVar(L"Speed",l_pbSpeed);
			m_pPortsName = new cPortsName();

			FileOpen("C:/Users/USER/Desktop/NewOnePiece/Client/Media/CommonData/Map/MapData.xml");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TradeRouteTimeCalculate()
		{
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(m_pRegionMapChange);
			SAFE_DELETE(g_strSelectDirectory);
			SAFE_DELETE(m_pPortsName);
			SAFE_DELETE(m_pMathFormulaXML);
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		//my
		HarborUI::TradeRouteTimeCalculateUI^m_pTradeRouteTimeCalculateUI;
		cRegionMapChange*					m_pRegionMapChange;
		cMathFormulaXML*					m_pMathFormulaXML;
		double*								m_pdbDistance;
		double*								m_pbSpeed;
		cPortsName*							m_pPortsName;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->Size = System::Drawing::Size(300,300);
			this->Text = L"TradeRouteTimeCalculate";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		}
#pragma endregion
		void								OutputResult(bool e_bDetail,cscommon::CTradeRouteList*e_pResult)
		{
			if( m_pbSpeed )
				*m_pbSpeed = System::Double::Parse(m_pTradeRouteTimeCalculateUI->m_SpeedTextbox->Text->ToString());
			if( e_pResult->size() )
			{
				if( e_bDetail )
				{
					cIDAndName*l_pIDAndNameStart = m_pPortsName->GetObject(DNCT::GcStringToWchar((*e_pResult)[0]->iStartPortID.ToString()));
					if( l_pIDAndNameStart )
					{
						String^l_strCombineString = l_pIDAndNameStart->m_iPortID.ToString()+","+DNCT::WcharToGcstring(l_pIDAndNameStart->m_PortName.c_str());
						m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text +=l_strCombineString;
					}
					else
						m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text +=(*e_pResult)[0]->iStartPortID.ToString();
					m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "->";
					for (size_t i = 0; i < e_pResult->size(); i++)
					{

						cIDAndName*l_pIDAndName = m_pPortsName->GetObject(DNCT::GcStringToWchar((*e_pResult)[i]->iEndPortID.ToString()));
						if( l_pIDAndName )
						{
							String^l_strCombineString = l_pIDAndName->m_iPortID.ToString()+","+DNCT::WcharToGcstring(l_pIDAndName->m_PortName.c_str());
							m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += l_strCombineString;
						}
						else
							m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += (*e_pResult)[i]->iEndPortID.ToString();
						if( m_pdbDistance )
							*m_pdbDistance = (*e_pResult)[i]->getDistance();
						double	l_fTime = m_pMathFormulaXML->GetResult();
						char l_strTime[260];
						sprintf(l_strTime,"%.2f",(float)l_fTime);
						m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += ":Time:"+gcnew String(l_strTime);
						m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += DNCT::GetChanglineString();
						if( i+1 != e_pResult->size())
							m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "->";
					}
				}
				else
				{
					m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text +=(*e_pResult)[0]->iStartPortID.ToString();
					m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "->";
					m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text +=(*e_pResult)[e_pResult->size()-1]->iEndPortID.ToString();
				}
				if( m_pdbDistance )
					*m_pdbDistance = e_pResult->getTotalDistance();
				double	l_fTime = m_pMathFormulaXML->GetResult();
				char l_strTime[260];
				sprintf(l_strTime,"%.2f",(float)l_fTime);
				m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += ":Time:"+gcnew String(l_strTime);
				m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += DNCT::GetChanglineString();
			}
			else
			{
				m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "same point or Data Error!!";
				m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += DNCT::GetChanglineString();
			}
		}
		System::Void f_PointDropEvent(System::Object^  sender, System::EventArgs^  e)
		{
			if( sender == m_pTradeRouteTimeCalculateUI->m_StartPortNameTextbox )
			{
			
			}
			else
			if( sender == m_pTradeRouteTimeCalculateUI->m_EndPortNameTextbox )
			{
			
			}
		}
        System::Void f_AllRoute(System::Object^  sender, System::EventArgs^  e)
		{
			array<String^>^l_str1 = m_pTradeRouteTimeCalculateUI->m_StartPortNameTextbox->Text->Split(',');
			int	l_iStartID = System::Int32::Parse(l_str1[0]);
			int	l_iStartMapId = l_iStartID/1000;
			m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text = "";
			for (int i = 0; i < m_pTradeRouteTimeCalculateUI->m_MapNameListbox->Items->Count; i++)
			{
				cRegionMap*l_pRegionMap = m_pRegionMapChange->GetObject(i);
				for (int j = 0; j < l_pRegionMap->m_pRegionMapPointInfoManager->Count(); j++)
				{
					int	l_iEndID = l_pRegionMap->m_pRegionMapPointInfoManager->GetObject(j)->GetID();
					int	l_iEndMapId = l_iEndID/1000;
					cscommon::CTradeRouteList	l_CTradeRouteList;
					cscommon::getRoute( l_iStartMapId, l_iStartID, l_iEndMapId,l_iEndID,&l_CTradeRouteList);
					OutputResult(true,&l_CTradeRouteList);
					//if( l_CTradeRouteList.size() )
					//{
					//	m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text +=l_CTradeRouteList[0]->iStartPortID.ToString();
					//	m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "->";
					//	for (size_t k = 0; k < l_CTradeRouteList.size(); k++)
					//	{
					//		m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += l_CTradeRouteList[k]->iEndPortID.ToString();
					//		if( k+1 != l_CTradeRouteList.size())
					//			m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "->";
					//	}
					//	m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += DNCT::GetChanglineString();
					//}
					//else
					//{
					//	m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "Data Error!!";
					//	m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += DNCT::GetChanglineString();
					//}
				}
			}
		}
        System::Void f_Output(System::Object^  sender, System::EventArgs^  e)
		{
			f_Apply(sender,e);
		}
		System::Void f_ApplyEquation(System::Object^  sender, System::EventArgs^  e)
		{
			std::wstring l_strEquation = DNCT::GcStringToWchar(m_pTradeRouteTimeCalculateUI->m_MUMathParserTextbox->Text);
			m_pMathFormulaXML->SetExpr(l_strEquation);			
		}
		System::Void f_ExcelToXML(System::Object^  sender, System::EventArgs^  e)
		{
			String^l_strFileName = DNCT::OpenFileAndGetName("xlsx files (*.xlsx)|*.xlsx|All files (*.*)|*.*");
			if( l_strFileName  )
				HarborUI::MyExcel::ExcelToXML(l_strFileName);
		}
        System::Void f_Excel(System::Object^  sender, System::EventArgs^  e)
		{
			String^l_strFileName = DNCT::SaveFileAndGetName("xlsx files (*.xlsx)|*.xlsx|All files (*.*)|*.*");
			if( l_strFileName  )
			{
				//because while excel is created,it will create sheet1 to 3,and I have no idea how to delete them.
				System::Data::DataTable^l_pTable = gcnew System::Data::DataTable("Sheet4");
				l_pTable->Columns->Add("Name");
				l_pTable->Columns->Add("ÖÐÎÄÃû×Ö");
				l_pTable->Columns->Add("Distance");
				l_pTable->Columns->Add("NewDistance");
				for (int i = 0; i < this->m_pRegionMapChange->Count(); i++)
				{
					cRegionMap*l_pRegionMap = m_pRegionMapChange->GetObject(i);
					cTradeRoutes*l_pTradeRoutes = nullptr;
					for (int j = 0; j < l_pRegionMap->m_pTradeRoutesManager->Count(); j++)
					{
						l_pTradeRoutes = (*l_pRegionMap->m_pTradeRoutesManager)[j];
						String^l_strName = DNCT::WcharToGcstring(l_pTradeRoutes->GetStartPoint()->GetName())+"_"+DNCT::WcharToGcstring(l_pTradeRoutes->GetEndPoint()->GetName());
						//String^l_strName = l_pTradeRoutes->GetStartPoint()->GetName();
						cIDAndName*l_pStartIDAndName = m_pPortsName->GetObject(l_pTradeRoutes->GetStartPoint()->GetName());
						cIDAndName*l_pEndIDAndName = m_pPortsName->GetObject(l_pTradeRoutes->GetEndPoint()->GetName());
						String^l_strStartPointName = "º½üc";
						String^l_strEndPointName = "º½üc";
						if( l_pStartIDAndName )
							l_strStartPointName = DNCT::WcharToGcstring(l_pStartIDAndName->m_PortName.c_str());
						if( l_pEndIDAndName )
							l_strEndPointName = DNCT::WcharToGcstring(l_pEndIDAndName->m_PortName.c_str());
						String^l_strChinese = l_strStartPointName+"_"+l_strEndPointName;
						l_pTable->Rows->Add(l_strName,l_strChinese,l_pTradeRoutes->GetDistance().ToString(),"0");
					}
				}
				{
					System::Data::DataSet^l_pDataSet = gcnew System::Data::DataSet("Test456");
					l_pDataSet->Tables->Add(l_pTable);
					HarborUI::MyExcel::Save(l_strFileName,l_pDataSet);
				}
				//HarborUI::MyExcel::ExcelToXML(l_strFileName);
			}
		}
        System::Void f_Apply(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pTradeRouteTimeCalculateUI->m_EndPortNameTextbox->Text->Length == 0||
				m_pTradeRouteTimeCalculateUI->m_StartPortNameTextbox->Text->Length == 0)
				return;
			try
			{
				array<String^>^l_str1 = m_pTradeRouteTimeCalculateUI->m_StartPortNameTextbox->Text->Split(',');
				array<String^>^l_str2 = m_pTradeRouteTimeCalculateUI->m_EndPortNameTextbox->Text->Split(',');
				int	l_iStartID = System::Int32::Parse(l_str1[0]);
				int	l_iEndID = System::Int32::Parse(l_str2[0]);
				int	l_iStartMapId = l_iStartID/1000;
				int	l_iEndMapId = l_iEndID/1000;
				m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text = "";
				cscommon::CTradeRouteList	l_CTradeRouteList;
				cscommon::getRoute( l_iStartMapId, l_iStartID, l_iEndMapId,l_iEndID,&l_CTradeRouteList);
				OutputResult(true,&l_CTradeRouteList);
				//if( l_CTradeRouteList.size() )
				//{
				//	m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text +=l_CTradeRouteList[0]->iStartPortID.ToString();
				//	m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "->";
				//	for (size_t i = 0; i < l_CTradeRouteList.size(); i++)
				//	{
				//		m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += l_CTradeRouteList[i]->iEndPortID.ToString();
				//		if( i+1 != l_CTradeRouteList.size())
				//			m_pTradeRouteTimeCalculateUI->m_OutputTextbox->Text += "->";
				//	}
				//}
			}
			catch(Exception^e)
			{
				MessageBox::Show(e->ToString());
			}

		}
        System::Void f_FileSave(System::Object^  sender, System::EventArgs^  e)
		{
		
		}

        System::Void f_MapNameListbox_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
		{
			m_pTradeRouteTimeCalculateUI->m_PortNameListbox->Items->Clear();
			int	l_iSelectedIndex = m_pTradeRouteTimeCalculateUI->m_MapNameListbox->SelectedIndex;
			if( l_iSelectedIndex != -1 )
			{
				cRegionMap*l_pRegionMap = m_pRegionMapChange->GetObject(l_iSelectedIndex);
				for (int i = 0; i < l_pRegionMap->m_pRegionMapPointInfoManager->Count(); i++)
				{
					cMapPointInfo*l_pMapPointInfo = l_pRegionMap->m_pRegionMapPointInfoManager->GetObject(i);

					
					cIDAndName*l_pIDAndName = m_pPortsName->GetObject(l_pMapPointInfo->GetName());
					if( l_pIDAndName )
					{
						String^l_strCombineString = l_pIDAndName->m_iPortID.ToString()+","+DNCT::WcharToGcstring(l_pIDAndName->m_PortName.c_str());
						m_pTradeRouteTimeCalculateUI->m_PortNameListbox->Items->Add(gcnew String(l_strCombineString));
					}
					else
						m_pTradeRouteTimeCalculateUI->m_PortNameListbox->Items->Add(gcnew String(l_pMapPointInfo->GetCharName().c_str()));

				}
			}
			
		}
		
		System::Void FileOpen(String^e_strFileName)
		{
			m_pTradeRouteTimeCalculateUI->m_MapNameListbox->Items->Clear();
			m_pTradeRouteTimeCalculateUI->m_PortNameListbox->Items->Clear();
			m_pRegionMapChange->Destroy();
			String^l_strfileName = e_strFileName;
			if( l_strfileName )
			{
				{
					std::string l_strPortName = DNCT::GcStringToChar(System::IO::Path::GetDirectoryName(l_strfileName));
					l_strPortName += "/";
					l_strPortName += "HorbarName.xml";

					String^l_strTempFileName = "Tempfile.qoo";
					System::IO::StreamReader^l_pReader = gcnew System::IO::StreamReader(gcnew String(l_strPortName.c_str()),System::Text::Encoding::UTF8);
					System::IO::StreamWriter^l_pWriter = gcnew System::IO::StreamWriter(l_strTempFileName,false,System::Text::Encoding::UTF32);
					l_pWriter->Write(l_pReader->ReadToEnd());
					l_pWriter->Close();
					m_pPortsName->ParseWithMyParse(DNCT::GcStringToChar(l_strTempFileName));
					System::IO::File::Delete(l_strTempFileName);
					delete l_pReader;
					delete l_pWriter;
				}
				String^l_strTempFileName = "Tempfile.qoo";
				System::IO::StreamReader^l_pReader = gcnew System::IO::StreamReader(l_strfileName,System::Text::Encoding::UTF8);
				System::IO::StreamWriter^l_pWriter = gcnew System::IO::StreamWriter(l_strTempFileName,false,System::Text::Encoding::UTF32);
				l_pWriter->Write(l_pReader->ReadToEnd());
				l_pWriter->Close();
				if( System::IO::File::Exists(l_strTempFileName) )
				{
					SAFE_DELETE(g_strSelectDirectory);
					String^l_strDirectory = System::IO::Path::GetDirectoryName(l_strfileName);
					g_strSelectDirectory = new char[2048];
					memset(g_strSelectDirectory,0,2048);
					strcpy(g_strSelectDirectory,DNCT::GcStringToChar(l_strDirectory));
					if(m_pRegionMapChange->ParseWithMyParse(DNCT::GcStringToChar(l_strTempFileName)) == false)
					{
						WARNING_MSG("file error1");
					}
					else
					{
						for (int i = 0; i < m_pRegionMapChange->Count(); i++)
						{
							m_pTradeRouteTimeCalculateUI->m_MapNameListbox->Items->Add(gcnew String(m_pRegionMapChange->GetObject(i)->GetCharName().c_str()));
						}						
					}
					System::IO::File::Delete(l_strTempFileName);

				}
				else
				{
					WARNING_MSG("file error2");
				}
				delete l_pReader;
				delete l_pWriter;
			}		
		}
        System::Void f_FileOpen(System::Object^  sender, System::EventArgs^  e)
		{
			m_pTradeRouteTimeCalculateUI->m_MapNameListbox->Items->Clear();
			m_pTradeRouteTimeCalculateUI->m_PortNameListbox->Items->Clear();
			m_pRegionMapChange->Destroy();
			String^l_strfileName = DNCT::OpenFileAndGetName();
			FileOpen(l_strfileName);
		}
	};
}
