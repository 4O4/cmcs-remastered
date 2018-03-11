/////////////////////////////////////////////////////////////////////////////
//
// ICodeTipFuncHighlight.hxx : Declaration of CICodeTipFuncHighlightBase
// template class
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICODETIPFUNCHIGHLIGHT_HXX__INCLUDED_
#define _ICODETIPFUNCHIGHLIGHT_HXX__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ICodeTip.hxx"

/////////////////////////////////////////////////////////////////////////////
// CICodeTipFuncHighlightBase

template<class Class, class TipCtrl, class Iface, const CLSID* pclsid,
	const GUID* plibid = &LIBID_CodeMax, WORD wMajor = CM_X_VERSION,
	WORD wMinor = 0, const IID* piidIface = &__uuidof(Iface)>
class ATL_NO_VTABLE CICodeTipFuncHighlightBase :
	public CICodeTipBase<Class, TipCtrl, Iface, pclsid, plibid, wMajor,
		wMinor, piidIface>
{

// CICodeTipFuncHighlightBase Protected Member Functions --------------------
protected:

	// Construction
	CICodeTipFuncHighlightBase()
	{
	}

	// Destruction
	virtual ~CICodeTipFuncHighlightBase()
	{
	}


// ICodeTipFuncHighlight Public Member Functions -------------------------
public:

	STDMETHOD(get_Argument)( long* plArgument )
	{
		ASSERT( NULL != m_pCtrl );

		CHECK_PTR( plArgument );

		*plArgument = m_pCtrl->GetArgument();

		return S_OK;
	}


	STDMETHOD(put_Argument)( long lArgument )
	{
		ASSERT( NULL != m_pCtrl );

		m_pCtrl->SetArgument( lArgument );

		return S_OK;
	}
};



/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _ICODETIPFUNCHIGHLIGHT_HXX__INCLUDED_
