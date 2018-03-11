
/////////////////////////////////////////////////////////////////////////////
//
// Wnd.inl : Inline functions for CWnd class
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////


inline HWND CWnd::GetSafeHwnd() {
	return ( NULL == this ? NULL : m_hWnd );
}

inline LRESULT CWnd::SendMessage( UINT msg, WPARAM wp, LPARAM lp ) {
	ASSERT( ::IsWindow( m_hWnd ) ); return ::SendMessage( m_hWnd, msg, wp, lp );
}

inline BOOL CWnd::ShowWindow( int nCmdShow ) {
	ASSERT( ::IsWindow( m_hWnd ) ); return ::ShowWindow( m_hWnd, nCmdShow );
}

inline void CWnd::UpdateWindow() {
	ASSERT( ::IsWindow( m_hWnd ) ); VERIFY( ::UpdateWindow( m_hWnd ) );
}

inline void CWnd::GetWindowRect( LPRECT lpRect ) const {
	ASSERT( ::IsWindow( m_hWnd ) ); VERIFY( ::GetWindowRect( m_hWnd, lpRect ) );
}

inline void CWnd::GetClientRect( LPRECT lpRect ) const {
	ASSERT( ::IsWindow( m_hWnd ) ); VERIFY( ::GetClientRect( m_hWnd, lpRect ) );
}

inline HDC CWnd::GetDC() const {
	ASSERT( ::IsWindow( m_hWnd ) ); return ::GetDC( m_hWnd );
}

inline int CWnd::ReleaseDC( HDC hdc ) {
	ASSERT( ::IsWindow( m_hWnd ) ); return ::ReleaseDC( m_hWnd, hdc );
}

inline HDC CWnd::BeginPaint( LPPAINTSTRUCT lpPaint ) {
	ASSERT( ::IsWindow( m_hWnd ) ); return ::BeginPaint( m_hWnd, lpPaint );
}

inline void CWnd::EndPaint( LPPAINTSTRUCT lpPaint ) {
	ASSERT( ::IsWindow( m_hWnd ) ); VERIFY( ::EndPaint( m_hWnd, lpPaint ) );
}

inline void CWnd::MoveWindow( LPCRECT lpRect, BOOL bRepaint )
{
	CRect rect( lpRect );
	MoveWindow( rect.left, rect.top, rect.Width(), rect.Height(), bRepaint );
}

inline void CWnd::MoveWindow( int x, int y, int nWidth, int nHeight,
							 BOOL bRepaint )
{
	ASSERT( ::IsWindow( m_hWnd ) );
	VERIFY( ::MoveWindow( m_hWnd, x, y, nWidth, nHeight, bRepaint ) );
}

inline int CWnd::GetWindowTextLength() const
{
	ASSERT( ::IsWindow( m_hWnd ) );
	return ::SendMessage( m_hWnd, WM_GETTEXTLENGTH, 0, 0 );
}

inline int CWnd::GetWindowText( LPTSTR lpszStringBuf, int nMaxCount ) const
{
	ASSERT( ::IsWindow( m_hWnd ) );
	return ::GetWindowText( m_hWnd, lpszStringBuf, nMaxCount );
}

inline void CWnd::InvalidateRect( LPCRECT lpRect, BOOL bErase )
{
	ASSERT( ::IsWindow( m_hWnd ) );
	VERIFY( ::InvalidateRect( m_hWnd, lpRect, bErase ) );
}

inline void CWnd::SetWindowText( LPCTSTR lpszText ) {
	ASSERT( ::IsWindow( m_hWnd ) ); VERIFY( ::SetWindowText( m_hWnd, lpszText ) );
}

inline LONG CWnd::GetWindowLong( int nIndex ) const {
	ASSERT( ::IsWindow( m_hWnd ) ); return ::GetWindowLong( m_hWnd, nIndex );
}

inline LONG CWnd::SetWindowLong( int nIndex, LONG lNewVal ) const {
	ASSERT( ::IsWindow( m_hWnd ) ); return ::SetWindowLong( m_hWnd, nIndex, lNewVal );
}

inline void CWnd::ClientToScreen( LPPOINT lpPoint ) const {
	ASSERT( ::IsWindow( m_hWnd ) ); ::ClientToScreen( m_hWnd, lpPoint );
}

inline void CWnd::ScreenToClient( LPPOINT lpPoint ) const {
	ASSERT( ::IsWindow( m_hWnd ) ); ::ScreenToClient( m_hWnd, lpPoint );
}
