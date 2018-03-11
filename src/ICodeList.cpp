/////////////////////////////////////////////////////////////////////////////
//
// ICodeList.cpp : Implementation of CICodeList
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"
#ifdef _ACTIVEX

/////////////////////////////////////////////////////////////////////////////

#include "editx.h"
#include "ICodeList.h"
#include "CodeListCtrl.h"

#define CHECK_PTR(ptr)		if( NULL == ptr ) return E_POINTER;

#define IS_VT_EMPTY(var)	( VT_EMPTY == var.vt || ( VT_ERROR == var.vt && \
								DISP_E_PARAMNOTFOUND == var.scode ) )


//***************************************************************************
// ICodeList Public Member Functions
//***************************************************************************

// get_hWnd -----------------------------------------------------------------
STDMETHODIMP CICodeList::get_hWnd( long* phwnd )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	CHECK_PTR( phwnd );
	*phwnd = reinterpret_cast<long>( m_pCtrl->m_hWnd );
	return S_OK;
}

// put_hImageList -----------------------------------------------------------
STDMETHODIMP CICodeList::put_hImageList( long hNewImageList )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	m_pCtrl->SetImageList( reinterpret_cast<HIMAGELIST>( hNewImageList ) );
	return S_OK;
}

// get_hImageList -----------------------------------------------------------
STDMETHODIMP CICodeList::get_hImageList( long* phImageList )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	CHECK_PTR( phImageList );
	*phImageList = reinterpret_cast<long>( m_pCtrl->GetImageList() );
	return S_OK;
}

// get_SelectedItem ---------------------------------------------------------
STDMETHODIMP CICodeList::get_SelectedItem( long* plIndex )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	CHECK_PTR( plIndex );
	*plIndex = m_pCtrl->GetCurSel();
	return S_OK;
}

// put_SelectedItem ---------------------------------------------------------
STDMETHODIMP CICodeList::put_SelectedItem( long ItemIndex )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	m_pCtrl->SelectItem( ItemIndex );
	return S_OK;
}

// AddItem ------------------------------------------------------------------
STDMETHODIMP CICodeList::AddItem( BSTR strAdd, VARIANT ImageIndex,
								 VARIANT ItemData, long* plIndex )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	long lIcon = 0, lParam = 0;

	// Image index is optional...
	if( !IS_VT_EMPTY( ImageIndex ) )
	{
		HRESULT hR = ::VariantChangeType( &ImageIndex, &ImageIndex, 0, VT_I4 );

		if( SUCCEEDED( hR ) )
			lIcon = ImageIndex.lVal;
	}

	// Item data is optional...
	if( !IS_VT_EMPTY( ItemData ) )
	{
		HRESULT hR = ::VariantChangeType( &ItemData, &ItemData, 0, VT_I4 );

		if( SUCCEEDED( hR ) )
			lParam = ItemData.lVal;
	}

	// Convert string
	LPTSTR psz = BSTR2TSTR( strAdd );

	if( NULL == psz )
		return E_OUTOFMEMORY;

	// Add to list view control
	int iItem = m_pCtrl->AddItem( psz, lIcon, lParam );

	if( plIndex )
		*plIndex = iItem;

	delete [] psz;

	return S_OK;
}

// GetItemText --------------------------------------------------------------
STDMETHODIMP CICodeList::GetItemText( long ItemIndex, BSTR* pstrVal )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	CHECK_PTR( pstrVal );
	int iLen = m_pCtrl->GetItemText( ItemIndex, NULL, 0 );
	LPTSTR pszText = new TCHAR[ iLen + 1 ];

	if( NULL == pszText )
		return E_OUTOFMEMORY;

	ZeroMemory( pszText, sizeof(TCHAR) * (iLen + 1) );
	m_pCtrl->GetItemText( ItemIndex, pszText, iLen );

	USES_CONVERSION;
	*pstrVal = T2BSTR( pszText );

	delete [] pszText;

	return S_OK;
}

// GetItemData --------------------------------------------------------------
STDMETHODIMP CICodeList::GetItemData( long ItemIndex, long* plParam )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	CHECK_PTR( plParam );
	*plParam = m_pCtrl->GetItemData( ItemIndex );
	return S_OK;
}

// FindString ---------------------------------------------------------------
STDMETHODIMP CICodeList::FindString( BSTR strFind, VARIANT bAcceptPartial,
									long* plItemIndex )
{
	ASSERT( m_pCtrl != NULL && ::IsWindow( m_pCtrl->m_hWnd ) );

	CHECK_PTR( plItemIndex );

	BOOL bPartial = TRUE;

	// bAcceptPartial flag is optional...
	if( !IS_VT_EMPTY( bAcceptPartial ) )
	{
		HRESULT hR = ::VariantChangeType( &bAcceptPartial, &bAcceptPartial,
			0, VT_BOOL );

		if( SUCCEEDED( hR ) )
			bPartial = ( VARIANT_FALSE != bAcceptPartial.boolVal );
	}

	// Convert string
	LPTSTR psz = BSTR2TSTR( strFind );

	if( NULL == psz )
		return E_OUTOFMEMORY;

	*plItemIndex = m_pCtrl->FindString( psz, bPartial );

	delete [] psz;

	return S_OK;
}

// Destroy ------------------------------------------------------------------
STDMETHODIMP CICodeList::Destroy()
{
	if( !::IsWindow( m_pCtrl->GetSafeHwnd() ) )
		return S_OK;// Already destroyed

	return m_pCtrl->DestroyCodeList() ? S_OK : ERROR_CANCELLED;
}


//***************************************************************************
// CICodeList Protected Member Functions
//***************************************************************************

// Construction -------------------------------------------------------------
CICodeList::CICodeList()
: m_pCtrl( NULL )
{
}

// Destruction --------------------------------------------------------------
CICodeList::~CICodeList()
{
}



/////////////////////////////////////////////////////////////////////////////

#endif//#ifdef _ACTIVEX
