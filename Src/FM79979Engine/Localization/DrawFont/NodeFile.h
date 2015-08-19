#pragma once
using namespace System::Xml;
ref class NodeFile
{	
public:
	System::Collections::Hashtable^m_pHashTable;
	NodeFile(){GCNEW(System::Collections::Hashtable,m_pHashTable);}

	System::Void	LoadFile(System::String^e_pFileName,System::String^e_pStringForPath);
	System::Void	WriteFile(System::String^e_pStringForPath);
};
