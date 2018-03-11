/////////////////////////////////////////////////////////////////////////////
//
// CodeTipFuncHighlightCtrl.cpp : Implementation of CCodeTipFuncHighlightCtrl
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"
#include <commctrl.h>
#include "CodeTipFuncHighlightCtrl.h"


//***************************************************************************
// Public Member Functions
//***************************************************************************

// Destruction --------------------------------------------------------------
CCodeTipFuncHighlightCtrl::~CCodeTipFuncHighlightCtrl()
{
}

// GetDispatch --------------------------------------------------------------
LPDISPATCH CCodeTipFuncHighlightCtrl::GetDispatch()
{
	return m_lpDispatch;
}


//***************************************************************************
// Protected Member Functions
//***************************************************************************

// Construction -------------------------------------------------------------
CCodeTipFuncHighlightCtrl::CCodeTipFuncHighlightCtrl( CEdit* pEdit )
: CCodeTipHighlightCtrl( pEdit )
, m_nArgument( -1 )
{
	m_nTipType = CM_TIPSTYLE_FUNCHIGHLIGHT;
}

// NotifyUpdate -------------------------------------------------------------
void CCodeTipFuncHighlightCtrl::NotifyUpdate()
{
	CM_CODETIPFUNCHIGHLIGHTDATA hdr = {0};

	if( NotifyParent( CMN_CODETIPUPDATE, (LPNMHDR)&hdr ) )
		m_nArgument = hdr.nArgument;
}

// NotifyParent -------------------------------------------------------------
LRESULT CCodeTipFuncHighlightCtrl::NotifyParent( UINT unNotification,
												LPNMHDR lpNMHDR )
{
	CM_CODETIPFUNCHIGHLIGHTDATA hdr = {0};

	if( NULL == lpNMHDR )
		lpNMHDR = (LPNMHDR)&hdr;

	CM_CODETIPFUNCHIGHLIGHTDATA* pitfhData =
		(CM_CODETIPFUNCHIGHLIGHTDATA*)lpNMHDR;

	pitfhData->nArgument = m_nArgument;

	// Don't call CCodeTipHighlightCtrl::NotifyParent()!!!
	return CCodeTipCtrl::NotifyParent( unNotification, lpNMHDR );
}

// DrawTipText --------------------------------------------------------------
void CCodeTipFuncHighlightCtrl::DrawTipText( HDC hdc, RECT& rect,
											BOOL bCalcRect )
{
	// Get the text selection range based on the current argument index
	GetSelection();

	// Now draw the tip with the appropriate selection range
	CCodeTipHighlightCtrl::DrawTipText( hdc, rect, bCalcRect );
}

// GetSelection -------------------------------------------------------------
void CCodeTipFuncHighlightCtrl::GetSelection()
{
	LPCTSTR lpszTip = GetTipText();
	int iTipLen = lstrlen( lpszTip ), iBegin = 0;

	m_iHighlightBegin = -1;
	m_iHighlightEnd = -1;

	int i; // FIXME: There is probably some better way to write this code
	UINT iArg; // FIXME: There is probably some better way to write this code
	for( i = 0, iArg = -1; -1 == iArg && i < iTipLen; i++ )
	{
		switch( lpszTip[i] )
		{
		case L'(':
			iBegin = ++i;
		case L',':
			iArg = 0;
			break;
		}
	}

	if( 0 == iArg )
	{
		for( int i = iBegin; iArg != m_nArgument && i < iTipLen; i++ )
		{
			if( L',' == lpszTip[i] )
			{
				iArg++;
				iBegin = ++i;
			}
		}
	}

	if( iArg == m_nArgument )
	{
		int iEnd; // FIXME: There is probably some better way to write this code
		for( iEnd = iBegin; iEnd < iTipLen; iEnd++ )
		{
			if( L',' == lpszTip[iEnd] || L')' == lpszTip[iEnd] )
			{
				iEnd--;
				break;
			}
		}

		m_iHighlightBegin = iBegin;
		m_iHighlightEnd = iEnd;
	}
}
