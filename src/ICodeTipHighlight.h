/////////////////////////////////////////////////////////////////////////////
//
// ICodeTipHighlight.h : Declaration of CICodeTipHighlight
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICODETIPHIGHLIGHT_H__INCLUDED_
#define _ICODETIPHIGHLIGHT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ICodeTip.hxx"
#include "CodeTipHighlightCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CICodeTipHighlight

class ATL_NO_VTABLE CICodeTipHighlight :
	public CICodeTipBase< CICodeTipHighlight, CCodeTipHighlightCtrl,
		ICodeTipHighlight, &CLSID_CodeTipHighlight >
{

// CICodeTipHighlight Protected Member Functions -------------------------
protected:

	// Construction
	CICodeTipHighlight();

	// Destruction
	virtual ~CICodeTipHighlight();


// ICodeTipHighlight Public Member Functions -----------------------------
public:
	STDMETHOD(get_HighlightStartPos)( long* plPos );
	STDMETHOD(put_HighlightStartPos)( long lPos );
	STDMETHOD(get_HighlightEndPos)( long* plPos );
	STDMETHOD(put_HighlightEndPos)( long lPos );
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _ICODETIPHIGHLIGHT_H__INCLUDED_
