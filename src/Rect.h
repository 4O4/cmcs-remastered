/////////////////////////////////////////////////////////////////////////////
//
// Rect.h : Declaration of CRect
//
//  Copyright © 2000-2001  Nathan Lewis <nlewis@programmer.net>
//
// This source code can be modified and distributed freely, so long as this
// copyright notice is not altered or removed.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _RECT_H__INCLUDED_
#define _RECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CRect

class CRect : public tagRECT
{
public:
	CRect()
	{
		ZeroMemory( this, sizeof(RECT) );
	}

	CRect( int l, int t, int r, int b )
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}

	CRect( LPCRECT lpSrcRect )
	{
		::CopyRect( this, lpSrcRect );
	}

	inline int Width() const {
		return ( right > left ? right - left : left - right );
	}

	inline int Height() const {
		return ( bottom > top ? bottom - top : top - bottom );
	}

	inline void InflateRect( int x, int y ) {
		::InflateRect( this, x, y );
	}

	inline void InflateRect( int l, int t, int r, int b )
	{
		left -= l;
		top -= t;
		right += r;
		bottom += b;
	}

	inline void DeflateRect( int x, int y ) {
		::InflateRect( this, -x, -y );
	}

	inline void DeflateRect( LPCRECT lpRect )
	{
		left += lpRect->left;
		top += lpRect->top;
		right -= lpRect->right;
		bottom -= lpRect->bottom;
	}
};


/////////////////////////////////////////////////////////////////////////////

#endif//#ifndef _RECT_H__INCLUDED_
