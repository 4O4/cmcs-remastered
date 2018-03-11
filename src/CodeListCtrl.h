/////////////////////////////////////////////////////////////////////////////
//
// CodeListCtrl.h : Declaration of CCodeListCtrl
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CODELISTCTRL_H__INCLUDED_
#define _CODELISTCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DispatchWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeListCtrl

class CCodeListCtrl : public CDispatchWnd
{

// Protected Member Variables -----------------------------------------------
protected:
	bool m_bAcceptSel;


// Public Member Functions --------------------------------------------------
public:

	// Destruction
	virtual ~CCodeListCtrl();

	// Creation
	static CCodeListCtrl* Create( POINT& pt, CEdit* pEdit );

	void AutoSizeControl();

	void SelectItem( int iItem );

	inline int GetCurSel() {
		return SendMessage( LVM_GETNEXTITEM, -1, LVIS_SELECTED );
	}

	inline int GetItemCount() {
		ASSERT( IsWindow( m_hWnd ) ); return ListView_GetItemCount( m_hWnd );
	}

	inline HIMAGELIST SetImageList( HIMAGELIST hImageList ) {
		ASSERT( IsWindow( m_hWnd ) ); return ListView_SetImageList( m_hWnd, hImageList, LVSIL_SMALL );
	}

	inline HIMAGELIST GetImageList() {
		ASSERT( IsWindow( m_hWnd ) ); return ListView_GetImageList( m_hWnd, LVSIL_SMALL );
	}

	virtual BOOL DestroyWindow();

	int AddItem( LPCTSTR lpszItem, int iImage = 0, LPARAM lParam = 0 );
	int GetItemText( int iItem, LPTSTR pszText, int cchTextMax );
	int GetItemData( int iItem );
	int FindString( LPCTSTR lpszString, BOOL bAcceptPartial );

	virtual LPDISPATCH GetDispatch();

	virtual BOOL DestroyCodeList() {
		return m_pEdit->DestroyCodeList();
	}


// Protected Member Functions -----------------------------------------------
protected:

	// Construction
	CCodeListCtrl( CEdit* pEdit );

	virtual LRESULT NotifyParent( UINT unNotification, LPNMHDR lpNMHDR = NULL );

	void InitControl();
	BOOL OnChar( WPARAM wp, LPARAM lp );
	void AcceptSelection();

	// Creation
	virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, LPCRECT lpRect, HWND hwndParent, UINT nID = 0 );

	virtual void OnDestroy();

	// Message handlers
	DECLARE_MSG_HANDLER()
		MSG_HANDLER( OnChar )
		MSG_HANDLER( OnLButtonDblClk )
		MSG_HANDLER( OnParentKeyDown )
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _CODELISTCTRL_H__INCLUDED_
