/////////////////////////////////////////////////////////////////////////////
//
// CodeTipCtrl.h : Declaration of CCodeTipCtrl
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CODETIPCTRL_H__INCLUDED_
#define _CODETIPCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DispatchWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeTipCtrl

class CCodeTipCtrl : public CDispatchWnd
{

// Protected Member Variables -----------------------------------------------
protected:
	HFONT m_hfText;
	UINT m_nTipType;
	LPTSTR m_lpszText;


// Public Member Functions --------------------------------------------------
public:

	// Destruction
	virtual ~CCodeTipCtrl();

	// Creation
	static CCodeTipCtrl* Create( CEdit* pEdit, UINT nType, POINT& pt );

	virtual void AutoSizeControl();

	virtual LPCTSTR GetTipText();
	virtual BOOL NotifyCancel();

	virtual BOOL DestroyCodeTip() {
		return m_pEdit->DestroyCodeTip();
	}

	// Overrides
	virtual LPDISPATCH GetDispatch();


// Protected Member Functions -----------------------------------------------
protected:

	// Construction
	CCodeTipCtrl( CEdit* pEdit );

	// Creation / Initialization
	virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, LPCRECT lpRect, HWND hwndParent, UINT nID = 0 );

	virtual void InitControl();

	virtual void NotifyUpdate();

	// Overrides
	virtual void OnDestroy();
	virtual LRESULT NotifyParent( UINT unNotification, LPNMHDR lpNMHDR = NULL );

	// Drawing
	virtual void DrawTipText( HDC hdc, RECT& rect, BOOL bCalcRect );

	// Message handlers
	DECLARE_MSG_HANDLER()
		MSG_HANDLER( OnChar )
		MSG_HANDLER( OnPaint )
		MSG_HANDLER( OnEraseBkgnd )
		MSG_HANDLER( OnLButtonDown )
		MSG_HANDLER( OnRButtonDown )
		MSG_HANDLER( OnMButtonDown )
		MSG_HANDLER( OnSetFont )
		MSG_HANDLER( OnGetFont )
};



/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _CODELISTCTRL_H__INCLUDED_
