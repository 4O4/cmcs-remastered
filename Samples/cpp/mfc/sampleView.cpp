// sampleView.cpp : implementation of the CSampleView class
//

#include "stdafx.h"
#include "sample.h"

#include "sampleDoc.h"
#include "sampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SECTION_FUNC_NAMES		_T("Function Names")
#define SECTION_FUNC_PROTO		_T("Function Prototypes")
#define SECTION_OVERLOADS		_T("Overloads")

/////////////////////////////////////////////////////////////////////////////
// CSampleView

IMPLEMENT_DYNCREATE(CSampleView, CCodeMaxView)

BEGIN_MESSAGE_MAP(CSampleView, CCodeMaxView)
	//{{AFX_MSG_MAP(CSampleView)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_NOTIFY_REFLECT_EX( CMN_CODELIST, OnCodeList )
	ON_NOTIFY_REFLECT_EX( CMN_CODELISTSELMADE, OnCodeListSelMade )
	ON_NOTIFY_REFLECT_EX( CMN_CODELISTCANCEL, OnCodeListCancel )
	ON_NOTIFY_REFLECT_EX( CMN_CODETIP, OnCodeTip )
	ON_NOTIFY_REFLECT_EX( CMN_CODETIPUPDATE, OnCodeTipUpdate )
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CCodeMaxView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleView construction/destruction

CSampleView::CSampleView()
{
	// The functions.ini file should be in the same directory as the app.
	GetModuleFileName( NULL, m_strIniFile.GetBuffer( MAX_PATH ), MAX_PATH );
	m_strIniFile.ReleaseBuffer();

	int i = m_strIniFile.ReverseFind( L'\\' );
	ASSERT( i != -1 );

	m_strIniFile = m_strIniFile.Left( i + 1 ) + _T("functions.ini");

#ifdef _DEBUG

	CFileFind f;

	// If this ASSERT fails, you need to make sure the functions.ini file is
	// in the same directory as the application executable.
	ASSERT( f.FindFile( m_strIniFile ) );

#endif

	VERIFY( m_ImageList.Create( 16, 16, ILC_COLOR | ILC_MASK, 1, 1 ) );
	VERIFY( m_ImageList.Add( AfxGetApp()->LoadIcon( IDI_CODELIST ) ) != -1 );
}

CSampleView::~CSampleView()
{
}

BOOL CSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CCodeMaxView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSampleView printing

BOOL CSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSampleView diagnostics

#ifdef _DEBUG
void CSampleView::AssertValid() const
{
	CCodeMaxView::AssertValid();
}

void CSampleView::Dump(CDumpContext& dc) const
{
	CCodeMaxView::Dump(dc);
}

CSampleDoc* CSampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSampleDoc)));
	return (CSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleView message handlers

int CSampleView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCodeMaxView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// turn on syntax highlighting
	GetCodeMaxCtrl().EnableColorSyntax();

	// default to C++ editor
	GetCodeMaxCtrl().SetLanguage( CMLANG_CPP );

	// Assign Ctrl + Shift + Space hotkey to CodeTip
	CM_HOTKEY hotkey = {0};

	hotkey.nVirtKey1 = VK_SPACE;
	hotkey.byModifiers1 = HOTKEYF_CONTROL | HOTKEYF_SHIFT;

	VERIFY( CME_SUCCESS == CMRegisterHotKey( &hotkey, CMD_CODETIP ) );

	return 0;
}

void CSampleView::OnInitialUpdate()
{
	CCodeMaxView::OnInitialUpdate();
	GetCodeMaxCtrl().OpenFile(GetDocument()->GetPathName());
}

void CSampleView::OnFilePrint()
{
	GetCodeMaxCtrl().Print(NULL, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CodeList / CodeTip support

// OnCodeList ---------------------------------------------------------------
BOOL CSampleView::OnCodeList( LPCM_CODELISTDATA lpData, LRESULT* pResult )
{
	// Attach the list view control to our member variable so that we can use
	// it to update the selection as the user types.
	m_CodeList.Attach( lpData->hListCtrl );

	// Attach our imagelist to the CodeList control.  Note that the list
	// control has the LVS_SHAREIMAGELISTS style set by default, so we don't
	// have to worry about detaching the imagelist later on.
	//
	m_CodeList.SetImageList( &m_ImageList, LVSIL_SMALL );

	// Read the keywords from INI file
	int iIndex = 0;
	CString strFunc;

	while( GetString( SECTION_FUNC_NAMES, iIndex++, &strFunc ) )
		m_CodeList.InsertItem( 0, strFunc );

	// Try to select the item that matches any function name entered in the
	// CodeMax control window.
	SelectClosestItemInCodeList();

	// Allow CodeList control to be displayed
	*pResult = TRUE;

	return TRUE;
}

// OnCodeListSelMade --------------------------------------------------------
BOOL CSampleView::OnCodeListSelMade( LPCM_CODELISTDATA, LRESULT* pResult )
{
	// Get the item selected in the list
	CM_RANGE range = {0};
	CCodeMaxCtrl& rCtrl = GetCodeMaxCtrl();
	int iPos = m_CodeList.GetNextItem( -1, LVIS_SELECTED );
	CString strText = m_CodeList.GetItemText( iPos, 0 );

	// Select the current function in the CodeMax control, if any
	SelectCurrentFunction();

	// Replace current selection
	VERIFY( rCtrl.ReplaceSel( strText ) == CME_SUCCESS );

	// Get new selection
	VERIFY( rCtrl.GetSel( &range ) == CME_SUCCESS );
	
	// Update range to end of newly inserted text
	range.posStart.nCol += lstrlen( strText );
	range.posEnd = range.posStart;
	
	// Move cursor
	VERIFY( rCtrl.SetSel( &range ) == CME_SUCCESS );

	// Detach list view control from our member variable
	m_CodeList.Detach();

	// Don't prevent the list control from being destroyed
	*pResult = FALSE;

	return TRUE;
}

// OnCodeListCancel ---------------------------------------------------------
BOOL CSampleView::OnCodeListCancel( LPCM_CODELISTDATA, LRESULT* pResult )
{
	// Detach list view control from our member variable
	m_CodeList.Detach();

	// Don't prevent the list control from being destroyed
	*pResult = FALSE;

	return TRUE;
}

// OnCodeTip ----------------------------------------------------------------
BOOL CSampleView::OnCodeTip( LPNMHDR, LRESULT* pResult )
{
	// We don't display a tooltip by default, in case of error.
	*pResult = CM_TIPSTYLE_NONE;

	// We don't want to display a tip inside quoted or commented-out lines...
	DWORD dwToken = GetCodeMaxCtrl().GetCurrentToken();

	if( CM_TOKENTYPE_TEXT == dwToken || CM_TOKENTYPE_KEYWORD == dwToken )
	{
		// See if there is a valid function on the current line
		if( !GetCurrentFunction().IsEmpty() )
		{
			// We want to use the tooltip control that automatically
			// highlights the arguments in the function prototypes for us.
			// This type also provides support for overloaded function
			// prototypes.
			//
			*pResult = CM_TIPSTYLE_MULTIFUNC;
		}
	}

	return TRUE;
}

// OnCodeTipUpdate ----------------------------------------------------------
BOOL CSampleView::OnCodeTipUpdate( LPCM_CODETIPDATA lpData, LRESULT* pResult )
{
	ASSERT( CM_TIPSTYLE_MULTIFUNC == lpData->nTipType );

	CWnd* pWnd = CWnd::FromHandle( lpData->hToolTip );
	ASSERT( pWnd != NULL );

	LPCM_CODETIPMULTIFUNCDATA pmfData = (LPCM_CODETIPMULTIFUNCDATA)lpData;

	// See if the tooltip control has been initialized yet
	if( 0 == pWnd->GetWindowTextLength() )
	{
		// Save name of current function
		m_strTipFunc = GetCurrentFunction();
		ASSERT ( !m_strTipFunc.IsEmpty() );

		// Default to first function prototype
		pmfData->nCurrFunc = 0;

		// Get first prototype for function
		CString strProto = GetPrototype( m_strTipFunc );
		ASSERT( !strProto.IsEmpty() );

		// Set tooltip text
		pWnd->SetWindowText( strProto );

		// Default to first argument
		pmfData->ctfData.nArgument = 0;

		// Get number of overloads, if any
		pmfData->nFuncCount = GetPrivateProfileInt( SECTION_OVERLOADS,
			m_strTipFunc, 0, m_strIniFile );
	}
	else
	{
		// Destroy the tooltip window if the caret is moved above or before
		// the starting point.
		//
		CM_RANGE range = {0};
		CCodeMaxCtrl& rCtrl = GetCodeMaxCtrl();

		VERIFY( CME_SUCCESS == rCtrl.GetSel( &range, TRUE ) );

		if( range.posEnd.nLine < m_posSel.nLine ||
			( range.posEnd.nCol < m_posSel.nCol &&
			range.posEnd.nLine <= m_posSel.nLine ) )
		{
			// Caret moved too far up / back
			pWnd->DestroyWindow();
		}
		else
		{
			// Determine which argument to highlight
			pmfData->ctfData.nArgument = GetCurrentArgument();

			if( -1 == pmfData->ctfData.nArgument )
				pWnd->DestroyWindow();// Right-paren found
			else if( pmfData->nFuncCount )
			{
				// Function is overloaded, so get current prototype
				CString strProto = GetPrototype( m_strTipFunc,
					pmfData->nCurrFunc );

				// Set tooltip text
				pWnd->SetWindowText( strProto );
			}
		}
	}

	// Apply changes to pmfData members
	*pResult = TRUE;
	return TRUE;
}

// OnChar -------------------------------------------------------------------
void CSampleView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CCodeMaxView::OnChar( nChar, nRepCnt, nFlags );

	// Invoke the CodeTip feature when the user presses the '(' key. We'll
	// determine whether or not we are actually ready to handle the message
	// in the OnCodeTip() handler.
	//
	if( L'(' == nChar )
		GetCodeMaxCtrl().ExecuteCmd( CMD_CODETIP );

	// If the CodeList control is active, update selected item based on
	// current word in CodeMax control window.
	SelectClosestItemInCodeList();
}

// GetCurrentFunction -------------------------------------------------------
CString CSampleView::GetCurrentFunction( bool bMustExist )
{
	CM_RANGE range = {0};
	CString strLine, strFunc;
	CCodeMaxCtrl& rCtrl = GetCodeMaxCtrl();

	// Note:  We can't use CCodeMaxCtrl::GetWord() here, because the user
	// could have typed a function name followed by a space, and *then* the
	// left parenthesis, i.e. "MessageBox ("

	// Get the current line
	rCtrl.GetSel( &range, TRUE );
	rCtrl.GetLine( range.posEnd.nLine, strLine );

	// There's nothing for us to do if the line is empty
	if( strLine.IsEmpty() )
		return _T("");

	int nEnd = range.posEnd.nCol - 1;

	// Trim off trailing '(', if found
	if( nEnd > 0 && nEnd < strLine.GetLength() )
	{
		if( L'(' == strLine[ nEnd ] )
			nEnd --;

		// Trim off trailing whitespace
		while( nEnd > 0 && L' ' == strLine[nEnd] )
			nEnd --;
	}

	if( nEnd < 0 )
		return _T("");

	// Save the starting position for use with the CodeTip.  This is so we
	// can destroy the tip window if the user moves the cursor back before or
	// above the starting point.
	//
	m_posSel.nCol = range.posEnd.nCol;
	m_posSel.nLine = range.posEnd.nLine;

	// The function name begins at the first alphanumeric character on line
	int nStart = min( nEnd, strLine.GetLength() );

	while( nStart > 0 && ( _istalnum( strLine[nStart - 1] ) ||
		L'_' == strLine[nStart - 1] ) )
	{
		--nStart;
	}

	// Extract the function name
	strFunc = strLine.Mid( nStart, nEnd - nStart + 1 );

	if( strFunc.IsEmpty() )
		return _T("");

	// If we don't care whether or not the function actually exists in the
	// list, just return the string.
	if( !bMustExist )
		return strFunc;

	// Look for the function name in the INI file.
	int iIndex = 0;
	CString strComp;

	while( GetString( SECTION_FUNC_NAMES, iIndex++, &strComp ) )
	{
		if( strComp == strFunc )
			return strComp;
	}

	return _T("");// Not found
}

// GetPrototype -------------------------------------------------------------
CString CSampleView::GetPrototype( CString strFunc, int iOverload )
{
	CString strProto;

	// Is this function overloaded?
	if( iOverload || GetPrivateProfileInt( SECTION_OVERLOADS, strFunc, 0,
		m_strIniFile ) )
	{
		// Yes, so return appropriate overload
		CString section;

		section.Format( _T("%s%d"), (LPCTSTR)strFunc, iOverload );
		VERIFY( GetString( section, 0, &strProto ) );
	}
	else
	{
		// Look for the function name in the INI file.
		int iIndex = 0;
		CString strComp;

		while( GetString( SECTION_FUNC_NAMES, iIndex++, &strComp ) )
		{
			if( strComp == strFunc )
			{
				GetString( SECTION_FUNC_PROTO, --iIndex, &strProto );
				ASSERT( !strProto.IsEmpty() );
				break;
			}
		}
	}

	return strProto;
}

// GetString ----------------------------------------------------------------
BOOL CSampleView::GetString( LPCTSTR lpszSection, int iIndex,
							CString* pstrFunc )
{
	CString key;
	key.Format( _T("Func%d"), iIndex );

	pstrFunc->Empty();

	GetPrivateProfileString( lpszSection, key, _T(""),
		pstrFunc->GetBuffer( 1000 ), 1000, m_strIniFile );

	pstrFunc->ReleaseBuffer();

	return !pstrFunc->IsEmpty();
}

// GetCurrentArgument -------------------------------------------------------
int CSampleView::GetCurrentArgument()
{
	// Parse the buffer to determine which argument to highlight...
	//
	int iArg = 0;
	CString strLine;
	CM_RANGE range = {0};
	CCodeMaxCtrl& rCtrl = GetCodeMaxCtrl();

	VERIFY( CME_SUCCESS == rCtrl.GetSel( &range, TRUE ) );

	enum
	{
		sNone,
		sQuote,
		sComment,
		sMultiComment,
		sEscape,
		sSubParen,
	
	} state = sNone;

	for( int nLine = m_posSel.nLine; nLine <= range.posEnd.nLine; nLine++ )
	{
		// Get next line from buffer
		VERIFY( CME_SUCCESS == rCtrl.GetLine( nLine, strLine ) );

		if( nLine == range.posEnd.nLine )
		{
			// Trim off any excess beyond cursor pos so the argument with the
			// cursor in it will be highlighted.
			//
			int iTrim = min( range.posEnd.nCol, strLine.GetLength() + 1 );
			strLine = strLine.Left( iTrim );
		}

		if( nLine == m_posSel.nLine )
		{
			// Strip off function name & open paren.
			int iTrim = strLine.Find( L'(' ) + 1;
			strLine = strLine.Mid( iTrim );
		}

		// Parse arguments from current line
		for( int iPos = 0, len = strLine.GetLength(); iPos < len; iPos++ )
		{
			switch( strLine[iPos] )
			{
			case L'(':// Sub-parenthesis
				{
					switch( state )
					{
					case sNone:
						state = sSubParen;
						break;
					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			case L'"':// String begin/end
				{
					switch( state )
					{
					case sQuote:
						state = sNone;
						break;
					case sComment:
					case sMultiComment:
					case sSubParen:
						break;
					default:
						state = sQuote;
						break;
					}
				}
				break;

			case L',':// Argument separator
				{
					switch( state )
					{
					case sNone:
						iArg++;
						break;
					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			case L')':// End of function statement
				{
					switch( state )
					{
					case sNone:
						return -1;// Destroy tooltip on return
					case sEscape:
						state = sQuote;
						break;
					case sSubParen:
						state = sNone;
						break;
					}
				}
				break;

			case L'\\':// Escape sequence
				{
					switch( state )
					{
					case sQuote:
						state = sEscape;
						break;
					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			case L'/':// Possible comment begin/end
				{
					switch( state )
					{
					case sNone:
						{
							if( iPos + 1 < len )
							{
								TCHAR c = strLine[iPos + 1];

								if( L'/' == c )
									state = sComment;
								else if( L'*' == c )
								{
									state = sMultiComment;
									iPos++;
								}
							}
						}
						break;

					case sMultiComment:
						{
							if( iPos && L'*' == strLine[iPos - 1] )
								state = sNone;
						}
						break;

					case sEscape:
						state = sQuote;
						break;
					}
				}
				break;

			default:
				{
					if( sEscape == state )
						state = sQuote;
				}
				break;
			}

			// No point in scanning each and every character in comment line!
			if( sComment == state )
				break;
		}

		// It's safe to clear this now that we're moving on to the next line
		if( sComment == state )
			state = sNone;
	}

	return iArg;
}

// SelectClosestItemInCodeList ----------------------------------------------
void CSampleView::SelectClosestItemInCodeList()
{
	if( ::IsWindow( m_CodeList ) )
	{
		CString strFunc = GetCurrentFunction( false );

		if( !strFunc.IsEmpty() )
		{
			LVFINDINFO lvfi = {0};

			lvfi.psz = strFunc;
			lvfi.flags = LVFI_STRING | LVFI_PARTIAL;

			int iItem = m_CodeList.FindItem( &lvfi );

			if( -1 != iItem )
			{
				m_CodeList.SetItemState( iItem, LVIS_SELECTED | LVIS_FOCUSED,
					LVIS_SELECTED | LVIS_FOCUSED );

				m_CodeList.EnsureVisible( iItem, FALSE );
			}
		}
	}
}

// SelectCurrentFunction ----------------------------------------------------
void CSampleView::SelectCurrentFunction()
{
	CString strLine;
	CM_RANGE range = {0};
	CCodeMaxCtrl& rCtrl = GetCodeMaxCtrl();

	// Note:  We can't use CCodeMaxCtrl::GetWord() here, because the user
	// could have typed a function name followed by a space, and *then* the
	// left parenthesis, i.e. "MessageBox ("

	// Get the current line
	rCtrl.GetSel( &range, TRUE );
	rCtrl.GetLine( range.posEnd.nLine, strLine );

	// There's nothing for us to do if the line is empty
	if( strLine.IsEmpty() )
		return;

	int nEnd = range.posEnd.nCol - 1;

	// Trim off trailing '(', if found
	if( nEnd > 0 && nEnd < strLine.GetLength() )
	{
		if( L'(' == strLine[ nEnd ] )
			nEnd --;

		// Trim off trailing whitespace
		while( nEnd > 0 && L' ' == strLine[nEnd] )
			nEnd --;
	}

	if( nEnd < 0 )
		return;

	// The function name begins at the first alphanumeric character on line
	int nStart = min( nEnd, strLine.GetLength() );

	while( nStart > 0 && ( _istalnum( strLine[nStart - 1] ) ||
		L'_' == strLine[nStart - 1] ) )
	{
		--nStart;
	}

	range.posStart.nCol = nStart;
	range.posEnd.nCol = nEnd + 1;
	
	VERIFY( rCtrl.SetSel( &range ) == CME_SUCCESS );
}
