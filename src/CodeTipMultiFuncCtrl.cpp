/////////////////////////////////////////////////////////////////////////////
//
// CodeTipMultiFuncCtrl.cpp : Implementation of CCodeTipMultiFuncCtrl
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"
#include <commctrl.h>
#include "CodeTipMultiFuncCtrl.h"
#include "resource.h"

#ifdef _ACTIVEX
#include "editx.h"
#include "ICodeTipMultiFunc.h"
#endif//#ifdef _ACTIVEX


#define CX_BTN_ICON		13 // Width, in pixels, of icons used for buttons
#define CY_BTN_ICON		13 // Height, in pixels, of icons used for buttons


//***************************************************************************
// Public Member Functions
//***************************************************************************

// Destruction --------------------------------------------------------------
CCodeTipMultiFuncCtrl::~CCodeTipMultiFuncCtrl()
{
}

// GetDispatch --------------------------------------------------------------
LPDISPATCH CCodeTipMultiFuncCtrl::GetDispatch()
{
#ifdef _ACTIVEX

	if( NULL == m_lpDispatch )
	{
		CComObject<CICodeTipMultiFunc>* pNew = NULL;
		HRESULT hR = pNew->CreateInstance( &pNew );

		if( SUCCEEDED( hR ) )
		{
			pNew->AddRef();
			pNew->m_pCtrl = this;
			m_lpDispatch = pNew;
		}
	}

#endif//#ifdef _ACTIVEX

	return m_lpDispatch;
}


//***************************************************************************
// Protected Member Functions
//***************************************************************************

// Construction -------------------------------------------------------------
CCodeTipMultiFuncCtrl::CCodeTipMultiFuncCtrl( CEdit* pEdit )
: CCodeTipFuncHighlightCtrl( pEdit )
, m_nFuncCount( 0 )
, m_nCurrFunc( 0 )
{
	m_nTipType = CM_TIPSTYLE_MULTIFUNC;

	// Load the button icons
	extern HINSTANCE hInstance;

	m_hiLeft = (HICON)LoadImage( hInstance, MAKEINTRESOURCE( IDI_BTN_LEFT ),
		IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );

	m_hiRight = (HICON)LoadImage( hInstance,
		MAKEINTRESOURCE( IDI_BTN_RIGHT ), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );

	ASSERT( NULL != m_hiLeft );
	ASSERT( NULL != m_hiRight );

	// Load the format string ("%d of %d")
	LoadString( hInstance, IDS_FMT_CODETIP_MULTIFUNC, m_szFmt,
		sizeof(m_szFmt) / sizeof(TCHAR) );
}

// NotifyUpdate -------------------------------------------------------------
void CCodeTipMultiFuncCtrl::NotifyUpdate()
{
	CM_CODETIPMULTIFUNCDATA hdr = {0};

	if( NotifyParent( CMN_CODETIPUPDATE, (LPNMHDR)&hdr ) )
	{
		m_nArgument = hdr.ctfData.nArgument;
		m_nFuncCount = hdr.nFuncCount;
		m_nCurrFunc = hdr.nCurrFunc;
	}
}

// NotifyParent -------------------------------------------------------------
LRESULT CCodeTipMultiFuncCtrl::NotifyParent( UINT unNotification,
											LPNMHDR lpNMHDR )
{
	CM_CODETIPMULTIFUNCDATA hdr = {0};

	if( NULL == lpNMHDR )
		lpNMHDR = (LPNMHDR)&hdr;

	CM_CODETIPMULTIFUNCDATA* pctmfData = (CM_CODETIPMULTIFUNCDATA*)lpNMHDR;

	pctmfData->nFuncCount = m_nFuncCount;
	pctmfData->nCurrFunc = m_nCurrFunc;

	return CCodeTipFuncHighlightCtrl::NotifyParent( unNotification, lpNMHDR );
}

// DrawTipText --------------------------------------------------------------
void CCodeTipMultiFuncCtrl::DrawTipText( HDC hdc, RECT& rect, BOOL bCalcRect )
{
	int x = rect.left;

	// Only display the buttons if the function is overloaded
	if( m_nFuncCount )
	{
		// Validate current function index
		if( m_nCurrFunc > m_nFuncCount )
			m_nCurrFunc = m_nFuncCount;

		// Determine how big the text between the buttons will be
		SIZE size = {0};
		TCHAR szText[50] = {0};
		HFONT hfOld = (HFONT)::SelectObject( hdc, m_hfText );

		wsprintf( szText, m_szFmt, m_nCurrFunc + 1, m_nFuncCount + 1 );

		int len = lstrlen( szText );
		VERIFY( GetTextExtentPoint32( hdc, szText, len, &size ) );

		// Center the button icons vertically on the line
		int yIcon = ( size.cy / 2 ) - ( CY_BTN_ICON / 2 ) + 1;

		if( !bCalcRect )
		{
			// Draw the left button
			VERIFY( DrawIconEx( hdc, x, yIcon, m_hiLeft, 0, 0, 0, NULL,
				DI_NORMAL ) );

			// Save the position of the button for hit-testing later
			m_rLeft.top = yIcon;
			m_rLeft.bottom = yIcon + CY_BTN_ICON;
			m_rLeft.left = x;
			m_rLeft.right = x + CX_BTN_ICON;
		}

		x += CX_BTN_ICON;

		// Draw the text between the buttons
		if( !bCalcRect )
			TextOut( hdc, x, rect.top + 1, szText, len );

		x += size.cx;

		if( !bCalcRect )
		{
			// Draw the right button
			VERIFY( DrawIconEx( hdc, x, yIcon, m_hiRight, 0, 0, 0, NULL,
				DI_NORMAL ) );

			// Save the position of the button for hit-testing later
			m_rRight.top = yIcon;
			m_rRight.bottom = yIcon + CY_BTN_ICON;
			m_rRight.left = x;
			m_rRight.right = x + CX_BTN_ICON;
		}

		x += CX_BTN_ICON;

		// Add some padding between the buttons and the actual tooltip text
		VERIFY( GetTextExtentPoint32( hdc, _T("  "), 2, &size ) );
		x += size.cx;

		::SelectObject( hdc, hfOld );
	}

	// Get the text selection range based on the current argument index
	GetSelection();

	// Now draw the tip with the appropriate selection range
	CCodeTipHighlightCtrl::DrawTipTextEx( hdc, rect, bCalcRect,
		x - rect.left );
}

// OnLButtonDown ------------------------------------------------------------
BOOL CCodeTipMultiFuncCtrl::OnLButtonDown( WPARAM wp, LPARAM lp,
										  LRESULT* pResult )
{
	if( m_nFuncCount )
	{
		// See if one of the buttons was clicked
		POINT pt = {0};
		pt.x = LOWORD( lp );
		pt.y = HIWORD( lp );

		if( PtInRect( &m_rLeft, pt ) )
			OnLeftButton();
		else if( PtInRect( &m_rRight, pt ) )
			OnRightButton();
	}

	return CCodeTipFuncHighlightCtrl::OnLButtonDown( wp, lp, pResult );
}

// OnLeftButton -------------------------------------------------------------
void CCodeTipMultiFuncCtrl::OnLeftButton()
{
	// Decrement the current function index, wrapping if necessary
	if( 0 == m_nCurrFunc )
		m_nCurrFunc = m_nFuncCount;
	else
		m_nCurrFunc--;

	// Notify parent of change
	AutoSizeControl();
}

// OnRightButton ------------------------------------------------------------
void CCodeTipMultiFuncCtrl::OnRightButton()
{
	// Increment the current function index, wrapping if necessary
	if( m_nCurrFunc >= m_nFuncCount )
		m_nCurrFunc = 0;
	else
		m_nCurrFunc++;

	// Notify parent of change
	AutoSizeControl();
}
