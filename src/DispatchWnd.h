/////////////////////////////////////////////////////////////////////////////
//
// DispatchWnd.h : Declaration of CDispatchWnd
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _DISPATCHWND_H__INCLUDED_
#define _DISPATCHWND_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEdit;

#include "Wnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDispatchWnd

class CDispatchWnd : public CWnd
{

// Protected Member Variables -----------------------------------------------
protected:
	CEdit* m_pEdit;
	LPDISPATCH m_lpDispatch;


// Public Member Functions --------------------------------------------------
public:

	// Construction
	CDispatchWnd( CEdit* pEdit )
	: m_lpDispatch( NULL )
	, m_pEdit( pEdit )
	{
	}

	// Destruction
	virtual ~CDispatchWnd()
	{
		if( m_lpDispatch )
			m_lpDispatch->Release();
	}

	// Overrideables
	virtual LPDISPATCH GetDispatch() {
		return m_lpDispatch;
	}

	virtual LRESULT NotifyParent( UINT unNotification, LPNMHDR lpNMHDR = NULL )
	{
		ASSERT( NULL != m_pEdit );
		return m_pEdit->NotifyParent( unNotification, lpNMHDR );
	}
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _DISPATCHWND_H__INCLUDED_
