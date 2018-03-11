/////////////////////////////////////////////////////////////////////////////
//
// ICodeTipMultiFunc.cpp : Implementation of CICodeTipMultiFunc
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
#include "ICodeTipMultiFunc.h"
#include "CodeTipMultiFuncCtrl.h"


//***************************************************************************
// CICodeTipMultiFunc Protected Member Functions
//***************************************************************************

// Construction -------------------------------------------------------------
CICodeTipMultiFunc::CICodeTipMultiFunc()
{
}


// Destruction --------------------------------------------------------------
CICodeTipMultiFunc::~CICodeTipMultiFunc()
{
}


//***************************************************************************
// ICodeTipMultiFunc Public Member Functions
//***************************************************************************

// get_FunctionCount --------------------------------------------------------
STDMETHODIMP CICodeTipMultiFunc::get_FunctionCount( long* lCount )
{
	ASSERT( NULL != m_pCtrl );

	CHECK_PTR( lCount );

	*lCount = m_pCtrl->GetFuncCount();

	return S_OK;
}

// put_FunctionCount --------------------------------------------------------
STDMETHODIMP CICodeTipMultiFunc::put_FunctionCount( long lCount )
{
	ASSERT( NULL != m_pCtrl );

	m_pCtrl->SetFuncCount( lCount );

	return S_OK;
}

// get_CurrentFunction ------------------------------------------------------
STDMETHODIMP CICodeTipMultiFunc::get_CurrentFunction( long* lIndex )
{
	ASSERT( NULL != m_pCtrl );

	CHECK_PTR( lIndex );

	*lIndex = m_pCtrl->GetCurrFunc();

	return S_OK;
}

// put_CurrentFunction ------------------------------------------------------
STDMETHODIMP CICodeTipMultiFunc::put_CurrentFunction( long lIndex )
{
	ASSERT( NULL != m_pCtrl );

	m_pCtrl->SetCurrFunc( lIndex );

	return S_OK;
}



/////////////////////////////////////////////////////////////////////////////

#endif//#ifdef _ACTIVEX
