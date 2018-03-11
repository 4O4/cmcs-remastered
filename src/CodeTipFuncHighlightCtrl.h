/////////////////////////////////////////////////////////////////////////////
//
// CodeTipFuncHighlightCtrl.h : Declaration of CCodeTipFuncHighlightCtrl
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CODETIPFUNCHIGHLIGHTCTRL_H__INCLUDED_
#define _CODETIPFUNCHIGHLIGHTCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CodeTipHighlightCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeTipFuncHighlightCtrl

class CCodeTipFuncHighlightCtrl : public CCodeTipHighlightCtrl
{
	friend class CCodeTipCtrl;

// Protected Member Variables -----------------------------------------------
protected:
	UINT m_nArgument;


// Public Member Functions --------------------------------------------------
public:

	// Destruction
	virtual ~CCodeTipFuncHighlightCtrl();

	// Overrides
	virtual LPDISPATCH GetDispatch();

	// Properties
	virtual UINT GetArgument() {
		return m_nArgument;
	}

	virtual void SetArgument( UINT nArg ) {
		m_nArgument = nArg;
	}


// Protected Member Functions -----------------------------------------------
protected:

	// Construction
	CCodeTipFuncHighlightCtrl( CEdit* pEdit );

	// Overrides
	virtual void NotifyUpdate();
	virtual LRESULT NotifyParent( UINT unNotification, LPNMHDR lpNMHDR = NULL );
	virtual void DrawTipText( HDC hdc, RECT& rect, BOOL bCalcRect );
	virtual void GetSelection();
};



/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _CODETIPFUNCHIGHLIGHTCTRL_H__INCLUDED_
