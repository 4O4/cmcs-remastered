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

#define FUNC_NAMES				0
#define FUNC_PROTO				1
#define MAX_FUNC_NAME			100
#define MAX_FUNC_PROTO			1000
#define SECTION_FUNC_NAMES		_T("Function Names")
#define SECTION_FUNC_PROTO		_T("Function Prototypes")
#define SECTION_OVERLOADS		_T("Overloads")


#define IMAGE_BREAKPOINT	1	// the image index for the breakpoint image in the imagelist (IDB_MARGIN)

CCodeFile::CCodeFile( CEditorApp *pApp )
{
	m_pApp = pApp;
	m_hWnd = m_hWndEdit = NULL;
	m_bDoIdleProcessing = FALSE;
	*m_szPath = _T('\0');
	UpdateFileTime();

	// The functions.ini file should be in the same directory as the app.
	GetModuleFileName( NULL, m_szIniFile, MAX_PATH );

	LPTSTR pEnd = _tcsrchr( m_szIniFile, L'\\' );
	ASSERT( pEnd != NULL );

	lstrcpy( ++pEnd, _T("functions.ini") );

#ifdef _DEBUG

	WIN32_FIND_DATA fd = {0};
	HANDLE hFind = FindFirstFile( m_szIniFile, &fd );

	// If this ASSERT fails, you need to make sure the functions.ini file is
	// in the same directory as the application executable.
	ASSERT( INVALID_HANDLE_VALUE != hFind );

	if( INVALID_HANDLE_VALUE != hFind )
		FindClose( hFind );

#endif

	// Initialize image list for CodeList control
	HICON hIcon = LoadIcon( m_pApp->GetHINSTANCE(), MAKEINTRESOURCE( IDI_CODELIST ) );
	ASSERT( hIcon != NULL );

	m_hImages = ImageList_Create( 16, 16, ILC_COLOR | ILC_MASK, 1, 1 );
	ASSERT( m_hImages != NULL );

	ImageList_AddIcon( m_hImages, hIcon );

	*m_szTipFunc = L'\0';
	m_hListCtrl = NULL;
}

CCodeFile::~CCodeFile()
{
	// Free image list
	if( m_hImages )
		ImageList_Destroy( m_hImages );
}

BOOL CCodeFile::RegisterClass( HINSTANCE hInstance )
{
	// register the CodeMax edit control
	CMRegisterControl();

	WNDCLASS    wndclass;

	wndclass.style         = 0;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = CODEFILEWNDCLASS;

	return ::RegisterClass( &wndclass );
}

BOOL CCodeFile::UnregisterClass( HINSTANCE hInstance )
{
	// unregister the CodeMax edit control
	CMUnregisterControl();

	return ::UnregisterClass( CODEFILEWNDCLASS, hInstance );
}

void CCodeFile::Initialize( HWND hWnd )
{
	m_hWnd = hWnd;
	m_hWndEdit = CreateWindow( CODEMAXWNDCLASS,
	                           _T(""),
							   WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
							   0,
							   0,
							   100,
							   100,
							   m_hWnd,
							   NULL,
							   m_pApp->GetHINSTANCE(),
							   NULL );
	ASSERT( IsWindow( m_hWndEdit ) );
	// default language to C++
	VERIFY( CM_SetLanguage( m_hWndEdit, CMLANG_CPP ) == CME_SUCCESS );

	// set the images to be shown in the left margin for the bookmark and the breakpoint
	VERIFY( CM_SetImageList( m_hWndEdit, m_pApp->GetImageList() ) == CME_SUCCESS );

	// Save the settings in the registry
	m_pApp->LoadProfile( m_hWndEdit );
}

// Open() will load a file into a code window
void CCodeFile::Open( LPCTSTR pszPath )
{
	SetPath( pszPath );
	if ( CM_OpenFile( m_hWndEdit, m_szPath ) != CME_SUCCESS )
	{
		DWORD dwLastError = GetLastError();
		if ( dwLastError != ERROR_FILE_NOT_FOUND )
		{
			TCHAR szMsg[ 1024 ];
			_tcscpy( szMsg, _T("Failed to open document:\n\n") );
			LPTSTR lpMessageBuffer;

			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	  					  NULL,
						  dwLastError,
	  					  MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
	  					  (LPTSTR) &lpMessageBuffer, 0, NULL );
			_tcscat( szMsg, lpMessageBuffer );
			LocalFree( lpMessageBuffer );	// FormatMessage() requires this
			MessageBox( m_pApp->GetHWND(), szMsg, APPNAME, MB_OK | MB_ICONEXCLAMATION );
		}
	}

	UpdateWindowTitle();
	// Tell CodeMax what the language is so it can do color syntax stuff
	// and auto indentation
	SetLanguageBasedOnFileType();
	UpdateFileTime();
}

// Save() will save the contents of the edit window.  This function should only be
// called after calling SetPath()
BOOL CCodeFile::Save()
{
	ASSERT( *m_szPath );
	BOOL bSuccess = CM_SaveFile( m_hWndEdit, m_szPath, FALSE ) == CME_SUCCESS;
	if ( !bSuccess )
	{
		// CodeMax could not save the file -- out of diskspace or readonly or something.
		TCHAR szMsg[ _MAX_PATH + 500 ];
		TCHAR szTitle[ _MAX_PATH + 1 ];
		GetTitle( szTitle );
		wsprintf( szMsg, _T("Failed to save %s:\n\n"), szTitle );
		DWORD dwLastError = GetLastError();
		LPTSTR lpMessageBuffer;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	  				  NULL,
					  dwLastError,
	  				  MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
	  				  (LPTSTR) &lpMessageBuffer, 0, NULL );
		_tcscat( szMsg, lpMessageBuffer );
		LocalFree( lpMessageBuffer );	// FormatMessage() requires this
		MessageBox( m_pApp->GetHWND(), szMsg, APPNAME, MB_OK | MB_ICONEXCLAMATION );
	}
	UpdateFileTime();
	return ( bSuccess );
}

// SetLanguageBasedOnFileType() will configure the CodeMax control to the language specific to
// this file
void CCodeFile::SetLanguageBasedOnFileType()
{
	// based on the file name extension, set the language
	LPCTSTR pszDot = _tcsrchr( m_szPath, '.' );
	if ( pszDot )
	{
		TCHAR szExt[ _MAX_PATH ];
		_tcscpy( szExt, pszDot + 1 );
		CharUpper( szExt );

		LPCTSTR pszLang = NULL;

		if ( !_tcscmp( szExt, _T("C") ) || !_tcscmp( szExt, _T("CPP") ) || 
			 !_tcscmp( szExt, _T("H") ) || !_tcscmp( szExt, _T("HPP") ) )
		{
			// C/C++
			pszLang = CMLANG_CPP;
		}
		else if ( !_tcscmp( szExt, _T("JAVA") ) )
		{
			// Java
			pszLang = CMLANG_JAVA;
		}
		else if ( !_tcscmp( szExt, _T("BAS") ) || !_tcscmp( szExt, _T("INC") ) )
		{
			// Basic
			pszLang = CMLANG_BASIC;
		}
		else if ( !_tcscmp( szExt, _T("PAS") ) )
		{
			// Pascal
			pszLang = CMLANG_PASCAL;
		}
		else if ( !_tcscmp( szExt, _T("SQL") ) || !_tcscmp( szExt, _T("DDL") ) )
		{
			// SQL
			pszLang = CMLANG_SQL;
		}
		else if ( !_tcscmp( szExt, _T("HTM") ) || !_tcscmp( szExt, _T("HTML") ) ||
		          !_tcscmp( szExt, _T("HTX") ) || !_tcscmp( szExt, _T("ASP") ) ||
		          !_tcscmp( szExt, _T("ALX") ) || !_tcscmp( szExt, _T("STM") ) ||
		          !_tcscmp( szExt, _T("SHTML") ) )
		{
			// HTML
			pszLang = CMLANG_HTML;
		}
		else if ( !_tcscmp( szExt, _T("XML") ) )
		{
			// XML
			pszLang = CMLANG_XML;
		}

		VERIFY( CM_SetLanguage( m_hWndEdit, pszLang ) == CME_SUCCESS );
	}
}

// GetTitle() calculates a window title suitable for display
LPCTSTR CCodeFile::GetTitle( LPTSTR pszBuff ) const
{
	ASSERT( pszBuff );
	if ( *m_szPath )
	{
		_tcscpy( pszBuff, m_szPath );
	}
	else
	{
		_tcscpy( pszBuff, _T("Untitled") );
	}
	return pszBuff;
}

// UpdateWindowTitle() updates the file caption based on the current state of the window contents.
// An asterisk (*) is appended if the window contents are modified
void CCodeFile::UpdateWindowTitle()
{
	TCHAR szTitle[ _MAX_PATH + 1 ];

	GetTitle( szTitle );
	if ( CM_IsModified( m_hWndEdit ) )
	{
		_tcscat( szTitle, _T("*") );
	}

	SetWindowText( m_hWnd, szTitle );
}

// SetPath() updates the file path of this code file
void CCodeFile::SetPath( LPCTSTR pszPath )
{
	_tcsncpy( m_szPath, pszPath, _MAX_PATH );
	UpdateWindowTitle();
	SetLanguageBasedOnFileType();
	UpdateFileTime();
	UpdateReadOnlyState();
	m_bDoIdleProcessing = FALSE;
	
	// get the root dir
	TCHAR szRootDir[ _MAX_PATH ];
	_tcscpy( szRootDir, m_szPath );
	if ( *szRootDir == _T('\\') && szRootDir[ 1 ] == _T('\\') )
	{
		// file is on a network resource
		// e.g. \\fileserver1\disk1\file.cpp
		// don't allow background processing -- it may be too slow
	}
	else
	{
		// file is on a disk with a drive letter (e.g. a:\file.cpp)
		LPTSTR psz = szRootDir;
		while ( *psz && *psz != _T('\\') )
		{
			psz++;
		}
		*psz = _T('\0');
		switch ( GetDriveType( szRootDir ) )
		{
			case DRIVE_UNKNOWN:
			case DRIVE_NO_ROOT_DIR:
			case DRIVE_REMOVABLE:
			case DRIVE_CDROM:
				// pointless, or slow
				m_bDoIdleProcessing = FALSE;
				break;
			case DRIVE_FIXED:
			case DRIVE_REMOTE:
			case DRIVE_RAMDISK:
				m_bDoIdleProcessing = TRUE;
				break;
		}
	}
}

// IsCodeUpToDate() will check to see the file on disk is newer than the window contents.
// If bStale is TRUE, then the code is not up to date if the file on disk is newer.
// If bStale is FALSE, then the code is not up to date if the file on disk is newer or older
BOOL CCodeFile::IsCodeUpToDate( BOOL bStale ) const
{
	BOOL bUpToDate = TRUE;
	if ( *m_szPath )
	{
		FILETIME time;
		GetFileTimeFromDisk( time );
		int nCompare = CompareFileTime( &time, &m_timeFileSaved );
		bUpToDate = bStale ? ( nCompare <= 0 ) : ( nCompare == 0 );
	}

	return bUpToDate;
}

// updates the internal file time
void CCodeFile::UpdateFileTime()
{
	FILETIME time;
	GetFileTimeFromDisk( time );
	m_timeFileSaved = time;
}

// simple function to get the FILETIME for the underlying file on disk.  If there is no
// file, then this function will set the time to 'zero'.
void CCodeFile::GetFileTimeFromDisk( FILETIME &time ) const
{
	time.dwLowDateTime = time.dwHighDateTime = 0;
	if ( *m_szPath )
	{
		HANDLE hFile = CreateFile( m_szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
		if ( hFile != INVALID_HANDLE_VALUE )
		{
			GetFileTime( hFile, NULL, NULL, &time );
			CloseHandle( hFile );
		}
	}
}
LRESULT CCodeFile::OnCreate( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	ASSERT( lParam );
	CREATESTRUCT *pcs = ( CREATESTRUCT * )lParam;
	ASSERT( pcs->lpCreateParams );
	MDICREATESTRUCT *pmcs = ( MDICREATESTRUCT * ) pcs->lpCreateParams;

	// associate the CCodeFile object with the window
	CCodeFile *pCode = ( CCodeFile * ) pmcs->lParam;
	SetWindowLong( hWnd, GWL_USERDATA, ( DWORD ) pCode );
	pCode->Initialize( hWnd );
	return DefMDIChildProc( hWnd, WM_CREATE, wParam, lParam );
}

LRESULT CCodeFile::OnNCDestroy( WPARAM wParam, LPARAM lParam )
{
	// clean up this memory
	LRESULT lRes = DefMDIChildProc( m_hWnd, WM_NCDESTROY, wParam, lParam );
	delete this;
	return lRes;
}


LRESULT CCodeFile::OnSize( WPARAM wParam, LPARAM lParam )
{
	int cx = LOWORD( lParam );
	int cy = HIWORD( lParam );

	// resize the CodeMax control
	ASSERT( IsWindow( m_hWndEdit ) );
	SetWindowPos( m_hWndEdit, NULL, 0, 0, cx, cy, SWP_NOZORDER );

	if ( ( wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED ) && IsWindowVisible( m_hWnd ) )
	{
		m_pApp->WriteProfileInt( CODE_SECTION, KEY_CODEMAXIMIZED, wParam == SIZE_MAXIMIZED );
	}

	return DefMDIChildProc( m_hWnd, WM_SIZE, wParam, lParam );
}

LRESULT CCodeFile::OnSetFocus( WPARAM wParam, LPARAM lParam )
{
	ASSERT( IsWindow( m_hWndEdit ) );
	SetFocus( m_hWndEdit );

	return DefMDIChildProc( m_hWnd, WM_SETFOCUS, wParam, lParam );
}

LRESULT CCodeFile::OnMDIActivate( WPARAM wParam, LPARAM lParam )
{
	// let the app know that this file is now the active file
	m_pApp->SetActiveCode( m_hWnd == ( HWND ) lParam ? this : NULL );
	// update the status bar text
	OnOvertypeChange();
	return DefMDIChildProc( m_hWnd, WM_MDIACTIVATE, wParam, lParam );
}

LRESULT CCodeFile::OnModifiedChanged()
{
	// Update the dirty flag (*) on the caption
	UpdateWindowTitle();
	return 0;
}

LRESULT CCodeFile::OnPropsChanged()
{
	// Save the settings in the registry
	m_pApp->SaveProfile( m_hWndEdit );
	return 0;
}

LRESULT CCodeFile::OnSelChanged()
{
	// CodeMax edit selection changed -- update the status bar
	CM_RANGE sel;
	CM_GetSel( m_hWndEdit, &sel, FALSE );
	m_pApp->OnSelChanged( sel.posEnd.nLine, sel.posEnd.nCol );
	return 0;
}

LRESULT CCodeFile::OnClose( WPARAM wParam, LPARAM lParam )
{
	BOOL bClose = TRUE;
	// if window contents are modified, prompt user to save
	if ( CM_IsModified( m_hWndEdit ) )
	{
		TCHAR szMsg[ _MAX_PATH + 100 ];
		TCHAR szTitle[ _MAX_PATH + 1 ];
		GetTitle( szTitle );
		wsprintf( szMsg, _T("Save changes to %s?"), szTitle );
		int nResult = MessageBox( m_pApp->GetHWND(), szMsg, APPNAME, MB_YESNOCANCEL | MB_ICONEXCLAMATION );
		if ( nResult == IDYES )
		{
			// yes -- save
			bClose = m_pApp->OnFileSave( this );
		}
		else if ( nResult == IDCANCEL )
		{
			// cancel the close operation
			bClose = FALSE;
		}
	}

	if ( lParam )
	{
		// tell the caller whether the window was closed.
		*( BOOL * ) lParam = bClose;
	}
	return bClose ? DefMDIChildProc( m_hWnd, WM_CLOSE, wParam, lParam ) : 0;
}

LRESULT CCodeFile::OnEraseBkgnd( WPARAM /* wParam */, LPARAM /* lParam */ )
{
	// don't bother erasing the bkgnd.  The edit control will do it.
	return 0;
}

// UpdateReadOnlyState() syncs the buffer's readonly state with the underlying file's 
// readonly state
void CCodeFile::UpdateReadOnlyState()
{
	if ( *m_szPath )
	{
		DWORD dwFlags = GetFileAttributes( m_szPath );
		if ( dwFlags != 0xffffffff )
		{
			BOOL bReadOnly = ( ( dwFlags & FILE_ATTRIBUTE_READONLY ) == FILE_ATTRIBUTE_READONLY );
			if ( bReadOnly != IsReadOnly() )
			{
				CM_SetReadOnly( m_hWndEdit, bReadOnly );
			}
		}
	}
}

LRESULT CCodeFile::OnDrawLine( CM_DRAWLINEDATA *pdld )
{
pdld;
/*
	if ( pdld->nLine % 4 == 3 )
		{

		HPEN hPenOld = ( HPEN ) SelectObject( pdld->hDC, GetStockObject( BLACK_PEN ) );
		HBRUSH hBrushOld = ( HBRUSH ) SelectObject( pdld->hDC, GetStockObject( NULL_BRUSH ) );
		Rectangle( pdld->hDC,
				   pdld->rcLine.left,
				   pdld->rcLine.top,
				   pdld->rcLine.right,
				   pdld->rcLine.bottom );
		SelectObject( pdld->hDC, hPenOld );
		SelectObject( pdld->hDC, hBrushOld );
		}
*/
	return 0;
}

LRESULT CCodeFile::OnDeleteLine( CM_DELETELINEDATA *pdld )
{
	// a line was deleted -- this is a good time to free any
	// allocated data that was stored with the line via CMM_SETITEMDATA
	pdld;
	return 0;
}

LRESULT CCodeFile::OnCmdFailure( CM_CMDFAILUREDATA *pcfd )
{
	// a command failed.  You can perform your own error handling (e.g. a
	// messagebox) and return 1 to avoid the default error handling (messagebeep).
	pcfd;
	return 0;
}

LRESULT CCodeFile::OnRegisteredCmd( CM_REGISTEREDCMDDATA *prcd )
{
	switch ( prcd->wCmd )
	{
		case CMD_STEPOVER:
		case CMD_STEPINTO:
		{
			// set the highlighted line to the current line
			CM_RANGE cmSel;
			VERIFY( CM_GetSel( m_hWndEdit, &cmSel, FALSE ) == CME_SUCCESS );

			// highlight the current line.
			CM_SetHighlightedLine( m_hWndEdit, cmSel.posEnd.nLine );

			// step into a line of code, or over a line of code.  
			TCHAR szCmd[ CM_MAX_CMD_STRING ];
			TCHAR szDesc[ CM_MAX_CMD_DESCRIPTION ];
			CMGetCommandString( prcd->wCmd, FALSE, szCmd, ARRAY_SIZE( szCmd ) );
			CMGetCommandString( prcd->wCmd, TRUE, szDesc, ARRAY_SIZE( szDesc ) );
			TCHAR szMsg[ 500 ];
			wsprintf( szMsg, _T("Cmd #%d, Name: \"%s\", Desc: \"%s\""), ( int ) prcd->wCmd, szCmd, szDesc );
			MessageBox( m_hWnd, szMsg, NULL, MB_OK );
			break;
		}
		case CMD_TOGGLEBREAKPOINT:
		{
			// show a breakpoint image in the left margin.
			CM_RANGE cmSel;
			VERIFY( CM_GetSel( m_hWndEdit, &cmSel, FALSE ) == CME_SUCCESS );
			BYTE byImages = CM_GetMarginImages( m_hWndEdit, cmSel.posEnd.nLine );
			// if image 1 (the breakpoint image) is set, turn it off.
			// else turn it on.  Image 1 is represented by bit 1 (2^1 = 2)
			byImages ^= ( 1 << IMAGE_BREAKPOINT );
			CM_SetMarginImages( m_hWndEdit, cmSel.posEnd.nLine, byImages );
			break;
		}
		default:
		{
			ASSERT( FALSE );
			break;
		}
	}
	prcd;
	return 0;
}

LRESULT CCodeFile::OnOvertypeChange()
{
	// The insert key was pressed.  Change the indicator on the status bar
	m_pApp->UpdateOvertypeStatus( CM_IsOvertypeEnabled( m_hWndEdit ) );
	return 0;
}

LRESULT CCodeFile::OnKeyDown( CM_KEYDATA *pkd )
{
	// a key was pressed down.  To prevent CodeMax from processing it, return 1.
	pkd;
	return 0;
}

LRESULT CCodeFile::OnKeyUp( CM_KEYDATA *pkd )
{
	// a key was released.  To prevent CodeMax from processing it, return 1.
	pkd;
	return 0;
}

LRESULT CCodeFile::OnKeyPress( CM_KEYDATA *pkd )
{
	// Invoke the CodeTip feature when the user presses the '(' key. We'll
	// determine whether or not we are actually ready to handle the message
	// in the OnCodeTip() handler.
	//
	if( L'(' == pkd->nKeyCode )
		CM_ExecuteCmd( m_hWndEdit, CMD_CODETIP, 0 );

	// If the CodeList control is active, update selected item based on
	// current word in CodeMax control window.
	SelectClosestItemInCodeList();

	return 0;
}

LRESULT CCodeFile::OnMouseDown( CM_MOUSEDATA *pmd )
{
	// a mouse button was pressed.  To prevent CodeMax from processing it, return 1.
	pmd;
	return 0;
}

LRESULT CCodeFile::OnMouseUp( CM_MOUSEDATA *pmd )
{
	// a mouse button was released.  To prevent CodeMax from processing it, return 1.
	pmd;
	return 0;
}

LRESULT CCodeFile::OnMouseMove( CM_MOUSEDATA *pmd )
{
	// a mouse button was pressed and released.  To prevent CodeMax from processing it, return 1.
	pmd;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CodeList / CodeTip support

// OnCodeList ---------------------------------------------------------------
LRESULT CCodeFile::OnCodeList( LPCM_CODELISTDATA pcld )
{
	// Save the handle to the CodeList control so that we can use it to
	// update the selection as the user types.
	m_hListCtrl = pcld->hListCtrl;

	// Attach our imagelist to the CodeList control.  Note that the list
	// control has the LVS_SHAREIMAGELISTS style set by default, so we don't
	// have to worry about detaching the imagelist later on.
	//
	ListView_SetImageList( m_hListCtrl, m_hImages, LVSIL_SMALL );

	// Add the functions to the list
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT;

	// Read the keywords from INI file
	int iIndex = 0;
	TCHAR szFunc[MAX_FUNC_NAME] = {0};

	while( GetString( FUNC_NAMES, iIndex++, szFunc ) )
	{
		lvi.pszText = szFunc;
		ListView_InsertItem( m_hListCtrl, &lvi );
	}

	// Try to select the item that matches any function name entered in the
	// CodeMax control window.
	SelectClosestItemInCodeList();

	// Allow the list view control to be displayed
	return TRUE;
}

// OnCodeListSelMade --------------------------------------------------------
LRESULT CCodeFile::OnCodeListSelMade( LPCM_CODELISTDATA )
{
	// Get the item selected in the list
	CM_RANGE range = {0};
	TCHAR szItem[MAX_FUNC_NAME] = {0};
	int iPos = ListView_GetNextItem( m_hListCtrl, -1, LVIS_SELECTED );

	ListView_GetItemText( m_hListCtrl, iPos, 0, szItem,
		sizeof( szItem ) / sizeof( TCHAR ) );

	// Select the current function in the CodeMax control, if any
	SelectCurrentFunction();

	// Replace current selection
	VERIFY( CME_SUCCESS == CM_ReplaceSel( m_hWndEdit, szItem ) );

	// Get new selection
	VERIFY( CME_SUCCESS == CM_GetSel( m_hWndEdit, &range, TRUE ) );
	
	// Update range to end of newly inserted text
	range.posStart.nCol += lstrlen( szItem );
	range.posEnd = range.posStart;
	
	// Move cursor
	VERIFY( CME_SUCCESS == CM_SetSel( m_hWndEdit, &range, TRUE ) );

	// Detach from list view control
	m_hListCtrl = NULL;

	// Don't prevent the list from being destroyed
	return FALSE;
}

// OnCodeListCancel ---------------------------------------------------------
LRESULT CCodeFile::OnCodeListCancel( LPCM_CODELISTDATA )
{
	// Detach from list view control
	m_hListCtrl = NULL;

	// Don't prevent the list from being destroyed
	return FALSE;
}

// OnCodeTip ----------------------------------------------------------------
LRESULT CCodeFile::OnCodeTip( LPCM_CODETIPDATA )
{
	// We don't want to display a tip inside quoted or commented-out lines...
	DWORD dwToken = CM_GetCurrentToken( m_hWndEdit );

	if( CM_TOKENTYPE_TEXT == dwToken || CM_TOKENTYPE_KEYWORD == dwToken )
	{
		// See if there is a valid function on the current line
		if( GetCurrentFunction( NULL ) )
		{
			// We want to use the tooltip control that automatically
			// highlights the arguments in the function prototypes for us.
			// This type also provides support for overloaded function
			// prototypes.
			//
			return CM_TIPSTYLE_MULTIFUNC;
		}
	}

	// Don't display a tooltip
	return CM_TIPSTYLE_NONE;
}

// OnCodeTipUpdate ----------------------------------------------------------
LRESULT CCodeFile::OnCodeTipUpdate( LPCM_CODETIPDATA pctd )
{
	ASSERT( CM_TIPSTYLE_MULTIFUNC == pctd->nTipType );

	HWND hToolTip = pctd->hToolTip;
	ASSERT( NULL != hToolTip );

	LPCM_CODETIPMULTIFUNCDATA pmfData = (LPCM_CODETIPMULTIFUNCDATA)pctd;

	// See if the tooltip control has been initialized yet
	if( 0 == GetWindowTextLength( hToolTip ) )
	{
		// Save name of current function
		VERIFY( GetCurrentFunction( m_szTipFunc ) );

		// Default to first function prototype
		pmfData->nCurrFunc = 0;

		// Get first prototype for function
		TCHAR szProto[MAX_FUNC_PROTO] = {0};
		VERIFY( GetPrototype( m_szTipFunc, szProto ) );

		// Set tooltip text
		SetWindowText( hToolTip, szProto );

		// Default to first argument
		pmfData->ctfData.nArgument = 0;

		// Get number of overloads, if any
		pmfData->nFuncCount = GetPrivateProfileInt( SECTION_OVERLOADS,
			m_szTipFunc, 0, m_szIniFile );
	}
	else
	{
		// Destroy the tooltip window if the caret is moved above or before
		// the starting point.
		//
		CM_RANGE range = {0};

		VERIFY( CME_SUCCESS == CM_GetSel( m_hWndEdit, &range, TRUE ) );

		if( range.posEnd.nLine < m_posSel.nLine ||
			( range.posEnd.nCol < m_posSel.nCol &&
			range.posEnd.nLine <= m_posSel.nLine ) )
		{
			// Caret moved too far up / back
			DestroyWindow( hToolTip );
		}
		else
		{
			// Determine which argument to highlight
			pmfData->ctfData.nArgument = GetCurrentArgument();

			if( -1 == pmfData->ctfData.nArgument )
				DestroyWindow( hToolTip );// Right-paren found
			else if( pmfData->nFuncCount )
			{
				// Function is overloaded, so get current prototype
				TCHAR szProto[MAX_FUNC_PROTO] = {0};

				VERIFY( GetPrototype( m_szTipFunc, szProto,
					pmfData->nCurrFunc ) );

				// Set tooltip text
				SetWindowText( hToolTip, szProto );
			}
		}
	}

	// Apply changes to pmfData members
	return TRUE;
}

// GetString ----------------------------------------------------------------
BOOL CCodeFile::GetString( int iSection, int iIndex, LPTSTR pszFunc )
{
	TCHAR key[20] = {0};
	wsprintf( key, _T("Func%d"), iIndex );

	*pszFunc = L'\0';

	int iSize = MAX_FUNC_NAME;
	LPTSTR lpszSection = SECTION_FUNC_NAMES;

	if( FUNC_PROTO == iSection )
	{
		iSize = MAX_FUNC_PROTO;
		lpszSection = SECTION_FUNC_PROTO;
	}

	GetPrivateProfileString( lpszSection, key, _T(""), pszFunc, iSize,
		m_szIniFile );

	return ( L'\0' != *pszFunc );
}

// GetCurrentFunction -------------------------------------------------------
BOOL CCodeFile::GetCurrentFunction( LPTSTR pszName, bool bMustExist )
{
	CM_RANGE range = {0};
	TCHAR szLine[1000] = {0};
	TCHAR szFunc[MAX_FUNC_NAME] = {0};

	// Note:  We can't use CM_GetWord() here, because the user could have
	// typed a function name followed by a space, and *then* the left
	// parenthesis, i.e. "MessageBox ("

	// Get the current line
	CM_GetSel( m_hWndEdit, &range, TRUE );
	CM_GetLine( m_hWndEdit, range.posEnd.nLine, szLine );

	// There's nothing for us to do if the line is empty
	if( L'\0' == *szLine )
		return FALSE;

	int nEnd = range.posEnd.nCol - 1;

	// Trim off trailing '(', if found
	if( nEnd > 0 && nEnd < lstrlen( szLine ) )
	{
		if( L'(' == szLine[ nEnd ] )
			nEnd --;

		// Trim off trailing whitespace
		while( nEnd > 0 && L' ' == szLine[nEnd] )
			nEnd --;
	}

	if( nEnd < 0 )
		return FALSE;

	// Save the starting position for use with the CodeTip.  This is so we
	// can destroy the tip window if the user moves the cursor back before or
	// above the starting point.
	//
	m_posSel.nCol = range.posEnd.nCol + 1;
	m_posSel.nLine = range.posEnd.nLine;

	// The function name begins at the first alphanumeric character on line
	int nStart = min( nEnd, lstrlen( szLine ) );

	while( nStart > 0 && ( _istalnum( szLine[nStart - 1] ) ||
		L'_' == szLine[nStart - 1] ) )
	{
		--nStart;
	}

	// Extract the function name
	lstrcpyn( szFunc, szLine + nStart, nEnd - nStart + 2 );

	if( L'\0' == *szFunc )
		return FALSE;

	// If we don't care whether or not the function actually exists in the
	// list, just return the string.
	if( !bMustExist )
	{
		if( pszName )
			lstrcpy( pszName, szFunc );

		return TRUE;
	}

	// Look for the function name in the INI file.
	int iIndex = 0;
	TCHAR szComp[MAX_FUNC_NAME] = {0};

	while( GetString( FUNC_NAMES, iIndex++, szComp ) )
	{
		if( 0 == lstrcmp( szComp, szFunc ) )
		{
			if( pszName )
				lstrcpy( pszName, szComp );

			return TRUE;
		}
	}

	return FALSE;
}

// GetPrototype -------------------------------------------------------------
BOOL CCodeFile::GetPrototype( LPCTSTR pszFunc, LPTSTR pszProto, int iOverload )
{
	// Is this function overloaded?
	if( iOverload || GetPrivateProfileInt( SECTION_OVERLOADS, pszFunc, 0,
		m_szIniFile ) )
	{
		// Yes, so return appropriate overload
		TCHAR section[20] = {0};

		wsprintf( section, _T("%s%d"), pszFunc, iOverload );
		GetPrivateProfileString( section, _T("Func0"), _T(""), pszProto,
			MAX_FUNC_PROTO, m_szIniFile );

		ASSERT( L'\0' != *pszProto );
	}
	else
	{
		// Look for the function name in the INI file.
		int iIndex = 0;
		TCHAR szComp[MAX_FUNC_NAME] = {0};

		while( GetString( FUNC_NAMES, iIndex++, szComp ) )
		{
			if( 0 == lstrcmp( szComp, pszFunc ) )
			{
				VERIFY( GetString( FUNC_PROTO, --iIndex, pszProto ) );
				break;
			}
		}
	}

	return ( L'\0' != *pszProto );
}

// GetCurrentArgument -------------------------------------------------------
int CCodeFile::GetCurrentArgument()
{
	// Parse the buffer to determine which argument to highlight...
	//
	int iArg = 0;
	CM_RANGE range = {0};
	TCHAR szLine[10000] = {0};

	VERIFY( CME_SUCCESS == CM_GetSel( m_hWndEdit, &range, TRUE ) );

	enum
	{
		sNone,
		sQuote,
		sComment,
		sMultiComment,
		sEscape,
		sSubParen,
	
	} state = sNone;

	for( int nLine = m_posSel.nLine; nLine <= range.posEnd.nLine; nLine++ )
	{
		// Get next line from buffer
		VERIFY( CME_SUCCESS == CM_GetLine( m_hWndEdit, nLine, szLine ) );

		if( nLine == range.posEnd.nLine )
		{
			// Trim off any excess beyond cursor pos so the argument with the
			// cursor in it will be highlighted.
			//
			int iTrim = min( range.posEnd.nCol, lstrlen( szLine ) );
			szLine[iTrim] = L'\0';
		}

		if( nLine == m_posSel.nLine )
		{
			// Strip off function name & open paren.
			LPTSTR psz = _tcschr( szLine, L'(' );
			ASSERT( psz != NULL );
			lstrcpy( szLine, ++psz );
		}

		// Parse arguments from current line
		for( int iPos = 0, len = lstrlen( szLine ); iPos < len; iPos++ )
		{
			switch( szLine[iPos] )
			{
			case L'(':// Sub-parenthesis
				{
					switch( state )
					{
					case sNone:
						state = sSubParen;
						break;
					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			case L'"':// String begin/end
				{
					switch( state )
					{
					case sQuote:
						state = sNone;
						break;
					case sComment:
					case sMultiComment:
					case sSubParen:
						break;
					default:
						state = sQuote;
						break;
					}
				}
				break;

			case L',':// Argument separator
				{
					switch( state )
					{
					case sNone:
						iArg++;
						break;
					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			case L')':// End of function statement
				{
					switch( state )
					{
					case sNone:
						return -1;// Destroy tooltip on return
					case sEscape:
						state = sQuote;
						break;
					case sSubParen:
						state = sNone;
						break;
					}
				}
				break;

			case L'\\':// Escape sequence
				{
					switch( state )
					{
					case sQuote:
						state = sEscape;
						break;
					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			case L'/':// Possible comment begin/end
				{
					switch( state )
					{
					case sNone:
						{
							if( iPos + 1 < len )
							{
								TCHAR c = szLine[iPos + 1];

								if( L'/' == c )
									state = sComment;
								else if( L'*' == c )
								{
									state = sMultiComment;
									iPos++;
								}
							}
						}
						break;

					case sMultiComment:
						{
							if( iPos && L'*' == szLine[iPos - 1] )
								state = sNone;
						}
						break;

					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			default:
				{
					if( sEscape == state )
						state = sQuote;
				}
				break;
			}

			// No point in scanning each and every character in comment line!
			if( sComment == state )
				break;
		}

		// It's safe to clear this now that we're moving on to the next line
		if( sComment == state )
			state = sNone;
	}

	return iArg;
}

// SelectClosestItemInCodeList ----------------------------------------------
void CCodeFile::SelectClosestItemInCodeList()
{
	if( ::IsWindow( m_hListCtrl ) )
	{
		TCHAR szFunc[MAX_FUNC_NAME] = {0};
		
		if( GetCurrentFunction( szFunc, false ) )
		{
			LVFINDINFO lvfi = {0};

			lvfi.psz = szFunc;
			lvfi.flags = LVFI_STRING | LVFI_PARTIAL;

			int iItem = ListView_FindItem( m_hListCtrl, -1, &lvfi );

			if( -1 != iItem )
			{
				ListView_SetItemState( m_hListCtrl, iItem, LVIS_SELECTED |
					LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );

				ListView_EnsureVisible( m_hListCtrl, iItem, FALSE );
			}
		}
	}
}

// SelectCurrentFunction ----------------------------------------------------
void CCodeFile::SelectCurrentFunction()
{
	CM_RANGE range = {0};
	TCHAR szLine[1000] = {0};

	// Note:  We can't use CM_GetWord() here, because the user could have
	// typed a function name followed by a space, and *then* the left
	// parenthesis, i.e. "MessageBox ("

	// Get the current line
	CM_GetSel( m_hWndEdit, &range, TRUE );
	CM_GetLine( m_hWndEdit, range.posEnd.nLine, szLine );

	// There's nothing for us to do if the line is empty
	if( L'\0' == *szLine )
		return;

	int nEnd = range.posEnd.nCol - 1;

	// Trim off trailing '(', if found
	if( nEnd > 0 && nEnd < lstrlen( szLine ) )
	{
		if( L'(' == szLine[ nEnd ] )
			nEnd --;

		// Trim off trailing whitespace
		while( nEnd > 0 && L' ' == szLine[nEnd] )
			nEnd --;
	}

	if( nEnd < 0 )
		return;

	// The function name begins at the first alphanumeric character on line
	int nStart = min( nEnd, lstrlen( szLine ) );

	while( nStart > 0 && ( _istalnum( szLine[nStart - 1] ) ||
		L'_' == szLine[nStart - 1] ) )
	{
		--nStart;
	}

	range.posStart.nCol = nStart;
	range.posEnd.nCol = nEnd + 1;

	VERIFY( CME_SUCCESS == CM_SetSel( m_hWndEdit, &range, TRUE ) );
}
