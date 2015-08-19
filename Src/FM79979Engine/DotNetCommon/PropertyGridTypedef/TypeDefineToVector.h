#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Globalization;

public ref class cVector3
{
	public:
	cVector3(){m_fx = "0";m_fy = "0";m_fz = "0"; m_pvVector3 = new float[3];}
	~cVector3(){delete m_pvVector3; }
	PUBLIC_OBJECT(String^,m_fx,X);
	PUBLIC_OBJECT(String^,m_fy,Y);
	PUBLIC_OBJECT(String^,m_fz,Z);
	float*	m_pvVector3;
	void	SetVector(float*e_vVector3)
	{
		memcpy(m_pvVector3,e_vVector3,sizeof(float)*3);
		m_fx = e_vVector3[0].ToString();
		m_fy = e_vVector3[1].ToString();
		m_fz = e_vVector3[2].ToString();
	}
	float*	GetVector()
	{
		return 	m_pvVector3;
	}
};

ref class Vector3Convert :ExpandableObjectConverter
{
public:virtual bool CanConvertTo(ITypeDescriptorContext^ context,
		System::Type^ destinationType) override
	{
		if (destinationType == cVector3::typeid)
			return true;

		return CanConvertTo(context, destinationType);
	}
	virtual Object^ ConvertTo(ITypeDescriptorContext^ context,
							   CultureInfo^ culture, 
							   Object^ value, 
							   Type^ destinationType) override
	{
		//if (destinationType == Double::typeid&& 
		if( destinationType == System::String::typeid &&
			value->GetType() == cVector3::typeid)
		{
			cVector3^ so = (cVector3^)value;
			int	l_iExp = 0;
			try{
				so->m_fx = Double::Parse(so->m_fx).ToString();
				l_iExp++;
				so->m_fy = Double::Parse(so->m_fy).ToString();
				l_iExp++;
				so->m_fz = Double::Parse(so->m_fz).ToString();
			}
			catch(System::Exception^exp)
			{
				//MessageBox(0,"Error","not numeral",MB_OK_;exp->ToString();
				if( l_iExp == 0 )
					so->m_fx = "1.0";
				else
				if( l_iExp == 1 )
					so->m_fy = "1.0";
				else
				if( l_iExp == 2 )
					so->m_fz = "1.0";
				exp->ToString();

			}
			return "x:"+so->m_fx + ","
				   "y:"+ so->m_fy +","
				   "z:"+ so->m_fz;
		}
		return ConvertTo(context, culture, value, destinationType);
	}
	virtual bool CanConvertFrom(ITypeDescriptorContext^ context,
						System::Type^ sourceType) override
	{
		//if (sourceType == Double::typeid)
		if (sourceType == System::String::typeid)
			return true;

		return CanConvertFrom(context, sourceType);
	}

	virtual Object^ ConvertFrom(ITypeDescriptorContext^ context,
								  CultureInfo^ culture, Object ^value) override
	{
		if (value->GetType()== String::typeid) {
			try 
			{
				String^ s = (String^) value;
				int colon = s->IndexOf(':');
				int comma = s->IndexOf(',');

				if (colon != -1 && comma != -1) 
				{
					String ^checkWhileTyping = s->Substring(colon + 1 ,
													(comma - colon - 1));

					colon = s->IndexOf(':', comma + 1);
					comma = s->IndexOf(',', comma + 1);

					String^ checkCaps = s->Substring(colon + 1 , 
													(comma - colon -1));

					colon = s->IndexOf(':', comma + 1);

					String^ suggCorr = s->Substring(colon + 1);

					cVector3^ so = gcnew cVector3();

					so->m_fx = Double::Parse(checkWhileTyping).ToString();
					so->m_fy = Double::Parse(checkCaps).ToString();
					so->m_fz = Double::Parse(suggCorr).ToString();
	                    
					return so;
				}
				else
				{
					//cVector3^ so = gcnew cVector3();
					//return so;
					throw gcnew ArgumentException(
						"Can not convert '" + (String^)value + 
						"' to type SpellingOptions");
				}
			}
			catch(System::Exception^exp) 
			{
				throw gcnew ArgumentException(
					"Can not convert '" + (String^)value + 
					"' to type SpellingOptions"+exp->ToString());
			}
		}  
		return ConvertFrom(context, culture, value);
	}
};







public ref class cVector4
{
	public:
		cVector4(){m_fr = "0";m_fg = "0";m_fb = "0";m_fa = "0";}
		PUBLIC_OBJECT(String^,m_fr,R);
		PUBLIC_OBJECT(String^,m_fg,G);
		PUBLIC_OBJECT(String^,m_fb,B);
		PUBLIC_OBJECT(String^,m_fa,A);
};

ref class Vector4Convert :ExpandableObjectConverter
{
public:virtual bool CanConvertTo(ITypeDescriptorContext^ context,
		System::Type^ destinationType) override
	{
		if (destinationType == cVector4::typeid)
			return true;

		return CanConvertTo(context, destinationType);
	}
	virtual Object^ ConvertTo(ITypeDescriptorContext^ context,
							   CultureInfo^ culture, 
							   Object^ value, 
							   Type^ destinationType) override
	{
		//if (destinationType == Double::typeid&& 
		if( destinationType == System::String::typeid &&
			value->GetType() == cVector4::typeid)
		{
			cVector4^ so = (cVector4^)value;
			int	l_iExp = 0;
			try{
				so->m_fr = Double::Parse(so->m_fr).ToString();
				l_iExp++;
				so->m_fg = Double::Parse(so->m_fg).ToString();
				l_iExp++;
				so->m_fb = Double::Parse(so->m_fb).ToString();
				l_iExp++;
				so->m_fa = Double::Parse(so->m_fa).ToString();
			}
			catch(System::Exception^exp)
			{
				System::Windows::Forms::MessageBox::Show(exp->ToString(),"waring",System::Windows::Forms::MessageBoxButtons::OK);
				if( l_iExp == 0 )
					so->m_fr = "1.0";
				else
				if( l_iExp == 1 )
					so->m_fg = "1.0";
				else
				if( l_iExp == 2 )
					so->m_fb = "1.0";
				else
				if( l_iExp == 3 )
					so->m_fa = "1.0";				
			}
			return "R:"+ so->m_fr + ","
				   "G:"+ so->m_fg +","
				   "B:"+ so->m_fb+","
				   "A:"+ so->m_fa;
		}
		return ConvertTo(context, culture, value, destinationType);
	}
	virtual bool CanConvertFrom(ITypeDescriptorContext^ context,
						System::Type^ sourceType) override
	{
		//if (sourceType == Double::typeid)
		if (sourceType == System::String::typeid)
			return true;

		return CanConvertFrom(context, sourceType);
	}

	virtual Object^ ConvertFrom(ITypeDescriptorContext^ context,
								  CultureInfo^ culture, Object ^value) override
	{
		if (value->GetType()== String::typeid) {
			try 
			{
				String^ s = (String^) value;
				int colon = s->IndexOf(':');
				int comma = s->IndexOf(',');

				if (colon != -1 && comma != -1) 
				{
					String ^l_sR = s->Substring(colon + 1 ,
													(comma - colon - 1));

					colon = s->IndexOf(':', comma + 1);
					comma = s->IndexOf(',', comma + 1);

					String^ l_sG = s->Substring(colon + 1 , 
													(comma - colon -1));

					colon = s->IndexOf(':', comma + 1);
					comma = s->IndexOf(',', comma + 1);

					String^ l_sB = s->Substring(colon + 1 , 
													(comma - colon -1));

					colon = s->IndexOf(':', comma + 1);

					String^ l_sA = s->Substring(colon + 1);

					cVector4^ so = gcnew cVector4();

					so->m_fr = Double::Parse(l_sR).ToString();
					so->m_fg = Double::Parse(l_sG).ToString();
					so->m_fb = Double::Parse(l_sB).ToString();
					so->m_fa = Double::Parse(l_sA).ToString();
	                    
					return so;
				}
				else
				{
					//cVector3^ so = gcnew cVector3();
					//return so;
					throw gcnew ArgumentException(
						"Can not convert '" + (String^)value + 
						"' to type SpellingOptions");
				}
			}
			catch(System::Exception^exp) 
			{
				throw gcnew ArgumentException(
					"Can not convert '" + (String^)value + 
					"' to type SpellingOptions"+exp->ToString());
			}
		}  
		return ConvertFrom(context, culture, value);
	}
};