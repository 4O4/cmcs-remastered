// sampleView.h : interface of the CSampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEVIEW_H__E848CDB0_1778_11D3_ABF2_000000000000__INCLUDED_)
#define AFX_SAMPLEVIEW_H__E848CDB0_1778_11D3_ABF2_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CodeMaxView.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleView
class CSampleView : public CCodeMaxView
{
protected: // create from serialization only
	CSampleView();
	DECLARE_DYNCREATE(CSampleView)


	CString m_strIniFile;
	CString m_strTipFunc;
	CListCtrl m_CodeList;


// Attributes
public:
	CSampleDoc* GetDocument();

// Operations
public:

	virtual BOOL SaveFile( LPCTSTR pszFilename ) {
		return GetCodeMaxCtrl().SaveFile(pszFilename) == CME_SUCCESS;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CImageList m_ImageList;
	CM_POSITION m_posSel;


	BOOL GetString( LPCTSTR lpszSection, int iIndex, CString* pstrFunc );
	CString GetCurrentFunction( bool bMustExist = true );
	CString GetPrototype( CString strFunc, int iOverload = 0 );
	int GetCurrentArgument();
	void SelectClosestItemInCodeList();
	void SelectCurrentFunction();


// Generated message map functions
protected:
	//{{AFX_MSG(CSampleView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnCodeList( LPCM_CODELISTDATA lpData, LRESULT* pResult );
	afx_msg BOOL OnCodeListSelMade( LPCM_CODELISTDATA lpData, LRESULT* pResult );
	afx_msg BOOL OnCodeListCancel( LPCM_CODELISTDATA lpData, LRESULT* pResult );
	afx_msg BOOL OnCodeTip( LPNMHDR lpNMH, LRESULT* pResult );
	afx_msg BOOL OnCodeTipUpdate( LPCM_CODETIPDATA lpData, LRESULT* );
	afx_msg void OnFilePrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in sampleView.cpp
inline CSampleDoc* CSampleView::GetDocument()
   { return (CSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEVIEW_H__E848CDB0_1778_11D3_ABF2_000000000000__INCLUDED_)
