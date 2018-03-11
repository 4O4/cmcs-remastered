/////////////////////////////////////////////////////////////////////////////
//
// CodeTipHighlightCtrl.h : Declaration of CCodeTipHighlightCtrl
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CODETIPHIGHLIGHTCTRL_H__INCLUDED_
#define _CODETIPHIGHLIGHTCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CodeTipCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeTipHighlightCtrl

class CCodeTipHighlightCtrl : public CCodeTipCtrl
{
	friend class CCodeTipCtrl;

// Protected Member Variables -----------------------------------------------
protected:
	HFONT m_hfBold;
	int m_iHighlightBegin, m_iHighlightEnd;


// Public Member Functions --------------------------------------------------
public:

	// Destruction
	virtual ~CCodeTipHighlightCtrl();

	// Overrides
	virtual LPDISPATCH GetDispatch();

	// Properties
	virtual void SetStartPos( int iPos );
	virtual int GetStartPos();
	virtual void SetEndPos( int iPos );
	virtual int GetEndPos();


// Protected Member Functions -----------------------------------------------
protected:

	// Construction
	CCodeTipHighlightCtrl( CEdit* pEdit );

	// Overrides
	virtual void NotifyUpdate();
	virtual LRESULT NotifyParent( UINT unNotification, LPNMHDR lpNMHDR = NULL );
	virtual void DrawTipText( HDC hdc, RECT& rect, BOOL bCalcRect );
	virtual void DrawTipTextEx( HDC hdc, RECT& rect, BOOL bCalcRect, int xIndent );
	virtual BOOL OnSetFont( WPARAM wp, LPARAM lp, LRESULT* pResult );

	virtual void GetSelection();

	virtual bool GetNextWord( LPCTSTR lpszBuffer, int iBufferLen,
		int& iBegin, int& iEnd );

	virtual void CalcWordSize( HDC hdc, int iBegin, int iWordLen,
		SIZE& sWord );

	virtual void DrawWord( HDC hdc, int iWordBegin, int iWordLen, int& x,
		int y );
};



/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _CODETIPHIGHLIGHTCTRL_H__INCLUDED_
