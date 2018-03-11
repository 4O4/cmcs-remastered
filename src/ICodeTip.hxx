/////////////////////////////////////////////////////////////////////////////
//
// ICodeTip.hxx : Declaration of CICodeTipBase template class
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICODETIP_HXX__INCLUDED_
#define _ICODETIP_HXX__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef CHECK_PTR
#define CHECK_PTR(ptr)		if( NULL == ptr ) return E_POINTER;
#endif

#include "CodeTipCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CICodeTipBase

template<class Class, class TipCtrl, class Iface, const CLSID* pclsid,
	const GUID* plibid = &LIBID_CodeMax, WORD wMajor = CM_X_VERSION,
	WORD wMinor = 0, const IID* piidIface = &__uuidof(Iface)>
class ATL_NO_VTABLE CICodeTipBase :
	public CComObjectRoot,
	public CComCoClass<Class, pclsid>,
	public IDispatchImpl<Iface, piidIface, plibid, wMajor>
{

// CICodeTipBase Public Member Variables ---------------------------------
public:
	TipCtrl* m_pCtrl;


// CICodeTipBase Protected Member Functions ------------------------------
protected:

	// Construction
	CICodeTipBase()
	: m_pCtrl( NULL )
	{
	}

	// Destruction
	virtual ~CICodeTipBase()
	{
	}

BEGIN_COM_MAP(CICodeTipBase)
	COM_INTERFACE_ENTRY_IID(*piidIface, Iface)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


// ICodeTip Public Member Functions --------------------------------------
public:

	STDMETHOD(get_hWnd)( long* phwnd )
	{
		ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

		CHECK_PTR( phwnd );
		*phwnd = reinterpret_cast<long>( m_pCtrl->m_hWnd );
		return S_OK;
	}

	STDMETHOD(get_TipText)( BSTR* Text )
	{
		ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

		CHECK_PTR( Text );

		LPCTSTR pszText = m_pCtrl->GetTipText();

		if( NULL == pszText )
			return E_OUTOFMEMORY;

		USES_CONVERSION;
		*Text = T2BSTR( pszText );

		return S_OK;
	}

	STDMETHOD(put_TipText)( BSTR Text )
	{
		ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

		LPTSTR psz = BSTR2TSTR( Text );

		if( NULL == psz )
			return E_OUTOFMEMORY;

		m_pCtrl->SetWindowText( psz );

		delete [] psz;

		return S_OK;
	}

	STDMETHOD(get_TipTextLength)( long* Length )
	{
		ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

		CHECK_PTR( Length );

		*Length = m_pCtrl->GetWindowTextLength();

		return S_OK;
	}

	STDMETHOD(Destroy)()
	{
		if( !::IsWindow( m_pCtrl->GetSafeHwnd() ) )
			return S_OK;// Already destroyed

		return m_pCtrl->DestroyCodeTip() ? S_OK : ERROR_CANCELLED;
	}
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _ICODETIP_HXX__INCLUDED_
