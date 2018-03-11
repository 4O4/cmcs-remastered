///////////////////////////////////////////////////////////////////
//
//  CodeMax sample app : CodeMax Editor
//
//  (c) WinMain Software 1998-2000
//
//
#ifndef __PRECOMP_H__
#define __PRECOMP_H__

#pragma warning (disable : 4201 4553 4552)
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "codemax.h"

UINT RegGetProfileInt( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, int nDefault );
LPCTSTR RegGetProfileString( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, LPTSTR pszValue, LPCTSTR pszDefault );
BOOL RegGetProfileBinary( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry,	BYTE** ppData, UINT* pBytes );
BOOL RegWriteProfileInt( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, int nValue );
BOOL RegWriteProfileString( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, LPCTSTR pszValue );
BOOL RegWriteProfileBinary( LPCTSTR pszRegistryKey, LPCTSTR pszProfileName, LPCTSTR pszSection, LPCTSTR pszEntry, LPBYTE pData, UINT nBytes );
#define ARRAY_SIZE( array ) ( sizeof( array ) / sizeof( array[ 0 ] ) )

#endif

