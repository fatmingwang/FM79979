#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{

	public ref class cFatmingUtilitiesBase
	{
	public:
		NamedTypedObject*m_pCurrentSelectedObject;
	public:
		cFatmingUtilitiesBase()
		{
			m_pCurrentSelectedObject = 0;
		}
		~cFatmingUtilitiesBase()
		{
		
		}
	};
}