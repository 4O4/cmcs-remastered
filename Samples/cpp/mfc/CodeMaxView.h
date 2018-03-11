/////////////////////////////////////////////////////////////////////////////
//
//  CodeMax Code Editor Control
//
//  Copyright © 2000  WinMain Software
//
//  This file declares an MFC view class that wraps the CCodeMaxCtrl.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CODEMAXVIEW_H__INCLUDED_
#define _CODEMAXVIEW_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cmaxafx.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeMaxView

class CCodeMaxView : public CCtrlView
{
	DECLARE_DYNCREATE( CCodeMaxView )

protected:
	// Construction
	CCodeMaxView();

public:
	// Attributes
	CCodeMaxCtrl& GetCodeMaxCtrl() const;

protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeMaxView)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CCodeMaxView)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// CCodeMaxView Inline Functions

inline CCodeMaxView::CCodeMaxView() : CCtrlView( CODEMAXWNDCLASS,
												AFX_WS_DEFAULT_VIEW)
{ }

inline CCodeMaxCtrl& CCodeMaxView::GetCodeMaxCtrl() const
{ return *(CCodeMaxCtrl*)this; }



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif//#ifndef _CODEMAXVIEW_H__INCLUDED_

