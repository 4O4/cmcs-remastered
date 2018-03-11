/////////////////////////////////////////////////////////////////////////////
//
// ICodeTipHighlight.cpp : Implementation of CICodeTipHighlight
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
#include "ICodeTipHighlight.h"
#include "CodeTipHighlightCtrl.h"


//***************************************************************************
// CICodeTipHighlight Protected Member Functions
//***************************************************************************

// Construction -------------------------------------------------------------
CICodeTipHighlight::CICodeTipHighlight()
{
}


// Destruction --------------------------------------------------------------
CICodeTipHighlight::~CICodeTipHighlight()
{
}


//***************************************************************************
// ICodeTipHighlight Public Member Functions
//***************************************************************************

// get_HighlightStartPos ----------------------------------------------------
STDMETHODIMP CICodeTipHighlight::get_HighlightStartPos( long* plPos )
{
	ASSERT( NULL != m_pCtrl );

	CHECK_PTR( plPos );

	*plPos = m_pCtrl->GetStartPos();

	return S_OK;
}

// put_HighlightStartPos ----------------------------------------------------
STDMETHODIMP CICodeTipHighlight::put_HighlightStartPos( long lPos )
{
	ASSERT( NULL != m_pCtrl );

	m_pCtrl->SetStartPos( lPos );

	return S_OK;
}

// get_HighlightEndPos ------------------------------------------------------
STDMETHODIMP CICodeTipHighlight::get_HighlightEndPos( long* plPos )
{
	ASSERT( NULL != m_pCtrl );

	CHECK_PTR( plPos );

	*plPos = m_pCtrl->GetEndPos();

	return S_OK;
}

// put_HighlightEndPos ------------------------------------------------------
STDMETHODIMP CICodeTipHighlight::put_HighlightEndPos( long lPos )
{
	ASSERT( NULL != m_pCtrl );

	m_pCtrl->SetEndPos( lPos );

	return S_OK;
}



/////////////////////////////////////////////////////////////////////////////

#endif//#ifdef _ACTIVEX
