#pragma once
#include "LinerTemplateDataProcess.h"

namespace	FATMING_CORE
{

	class cLinerDataMouseChanger:public cLinerDataContainer
	{
	public:
		void	MouseDown(int e_iPosX,int e_iPosY);
		void	MouseMove(int e_iPosX,int e_iPosY);
		void	MouseUp(int e_iPosX,int e_iPosY);
	};

}