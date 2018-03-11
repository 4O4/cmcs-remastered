/////////////////////////////////////////////////////////////////////////////
//
// Wnd.cpp : Implementation of CWnd
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"
#include "Wnd.h"


CRegClassList CWnd::m_lClasses;

//***************************************************************************
// Public Member Functions
//***************************************************************************

// Construction -------------------------------------------------------------
CWnd::CWnd()
: m_hWnd( NULL )
, m_OldWndProc( NULL )
{
	ZeroMemory( &m_msgLast, sizeof(m_msgLast) );
}

// Destruction --------------------------------------------------------------
CWnd::~CWnd()
{
	if( m_hWnd )
	{
		CMTRACE0( "~CWnd() calling DestroyWindow()\n" );
		DestroyWindow();
	}
}

// Create -------------------------------------------------------------------
BOOL CWnd::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
				  DWORD dwStyle, LPCRECT lpRect, HWND hwndParent, UINT nID )
{
	ASSERT( NULL == m_hWnd );

	int x = CW_USEDEFAULT, y = 0, w = CW_USEDEFAULT, h = 0;

	if( lpRect )
	{
		CRect rect( lpRect );
		x = rect.left;
		y = rect.top;
		w = rect.Width();
		h = rect.Height();
	}

	extern HINSTANCE hInstance;
	HWND hWnd = ::CreateWindow( lpszClassName, lpszWindowName, dwStyle, x, y,
		w, h, hwndParent, (HMENU)nID, hInstance, NULL );

	if( NULL == hWnd )
	{
		CMTRACE0("::CreateWindow() failed in CWnd::Create()\n");
		return FALSE;
	}

	return SubclassWindow( hWnd );
}

// ScreenToClient -----------------------------------------------------------
void CWnd::ScreenToClient( LPRECT lpRect ) const
{
	ASSERT( ::IsWindow( m_hWnd ) );

	::ScreenToClient( m_hWnd, (LPPOINT)lpRect );
	::ScreenToClient( m_hWnd, ((LPPOINT)lpRect) + 1 );

//	if( GetExStyle() & WS_EX_LAYOUTRTL )
//		CRect::SwapLeftRight( lpRect );
}

// ClientToScreen -----------------------------------------------------------
void CWnd::ClientToScreen(LPRECT lpRect) const
{
	ASSERT( ::IsWindow( m_hWnd ) );

	::ClientToScreen( m_hWnd, (LPPOINT)lpRect );
	::ClientToScreen( m_hWnd, ((LPPOINT)lpRect) + 1 );

//	if( GetExStyle() & WS_EX_LAYOUTRTL )
//		CRect::SwapLeftRight( lpRect );
}

// DestroyWindow ------------------------------------------------------------
BOOL CWnd::DestroyWindow()
{
	ASSERT( ::IsWindow( m_hWnd ) );
	return ::DestroyWindow( m_hWnd );
}

// FromHandle ---------------------------------------------------------------
CWnd* CWnd::FromHandle( HWND hWnd )
{
	return (CWnd*)::GetWindowLong( hWnd, GWL_USERDATA );
}

// RegisterClass ------------------------------------------------------------
LPCTSTR CWnd::RegisterClass( UINT nClassStyle, HCURSOR hCursor,
							HBRUSH hbrBackground, HICON hIcon )
{
	WNDCLASS wc = {0};
	extern HINSTANCE hInstance;
	static TCHAR szClass[MAX_PATH] = {0};

	wsprintf( szClass, _T("CodeMax:%x:%x:%x:%x:%x"), (UINT)hInstance,
		nClassStyle, (UINT)hCursor, (UINT)hbrBackground, (UINT)hIcon );

	if( ::GetClassInfo( hInstance, szClass, &wc ) )
	{
		ASSERT( wc.style == nClassStyle );
		return szClass;// Class already registered
	}

	wc.style = nClassStyle;
	wc.lpfnWndProc = CWnd::GlobalWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = hIcon;
	wc.hCursor = hCursor;
	wc.hbrBackground = hbrBackground;
	wc.lpszClassName = szClass;

	if( !RegisterClassEx( &wc ) )
		*szClass = L'\0';// Failed to register class

	return szClass;
}

// RegisterClass ------------------------------------------------------------
BOOL CWnd::RegisterClassEx( const WNDCLASS* lpWndClass )
{
	return m_lClasses.RegisterClass( lpWndClass );
}


//***************************************************************************
// Protected Member Functions
//***************************************************************************

// SubclassWindow -----------------------------------------------------------
BOOL CWnd::SubclassWindow( HWND hWnd )
{
	ASSERT( NULL == m_hWnd );

	m_hWnd = hWnd;
	::SetWindowLong( hWnd, GWL_USERDATA, (LONG)this );

	m_OldWndProc = (WNDPROC)::SetWindowLong( hWnd, GWL_WNDPROC,
		(LONG)WndProc );

	return ( m_OldWndProc != NULL );
}

// UnsubclassWindow ---------------------------------------------------------
HWND CWnd::UnsubclassWindow()
{
	HWND hWnd = m_hWnd;

	if( m_hWnd )
	{
		if( m_OldWndProc )
			::SetWindowLong( m_hWnd, GWL_WNDPROC, (LONG)m_OldWndProc );
	}

	m_hWnd = NULL;
	m_OldWndProc = NULL;

	return hWnd;
}

// WindowProc ---------------------------------------------------------------
LRESULT CWnd::WindowProc( UINT msg, WPARAM wp, LPARAM lp )
{
	switch( msg )
	{
	case WM_DESTROY:
		OnDestroy();
		return 0;
	}

	return Default();
}

// Default ------------------------------------------------------------------
LRESULT CWnd::Default()
{
	ASSERT( ::IsWindow( m_hWnd ) );

	return ::CallWindowProc( m_OldWndProc, m_hWnd, m_msgLast.message,
		m_msgLast.wParam, m_msgLast.lParam );
}

// OnDestroy ----------------------------------------------------------------
void CWnd::OnDestroy()
{
	UnsubclassWindow();
}

// WndProc ------------------------------------------------------------------
LRESULT CALLBACK CWnd::WndProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
{
	CWnd* pWnd = CWnd::FromHandle( hWnd );
	ASSERT( pWnd );

	// Save last message for use with CWnd::Default()
	pWnd->m_msgLast.hwnd = hWnd;
	pWnd->m_msgLast.message = msg;
	pWnd->m_msgLast.wParam = wp;
	pWnd->m_msgLast.lParam = lp;

	return pWnd->WindowProc( msg, wp, lp );
}

// GlobalWndProc ------------------------------------------------------------
LRESULT CALLBACK CWnd::GlobalWndProc( HWND hwnd, UINT msg, WPARAM wp,
									 LPARAM lp )
{
	return ::DefWindowProc( hwnd, msg, wp, lp );
}
