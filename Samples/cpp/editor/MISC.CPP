///////////////////////////////////////////////////////////////////
//
//  CodeMax sample app : CodeMax Editor
//
//  (c) WinMain Software 1998-2000
//
//
#include "precomp.h"
#include "resource.h"
#include "editapp.h"
#include "codefile.h"

#define HANDLE_MESSAGE( msg, fn ) case msg: return pClass->fn( wParam, lParam );
#define HANDLE_MENU_COMMAND( cmd, fn ) case cmd: if ( pClass->CommandIsValid( cmd ) ) pClass->fn(); return 0;
#define HANDLE_NOTIFY( cmd, fn ) case cmd: return pClass->fn();
#define HANDLE_NOTIFY_PARAM( cmd, fn, type ) case cmd: return pClass->fn( (##type *) lParam );

// WNDPROC for the main app window
long PASCAL CEditorApp::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CEditorApp *pClass = ( CEditorApp * ) GetWindowLong( hWnd, GWL_USERDATA );
	switch ( uMsg )
	{
		HANDLE_MESSAGE( WM_DESTROY, OnDestroy );
		HANDLE_MESSAGE( WM_SIZE, OnSize );
		HANDLE_MESSAGE( WM_INITMENUPOPUP, OnInitMenuPopup );
		HANDLE_MESSAGE( WM_CLOSE, OnClose );
		HANDLE_MESSAGE( WM_NOTIFY, OnNotify );
		HANDLE_MESSAGE( WM_SYSCOLORCHANGE, OnSysColorChange );

		case WM_COMMAND:
		{
			int nID = LOWORD( wParam );
			if ( nID >= ID_MRU_LIST_START && nID < ID_MRU_LIST_START + MAX_MRU_SIZE )
			{
				pClass->OnMRUList( nID - ID_MRU_LIST_START );
			}
			else
			{
				switch ( nID )
				{
					HANDLE_MENU_COMMAND( ID_FILE_NEW, OnFileNew );
					HANDLE_MENU_COMMAND( ID_FILE_OPEN, OnFileOpen );
					HANDLE_MENU_COMMAND( ID_FILE_CLOSE, OnFileClose );
					HANDLE_MENU_COMMAND( ID_FILE_SAVE, OnFileSave );
					HANDLE_MENU_COMMAND( ID_FILE_SAVEAS, OnFileSaveAs );
					HANDLE_MENU_COMMAND( ID_FILE_SAVEALL, OnFileSaveAll );
					HANDLE_MENU_COMMAND( ID_FILE_EXIT, OnFileExit );
					HANDLE_MENU_COMMAND( ID_FILE_PRINT, OnFilePrint );
					HANDLE_MENU_COMMAND( ID_WINDOW_TILE, OnWindowTile );
					HANDLE_MENU_COMMAND( ID_WINDOW_CASCADE, OnWindowCascade );
					HANDLE_MENU_COMMAND( ID_WINDOW_ARRANGEICONS, OnWindowArrangeIcons );
					HANDLE_MENU_COMMAND( ID_WINDOW_CLOSEALL, OnWindowCloseAll );
					HANDLE_MENU_COMMAND( ID_EDIT_CUT, OnEditCut );
					HANDLE_MENU_COMMAND( ID_EDIT_COPY, OnEditCopy );
					HANDLE_MENU_COMMAND( ID_EDIT_PASTE, OnEditPaste );
					HANDLE_MENU_COMMAND( ID_EDIT_UNDO, OnEditUndo );
					HANDLE_MENU_COMMAND( ID_EDIT_REDO, OnEditRedo );
					HANDLE_MENU_COMMAND( ID_EDIT_PROPERTIES, OnEditProperties );
					HANDLE_MENU_COMMAND( ID_EDIT_FIND, OnEditFind );
					HANDLE_MENU_COMMAND( ID_EDIT_REPLACE, OnEditReplace );
					HANDLE_MENU_COMMAND( ID_EDIT_MACRO, OnEditMacro );
				}
			}
			break;
		}
	}
	
	if ( pClass && ::IsWindow( pClass->m_hWnd ) && ::IsWindow( pClass->m_hWndClient ) )
	{
		return DefFrameProc( pClass->m_hWnd, pClass->m_hWndClient, uMsg, wParam, lParam );
	}
	else
	{
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
}
// WNDPROC for code window frame
long PASCAL CCodeFile::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CCodeFile *pClass = ( CCodeFile * ) GetWindowLong( hWnd, GWL_USERDATA );
	switch ( uMsg )
	{
		case WM_CREATE:
		{
			return OnCreate( hWnd, wParam, lParam );
		}

		HANDLE_MESSAGE( WM_NCDESTROY, OnNCDestroy );
		HANDLE_MESSAGE( WM_SIZE, OnSize );
		HANDLE_MESSAGE( WM_SETFOCUS, OnSetFocus );
		HANDLE_MESSAGE( WM_MDIACTIVATE, OnMDIActivate );
		HANDLE_MESSAGE( WM_CLOSE, OnClose );
		HANDLE_MESSAGE( WM_ERASEBKGND, OnEraseBkgnd );

		case WM_NOTIFY:
		{
			switch ( ( ( LPNMHDR ) lParam )->code )
			{
				HANDLE_NOTIFY( CMN_MODIFIEDCHANGE, OnModifiedChanged );
				HANDLE_NOTIFY( CMN_PROPSCHANGE, OnPropsChanged );
				HANDLE_NOTIFY( CMN_SELCHANGE, OnSelChanged );
				HANDLE_NOTIFY( CMN_OVERTYPECHANGE, OnOvertypeChange );
				HANDLE_NOTIFY_PARAM( CMN_DRAWLINE, OnDrawLine, CM_DRAWLINEDATA );
				HANDLE_NOTIFY_PARAM( CMN_DELETELINE, OnDeleteLine, CM_DELETELINEDATA );
				HANDLE_NOTIFY_PARAM( CMN_CMDFAILURE, OnCmdFailure, CM_CMDFAILUREDATA );
				HANDLE_NOTIFY_PARAM( CMN_REGISTEREDCMD, OnRegisteredCmd, CM_REGISTEREDCMDDATA );
				HANDLE_NOTIFY_PARAM( CMN_KEYDOWN, OnKeyDown, CM_KEYDATA );
				HANDLE_NOTIFY_PARAM( CMN_KEYUP, OnKeyUp, CM_KEYDATA );
				HANDLE_NOTIFY_PARAM( CMN_KEYPRESS, OnKeyPress, CM_KEYDATA );
				HANDLE_NOTIFY_PARAM( CMN_MOUSEDOWN, OnMouseDown, CM_MOUSEDATA );
				HANDLE_NOTIFY_PARAM( CMN_MOUSEUP, OnMouseUp, CM_MOUSEDATA );
				HANDLE_NOTIFY_PARAM( CMN_MOUSEMOVE, OnMouseMove, CM_MOUSEDATA );
				HANDLE_NOTIFY_PARAM( CMN_CODELIST, OnCodeList, CM_CODELISTDATA );
				HANDLE_NOTIFY_PARAM( CMN_CODELISTSELMADE, OnCodeListSelMade, CM_CODELISTDATA );
				HANDLE_NOTIFY_PARAM( CMN_CODELISTCANCEL, OnCodeListCancel, CM_CODELISTDATA );
				HANDLE_NOTIFY_PARAM( CMN_CODETIP, OnCodeTip, CM_CODETIPDATA );
				HANDLE_NOTIFY_PARAM( CMN_CODETIPUPDATE, OnCodeTipUpdate, CM_CODETIPDATA );
			}

			break;
		}

		default:
		{
			break;
		}
	}
	return DefMDIChildProc( hWnd, uMsg, wParam, lParam );
}

#ifdef _DEBUG
BOOL _ASSERT( BOOL bCond, LPCSTR pszFile, int nLine )
{
	if ( !bCond )
	{
		char szMsg[ 200 ];
		wsprintfA( szMsg, "Assertion Failure: %s, Line %d", pszFile, nLine );

		int nResult = MessageBoxA( NULL, szMsg, "CodeMax Editor", MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION );
		if ( nResult == IDABORT )
		{
			exit( 0 );
		}
		else if ( nResult == IDRETRY )
		{
			return FALSE;
		}
	}

	return TRUE;
}
#endif

// returns key for HKEY_CURRENT_USER\"Software"\RegistryKey\ProfileName
// creating it if it doesn't exist
// responsibility of the caller to call RegCloseKey() on the returned HKEY
HKEY GetAppRegistryKey( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName  )
{
	ASSERT( pszRegistryKey != NULL  );
	ASSERT( pszProfileName != NULL  );

	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	if ( RegOpenKeyEx( HKEY_CURRENT_USER, _T( "Software" ), 0, KEY_WRITE|KEY_READ,
		&hSoftKey ) == ERROR_SUCCESS )
	{
		DWORD dw;
		if ( RegCreateKeyEx( hSoftKey, pszRegistryKey, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw ) == ERROR_SUCCESS )
		{
			RegCreateKeyEx( hCompanyKey, pszProfileName, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw );
		}
	}
	if ( hSoftKey != NULL )
		RegCloseKey( hSoftKey );
	if ( hCompanyKey != NULL )
		RegCloseKey( hCompanyKey );

	return hAppKey;
}

// returns key for:
//      HKEY_CURRENT_USER\"Software"\RegistryKey\AppName\pszSection
// creating it if it doesn't exist.
// responsibility of the caller to call RegCloseKey() on the returned HKEY
HKEY GetSectionKey( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection  )
{
	ASSERT( pszSection != NULL );

	HKEY hSectionKey = NULL;
	HKEY hAppKey = GetAppRegistryKey( pszRegistryKey, pszProfileName  );
	if ( hAppKey == NULL )
		return NULL;

	DWORD dw;
	RegCreateKeyEx( hAppKey, pszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw );
	RegCloseKey( hAppKey );
	return hSectionKey;
}

UINT RegGetProfileInt( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, int nDefault )
{
	ASSERT( pszSection != NULL );
	ASSERT( pszEntry != NULL );
	HKEY hSecKey = GetSectionKey( pszRegistryKey, pszProfileName, pszSection );
	if ( hSecKey == NULL )
	{
		return nDefault;
	}
	else
	{
		DWORD dwValue;
		DWORD dwType;
		DWORD dwCount = sizeof( DWORD );
		LONG lResult = RegQueryValueEx( hSecKey, ( LPTSTR )pszEntry, NULL, &dwType,
		( LPBYTE )&dwValue, &dwCount );
		RegCloseKey( hSecKey );
		if ( lResult == ERROR_SUCCESS )
		{
			ASSERT( dwType == REG_DWORD );
			ASSERT( dwCount == sizeof( dwValue ) );
			return ( UINT )dwValue;
		}
	}
	return nDefault;
}

// buffer pointed to by pszText must be 1024 bytes long.
LPCTSTR RegGetProfileString( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, LPTSTR pszValue, LPCTSTR pszDefault )
{
	ASSERT( pszSection != NULL );
	ASSERT( pszEntry != NULL );
	HKEY hSecKey = GetSectionKey( pszRegistryKey, pszProfileName, pszSection );
	if ( hSecKey == NULL )
		return pszDefault;
	DWORD dwType, dwCount;
	LONG lResult = RegQueryValueEx( hSecKey, ( LPTSTR )pszEntry, NULL, &dwType,
		NULL, &dwCount );
	if ( lResult == ERROR_SUCCESS )
	{
		ASSERT( dwType == REG_SZ );
		LPTSTR pszOut = new TCHAR[ dwCount + 1 ];
		lResult = RegQueryValueEx( hSecKey, ( LPTSTR )pszEntry, NULL, &dwType,
			( LPBYTE )pszOut, &dwCount );
		_tcsncpy( pszValue, pszOut, min( 1024, dwCount ) );
		pszValue[ 1024 ] = _T( '\0' );
		delete pszOut;
	}
	RegCloseKey( hSecKey );
	if ( lResult == ERROR_SUCCESS )
	{
		ASSERT( dwType == REG_SZ );
		return pszValue;
	}
	return pszDefault;
}

BOOL RegGetProfileBinary( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry,	BYTE** ppData, UINT* pBytes )
{
	ASSERT( pszSection != NULL );
	ASSERT( pszEntry != NULL );
	ASSERT( ppData != NULL );
	ASSERT( pBytes != NULL );
	*ppData = NULL;
	*pBytes = 0;
	HKEY hSecKey = GetSectionKey( pszRegistryKey, pszProfileName, pszSection );
	if ( hSecKey == NULL )
		return FALSE;

	DWORD dwType, dwCount;
	LONG lResult = RegQueryValueEx( hSecKey, ( LPTSTR )pszEntry, NULL, &dwType,
		NULL, &dwCount );
	*pBytes = dwCount;
	if ( lResult == ERROR_SUCCESS )
	{
		ASSERT( dwType == REG_BINARY );
		*ppData = new BYTE[*pBytes];
		lResult = RegQueryValueEx( hSecKey, ( LPTSTR )pszEntry, NULL, &dwType,
			*ppData, &dwCount );
	}
	RegCloseKey( hSecKey );
	if ( lResult == ERROR_SUCCESS )
	{
		ASSERT( dwType == REG_BINARY );
		return TRUE;
	}
	else
	{
		delete [] *ppData;
		*ppData = NULL;
	}
	return FALSE;
}

BOOL RegWriteProfileInt( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, int nValue )
{
	ASSERT( pszSection != NULL );
	ASSERT( pszEntry != NULL );
	HKEY hSecKey = GetSectionKey( pszRegistryKey, pszProfileName, pszSection );
	if ( hSecKey == NULL )
		return FALSE;
	LONG lResult = RegSetValueEx( hSecKey, pszEntry, NULL, REG_DWORD,
		( LPBYTE )&nValue, sizeof( nValue ) );
	RegCloseKey( hSecKey );
	return lResult == ERROR_SUCCESS;
}

BOOL RegWriteProfileString( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue )
{
	ASSERT( pszSection != NULL );
	LONG lResult;
	if ( pszEntry == NULL ) //delete whole section
	{
		HKEY hAppKey = GetAppRegistryKey( pszRegistryKey, pszProfileName );
		if ( hAppKey == NULL )
			return FALSE;
		lResult = RegDeleteKey( hAppKey, pszSection );
		RegCloseKey( hAppKey );
	}
	else if ( pszValue == NULL )
	{
		HKEY hSecKey = GetSectionKey( pszRegistryKey, pszProfileName, pszSection );
		if ( hSecKey == NULL )
			return FALSE;
		// necessary to cast away const below
		lResult = RegDeleteValue( hSecKey, ( LPTSTR )pszEntry );
		RegCloseKey( hSecKey );
	}
	else
	{
		HKEY hSecKey = GetSectionKey( pszRegistryKey, pszProfileName, pszSection );
		if ( hSecKey == NULL )
			return FALSE;
		lResult = RegSetValueEx( hSecKey, pszEntry, NULL, REG_SZ,
			( LPBYTE )pszValue, ( lstrlen( pszValue )+1 )*sizeof( TCHAR ) );
		RegCloseKey( hSecKey );
	}
	return lResult == ERROR_SUCCESS;
}

BOOL RegWriteProfileBinary( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, LPBYTE pData, UINT nBytes )
{
	ASSERT( pszSection != NULL );
	LONG lResult;
	HKEY hSecKey = GetSectionKey( pszRegistryKey, pszProfileName, pszSection );
	if ( hSecKey == NULL )
		return FALSE;
	lResult = RegSetValueEx( hSecKey, pszEntry, NULL, REG_BINARY,
		pData, nBytes );
	RegCloseKey( hSecKey );
	return lResult == ERROR_SUCCESS;
}
