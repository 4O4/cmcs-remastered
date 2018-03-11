/////////////////////////////////////////////////////////////////////////////
//
// CodeTipMultiFuncCtrl.h : Declaration of CCodeTipMultiFuncCtrl
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CODETIPMULTIFUNCCTRL_H__INCLUDED_
#define _CODETIPMULTIFUNCCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CodeTipFuncHighlightCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeTipMultiFuncCtrl

class CCodeTipMultiFuncCtrl : public CCodeTipFuncHighlightCtrl
{
	friend class CCodeTipCtrl;

// Protected Member Variables -----------------------------------------------
protected:
	UINT m_nFuncCount;
	UINT m_nCurrFunc;
	HICON m_hiLeft;
	HICON m_hiRight;
	TCHAR m_szFmt[50];
	RECT m_rLeft;
	RECT m_rRight;


// Public Member Functions --------------------------------------------------
public:

	// Destruction
	virtual ~CCodeTipMultiFuncCtrl();

	// Overrides
	virtual LPDISPATCH GetDispatch();

	// Properties
	virtual UINT GetFuncCount() {
		return m_nFuncCount;
	}

	virtual void SetFuncCount( UINT nCount ) {
		m_nFuncCount = nCount;
	}

	virtual UINT GetCurrFunc() {
		return m_nCurrFunc;
	}

	virtual void SetCurrFunc( UINT nFunc ) {
		m_nCurrFunc = nFunc;
	}


// Protected Member Functions -----------------------------------------------
protected:

	// Construction
	CCodeTipMultiFuncCtrl( CEdit* pEdit );

	// Overrides
	virtual void NotifyUpdate();
	virtual LRESULT NotifyParent( UINT unNotification, LPNMHDR lpNMHDR = NULL );
	virtual void DrawTipText( HDC hdc, RECT& rect, BOOL bCalcRect );
	virtual BOOL OnLButtonDown( WPARAM wp, LPARAM lp, LRESULT* pResult );

	virtual void OnLeftButton();
	virtual void OnRightButton();
};



/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _CODETIPMULTIFUNCCTRL_H__INCLUDED_
