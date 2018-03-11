///////////////////////////////////////////////////////////////////
//
//  CodeMax sample app : CodeMax Editor
//
//  (c) WinMain Software 1998-2000
//
//
#ifndef __EDITAPP_H__
#define __EDITAPP_H__

#define APPNAME _T("CodeMax Editor")
#define COMPANY _T("WinMain Software")
#define EDITAPPWNDCLASS _T("CodeMaxSample")
#define DECLARE_MESSAGE_HANDLER( fn ) LRESULT fn( WPARAM wParam, LPARAM lParam )
#define DECLARE_MENU_HANDLER( fn ) void fn()
#define DECLARE_NOTIFY_HANDLER( fn ) LRESULT fn()
#define DECLARE_NOTIFY_HANDLER_PARAM( fn, type ) LRESULT fn(##type*)
#ifdef _UNICODE
#define CODE_SECTION _T("Code Window-Unicode")
#else
#define CODE_SECTION _T("Code Window-Ansi")
#endif
#define KEY_CODEMAXIMIZED _T("CODEMAXIMIZED")

#define MAX_MRU_SIZE 10

#define CMD_STEPOVER			CMD_USER_BASE + 0
#define CMD_STEPINTO			CMD_USER_BASE + 1
#define CMD_TOGGLEBREAKPOINT	CMD_USER_BASE + 2

#ifdef _DEBUG
	#define VERIFY( x ) ASSERT( ( x ) != 0 )
	#define ASSERT( x ) if ( !_ASSERT( ( x ) != 0, __FILE__, __LINE__ ) ) _asm { int 3 };

	BOOL _ASSERT( BOOL bCond, LPCSTR pszFile, int nLine );
#else
	#define ASSERT( x )
	#define VERIFY( x ) x
#endif

class CCodeFile;

class CEditorApp
{
	public:
		CEditorApp();
		~CEditorApp();

		int Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow );

		HWND GetHWND() const
			{ ASSERT( IsWindow( m_hWnd ) ); return m_hWnd; }
		HWND GetMDIClient() const
			{ ASSERT( IsWindow( m_hWndClient ) ); return m_hWndClient; }

		HINSTANCE GetHINSTANCE() const
			{ return m_hInst; }

		HIMAGELIST GetImageList() const
			{ return m_hilMargin; }

	UINT CEditorApp::GetProfileInt( LPCTSTR pszSection, LPCTSTR pszEntry, int nDefault ) const
		{ return RegGetProfileInt( COMPANY, APPNAME, pszSection, pszEntry, nDefault ); }
	LPCTSTR CEditorApp::GetProfileString( LPCTSTR pszSection, LPCTSTR pszEntry, LPTSTR pszValue, LPCTSTR pszDefault ) const
		{ return RegGetProfileString( COMPANY, APPNAME, pszSection, pszEntry, pszValue, pszDefault ); }
	BOOL CEditorApp::GetProfileBinary( LPCTSTR pszSection, LPCTSTR pszEntry, BYTE** ppData, UINT* pBytes ) const
		{ return RegGetProfileBinary( COMPANY, APPNAME, pszSection, pszEntry, ppData, pBytes ); }
	BOOL CEditorApp::WriteProfileInt( LPCTSTR pszSection, LPCTSTR pszEntry, int nValue ) const
		{ return RegWriteProfileInt( COMPANY, APPNAME, pszSection, pszEntry, nValue ); }
	BOOL CEditorApp::WriteProfileString( LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue ) const
		{ return RegWriteProfileString( COMPANY, APPNAME, pszSection, pszEntry, pszValue ); }
	BOOL CEditorApp::WriteProfileBinary( LPCTSTR pszSection, LPCTSTR pszEntry, LPBYTE pData, UINT nBytes ) const
		{ return RegWriteProfileBinary( COMPANY, APPNAME, pszSection, pszEntry, pData, nBytes ); }

	private:

		// idle processing flags
		enum { eMENU = 0x1, eFILES = 0x2, eSTATBAR = 0x4 };


	public:

		CCodeFile *CreateMDIChild();
		CCodeFile *GetActiveCode() const
			{ return m_pActiveCode; }
		void SetActiveCode( CCodeFile *pCode )
			{ m_pActiveCode = pCode; m_dwIdleFlags |= eMENU | eSTATBAR; }
		
		BOOL CommandIsValid( UINT nCmd ) const;
		BOOL OnFileSave( CCodeFile *pCode = m_pActiveCode );
		BOOL OnFileSaveAs( CCodeFile *pCode = m_pActiveCode );
		void OnSelChanged( int nLine, int nCol ) const;
		void UpdateOvertypeStatus( BOOL bOvertype );

		void SaveProfile( HWND hWnd ) const;
		void LoadProfile( HWND hWnd );

	private:
		HWND m_hWnd;
		HWND m_hWndClient;
		HWND m_hWndToolBar;
		HWND m_hWndStatusBar;
		HMENU m_hMenuCode;
		HMENU m_hMenuEmpty;
		HFONT m_hFont;
		HINSTANCE m_hInst;
		static CCodeFile *m_pActiveCode;
		DWORD m_dwIdleFlags;
		TCHAR m_pszMRUList[ MAX_MRU_SIZE ][ _MAX_PATH + 1 ];
		int m_nMRUCount;
		HIMAGELIST m_hilMargin;

		void MoveToFrontOfMRUList( LPCTSTR pszPath );
		void DoIdleProcessing();
		void RecalcLayout( int cx, int cy );
		BOOL ActivateIfOpen( LPCTSTR pszPath ) const;

		static long FAR PASCAL WndProc( HWND, UINT, UINT, LONG );

		void OnMRUList( int nFile );
		BOOL OnWindowCloseAll();

		DECLARE_MESSAGE_HANDLER( OnDestroy );
		DECLARE_MESSAGE_HANDLER( OnSize );
		DECLARE_MESSAGE_HANDLER( OnInitMenuPopup );
		DECLARE_MESSAGE_HANDLER( OnClose );
		DECLARE_MESSAGE_HANDLER( OnNotify );
		DECLARE_MESSAGE_HANDLER( OnSysColorChange );

		DECLARE_MENU_HANDLER( OnFileNew );
		DECLARE_MENU_HANDLER( OnFileOpen );
		DECLARE_MENU_HANDLER( OnFileClose );
		DECLARE_MENU_HANDLER( OnFileSaveAll );
		DECLARE_MENU_HANDLER( OnFileExit );
		DECLARE_MENU_HANDLER( OnFilePrint );
		DECLARE_MENU_HANDLER( OnWindowTile );
		DECLARE_MENU_HANDLER( OnWindowCascade );
		DECLARE_MENU_HANDLER( OnWindowArrangeIcons );
		DECLARE_MENU_HANDLER( OnEditCut );
		DECLARE_MENU_HANDLER( OnEditCopy );
		DECLARE_MENU_HANDLER( OnEditPaste );
		DECLARE_MENU_HANDLER( OnEditUndo );
		DECLARE_MENU_HANDLER( OnEditRedo );
		DECLARE_MENU_HANDLER( OnEditProperties );
		DECLARE_MENU_HANDLER( OnEditFind );
		DECLARE_MENU_HANDLER( OnEditReplace );
		DECLARE_MENU_HANDLER( OnEditMacro );
		DECLARE_MENU_HANDLER( OnStepInto );
		DECLARE_MENU_HANDLER( OnStepOver );
		DECLARE_MENU_HANDLER( OnToggleBreakpoint );
};
#endif