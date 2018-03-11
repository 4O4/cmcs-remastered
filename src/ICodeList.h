/////////////////////////////////////////////////////////////////////////////
//
// ICodeList.h : Declaration of CICodeList
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICODELIST_H__INCLUDED_
#define _ICODELIST_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CICodeList

class ATL_NO_VTABLE CICodeList :
	public CComObjectRoot,
	public CComCoClass<CICodeList, &CLSID_CodeList>,
	public IDispatchImpl<ICodeList, &IID_ICodeList, &LIBID_CodeMax, CM_X_VERSION>
{
	friend class CCodeListCtrl;

// CICodeList Protected Member Variables ---------------------------------
protected:
	CCodeListCtrl* m_pCtrl;


// ICodeList Public Member Functions -------------------------------------
public:
	STDMETHOD(get_hWnd)( long* phwnd );
	STDMETHOD(put_hImageList)( long hImageList );
	STDMETHOD(get_hImageList)( long* phImageList );
	STDMETHOD(get_SelectedItem)( long* plIndex );
	STDMETHOD(put_SelectedItem)( long ItemIndex );
	STDMETHOD(AddItem)( BSTR strAdd, VARIANT ImageIndex, VARIANT ItemData, long* plIndex );
	STDMETHOD(GetItemText)( long ItemIndex, BSTR* pstrVal );
	STDMETHOD(GetItemData)( long ItemIndex, long* plParam );
	STDMETHOD(FindString)( BSTR strFind, VARIANT bAcceptPartial, long* plItemIndex );
	STDMETHOD(Destroy)();


// CICodeList Protected Member Functions ---------------------------------
protected:

	// Construction
	CICodeList();

	// Destruction
	virtual ~CICodeList();


BEGIN_COM_MAP(CICodeList)
	COM_INTERFACE_ENTRY(ICodeList)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _ICODELIST_H__INCLUDED_
