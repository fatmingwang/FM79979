#include "StdAfx.h"
#include "ProjectManager.h"

//#include "..\PI\PIForm.h"

//#import "..\..\..\..\Media\PI.dll"
//#include <comdef.h>		// for using bstr_t class 
//#include "comutil.h"
//
//#pragma comment(lib, "comsuppw.lib")
//#pragma comment(lib, "comsuppwd.lib")
//#pragma comment(lib, "Advapi32.lib")
//			std::vector<DWORD> SetOfPID;
//			GetProcessID(L"PI",SetOfPID); 
//				 // get all process id's of Rundll32
//
//			if (SetOfPID.empty())   // Process is not running
//			{
//				OutputDebugString(L"Process is not running\n");
//			}
//			else    // Process is running
//			{
//				for (int i=0;i < SetOfPID.size(); i++)
//				{
//					OutputDebugString(UT::ComposeMsgByFormat(L"Process ID is %d\n", SetOfPID[i]).c_str());
//					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,SetOfPID[i]);
//					// some code...
//					CloseHandle(hProcess);
//				}
//			}

//PPERF_OBJECT_TYPE FirstObject( PPERF_DATA_BLOCK PerfData )
//{
//    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfData + 
//        PerfData->HeaderLength) );
//}
//
//PPERF_OBJECT_TYPE NextObject( PPERF_OBJECT_TYPE PerfObj )
//{
//    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfObj + 
//        PerfObj->TotalByteLength) );
//}
//
//PPERF_INSTANCE_DEFINITION FirstInstance( PPERF_OBJECT_TYPE PerfObj )
//{
//    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfObj + 
//        PerfObj->DefinitionLength) );
//}
//
//PPERF_INSTANCE_DEFINITION NextInstance( 
//    PPERF_INSTANCE_DEFINITION PerfInst )
//{
//    PPERF_COUNTER_BLOCK PerfCntrBlk;
//
//    PerfCntrBlk = (PPERF_COUNTER_BLOCK)((PBYTE)PerfInst + 
//        PerfInst->ByteLength);
//
//    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfCntrBlk + 
//        PerfCntrBlk->ByteLength) );
//}
//
//PPERF_COUNTER_DEFINITION FirstCounter( PPERF_OBJECT_TYPE PerfObj )
//{
//    return( (PPERF_COUNTER_DEFINITION) ((PBYTE)PerfObj + 
//        PerfObj->HeaderLength) );
//}
//
//PPERF_COUNTER_DEFINITION NextCounter( 
//    PPERF_COUNTER_DEFINITION PerfCntr )
//{
//    return( (PPERF_COUNTER_DEFINITION)((PBYTE)PerfCntr + 
//        PerfCntr->ByteLength) );
//}
//
//
//PPERF_COUNTER_BLOCK CounterBlock(PPERF_INSTANCE_DEFINITION PerfInst)
//{
//	return (PPERF_COUNTER_BLOCK) ((LPBYTE) PerfInst + PerfInst->ByteLength);
//}
//
//#define TOTALBYTES    64*1024
//#define BYTEINCREMENT 1024
//
//#define PROCESS_OBJECT_INDEX	230
//#define PROC_ID_COUNTER			784
//
//
//void GetProcessID(LPCTSTR pProcessName, std::vector<DWORD>& SetOfPID)
//{
//    PPERF_DATA_BLOCK pPerfData = nullptr;
//    PPERF_OBJECT_TYPE pPerfObj;
//    PPERF_INSTANCE_DEFINITION pPerfInst;
//    PPERF_COUNTER_DEFINITION pPerfCntr, pCurCntr;
//    PPERF_COUNTER_BLOCK PtrToCntr;
//    DWORD BufferSize = TOTALBYTES;
//    DWORD i, j;
//	LONG k;
//
//// Allocate the buffer for the performance data.
//
//    pPerfData = (PPERF_DATA_BLOCK) malloc( BufferSize );
//
//	WCHAR szKey[32];
//	swprintf(szKey,L"%d %d",PROCESS_OBJECT_INDEX, PROC_ID_COUNTER);
//	LONG lRes;
//    while( (lRes = RegQueryValueEx( HKEY_PERFORMANCE_DATA,
//                               szKey,
//                               nullptr,
//                               nullptr,
//                               (LPBYTE) pPerfData,
//                               &BufferSize )) == ERROR_MORE_DATA )
//    {
//		// Get a buffer that is big enough.
//
//        BufferSize += BYTEINCREMENT;
//        pPerfData = (PPERF_DATA_BLOCK) realloc( pPerfData, BufferSize );
//    }
//
//	// Get the first object type.
//
//    pPerfObj = FirstObject( pPerfData );
//
//	// Process all objects.
//
//    for( i=0; i < pPerfData->NumObjectTypes; i++ )
//    {
//
//		if (pPerfObj->ObjectNameTitleIndex != PROCESS_OBJECT_INDEX)
//		{
//			pPerfObj = NextObject( pPerfObj );
//			continue;
//		}
//    
//		SetOfPID.clear();
//
//		// Get the first counter.
//
//        pPerfCntr = FirstCounter( pPerfObj );
//
//		// Get the first instance.
//
//        pPerfInst = FirstInstance( pPerfObj );
//
//		_bstr_t bstrProcessName,bstrInput;
//
//		// Retrieve all instances.
//
//        for( k=0; k < pPerfObj->NumInstances; k++ )
//        {
//			pCurCntr = pPerfCntr;
//			bstrInput = pProcessName;
//			bstrProcessName = (wchar_t *)((PBYTE)pPerfInst + pPerfInst->NameOffset);
//			if (!wcsicmp((LPCTSTR)bstrProcessName, (LPCTSTR) bstrInput))
//			{
//			
//				// Retrieve all counters.
//
//				for( j=0; j < pPerfObj->NumCounters; j++ )
//				{
//					if (pCurCntr->CounterNameTitleIndex == PROC_ID_COUNTER)
//					{
//						PtrToCntr = CounterBlock(pPerfInst);
//						DWORD *pdwValue = (DWORD*)((LPBYTE) PtrToCntr + pCurCntr->CounterOffset);
//						SetOfPID.push_back(*pdwValue);
//						break;
//					}
//
//
//					// Get the next counter.
//
//					pCurCntr = NextCounter( pCurCntr );
//				}
//			}
//
//			// Get the next instance.
//
//            pPerfInst = NextInstance( pPerfInst );
//        }
//    }
//	free(pPerfData);
//	RegCloseKey(HKEY_PERFORMANCE_DATA);
//}

namespace TextureEditor 
{

	template<class T>void	CreatePI(ProjectManager^e_pProjectManager,String^e_strName)
	{
		GCFORM::TabControl^l_ptabControl;
		GCFORM::Form^l_pForm = e_pProjectManager->GetForm();
		T^l_pPIEditor = gcnew T(l_ptabControl,l_pForm,e_strName);
	}

	ProjectManager::ProjectManager(GCFORM::SplitContainer^e_pSplitPanel)
	{
		m_pSplitPanel = e_pSplitPanel;
		m_pAllForm = gcnew System::Collections::ArrayList();
		//m_pEditorInterfaceList = gcnew System::Collections::ArrayList;
		m_pProjectUI = gcnew TextureEditorWPF::MPDI::ProjectUI();
		System::Windows::Forms::Integration::ElementHost^l_pElementHots = gcnew System::Windows::Forms::Integration::ElementHost;
		l_pElementHots->Dock = DockStyle::Fill;
		l_pElementHots->Child = m_pProjectUI;
		m_pSplitPanel->Panel1->Controls->Add(l_pElementHots);
		array<String^>^l_strArray = {".mpdi",".pi",".path",".xml",".collision"};

		m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->m_strExtensionName = l_strArray;
		CheckWorkingDirectory();

		m_pProjectUI->m_FMCFolderExplorerTree->f_NewFolder += gcnew EventHandler(this,&ProjectManager::NewFolder);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewMPDI += gcnew EventHandler(this,&ProjectManager::NewMPDI);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewPI += gcnew EventHandler(this,&ProjectManager::NewPI);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewPath += gcnew EventHandler(this,&ProjectManager::NewPath);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewPhysical  += gcnew EventHandler(this,&ProjectManager::NewPhysical);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewSound  += gcnew EventHandler(this,&ProjectManager::NewSound);
		m_pProjectUI->m_FMCFolderExplorerTree->f_Copy += gcnew EventHandler(this,&ProjectManager::Copy);
		m_pProjectUI->m_FMCFolderExplorerTree->f_Paste += gcnew EventHandler(this,&ProjectManager::Paste);
		m_pProjectUI->m_FMCFolderExplorerTree->f_Delete += gcnew EventHandler(this,&ProjectManager::Delete);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewParticle += gcnew EventHandler(this,&ProjectManager::NewParticle);
		m_pProjectUI->m_FMCFolderExplorerTree->f_SetWorkingDirectory += gcnew EventHandler(this,&ProjectManager::SetWorkingDirectory);
		m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->f_Drop += gcnew EventHandler(this,&ProjectManager::Drop);
		m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->f_MouseDoubleClick += gcnew EventHandler(this,&ProjectManager::MouseDoubleClick);
		//m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->f_KeyUp += gcnew EventHandler(this,&ProjectManager::KeyUp);

	}
	ProjectManager::~ProjectManager()
	{
		delete m_pAllForm;
		//for(int i=0;i<m_pEditorInterfaceList->Count;++i)
		//{
		//	System::Object^l_pObject = m_pEditorInterfaceList[i];
		//	delete l_pObject;
		//}
		//delete m_pEditorInterfaceList;
	}



	//<Root	Directory="C:\Users\Fatming\Desktop\FM79979\Media">
	System::Void ProjectManager::CheckWorkingDirectory()
	{
		cNodeISAX	l_NodeISAX;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode("AnimationEditorWorkingdirectory.xml");
		if(l_b)
		{
			TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strDirectory = l_pTiXmlElement->Attribute(L"Directory");
			if( l_strDirectory )
			{
				m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->SetWorkingProject(DNCT::WcharToGcstring(l_strDirectory));
			}
		}
		else
		{
			WCHAR	l_str[MAX_PATH];
			GetCurrentDirectory(MAX_PATH,l_str);
			m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->SetWorkingProject(DNCT::WcharToGcstring(l_str));
		}
	}



	GCFORM::Form^ProjectManager::GetForm()
	{
		GCFORM::Form^l_pForm = gcnew GCFORM::Form();
		l_pForm->TopLevel = false;
		HWND l_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
		HWND l_pFormHandle = (HWND)l_pForm->Handle.ToPointer();
		//::Threading::Thread::Sleep(1000);
		SetParent(l_pFormHandle,l_pParentHandle);
		l_pForm->Show();
		l_pForm->WindowState = GCFORM::FormWindowState::Maximized;
		//l_pForm->Parent = m_pProjectUI->m_ContainerBorder->Child;
		l_pForm->KeyPreview = true;
		l_pForm->KeyUp += gcnew KeyEventHandler(this,&ProjectManager::ProjectManager_KeyUp);
		m_pAllForm->Add(l_pForm);
		return l_pForm;
	}
	System::Void ProjectManager::NewFolder(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void ProjectManager::NewInstance(String^e_strFileName,String^e_strTargetFileName)
	{
		if( System::IO::File::Exists(e_strFileName) )
		{
			HWND l_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
			GCFORM::Form^l_pForm = GetForm();
			OutputDebugString(DNCT::GcStringToWchar(this->m_pSplitPanel->Panel2->Handle.ToString()));
			//System::Diagnostics::ProcessStartInfo^l_pProcessStartInfo = gcnew System::Diagnostics::ProcessStartInfo(e_strFileName,this->m_pSplitPanel->Panel2->Handle.ToString());
			System::Diagnostics::ProcessStartInfo^l_pProcessStartInfo = gcnew System::Diagnostics::ProcessStartInfo(e_strFileName,e_strTargetFileName);
			l_pProcessStartInfo->UseShellExecute = false;
			//System::Diagnostics::ProcessStartInfo^l_pProcessStartInfo = gcnew System::Diagnostics::ProcessStartInfo("Notepad.exe","");
			System::Diagnostics::Process^l_pProcess = System::Diagnostics::Process::Start(l_pProcessStartInfo);

			try 
			{
				//l_pProcess->WaitForInputIdle();
				//while (!l_pProcess->HasExited && l_pProcess->MainWindowHandle == IntPtr::Zero )
				{
					//Threading::
					//Sleep(1000);
				}
				//l_pProcess->SynchronizingObject = l_pForm;
				//SetWindowLong((HWND)l_pProcess->MainWindowHandle.ToPointer(), GWL_STYLE, WS_VISIBLE);
				//SetParent((HWND)l_pProcess->MainWindowHandle.ToPointer(),(HWND)l_pForm->Handle.ToPointer());
				//SetParent((HWND)l_pProcess->MainWindowHandle.ToPointer(),l_pParentHandle);
				//MoveWindow((HWND)l_pProcess->MainWindowHandle.ToPointer(), 0, 0, l_pForm->Width, l_pForm->Height, true);
				//MoveWindow((HWND)l_pProcess->MainWindowHandle.ToPointer(), 0, 0, m_pSplitPanel->Panel2->Width, m_pSplitPanel->Panel2->Height, true);
				//l_pForm->Controls->Add(GCFORM::Control::FromHandle(l_pProcess->MainWindowHandle));
				//m_pSplitPanel->Panel2->Controls->Add(GCFORM::Control::FromHandle(l_pProcess->MainWindowHandle));
			}
			catch(Exception^ ex)
			{
				WARNING_MSG(String::Format("Could not kill process {0}, exception {1}", l_pProcess->ToString(), ex->ToString()));
			}
		}
		else
		{
			WARNING_MSG(e_strFileName +" is not exist!");
		}	 
	}

	System::Void ProjectManager::NewMPDI(System::Object^  sender, System::EventArgs^  e)
	{
		NewInstance("MPDI.exe","");
		//String^l_strFileName;
		//CreatePI<MPDI::MPDIEditor>(this,l_strFileName);
		//GCFORM::Form^l_pForm = PI::cPIEditor::CallForm(l_strFileName);
		//m_pAllForm->Add(l_pForm);
	}

	System::Void ProjectManager::NewPI(System::Object^  sender, System::EventArgs^  e)
	{
		//String^l_strFileName;
		//CreatePI<PI::cPIEditor>(this,l_strFileName);
		NewInstance("PI.exe","");
	}

	System::Void ProjectManager::NewPath(System::Object^  sender, System::EventArgs^  e)
	{
		NewInstance("PathEditor.exe","");
	}

	System::Void ProjectManager::NewPhysical(System::Object^  sender, System::EventArgs^  e)
	{
		NewInstance("BulletDataEditor.exe","");
	}

	System::Void ProjectManager::NewParticle(System::Object^  sender, System::EventArgs^  e)
	{
		NewInstance("ParticalEditor.exe","");
	}

	System::Void ProjectManager::NewSound(System::Object^  sender, System::EventArgs^  e)
	{
		NewInstance("Sound.exe","");
	}

	System::Void ProjectManager::Copy(System::Object^  sender, System::EventArgs^  e)
	{
		
	}

	System::Void ProjectManager::Paste(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void ProjectManager::Delete(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void ProjectManager::Drop(System::Object^  sender, System::EventArgs^  e)
	{
	
	}

	System::Void ProjectManager::MouseDoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strFileName = m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->m_SelectedFileNameTextBlock->Text;
		String^l_strExtensionName = System::IO::Path::GetExtension(l_strFileName);
		GCFORM::Form^l_pForm;
		bool	l_bMatched = false;
		int	l_iTargetIndex = -1;//0 for mpdi 1 for pi
		if( l_strExtensionName->Equals(".mpdi") )
		{
			l_iTargetIndex = 0;
			bool	l_bExist = false;
			//check ignore edited object?
		}
		else
		if( l_strExtensionName->Equals(".pi") )
		{
			l_iTargetIndex = 1;
		}
		else
		if( l_strExtensionName->Equals(".collision") )
		{
			l_iTargetIndex = 4;
		}
		if( l_iTargetIndex != -1 )
		{
			//System::Windows::Forms::UserControl^l_pUserControl;
			if( l_iTargetIndex == 0 )
			{
				//CreatePI<MPDI::MPDIEditor>(this,l_strFileName);
				NewInstance("MPDI.exe",l_strFileName);
			}
			else
			if( l_iTargetIndex == 1 )
			{
				//CreatePI<PI::cPIEditor>(this,l_strFileName);
				NewInstance("PI.exe",l_strFileName);
			}
			else
			if( l_iTargetIndex == 4 )
			{
				NewInstance("BulletDataEditor.exe",l_strFileName);
			}
			if( l_pForm )
			{
				l_pForm->Text = l_strFileName;
				l_pForm->KeyPreview = true;
				l_pForm->KeyUp += gcnew KeyEventHandler(this,&ProjectManager::ProjectManager_KeyUp);
			}
		}
	}
	System::Void ProjectManager::SetWorkingDirectory(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strDirectory = DNCT::SelectDirectory();
		if(l_strDirectory)
		{
			TiXmlDocument doc;
			TiXmlElement * root = new TiXmlElement( L"Root" );  
			root->SetAttribute(L"Directory",DNCT::GcStringToWchar(l_strDirectory));
			doc.LinkEndChild( root );  	
			doc.SaveFile("AnimationEditorWorkingdirectory.xml");
			m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->SetWorkingProject(l_strDirectory);
		}
	}

	System::Void ProjectManager::ProjectManager_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				//for each(GCFORM::Form^l_pForm in m_pAllForm)
				//{
				//	if( l_pForm == sender )
				//	{
				//		if( e->KeyCode == GCFORM::Keys::F11 )
				//		{
				//			 //MPDIEditor^l_pMPDIEditor = dynamic_cast<MPDIEditor^>(l_pForm->Controls[0]);
				//			 //if(l_pForm->FormBorderStyle == GCFORM::FormBorderStyle::None )
				//			 //{
				//				// l_pForm->FormBorderStyle = GCFORM::FormBorderStyle::SizableToolWindow;
				//			 //}
				//			 //else
				//			 //{
				//				// l_pForm->FormBorderStyle = GCFORM::FormBorderStyle::None;
				//			 //}
				//			 //if(l_pForm->WindowState != GCFORM::FormWindowState::Maximized )
				//			 //{
				//				//l_pForm->WindowState = GCFORM::FormWindowState::Maximized;
				//			 //}
				//		}
				//	}
				//}
			 }
	//System::Void ProjectManager::KeyUp(System::Object^  sender, System::EventArgs^  e)
	//{
	//	//String^l_strExtenStionName = System::IO::Path::GetExtension(m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->m_SelectedFileNameTextBlock->Text)
	//}
}