/////////////////////////////////////////////////////////////////////////////
//
// RegClassList.h : Declaration of CRegClassList
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _REGCLASSLIST_H__INCLUDED_
#define _REGCLASSLIST_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

/////////////////////////////////////////////////////////////////////////////
// CRegClassList

class CRegClassList : public std::list<LPTSTR>
{
	friend class CWnd;

// Public Member Functions --------------------------------------------------
public:

	~CRegClassList()
	{
		extern HINSTANCE hInstance;

		for( iterator it = begin(); it != end(); it++ )
		{
			LPTSTR psz = *it;

			if( psz )
			{
				::UnregisterClass( psz, hInstance );
				free( psz );
			}
		}
	}


// Protected Member Functions -----------------------------------------------
protected:

	BOOL RegisterClass( const WNDCLASS* lpWndClass )
	{
		WNDCLASS wndcls = {0};

		if( GetClassInfo( lpWndClass->hInstance, lpWndClass->lpszClassName,
			&wndcls ) )
		{
			return TRUE;// Already registered
		}

		if( !::RegisterClass( lpWndClass ) )
		{
			CMTRACE0( "CRegClassList::RegisterClass() failed\n" );
			return FALSE;
		}

		push_back( _tcsdup( lpWndClass->lpszClassName ) );

		return TRUE;
	}
};



/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _REGCLASSLIST_H__INCLUDED_
