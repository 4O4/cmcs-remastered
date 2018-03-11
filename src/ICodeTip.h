/////////////////////////////////////////////////////////////////////////////
//
// ICodeTip.h : Declaration of CICodeTip
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICODETIP_H__INCLUDED_
#define _ICODETIP_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ICodeTip.hxx"

/////////////////////////////////////////////////////////////////////////////
// CICodeTip

class ATL_NO_VTABLE CICodeTip :
	public CICodeTipBase< CICodeTip, CCodeTipCtrl, ICodeTip, &CLSID_CodeTip >
{

// CICodeTip Protected Member Functions ----------------------------------
protected:

	// Construction
	CICodeTip();

	// Destruction
	virtual ~CICodeTip();
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _ICODETIP_H__INCLUDED_
