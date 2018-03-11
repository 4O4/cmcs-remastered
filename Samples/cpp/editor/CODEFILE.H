///////////////////////////////////////////////////////////////////
//
//  CodeMax sample app : CodeMax Editor
//
//  (c) WinMain Software 1998-2000
//
//

#ifndef __CODEFILE_H__
#define __CODEFILE_H__

#define CODEFILEWNDCLASS _T("CodeFile")

// CCodeFile represents one document window's contents.
class CCodeFile  
{
	public:
		CCodeFile( CEditorApp *pApp );
		virtual ~CCodeFile();

		HWND GetHWND() const
			{ ASSERT( IsWindow( m_hWnd ) ); return m_hWnd; }

		HWND GetEditHWND() const
			{ ASSERT( IsWindow( m_hWndEdit ) ); return m_hWndEdit; }

		static BOOL RegisterClass( HINSTANCE hInstance );
		static BOOL UnregisterClass( HINSTANCE hInstance );

		void Open( LPCTSTR pszPath );
		BOOL Save();
		LPCTSTR GetPath() const { return m_szPath; }
		void SetPath( LPCTSTR pszPath );
		void UpdateWindowTitle();
		LPCTSTR GetTitle( LPTSTR pszBuff ) const;
		BOOL IsReadOnly() const
			{ return SendMessage( m_hWndEdit, CMM_ISREADONLY, 0, 0 ); }
		void UpdateReadOnlyState();
		BOOL IsCodeUpToDate( BOOL bStale = TRUE ) const;
		void UpdateFileTime();
		BOOL CanDoIdleProcessing() const
			{ return m_bDoIdleProcessing; }

	private:
		HWND m_hWnd;     // The MDI child frame
		HWND m_hWndEdit; // The CodeMax editor
		BOOL m_bDoIdleProcessing;
		TCHAR m_szPath[ _MAX_PATH + 1 ];  // the file path
		FILETIME m_timeFileSaved;  // the file time -- used for refreshing window contents
		CEditorApp *m_pApp;  // the owner app

		HIMAGELIST m_hImages; // Image list for use with CodeList control
		TCHAR m_szIniFile[MAX_PATH];// Path to functions.ini file
		CM_POSITION m_posSel;
		TCHAR m_szTipFunc[100];
		HWND m_hListCtrl;

		void Initialize( HWND hWnd );
		void SetLanguageBasedOnFileType();
		void GetFileTimeFromDisk( FILETIME &fileTime ) const;

		static long FAR PASCAL WndProc( HWND, UINT, UINT, LONG );

		static LRESULT OnCreate( HWND, WPARAM, LPARAM );
		DECLARE_MESSAGE_HANDLER( OnNCDestroy );
		DECLARE_MESSAGE_HANDLER( OnSize );
		DECLARE_MESSAGE_HANDLER( OnSetFocus );
		DECLARE_MESSAGE_HANDLER( OnMDIActivate );
		DECLARE_MESSAGE_HANDLER( OnClose );
		DECLARE_MESSAGE_HANDLER( OnEraseBkgnd );

		DECLARE_NOTIFY_HANDLER( OnModifiedChanged );
		DECLARE_NOTIFY_HANDLER( OnPropsChanged );
		DECLARE_NOTIFY_HANDLER( OnOvertypeChange );
		DECLARE_NOTIFY_HANDLER_PARAM( OnDrawLine, CM_DRAWLINEDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnDeleteLine, CM_DELETELINEDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnCmdFailure, CM_CMDFAILUREDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnRegisteredCmd, CM_REGISTEREDCMDDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnKeyDown, CM_KEYDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnKeyUp, CM_KEYDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnKeyPress, CM_KEYDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnMouseDown, CM_MOUSEDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnMouseUp, CM_MOUSEDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnMouseMove, CM_MOUSEDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnCodeList, CM_CODELISTDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnCodeListSelMade, CM_CODELISTDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnCodeListCancel, CM_CODELISTDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnCodeTip, CM_CODETIPDATA );
		DECLARE_NOTIFY_HANDLER_PARAM( OnCodeTipUpdate, CM_CODETIPDATA );

		BOOL GetString( int iSection, int iIndex, LPTSTR pszFunc );
		BOOL GetCurrentFunction( LPTSTR pszName, bool bMustExist = true );
		BOOL GetPrototype( LPCTSTR pszFunc, LPTSTR pszProto, int iOverload = 0 );
		int GetCurrentArgument();
		void SelectClosestItemInCodeList();
		void SelectCurrentFunction();

	public:
		DECLARE_NOTIFY_HANDLER( OnSelChanged );

};

#endif
