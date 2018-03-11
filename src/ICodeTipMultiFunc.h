/////////////////////////////////////////////////////////////////////////////
//
// ICodeTipMultiFunc.h : Declaration of CICodeTipMultiFunc
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICODETIPMULTIFUNC_H__INCLUDED_
#define _ICODETIPMULTIFUNC_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ICodeTipFuncHighlight.hxx"
#include "CodeTipMultiFuncCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CICodeTipMultiFunc

class ATL_NO_VTABLE CICodeTipMultiFunc :
	public CICodeTipFuncHighlightBase< CICodeTipMultiFunc,
		CCodeTipMultiFuncCtrl, ICodeTipMultiFunc, &CLSID_CodeTipMultiFunc >
{

// CICodeTipMultiFunc Protected Member Functions ---------------------
protected:

	// Construction
	CICodeTipMultiFunc();

	// Destruction
	virtual ~CICodeTipMultiFunc();


// ICodeTipMultiFunc Public Member Functions -------------------------
public:

	STDMETHOD(get_FunctionCount)( long* lCount );
	STDMETHOD(put_FunctionCount)( long lCount );
	STDMETHOD(get_CurrentFunction)( long* lIndex );
	STDMETHOD(put_CurrentFunction)( long lIndex );
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _ICODETIPMULTIFUNC_H__INCLUDED_
