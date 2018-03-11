/////////////////////////////////////////////////////////////////////////////
//
// ICodeTipFuncHighlight.h : Declaration of CICodeTipFuncHighlight
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICODETIPFUNCHIGHLIGHT_H__INCLUDED_
#define _ICODETIPFUNCHIGHLIGHT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ICodeTipFuncHighlight.hxx"
#include "CodeTipFuncHighlightCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CICodeTipFuncHighlight

class ATL_NO_VTABLE CICodeTipFuncHighlight :
	public CICodeTipFuncHighlightBase< CICodeTipFuncHighlight,
		CCodeTipFuncHighlightCtrl, ICodeTipFuncHighlight,
		&CLSID_CodeTipFuncHighlight >
{

// CICodeTipFuncHighlight Protected Member Functions ---------------------
protected:

	// Construction
	CICodeTipFuncHighlight();

	// Destruction
	virtual ~CICodeTipFuncHighlight();
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _ICODETIPFUNCHIGHLIGHT_H__INCLUDED_
