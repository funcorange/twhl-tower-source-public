#include "shaderlib/cshader.h"
class sdk_windowimposter_ps20b_Static_Index
{
private:
	int m_nPARALLAXCORRECT;
#ifdef _DEBUG
	bool m_bPARALLAXCORRECT;
#endif
public:
	void SetPARALLAXCORRECT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPARALLAXCORRECT = i;
#ifdef _DEBUG
		m_bPARALLAXCORRECT = true;
#endif
	}
	void SetPARALLAXCORRECT( bool i )
	{
		m_nPARALLAXCORRECT = i ? 1 : 0;
#ifdef _DEBUG
		m_bPARALLAXCORRECT = true;
#endif
	}
public:
	sdk_windowimposter_ps20b_Static_Index( )
	{
#ifdef _DEBUG
		m_bPARALLAXCORRECT = false;
#endif // _DEBUG
		m_nPARALLAXCORRECT = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bPARALLAXCORRECT;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 2 * m_nPARALLAXCORRECT ) + 0;
	}
};
#define shaderStaticTest_sdk_windowimposter_ps20b psh_forgot_to_set_static_PARALLAXCORRECT + 0
class sdk_windowimposter_ps20b_Dynamic_Index
{
private:
	int m_nPIXELFOGTYPE;
#ifdef _DEBUG
	bool m_bPIXELFOGTYPE;
#endif
public:
	void SetPIXELFOGTYPE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPIXELFOGTYPE = i;
#ifdef _DEBUG
		m_bPIXELFOGTYPE = true;
#endif
	}
	void SetPIXELFOGTYPE( bool i )
	{
		m_nPIXELFOGTYPE = i ? 1 : 0;
#ifdef _DEBUG
		m_bPIXELFOGTYPE = true;
#endif
	}
public:
	sdk_windowimposter_ps20b_Dynamic_Index()
	{
#ifdef _DEBUG
		m_bPIXELFOGTYPE = false;
#endif // _DEBUG
		m_nPIXELFOGTYPE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bPIXELFOGTYPE;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nPIXELFOGTYPE ) + 0;
	}
};
#define shaderDynamicTest_sdk_windowimposter_ps20b psh_forgot_to_set_dynamic_PIXELFOGTYPE + 0
