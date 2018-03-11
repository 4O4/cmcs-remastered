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

CCodeFile *CEditorApp::m_pActiveCode = NULL;
HMENU g_hMenuCurr = NULL;

// main entry point
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
	// let the app do the rest
	CEditorApp App;
	return App.Run( hInstance, hPrevInstance, lpszCmdLine, nCmdShow );
}

CEditorApp::CEditorApp()
{
	m_hWnd = m_hWndClient = m_hWndToolBar = m_hWndStatusBar = NULL;
	m_hInst = NULL;
	m_hMenuCode = m_hMenuEmpty = NULL;
	m_dwIdleFlags = eFILES;
	memset( m_pszMRUList, 0, sizeof( m_pszMRUList ) );
	m_nMRUCount = 0;
	m_hFont = NULL;
	m_hilMargin = NULL;
}

CEditorApp::~CEditorApp()
{
	if ( m_hFont )
		DeleteObject( m_hFont );

	if ( m_hilMargin )
		ImageList_Destroy( m_hilMargin );
}

// Run() will perform app initialization and contain the main message loop
int CEditorApp::Run( HINSTANCE hInstance, HINSTANCE /* hPrevInstance */, LPSTR lpszCmdLine, int /* nCmdShow */ )
{
	m_hInst = hInstance;

	WNDCLASS    wndclass;

	wndclass.style         = 0;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_APPICON ) );
	wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = MAKEINTRESOURCE( IDR_EMPTY_MENU );
	wndclass.lpszClassName = EDITAPPWNDCLASS;

	VERIFY( RegisterClass( &wndclass ) );

	m_hMenuCode = LoadMenu( hInstance, MAKEINTRESOURCE( IDR_CODE_MENU ) );
	m_hMenuEmpty = LoadMenu( hInstance, MAKEINTRESOURCE( IDR_EMPTY_MENU ) );

	m_hWnd = CreateWindow( EDITAPPWNDCLASS,
	                       APPNAME,
	                       WS_OVERLAPPEDWINDOW,
	                       CW_USEDEFAULT, 
						   CW_USEDEFAULT,
	                       CW_USEDEFAULT,
						   CW_USEDEFAULT,
	                       NULL,
						   NULL,
						   hInstance,
						   NULL );

	SetWindowLong( m_hWnd, GWL_USERDATA, ( DWORD ) this );

	// register the MDI child wnd class
	VERIFY( CCodeFile::RegisterClass( hInstance ) );

	// get ready to use the common controls
	InitCommonControls();

	m_hWndToolBar = CreateWindow( TOOLBARCLASSNAME,
	                              _T(""),
	                              WS_CHILD | WS_VISIBLE | CCS_TOP | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
	                              0, 
						          0,
	                              100,
						          100,
								  m_hWnd,
						          NULL,
						          hInstance,
						          NULL );

	ASSERT( IsWindow( m_hWndToolBar ) );
	SendMessage( m_hWndToolBar, TB_AUTOSIZE, 0, 0 );

	TBADDBITMAP ab = { hInstance, IDB_TOOLBAR };
	SendMessage( m_hWndToolBar, TB_BUTTONSTRUCTSIZE, sizeof( TBBUTTON ), 0 );
	SendMessage( m_hWndToolBar, TB_ADDBITMAP, 10, ( LPARAM ) &ab );

	TBBUTTON Buttons[] = {
		{ 0, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 1, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 2, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 3, ID_FILE_PRINT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ -1, 0, 0, TBSTYLE_SEP, 0, 0 },
		{ 4, ID_EDIT_CUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 5, ID_EDIT_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 6, ID_EDIT_PASTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ -1, 0, 0, TBSTYLE_SEP, 0, 0 },
		{ 7, ID_EDIT_UNDO, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 8, ID_EDIT_REDO, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ -1, 0, 0, TBSTYLE_SEP, 0, 0 },
		{ 9, ID_EDIT_PROPERTIES, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
	};

	VERIFY( SendMessage( m_hWndToolBar, TB_ADDBUTTONS, ARRAY_SIZE( Buttons ), ( LPARAM ) Buttons ) );

	m_hWndStatusBar = CreateWindow( STATUSCLASSNAME,
	                              _T(""),
	                              WS_CHILD | WS_VISIBLE | CCS_BOTTOM,
	                              0, 
						          0,
	                              100,
						          100,
								  m_hWnd,
						          NULL,
						          hInstance,
						          NULL );

	HDC hDC = GetDC( NULL );
	int cxInch = GetDeviceCaps( hDC, LOGPIXELSX );
	ReleaseDC( NULL, hDC );
	int cxParts[] = { cxInch, MulDiv( 3, cxInch, 2 ) };
	                  
	SendMessage( m_hWndStatusBar, SB_SETPARTS, ARRAY_SIZE( cxParts ), ( LPARAM ) cxParts );

	CLIENTCREATESTRUCT ccs = 
	{
		GetSubMenu( m_hMenuCode, 2 ),
		ID_MDICHILD_1
	};

	m_hWndClient = CreateWindowEx( WS_EX_CLIENTEDGE,
	                               _T("MDICLIENT"),
	                               NULL,
								   WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   CW_USEDEFAULT,
								   m_hWnd,
								   NULL,
								   hInstance,
								   &ccs );

	SetWindowLong( m_hWndClient, GWL_USERDATA, ( DWORD ) this );

	// load the margin images to be used by CodeMax
	m_hilMargin = ImageList_LoadBitmap( hInstance,
	                                    MAKEINTRESOURCE( IDB_MARGIN ),
										14,
										1,
										RGB( 0, 255, 0 ) );

    // load the global settings
	LoadProfile( NULL );
	UpdateWindow( m_hWnd );

	if ( lpszCmdLine && *lpszCmdLine )
	{
		LPTSTR pszCmdLine;
#ifdef _UNICODE
		int nLen = strlen( lpszCmdLine );
		LPTSTR pszWCmdLine = new TCHAR[ nLen + 1 ];
		MultiByteToWideChar( CP_ACP, 0, lpszCmdLine, nLen, pszWCmdLine, nLen );
		pszWCmdLine[ nLen ] = _T('\0');
		pszCmdLine = pszWCmdLine;
#else
		pszCmdLine = lpszCmdLine;
#endif
		TCHAR szTemp[ _MAX_PATH + 1 ];
		// Windows Explorer quotes the filename passed in.  Strip off the quotes now
		_tcscpy( szTemp, pszCmdLine );
		pszCmdLine = szTemp;
		if ( *szTemp == '"' )
		{
			pszCmdLine++;
			*( pszCmdLine + _tcslen( pszCmdLine ) - 1 ) = '\0';
		} 
		TCHAR szFullPath[ _MAX_PATH + 1 ];
		LPTSTR pszDontCare;
		if ( GetFullPathName( pszCmdLine, _MAX_PATH, szFullPath, &pszDontCare ) > 0 )
		{
			CCodeFile *pCode = CreateMDIChild();
			pCode->Open( szFullPath );
			// update the MRU list
			MoveToFrontOfMRUList( szFullPath );
		}
#ifdef _UNICODE
		delete pszWCmdLine;
#endif
	}
	
	HACCEL hAccel = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDR_ACCELS ) );
	MSG msg;

	for ( ;; )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( GetMessage( &msg, NULL, 0, 0 ) )
			{
				if ( !TranslateMDISysAccel( m_hWndClient, &msg ) && 
					 !TranslateAccelerator( m_hWnd, hAccel, &msg ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
			else
			{
				// WM_EXIT
				break;
			}
		}
		else
		{
			// perform background processing now
			DoIdleProcessing();
			WaitMessage();
		}	
	}

	// the current menu was destroy with the window.  We just need to destroy the 
	// menu that wasn't active.  This is no big deal, but it keeps BoundsChecker
	// from popping up (rightly so).
	DestroyMenu( g_hMenuCurr == m_hMenuCode ? m_hMenuEmpty : m_hMenuCode );
	DestroyAcceleratorTable( hAccel );
	VERIFY( UnregisterClass( EDITAPPWNDCLASS, hInstance ) );
	VERIFY( CCodeFile::UnregisterClass( hInstance ) );
	return msg.wParam;
}

// CreateMDIChild() creates a new MDI Child window and initializes it's contents
CCodeFile *CEditorApp::CreateMDIChild()
{
	ASSERT( IsWindow( m_hWndClient ) );

	// restore code frame window to last maximized/restored state 
	DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
	                ( GetProfileInt( CODE_SECTION, KEY_CODEMAXIMIZED, FALSE ) ? WS_MAXIMIZE : 0 );

	CCodeFile *pCode = new CCodeFile( this );
	if ( pCode )
	{
		// must have code menu active before WM_MDICREATE, or else the system menu icons
		// will not be painted if the MDI child is initially maximized
		g_hMenuCurr = m_hMenuCode;
		SetMenu( m_hWnd, m_hMenuCode );

		MDICREATESTRUCT mcs = {
			CODEFILEWNDCLASS,
			_T(""),
			m_hInst,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			dwStyle,
			( LPARAM ) pCode
		};

		HWND hWnd = ( HWND ) SendMessage( m_hWndClient, WM_MDICREATE, 0, ( LPARAM ) &mcs );

		if ( hWnd )
		{
			SetFocus( hWnd );
		}
		else
		{
			delete pCode;
			pCode = NULL;
		}
	}
	m_dwIdleFlags |= eMENU;
	return pCode;
}

// recalcs the toolbar, status bar, and MDI client window
void CEditorApp::RecalcLayout( int cx, int cy )
{
	// resize the toolbar and MDI client
	SendMessage( m_hWndToolBar, TB_AUTOSIZE, 0, 0 );
	RECT rcToolBar;
	GetWindowRect( m_hWndToolBar, &rcToolBar );
	RECT rcStatusBar;
	GetWindowRect( m_hWndStatusBar, &rcStatusBar );
	int cyStatusBar = ( rcStatusBar.bottom - rcStatusBar.top );
	int cyToolBar = ( rcToolBar.bottom - rcToolBar.top );
	int cyClient = cy - cyToolBar - cyStatusBar;
	SetWindowPos( m_hWndStatusBar, NULL, 0, cy - cyStatusBar, cx, cyStatusBar, SWP_NOZORDER );
	SetWindowPos( m_hWndClient, NULL, 0, cyToolBar, cx, cyClient, SWP_NOZORDER );
}

// DoIdleProcessing() performs idle tasks:
//   - Toolbar button enable/disable
//   - Menu swapping
//   - Stale file detection
//   - status bar selection update
void CEditorApp::DoIdleProcessing()
{
	// Update the toolbar
	BOOL bActive = ( GetForegroundWindow() == m_hWnd );

	if ( bActive && IsWindowVisible( m_hWndToolBar ) )
	{
		int nButtons = SendMessage( m_hWndToolBar, TB_BUTTONCOUNT, 0, 0 );
		for ( int i = 0; i < nButtons; i++ )
		{
			TBBUTTON btn;
			VERIFY( SendMessage( m_hWndToolBar, TB_GETBUTTON, i, ( LPARAM ) &btn ) );
			VERIFY( SendMessage( m_hWndToolBar, TB_SETSTATE, btn.idCommand, CommandIsValid( btn.idCommand ) ? TBSTATE_ENABLED : 0 ) );
		}
	}

	if ( ( m_dwIdleFlags & eMENU ) == eMENU )
	{
		m_dwIdleFlags &= ~eMENU;
		HMENU hMenu = m_pActiveCode ? m_hMenuCode : m_hMenuEmpty;
		if ( hMenu != GetMenu( m_hWnd ) )
		{
			g_hMenuCurr = hMenu;
			SetMenu( m_hWnd, hMenu );
			DrawMenuBar( m_hWnd );
		}
	}

	if ( ( m_dwIdleFlags & eFILES ) == eFILES && bActive )
	{
		// prevent re-entrancy.
		m_dwIdleFlags &= ~eFILES;
		HWND hWndMDIChild = GetWindow( m_hWndClient, GW_CHILD );
		while ( hWndMDIChild )
		{
			CCodeFile *pCode = ( CCodeFile * ) GetWindowLong( hWndMDIChild, GWL_USERDATA );
			ASSERT( pCode );
			if ( pCode->CanDoIdleProcessing() )
			{
				pCode->UpdateReadOnlyState();
				if ( !pCode->IsCodeUpToDate() )
				{
					TCHAR szMsg[ _MAX_PATH + 100 ];
					TCHAR szTitle[ _MAX_PATH + 1 ];
					pCode->GetTitle( szTitle );
					wsprintf( szMsg, _T("%s\n\nThis file has been modified outside of\nthis editor.  Do you want to reload it?"), szTitle );
					if ( MessageBox( m_hWnd, szMsg, APPNAME, MB_YESNO | MB_ICONQUESTION ) == IDYES )
					{
						pCode->Open( pCode->GetPath() );
					}
					pCode->UpdateFileTime();
				}
			}
			hWndMDIChild = GetWindow( hWndMDIChild, GW_HWNDNEXT );
		}
		m_dwIdleFlags |= eFILES;
	}

	if ( ( m_dwIdleFlags & eSTATBAR ) == eSTATBAR )
	{
		m_dwIdleFlags &= ~eSTATBAR;
		if ( m_pActiveCode )
		{
			m_pActiveCode->OnSelChanged();
		}
		else
		{
			OnSelChanged( -1, -1 );
		}
	}
}

// MoveToFontOfMRUList() moves a file to the front of the MRU list on the file menu
void CEditorApp::MoveToFrontOfMRUList( LPCTSTR pszPath )
{
	ASSERT( pszPath );
	ASSERT( MAX_MRU_SIZE > 0 );

	TCHAR szPath[ _MAX_PATH + 1 ];
	_tcscpy( szPath, pszPath );

	// remove the entry from the previous slot, if found
	for ( int i = 0; i < m_nMRUCount; i++ )
	{
		if ( !_tcsicmp( szPath, m_pszMRUList[ i ] ) )
		{
			// found it! - now shift up paths after it and clear the last entry
			if ( i + 1 < MAX_MRU_SIZE )
			{
				memmove( m_pszMRUList + i, m_pszMRUList + i + 1, ( MAX_MRU_SIZE - i - 1 ) * sizeof( m_pszMRUList[ 0 ] ) );
			}
			*m_pszMRUList[ MAX_MRU_SIZE - 1 ] = '\0';
			m_nMRUCount--;
			break;
		}
	}

	// Now, shift back the entries in the list to make room at the front
	memmove( m_pszMRUList + 1, m_pszMRUList, ( MAX_MRU_SIZE - 1 ) * sizeof( m_pszMRUList[ 0 ] ) );
	_tcscpy( m_pszMRUList[ 0 ], szPath );
	m_nMRUCount++;
	m_nMRUCount = min( m_nMRUCount, MAX_MRU_SIZE );
}

// ActivateIfOpen() will try to locate the requested file among the open documents and activate if open.
// Returns TRUE if the file was already open, FALSE otherwise.
BOOL CEditorApp::ActivateIfOpen( LPCTSTR pszPath ) const
{
	// First see if the window is already open
	HWND hWndChild = GetWindow( m_hWndClient, GW_CHILD );
	while ( hWndChild )
	{
		CCodeFile *pCode = ( CCodeFile * ) GetWindowLong( hWndChild, GWL_USERDATA );
		ASSERT( pCode );
		if ( _tcsicmp( pCode->GetPath(), pszPath ) == 0 )
		{
			break;
		}
		hWndChild = GetWindow( hWndChild, GW_HWNDNEXT );
	}

	if ( hWndChild )
	{
		// file is already open -- reactivate the window
		SendMessage( m_hWndClient, WM_MDIACTIVATE, ( WPARAM ) hWndChild, 0 );
	}
	
	return ( hWndChild != NULL );
}


// Registry keys
#define KEY_WINDOWPOS _T("WINPOS")
#define KEY_COLORSYNTAX _T("COLORSYNTAX")
#define KEY_COLORS _T("COLORS")
#define KEY_WHITESPACEDISPLAY _T("WHITESPACEDISPLAY")
#define KEY_TABEXPAND _T("TABEXPAND")
#define KEY_SMOOTHSCROLLING _T("SMOOTHSCROLLING")
#define KEY_TABSIZE _T("TABSIZE")
#define KEY_LINETOOLTIPS _T("LINETOOLTIPS")
#define KEY_LEFTMARGIN _T("LEFTMARGIN")
#define KEY_COLUMNSEL _T("COLUMNSEL")
#define KEY_DRAGDROP _T("DRAGDROP")
#define KEY_CASESENSITIVE _T("CASESENSITIVE")
#define KEY_PRESERVECASE _T("PRESERVECASE")
#define KEY_WHOLEWORD _T("WHOLEWORD")
#define KEY_AUTOINDENTMODE _T("AUTOINDENTMODE")
#define KEY_HSCROLLBAR _T("HSCROLLBAR")
#define KEY_VSCROLLBAR _T("VSCROLLBAR")
#define KEY_HSPLITTER _T("HSPLITTER")
#define KEY_VSPLITTER _T("VSPLITTER")
#define KEY_HOTKEYS _T("HOTKEYS")
#define KEY_FINDMRULIST _T("FINDMRULIST")
#define KEY_REPLMRULIST _T("REPLMRULIST")
#define KEY_FONT _T("FONT")
#define KEY_LINENUMBERING _T("LINENUMBERING")
#define KEY_FONTSTYLES _T("FONTSTYLES")
#define KEY_NORMALIZECASE _T("NORMALIZECASE")
#define KEY_SELBOUNDS _T("SELBOUNDS")


// SaveProfile() will save the current settings of a window (hWnd != NULL) or the global settings 
// (hWnd == NULL) to the registry.
void CEditorApp::SaveProfile( HWND hWnd ) const
{
	if ( hWnd )
	{
		// misc props
		WriteProfileInt( CODE_SECTION, KEY_COLORSYNTAX, CM_IsColorSyntaxEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_HSPLITTER, CM_IsSplitterEnabled( hWnd, TRUE ) );
		WriteProfileInt( CODE_SECTION, KEY_VSPLITTER, CM_IsSplitterEnabled( hWnd, FALSE ) );
		WriteProfileInt( CODE_SECTION, KEY_HSCROLLBAR, CM_HasScrollBar( hWnd, TRUE ) );
		WriteProfileInt( CODE_SECTION, KEY_VSCROLLBAR, CM_HasScrollBar( hWnd, FALSE ) );
		WriteProfileInt( CODE_SECTION, KEY_WHOLEWORD, CM_IsWholeWordEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_AUTOINDENTMODE, CM_GetAutoIndentMode( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_SMOOTHSCROLLING, CM_IsSmoothScrollingEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_LINETOOLTIPS, CM_IsLineToolTipsEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_LEFTMARGIN, CM_IsLeftMarginEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_COLUMNSEL, CM_IsColumnSelEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_DRAGDROP, CM_IsDragDropEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_CASESENSITIVE, CM_IsCaseSensitiveEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_PRESERVECASE, CM_IsPreserveCaseEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_TABEXPAND, CM_IsTabExpandEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_TABSIZE, CM_GetTabSize( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_NORMALIZECASE, CM_IsNormalizeCaseEnabled( hWnd ) );
		WriteProfileInt( CODE_SECTION, KEY_SELBOUNDS, CM_IsSelBoundsEnabled( hWnd ) );

		// color info
		CM_COLORS colors;
		CM_GetColors( hWnd, &colors );
		WriteProfileBinary( CODE_SECTION, KEY_COLORS, ( LPBYTE ) &colors, sizeof( colors ) );

		// font info
		CM_FONTSTYLES fs;
		CM_GetFontStyles( hWnd, &fs );
		WriteProfileBinary( CODE_SECTION, KEY_FONTSTYLES, ( LPBYTE ) &fs, sizeof( fs ) );

		// font
		LOGFONT lf;
		HFONT hFont = ( HFONT ) SendMessage( hWnd, WM_GETFONT, 0, 0 );
		int cbSize = GetObject( hFont, sizeof( lf ), NULL );
		VERIFY( GetObject( hFont, cbSize, &lf ) );
		WriteProfileBinary( CODE_SECTION, KEY_FONT, ( LPBYTE ) &lf, sizeof( lf ) );

		CM_LINENUMBERING cmLineNum;
		CM_GetLineNumbering( hWnd, &cmLineNum );
		WriteProfileBinary( CODE_SECTION, KEY_LINENUMBERING, ( LPBYTE ) &cmLineNum, sizeof( cmLineNum ) );
	}
	else
	{
		// save global settings

		// window position
		WINDOWPLACEMENT wp;
		wp.length = sizeof( wp );
		GetWindowPlacement( m_hWnd, &wp );
		VERIFY( WriteProfileBinary( CODE_SECTION, KEY_WINDOWPOS, ( LPBYTE ) &wp, sizeof( wp ) ) );

		// File MRU list
		for ( int i = 0; i < m_nMRUCount; i++ )
		{
			TCHAR szKey[ 10 ];
			wsprintf( szKey, _T("File%d"), i );
			WriteProfileString( CODE_SECTION, szKey, m_pszMRUList[ i ] );
		}

		// macros
		for ( i = 0; i < CM_MAX_MACROS; i++ )
		{
			int cbMacro = CMGetMacro( i, NULL );
			LPBYTE pMacro = cbMacro ? new BYTE[ cbMacro ] : 0;
			if ( pMacro )
			{
				CMGetMacro( i, pMacro );
			}
			TCHAR szKey[ 10 ];
			wsprintf( szKey, _T("Macro%d"), i );
			WriteProfileBinary( CODE_SECTION, szKey, pMacro, cbMacro );
			if ( pMacro )
			{
				delete pMacro;
			}
		}

		// hotkeys
		int cbHotKeys = CMGetHotKeys( NULL );
		LPBYTE pHotKeys = cbHotKeys ? new BYTE[ cbHotKeys ] : 0;
		if ( pHotKeys )
		{
			CMGetHotKeys( pHotKeys );
		}
		WriteProfileBinary( CODE_SECTION, KEY_HOTKEYS, pHotKeys, cbHotKeys );
		if ( pHotKeys )
		{
			delete pHotKeys;
		}

		// Find/Replace MRU lists
		TCHAR szMRUList[ CM_FIND_REPLACE_MRU_BUFF_SIZE ];
		CMGetFindReplaceMRUList( szMRUList, TRUE );
		WriteProfileString( CODE_SECTION, KEY_FINDMRULIST, szMRUList );
		CMGetFindReplaceMRUList( szMRUList, FALSE );
		WriteProfileString( CODE_SECTION, KEY_REPLMRULIST, szMRUList );
	}
}

void CEditorApp::LoadProfile( HWND hWnd )
{
	if ( hWnd )
	{
		// load window settings
		CM_EnableColorSyntax( hWnd, GetProfileInt( CODE_SECTION, KEY_COLORSYNTAX, TRUE ) );
		CM_ShowScrollBar( hWnd, TRUE, GetProfileInt( CODE_SECTION, KEY_HSCROLLBAR, TRUE ) );
		CM_ShowScrollBar( hWnd, FALSE, GetProfileInt( CODE_SECTION, KEY_VSCROLLBAR, TRUE ) );
		CM_EnableSplitter( hWnd, TRUE, GetProfileInt( CODE_SECTION, KEY_HSPLITTER, TRUE ) );
		CM_EnableSplitter( hWnd, FALSE, GetProfileInt( CODE_SECTION, KEY_VSPLITTER, TRUE ) );
		CM_EnableWholeWord( hWnd, GetProfileInt( CODE_SECTION, KEY_WHOLEWORD, FALSE ) );
		CM_SetAutoIndentMode( hWnd, GetProfileInt( CODE_SECTION, KEY_AUTOINDENTMODE, CM_INDENT_PREVLINE ) );
		CM_EnableSmoothScrolling( hWnd, GetProfileInt( CODE_SECTION, KEY_SMOOTHSCROLLING, FALSE ) );
		CM_EnableLineToolTips( hWnd, GetProfileInt( CODE_SECTION, KEY_LINETOOLTIPS, TRUE ) );
		CM_EnableLeftMargin( hWnd, GetProfileInt( CODE_SECTION, KEY_LEFTMARGIN, TRUE ) );
		CM_EnableColumnSel( hWnd, GetProfileInt( CODE_SECTION, KEY_COLUMNSEL, TRUE ) );
		CM_EnableDragDrop( hWnd, GetProfileInt( CODE_SECTION, KEY_DRAGDROP, TRUE ) );
		CM_EnableCaseSensitive( hWnd, GetProfileInt( CODE_SECTION, KEY_CASESENSITIVE, FALSE ) );
		CM_EnablePreserveCase( hWnd, GetProfileInt( CODE_SECTION, KEY_PRESERVECASE, FALSE ) );
		CM_EnableTabExpand( hWnd, GetProfileInt( CODE_SECTION, KEY_TABEXPAND, FALSE ) );
		CM_SetTabSize( hWnd, GetProfileInt( CODE_SECTION, KEY_TABSIZE, 4 ) );
		CM_EnableNormalizeCase( hWnd, GetProfileInt( CODE_SECTION, KEY_NORMALIZECASE, FALSE ) );
		CM_EnableSelBounds( hWnd, GetProfileInt( CODE_SECTION, KEY_SELBOUNDS, FALSE ) );

		// colors
		LPBYTE pColors;
		UINT unSize;
		if ( GetProfileBinary( CODE_SECTION, KEY_COLORS, &pColors, &unSize ) )
		{
			if ( unSize == sizeof( CM_COLORS ) )
			{
				CM_SetColors( hWnd, pColors );
			}
			delete pColors;
		}

		// font info
		LPBYTE pFontStyles;
		if ( GetProfileBinary( CODE_SECTION, KEY_FONTSTYLES, &pFontStyles, &unSize ) )
		{
			if ( unSize == sizeof( CM_FONTSTYLES ) )
			{
				CM_SetFontStyles( hWnd, pFontStyles );
			}
			delete pFontStyles;
		}

		// font
		if ( !m_hFont )
		{
			LOGFONT *plf;
			if ( GetProfileBinary( CODE_SECTION, KEY_FONT, ( LPBYTE * ) &plf, &unSize ) )
			{
				m_hFont = CreateFontIndirect( plf );
				delete plf;
				SendMessage( hWnd, WM_SETFONT, ( WPARAM ) m_hFont, 0 );
			}
		}

		// colors
		LPBYTE pLineNum;
		if ( GetProfileBinary( CODE_SECTION, KEY_LINENUMBERING, &pLineNum, &unSize ) )
		{
			if ( unSize == sizeof( CM_LINENUMBERING ) )
			{
				CM_SetLineNumbering( hWnd, pLineNum );
			}
			delete pLineNum;
		}
	}
	else
	{
		// load global settings

		// window position
		WINDOWPLACEMENT *pwp;
		UINT unSize;
		if ( GetProfileBinary( CODE_SECTION, KEY_WINDOWPOS, ( LPBYTE * ) &pwp, &unSize ) )
		{
			SetWindowPlacement( m_hWnd, pwp );
			delete pwp;
		}
		else
		{
			ShowWindow( m_hWnd, SW_SHOW );
		}

		// File MRU list
		for ( int i = MAX_MRU_SIZE - 1; i >= 0; i-- )
		{
			TCHAR szKey[ 10 ];
			wsprintf( szKey, _T("File%d"), i );
			TCHAR szPath[ _MAX_PATH + 1 ];
			if ( GetProfileString( CODE_SECTION, szKey, szPath, NULL ) )
			{
				MoveToFrontOfMRUList( szPath );
			}
		}

		// macros
		for ( i = 0; i < CM_MAX_MACROS; i++ )
		{
			TCHAR szKey[ 10 ];
			wsprintf( szKey, _T("Macro%d"), i );
			LPBYTE pMacro;
			UINT unSize;
			
			if ( GetProfileBinary( CODE_SECTION, szKey, &pMacro, &unSize ) )
			{
				if ( unSize )
				{
					CMSetMacro( i, pMacro );
				}
				delete pMacro;
			}
		}

		// hotkeys
		LPBYTE pHotKeys;
		if ( GetProfileBinary( CODE_SECTION, KEY_HOTKEYS, &pHotKeys, &unSize ) )
		{
			if ( unSize )
			{
				CMSetHotKeys( pHotKeys );
			}
			delete pHotKeys;
		}

		// Find/Replace MRU lists
		TCHAR szMRUList[ CM_FIND_REPLACE_MRU_BUFF_SIZE ];
		CMSetFindReplaceMRUList( GetProfileString( CODE_SECTION, KEY_FINDMRULIST, szMRUList, _T("") ), TRUE );
		CMSetFindReplaceMRUList( GetProfileString( CODE_SECTION, KEY_REPLMRULIST, szMRUList, _T("") ), FALSE );

		// register some sample commands and hotkeys for each one
		CMRegisterCommand( CMD_STEPOVER, _T("DebugStepOver"), _T("Step over the current line of code") );
		CM_HOTKEY cmHotKey = { 0, VK_F10, 0, 0 };
		CMRegisterHotKey( &cmHotKey, CMD_STEPOVER );

		CMRegisterCommand( CMD_STEPINTO, _T("DebugStepInto"), _T("Step into the current function") );
		cmHotKey.nVirtKey1 = VK_F8;
		CMRegisterHotKey( &cmHotKey, CMD_STEPINTO );

		CMRegisterCommand( CMD_TOGGLEBREAKPOINT, _T("DebugToggleBreakpoint"), _T("Toggle a breakpoint at the current location") );
		cmHotKey.nVirtKey1 = VK_F9;
		CMRegisterHotKey( &cmHotKey, CMD_TOGGLEBREAKPOINT );
	}
}

// OnSelChanged() is called when the status bar should update it's line and column stats
void CEditorApp::OnSelChanged( int nLine, int nCol ) const
{
	TCHAR szSel[ 50 ];
	if ( nLine == -1 || nCol == -1 )
	{
		*szSel = '\0';
	}
	else
	{
		wsprintf( szSel, _T("Ln %d, Col %d"), nLine + 1, nCol + 1 );
	}
	SendMessage( m_hWndStatusBar, SB_SETTEXT, 0, ( LPARAM ) szSel );
}

void CEditorApp::UpdateOvertypeStatus( BOOL bOvertype )
{
	LPCTSTR psz = bOvertype ? _T("OVR") : _T("INS");
	SendMessage( m_hWndStatusBar, SB_SETTEXT, 1, ( LPARAM ) psz );
}
LRESULT CEditorApp::OnDestroy( WPARAM /* wParam */, LPARAM /* lParam */ )
{
    // save the global settings
	SaveProfile( NULL );

	PostQuitMessage( 0 );
	return 0;
}

LRESULT CEditorApp::OnSize( WPARAM /* wParam */, LPARAM lParam )
{
	int cx = LOWORD( lParam );
	int cy = HIWORD( lParam );

	// Reposition the child windows
	RecalcLayout( cx, cy );
	return 0;
}

LRESULT CEditorApp::OnInitMenuPopup( WPARAM wParam, LPARAM lParam )
{
	// If not the app system menu
	if ( !HIWORD( lParam ) )
	{
		HMENU hMenu = ( HMENU ) wParam;
		ASSERT( hMenu );

		BOOL bMaximized = FALSE;

		// figure out where the file menu is.  If there is a MDI child window that is maximized, then
		// the MDI Client window will put the MDI child window's system menu up with the App's menu
		// at position 0 and the file menu at position 1.  Otherwise, the file menu is at position 0.
		HWND hWndMDIChild = ( HWND ) SendMessage( m_hWndClient, WM_MDIGETACTIVE, 0, ( LPARAM ) &bMaximized );
		int nSysMenu = ( hWndMDIChild && bMaximized ) ? 0 : -1;
		int nFileMenu = nSysMenu + 1;
		int nEditMenu = nFileMenu + 1;
		// Check if this is the file menu
		if ( hMenu == GetSubMenu( GetMenu( m_hWnd ), nFileMenu ) )
		{
			// this is the file menu -- update the MRU List
			// first, remove the old entries
			BOOL bFoundList = FALSE;
			int nItems = GetMenuItemCount( hMenu );
			for ( int i = nItems - 2; i >= 0; i-- )
			{
				int nID = GetMenuItemID( hMenu, i );
				if ( nID >= ID_MRU_LIST_START && nID < ID_MRU_LIST_START + MAX_MRU_SIZE )
				{
					bFoundList = TRUE;
					RemoveMenu( hMenu, i, MF_BYPOSITION );
				}
			}

			// now, regenerate the MRU list.
			nItems = GetMenuItemCount( hMenu );
			if ( m_nMRUCount )
			{
				if( !bFoundList )
				{
					// append the separator
					InsertMenu( hMenu, nItems - 2, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );
					nItems++;
				}

				int nInsertAt = nItems - 2;
				for ( int i = m_nMRUCount - 1; i >= 0; i-- )
				{
					TCHAR szItem[ _MAX_PATH + 10 ];
					LPCTSTR pszPath = m_pszMRUList[ i ];
					if ( i <= 9 )
					{
						int nKey = ( i == 9 ) ? 0 : ( i + 1 );
						wsprintf( szItem, _T("&%d %s"), nKey,  pszPath );
					}
					else
					{
						wsprintf( szItem, _T("%s"), pszPath ); 
					}
					InsertMenu( hMenu, nInsertAt, MF_BYPOSITION | MF_STRING, ID_MRU_LIST_START + i, szItem );
				}
			}
			else
			{
				if ( bFoundList )
				{
					// remove the separator
					RemoveMenu( hMenu, nItems - 2, MF_BYPOSITION );
				}
			}


		}
		else if ( hMenu == GetSubMenu( GetMenu( m_hWnd ), nEditMenu ) )
		{
			// morph the macro menuitem to Record Macro or Stop Recording Macro
			ModifyMenu( hMenu, ID_EDIT_MACRO, MF_BYCOMMAND | MF_STRING, ID_EDIT_MACRO, 
			                   ( m_pActiveCode && CM_IsRecordingMacro( m_pActiveCode->GetEditHWND() ) ) ? _T("Stop Recording &Macro\tCtrl+Shift+R") : _T("Record &Macro\tCtrl+Shift+R") );
		}

		// enable/disable the menuitems if not the system menu -- DefFrameProc() will handle the system menu.
		if ( nSysMenu != 0 || hMenu != GetSubMenu( GetMenu( m_hWnd ), 0 ) )
		{
			int nItems = GetMenuItemCount( hMenu );
			for ( int i = 0; i < nItems; i++ )
			{
				EnableMenuItem( hMenu,
								i,
								MF_BYPOSITION | ( CommandIsValid( GetMenuItemID( hMenu, i ) ) ? MF_ENABLED : ( MF_DISABLED | MF_GRAYED ) ) );
			}
		}
	}	
	return DefFrameProc( m_hWnd, m_hWndClient, WM_INITMENUPOPUP, wParam, lParam );
}

LRESULT CEditorApp::OnClose( WPARAM wParam, LPARAM lParam )
{
	// if all windows were successfully closed, then go ahead and close the app window.
	return OnWindowCloseAll() ? DefFrameProc( m_hWnd, m_hWndClient, WM_CLOSE, wParam, lParam ) : 0;
}

// Command handler for the File MRU list
void CEditorApp::OnMRUList( int nFile )
{
	LPCTSTR pszPath = m_pszMRUList[ nFile ];
	ASSERT( *pszPath );

	// don't allow more than one window to show the same file
	if ( !ActivateIfOpen( pszPath ) )
	{
		// open the file
		CCodeFile *pCode = CreateMDIChild();
		pCode->Open( pszPath );
	}

	// update the MRU list
	MoveToFrontOfMRUList( pszPath );
}

// File | New handler
void CEditorApp::OnFileNew()
{
	CCodeFile *pCode = CreateMDIChild();
	if ( pCode )
	{
		pCode->UpdateWindowTitle();
	}
}

static 	LPTSTR g_pszFilter = _T("C/C++ files (*.cpp,*.c,*.h,*.hpp)\0*.cpp;*.c;*.h;*.hpp\0")
                             _T("Java files (*.java)\0*.java\0")
                             _T("Web files (*.htm,*.html,*.htx,*.asp,*.alx,*.stm,*.shtml)\0*.htm;*.html;*.htx;*.asp;*.alx;*.stm;*.shtml\0")
                             _T("XML files (*.xml)\0*.xml\0")
                             _T("Basic files (*.bas,*.inc)\0*.bas;*.inc\0")
                             _T("Pascal files (*.pas)\0*.pas\0")
                             _T("SQL files (*.sql,*.ddl)\0*.sql;*.ddl\0")
                             _T("Text files (*.txt,*.asc,*.csv,*.tab)\0*.txt;*.asc;*.csv;*.tab\0")
                             _T("All files (*.*)\0*.*\0");

// File | Open handler
void CEditorApp::OnFileOpen()
{
	TCHAR szPath[ _MAX_PATH + 1 ];
	*szPath = _T('\0');
	OPENFILENAME ofn = {
		sizeof( OPENFILENAME ),
		m_hWnd,
		m_hInst,
		g_pszFilter,
		NULL,
		0,
		0,
		szPath,
		ARRAY_SIZE( szPath ) - 1,
		NULL,
		0,
		NULL,
		NULL,
		OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		0,
		0,
		_T(".cpp"),
		0,
		NULL,
		NULL
	};
	if ( GetOpenFileName( &ofn ) )
	{
		// only allow one copy to be open at a time
		if ( !ActivateIfOpen( szPath ) )
		{
			CCodeFile *pCode = CreateMDIChild();
			pCode->Open( szPath );
		}
		// update the MRU list
		MoveToFrontOfMRUList( szPath );
	}
}

// File | Close handler
void CEditorApp::OnFileClose()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetHWND(), WM_CLOSE, 0, 0 );
}

// File | Save handler
BOOL CEditorApp::OnFileSave( CCodeFile *pCode )
{
	ASSERT( pCode );
	BOOL bSuccess = FALSE;
	if ( *pCode->GetPath() )
	{
		// saved before -- do silently
		bSuccess = pCode->Save();
	}
	else
	{
		// first time saved -- prompt for name
		bSuccess = OnFileSaveAs( pCode );
	}

	return bSuccess;
}

// File | SaveAs handler
BOOL CEditorApp::OnFileSaveAs( CCodeFile *pCode )
{
	BOOL bSuccess = FALSE;
	ASSERT( pCode );
	TCHAR szPath[ _MAX_PATH + 1 ];
	_tcscpy( szPath, pCode->GetPath() );
	OPENFILENAME ofn = {
		sizeof( OPENFILENAME ),
		m_hWnd,
		m_hInst,
		g_pszFilter,
		NULL,
		0,
		0,
		szPath,
		ARRAY_SIZE( szPath ) - 1,
		NULL,
		0,
		NULL,
		NULL,
		OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		0,
		0,
		_T(".cpp"),
		0,
		NULL,
		NULL
	};
	if ( GetSaveFileName( &ofn ) )
	{
		pCode->SetPath( szPath );
		bSuccess = OnFileSave( pCode );
		if ( bSuccess )
		{
			// update MRU list
			MoveToFrontOfMRUList( szPath );
		}
	}

	return bSuccess;
}

// File | SaveAll handler
void CEditorApp::OnFileSaveAll()
{
	ASSERT( m_pActiveCode );

	// iterate each child window and save
	BOOL bContinue = TRUE;
	HWND hWndMDIChild = GetWindow( m_hWndClient, GW_CHILD );
	while ( bContinue && hWndMDIChild )
	{
		CCodeFile *pCode = ( CCodeFile * ) GetWindowLong( hWndMDIChild, GWL_USERDATA );
		// save if a) never saved before
		//         b) buffer is modified
		//         c) file is newer than what's on disk
		if ( !*pCode->GetPath() ||
		     SendMessage( pCode->GetEditHWND(), CMM_ISMODIFIED, 0, 0 )  ||
			 !pCode->IsCodeUpToDate( FALSE ) )
		{
			bContinue = OnFileSave( pCode );
		}
		hWndMDIChild = GetWindow( hWndMDIChild, GW_HWNDNEXT );
	}
}

// File | Exit handler
void CEditorApp::OnFileExit()
{
	SendMessage( m_hWnd, WM_CLOSE, 0, 0 ); 
}

// File | Print handler
void CEditorApp::OnFilePrint()
{
	ASSERT( m_pActiveCode );
	CM_Print( m_pActiveCode->GetEditHWND(), NULL, CM_PRINT_PROMPTDLG | CM_PRINT_COLOR | CM_PRINT_RICHFONTS | CM_PRINT_BORDERTHIN | CM_PRINT_BORDERDOUBLE | CM_PRINT_PAGENUMS | CM_PRINT_DATETIME );
}

// Window | Tile handler
void CEditorApp::OnWindowTile()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_hWndClient, WM_MDITILE, 0, 0 );
}

// Window | Cascade handler
void CEditorApp::OnWindowCascade()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_hWndClient, WM_MDICASCADE, 0, 0 );
}

// Window | Arrange Icons handler
void CEditorApp::OnWindowArrangeIcons()
{
	ASSERT( m_pActiveCode );
	ArrangeIconicWindows( m_hWndClient );
}

// Window | Close All handler
BOOL CEditorApp::OnWindowCloseAll()
{
	BOOL bClose = TRUE;

	// close all children.  If any children cannot close (due to user request), then
	// stop closing windows
	HWND hWndMDIChild = GetWindow( m_hWndClient, GW_CHILD );
	while ( bClose && hWndMDIChild )
	{
		HWND hWndNext = GetWindow( hWndMDIChild, GW_HWNDNEXT );
		SendMessage( hWndMDIChild, WM_CLOSE, 0, ( LPARAM ) &bClose );	
		hWndMDIChild = hWndNext;
	}

	return bClose;
}

// Edit | Cut handler
void CEditorApp::OnEditCut()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_CUT, 0, 0 );
}

// Edit | Copy handler
void CEditorApp::OnEditCopy()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_COPY, 0, 0 );
}

// Edit | Paste handler
void CEditorApp::OnEditPaste()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_PASTE, 0, 0 );
}

// Edit | Undo handler
void CEditorApp::OnEditUndo()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_UNDO, 0, 0 );
}

// Edit | Redo handler
void CEditorApp::OnEditRedo()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_REDO, 0, 0 );
}

// Edit | Properties handler
void CEditorApp::OnEditProperties()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_EXECUTECMD, CMD_PROPERTIES, 0 );
}

// Edit | Find handler
void CEditorApp::OnEditFind()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_EXECUTECMD, CMD_FIND, 0 );
}

// Edit | Replace handler
void CEditorApp::OnEditReplace()
{
	ASSERT( m_pActiveCode && !m_pActiveCode->IsReadOnly() );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_EXECUTECMD, CMD_FINDREPLACE, 0 );
}

// Edit | Macro handler
void CEditorApp::OnEditMacro()
{
	ASSERT( m_pActiveCode );
	SendMessage( m_pActiveCode->GetEditHWND(), CMM_EXECUTECMD, CMD_RECORDMACRO, 0 );
}

// CommandIsValid() is used to check if a particular menu command is valid at the time
// of calling.
BOOL CEditorApp::CommandIsValid( UINT nCmd ) const
{
	// Window MRU list and File MRU list are always enabled
	BOOL bValid = ( nCmd == 0 || ( nCmd >= ID_MDICHILD_1 && nCmd <= ID_MDICHILD_1 + 10 ) ||
	                ( nCmd >= ID_MRU_LIST_START && nCmd < ID_MRU_LIST_START + MAX_MRU_SIZE ) );

	if ( !bValid )
	{
		switch ( nCmd )
		{
			case ID_FILE_NEW:
			case ID_FILE_OPEN:
			case ID_FILE_EXIT:
			{
				// always valid
				bValid = TRUE;
				break;
			}

			case ID_FILE_PRINT:
			case ID_EDIT_PROPERTIES:
			case ID_EDIT_FIND:
			case ID_FILE_SAVEAS:
			case ID_FILE_CLOSE:
			case ID_WINDOW_ARRANGEICONS:
			case ID_FILE_SAVEALL:
			case ID_WINDOW_TILE:
			case ID_WINDOW_CASCADE:
			case ID_WINDOW_CLOSEALL:
			{
				// need an active doc
				bValid = m_pActiveCode != NULL;
				break;
			}

			case ID_FILE_SAVE:
			case ID_EDIT_REPLACE:
			case ID_EDIT_MACRO:
			{
				// need an active doc that is not read-only
				bValid = m_pActiveCode && !m_pActiveCode->IsReadOnly();
				break;
			}

			case ID_EDIT_CUT:
			{
				// need an active doc that has a selection and is not read only
				bValid = m_pActiveCode && SendMessage( m_pActiveCode->GetEditHWND(), CMM_CANCUT, 0, 0 );
				break;
			}
			case ID_EDIT_COPY:
			{
				// need an active doc that has a selection
				bValid = m_pActiveCode && SendMessage( m_pActiveCode->GetEditHWND(), CMM_CANCOPY, 0, 0 );
				break;
			}
			case ID_EDIT_PASTE:
			{
				// need an active doc that has a selection and clipboard text is ready
				bValid = m_pActiveCode && SendMessage( m_pActiveCode->GetEditHWND(), CMM_CANPASTE, 0, 0 );
				break;
			}
			case ID_EDIT_UNDO:
			{
				// need and active doc that is not readonly and has undo info
				bValid = m_pActiveCode && SendMessage( m_pActiveCode->GetEditHWND(), CMM_CANUNDO, 0, 0 );
				break;
			}
			case ID_EDIT_REDO:
			{
				// need and active doc that is not readonly and has redo info
				bValid = m_pActiveCode && SendMessage( m_pActiveCode->GetEditHWND(), CMM_CANREDO, 0, 0 );
				break;
			}
		}
	}

	return bValid;
}

LRESULT CEditorApp::OnNotify( WPARAM /* wParam */, LPARAM lParam )
{
	NMHDR *pnmhdr = ( NMHDR * ) lParam;
	if ( pnmhdr->code == TTN_NEEDTEXT )
	{
		TOOLTIPTEXT *pTTT = ( TOOLTIPTEXT * ) lParam;
		if ( ( pTTT->uFlags & TTF_IDISHWND ) == 0 )
		{
			if ( pnmhdr->idFrom )
			{
				pTTT->lpszText = MAKEINTRESOURCE( pnmhdr->idFrom );
				pTTT->hinst = m_hInst;
				return TRUE;
			}
		}
	}

	return 0;
}

void SendMessageToDescendants( HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam, BOOL bSkipFirst )
{
	if ( !bSkipFirst )
		SendMessage( hWnd, unMsg, wParam, lParam );
	HWND hWndChild = GetWindow( hWnd, GW_CHILD );
	while ( hWndChild )
		{
		SendMessageToDescendants( hWndChild, unMsg, wParam, lParam, FALSE );
		hWndChild = GetWindow( hWndChild, GW_HWNDNEXT );
		}
	
}

LRESULT CEditorApp::OnSysColorChange( WPARAM wParam, LPARAM lParam )
{
	// broadcast message to all children so they can fixup their sizes
	SendMessageToDescendants( m_hWnd, WM_SYSCOLORCHANGE, wParam, lParam, TRUE );
	return 0;
}
