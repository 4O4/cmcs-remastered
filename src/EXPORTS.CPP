#include "precomp.h"
#include "misc.h"
#include "hkeyctrl.h"
#include "cedit.h"

HINSTANCE hInstance;

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /* lpReserved */)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		::hInstance = hInstance;
		RegisterLineToolTip();
		InitCommonControls();
		RegisterCMHotKey( hInstance );
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		UnregisterLineToolTip();
		UnregisterCMHotKey( hInstance );
	}
	return 1;   // ok
}

extern "C" CME_CODE CMRegisterControl( DWORD dwVersion )
{
	if ( dwVersion == CM_VERSION || dwVersion == CM_COMPATIBLE_VERSION )
	{
		CEdit::g_dwVersion = dwVersion;
		return ( CEdit::Register() ? CME_SUCCESS : CME_FAILURE );
	}
	else
	{
		return CME_BADARGUMENT;
	}
}

extern "C" CME_CODE CMUnregisterControl()
{
	CEdit::Unregister();
	return CME_SUCCESS;
}

extern "C" void CMSetFindReplaceMRUList( LPCTSTR pszMRUList, BOOL bFind )
{
	if ( bFind )
	{
		if ( CEdit::g_FindReplaceData.m_pszFindMRUList )
		{
			free( CEdit::g_FindReplaceData.m_pszFindMRUList );
			CEdit::g_FindReplaceData.m_pszFindMRUList = NULL;
		}
	}
	else
	{
		if ( CEdit::g_FindReplaceData.m_pszReplaceMRUList )
		{
			free( CEdit::g_FindReplaceData.m_pszReplaceMRUList );
			CEdit::g_FindReplaceData.m_pszReplaceMRUList = NULL;
		}
	}

	if ( pszMRUList )
	{
		int nLen = _tcslen( pszMRUList );
		int cb = ( nLen + 1 ) * sizeof( TCHAR );
		cb = min( CM_FIND_REPLACE_MRU_BUFF_SIZE, cb );
		LPTSTR pszNewList = ( LPTSTR ) malloc( cb + 1 );
		if ( bFind )
		{
			CEdit::g_FindReplaceData.m_pszFindMRUList = pszNewList;
		}
		else
		{
			CEdit::g_FindReplaceData.m_pszReplaceMRUList = pszNewList;
		}
		_tcsncpy( pszNewList, pszMRUList, nLen );
		pszNewList[ nLen ] = _T('\0');
	}
}

extern "C" void CMGetFindReplaceMRUList( LPTSTR pszMRUList, BOOL bFind )
{
	LPCTSTR psz = bFind ? CEdit::g_FindReplaceData.m_pszFindMRUList : 
	                      CEdit::g_FindReplaceData.m_pszReplaceMRUList;
	if ( psz )
	{	_tcscpy( pszMRUList, psz );
	}
	else
	{
		*pszMRUList = _T('\0');
	}
}

extern "C" CME_CODE CMRegisterLanguage( LPCTSTR pszName, CM_LANGUAGE *pLang )
{
	return CEdit::RegisterLanguage( pszName, pLang );
}

extern "C" CME_CODE CMUnregisterLanguage( LPCTSTR pszName )
{
	return ( CEdit::UnregisterLanguage( pszName ) ? CME_SUCCESS : CME_FAILURE );
}

extern "C" CME_CODE CMUnregisterAllLanguages()
{
	return CEdit::UnregisterAllLanguages();
}

extern "C" int CMGetLanguageDef( LPCTSTR pszName, CM_LANGUAGE *pLang )
{
	return CEdit::GetLanguageDef( pszName, pLang );
}

extern "C" int CMGetHotKeys( LPBYTE pBuff )
{
	return CEdit::GetHotKeys( pBuff );
}

extern "C" CME_CODE CMSetHotKeys( const LPBYTE pBuff )
{
	BOOL bResult = CEdit::SetHotKeys( pBuff );
	return ( bResult ? CME_SUCCESS : CME_FAILURE );
}

extern "C" void CMResetDefaultHotKeys()
{
	CEdit::SetDefaultHotKeys();
}

extern "C" int CMGetHotKeysForCmd( WORD wCmd, CM_HOTKEY *pHotKeys )
{
	return CEdit::FindHotKeysForCommand( wCmd, pHotKeys );
}

extern "C" CME_CODE CMRegisterHotKey( CM_HOTKEY *pHotKey, WORD wCmd )
{
	if ( pHotKey )
	{
		CEdit::RegisterHotKey( *pHotKey, wCmd );
		return CME_SUCCESS;
	}
	else
	{
		return CME_BADARGUMENT;
	}
}

extern "C" CME_CODE CMUnregisterHotKey( CM_HOTKEY *pHotKey )
{
	if ( pHotKey )
	{
		return ( CEdit::UnregisterHotKey( *pHotKey ) ? CME_SUCCESS : CME_FAILURE );
	}
	else
	{
		return CME_BADARGUMENT;
	}
}

extern "C" CME_CODE CMRegisterCommand( WORD wCmd, LPCTSTR pszName, LPCTSTR pszDesc )
{
	if ( ( wCmd >= CMD_USER_BASE ) && pszName && pszDesc )
	{
		return ( CEdit::RegisterCommand( wCmd, pszName, pszDesc ) ? CME_SUCCESS : CME_FAILURE );
	}
	else
	{
		return CME_BADARGUMENT;
	}
}

extern "C" CME_CODE CMUnregisterCommand( WORD wCmd )
{
	if ( wCmd >= CMD_USER_BASE )
	{
		return ( CEdit::UnregisterCommand( wCmd ) ? CME_SUCCESS : CME_FAILURE );
	}
	else
	{
		return CME_BADARGUMENT;
	}
}

extern "C" void CMGetCommandString( WORD wCmd, BOOL bDescription, LPTSTR pszBuff, int nBuffLen )
{
	CEdit::GetCommandString( wCmd, bDescription, pszBuff, nBuffLen );
}

extern "C" int CMGetMacro( int nMacro, LPBYTE pMacroBuff )
{
	return CEdit::GetMacro( nMacro, pMacroBuff );
}

extern "C" int CMSetMacro( int nMacro, const LPBYTE pMacroBuff )
{
	return CEdit::SetMacro( nMacro, pMacroBuff );
}