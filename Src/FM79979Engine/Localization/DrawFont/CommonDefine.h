#pragma once
//extern  bool g_bThreadSafe;
//extern CRITICAL_SECTION g_cs;


//WCHAR to CHAR or invert
#define	INPUT_CHAR_RETURN_WCHAR( p,q )	 MultiByteToWideChar( CP_ACP, 0, p, -1, q, 512 );//q for return p for convert
#define	INPUT_WCHAR_RETURN_CHAR( p,q )  WideCharToMultiByte( CP_ACP, 0, p, -1, q, 256, NULL, NULL );//q for return p for convert
//be have a hr with declared before use
#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif

#define XMMATRIX_STRIP_POS(x) x.r[3].x = 0.0f; x.r[3].y = 0.0f; x.r[3].z = 0.0f;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif


//XMMATRIX to D3DMATRIX
#define MATRIX_360_TO_PC(mat360,matPc){		\
for(int i=0;i<4;i++)	\
	for(int j=0;j<4;j++)	\
	matPc.m[i][j] = mat360.m[i][j];}
//D3DMATRIX to XMMATRIX
#define MATRIX_PC_TO_360(matPc,mat360){		\
for(int i=0;i<4;i++)	\
	for(int j=0;j<4;j++)	\
	mat360.m[i][j] = matPc.m[i][j];}

#ifndef _DEBUG
	#define SHADE_FLAG D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT|D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT
#else
	#define SHADE_FLAG D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT|D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT
#endif
////--------------------------------------------------------------------------------------
//// Automatically enters & leaves the CS upon object creation/deletion
////--------------------------------------------------------------------------------------
//class DXUTLock
//{
//public:
//    inline DXUTLock()  { if( g_bThreadSafe ) EnterCriticalSection( &g_cs ); }
//    inline ~DXUTLock() { if( g_bThreadSafe ) LeaveCriticalSection( &g_cs ); }
//};
//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables with thread safety
//--------------------------------------------------------------------------------------
#define SET_ACCESSOR( x, y )       inline void Set##y( x t )  { DXUTLock l; m_##y = t; };
#define GET_ACCESSOR( x, y )       inline x Get##y() { DXUTLock l; return m_##y; };
#define GET_SET_ACCESSOR( x, y )   SET_ACCESSOR( x, y ) GET_ACCESSOR( x, y )

#define SETP_ACCESSOR( x, y )      inline void Set##y( x* t )  { DXUTLock l; m_p##y = *t; };
#define GETP_ACCESSOR( x, y )      inline x* Get##y() { DXUTLock l; return &m_p##y; };
#define GETP_SETP_ACCESSOR( x, y ) SETP_ACCESSOR( x, y ) GETP_ACCESSOR( x, y )


#define DELETE_LIST(List,Type)	{for each ( Type* l_p in List)	SAFE_DELETE(l_p);	List.clear();}
#define	RELEASE_LIST(List,Type)	{for each ( Type* l_p in List)	SAFE_RELEASE(l_p);	List.clear();}


//here the T must not pointer ,necause the Object is a pointer ,
//or u need to be care use and control
template <class T> 
class ObjectAndName
{
public:
	T	*m_pObject;
	char	m_sName[MAX_PATH];
	ObjectAndName(char*e_pString,T* e_p){ sprintf_s(m_sName,MAX_PATH,"%s\0",e_pString); m_pObject = e_p; };
	~ObjectAndName(){};
	char*	GetName(){ return m_sName; }
	T*		GetObject(){ return m_pObject; }
	void	SetObject(T*e_p){ m_pObject = e_p; }
};

/*std::vector<ObjectAndName<int*>>	g_ptest;

template <class T>
ObjectAndName<T>	GetObjectFromVector(char*e_pString,std::vector<T*> *e_pVector)
{
	for each(T* l_p in e_pVector)
	{
		//l_p->
	}
}*/