#ifndef _DOT_NET_DEFINE_H_
#define _DOT_NET_DEFINE_H_
//for default get and set method
#define GET_SET(Type,VariableName,FuncName) property Type FuncName{Type get(){ return VariableName; }System::Void set(Type value){ VariableName = value; }}
//for default get and set method with category attribute name
#define GET_SET_CATE(Type,VariableName,FuncName,CateAttName) [CategoryAttribute(CateAttName)]GET_SET(Type,VariableName,FuncName)

//for declar variable and default get set method
#define PUBLIC_OBJECT( Type,VariableName,FuncName) public:Type VariableName;public:GET_SET(Type,VariableName,FuncName);
#define PUBLIC_OBJECT_DESCRIPTION( Type,VariableName,FuncName,Description) public:Type VariableName;public:[DescriptionAttribute(Description)]GET_SET(Type,VariableName,FuncName);
//for declar variable and default get set method with category attributename
#define PUBLIC_OBJECT_CATE( Type,VariableName,FuncName,CateAttName) public:Type Variable;public:GET_SET_CATE(Type,VariableName,FuncName,CateAttName);

//declare variable and convert to specification type
#define PUBLIC_OBJECT_AND_CONVERT(Type,VariableName,FuncName,ConvertType,Description)public:Type	VariableName;	\
	public:[TypeConverterAttribute(ConvertType::typeid),	\
	DescriptionAttribute(Description)]	\
	GET_SET(Type,VariableName,FuncName)


//declare variable and convert to specification type
#define PUBLIC_OBJECT_CATE_AND_CONVERT(Type,VariableName,FuncName,CateAttName,ConvertType,Description)public:Type	VariableName;	\
	public:[TypeConverterAttribute(ConvertType::typeid),	\
	DescriptionAttribute(Description)]	\
	GET_SET_CATE(Type,VariableName,FuncName,CateAttName)

#ifndef DegreeToRadian
#define DegreeToRadian(degree) ((degree) * (System::Math::PI / 180.0f))
#endif
#ifndef RadianToDegree
#define RadianToDegree( radian ) ((radian) * (180.0f / System::Math::PI))
#endif
//end _DOT_NET_DEFINE_H_
#endif