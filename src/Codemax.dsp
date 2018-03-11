# Microsoft Developer Studio Project File - Name="codemax" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=codemax - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Codemax.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Codemax.mak" CFG="codemax - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "codemax - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "codemax - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "codemax - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "codemax - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "codemax - Win32 ActiveX Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "codemax - Win32 ActiveX Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "codemax - Win32 Profile" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Dev/CodeMax/Source/CodeSense/src", CUBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W4 /GX /O2 /I "../include" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "../include" /d "NDEBUG" /d "_MBCS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/cmax20.dll"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W4 /Gm /GX /Zi /Od /I "../include" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /FR /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../include" /d "_DEBUG" /d "_MBCS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"Debug/cmax20.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "codemax_"
# PROP BASE Intermediate_Dir "codemax_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UnicodeRel"
# PROP Intermediate_Dir "UnicodeRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"precomp.h" /FD /c
# ADD CPP /nologo /W4 /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "../include" /d "NDEBUG" /d "_UNICODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /dll /machine:I386 /out:"UnicodeRel/cmax20u.dll"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "codemax0"
# PROP BASE Intermediate_Dir "codemax0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UnicodeDbg"
# PROP Intermediate_Dir "UnicodeDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"precomp.h" /FD /c
# ADD CPP /nologo /W4 /Gm /GX /Zi /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../include" /d "_DEBUG" /d "_UNICODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"UnicodeDbg/cmax20u.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "codemax_"
# PROP BASE Intermediate_Dir "codemax_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "XDebug"
# PROP Intermediate_Dir "XDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"precomp.h" /FD /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "../include" /D "_DEBUG" /D "_WINDOWS_USRDLL" /D "_ACTIVEX" /D "WIN32" /D "_MBCS" /FR /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../include" /d "_DEBUG" /d "_ACTIVEX" /d "_MBCS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"Debug/cmax20.dll" /pdbtype:sept
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"XDebug/cmax20.ocx" /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\XDebug
TargetPath=.\XDebug\cmax20.ocx
InputPath=.\XDebug\cmax20.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "codemax_"
# PROP BASE Intermediate_Dir "codemax_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "XRelease"
# PROP Intermediate_Dir "XRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_ACTIVEX" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "../include" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_ACTIVEX" /D "WIN32" /D "_MBCS" /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_ACTIVEX"
# ADD RSC /l 0x409 /i "../include" /d "NDEBUG" /d "_ACTIVEX" /d "_MBCS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"Debug/cmax20.ocx" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /dll /pdb:none /machine:I386 /out:"XRelease/cmax20.ocx"
# SUBTRACT LINK32 /debug
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\XRelease
TargetPath=.\XRelease\cmax20.ocx
InputPath=.\XRelease\cmax20.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "codemax_"
# PROP BASE Intermediate_Dir "codemax_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Profile"
# PROP Intermediate_Dir "Profile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W4 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"precomp.h" /FD /c
# ADD CPP /nologo /W4 /Gm /GX /Zi /Od /I "../include" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /Yu"precomp.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../include" /d "_DEBUG" /d "_MBCS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"Debug/cmax20.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"Profile\cmax20.dll"

!ENDIF 

# Begin Target

# Name "codemax - Win32 Release"
# Name "codemax - Win32 Debug"
# Name "codemax - Win32 Unicode Release"
# Name "codemax - Win32 Unicode Debug"
# Name "codemax - Win32 ActiveX Debug"
# Name "codemax - Win32 ActiveX Release"
# Name "codemax - Win32 Profile"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bookmark.cpp
# End Source File
# Begin Source File

SOURCE=.\buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\cedit.cpp
# End Source File
# Begin Source File

SOURCE=.\ceditmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\ceditx.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdarry.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\codemax.def

!IF  "$(CFG)" == "codemax - Win32 Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\codemax.rc
# End Source File
# Begin Source File

SOURCE=.\codemaxx.def

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CodeTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeTipFuncHighlightCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeTipHighlightCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeTipMultiFuncCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ctlstate.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\dragdrop.cpp
# End Source File
# Begin Source File

SOURCE=.\editcmd1.cpp
# End Source File
# Begin Source File

SOURCE=.\editdlgs.cpp
# End Source File
# Begin Source File

SOURCE=.\editsel.cpp
# End Source File
# Begin Source File

SOURCE=.\editview.cpp
# End Source File
# Begin Source File

SOURCE=.\editx.idl

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\editx.idl

BuildCmds= \
	midl /Oicf /client none /server none /h "editx.h" /iid "editx_i.c" "editx.idl"

".\editx.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\editx.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\editx_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Performing MIDL step
InputPath=.\editx.idl

BuildCmds= \
	midl /Oicf /client none /server none /h "editx.h" /iid "editx_i.c" "editx.idl"

".\editx.tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\editx.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\editx_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\exports.cpp
# End Source File
# Begin Source File

SOURCE=.\findrepl.cpp
# End Source File
# Begin Source File

SOURCE=.\globalsx.cpp
# End Source File
# Begin Source File

SOURCE=.\hkeyctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\hotkey.cpp
# End Source File
# Begin Source File

SOURCE=.\hotkeyx.cpp
# End Source File
# Begin Source File

SOURCE=.\ICodeList.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ICodeTip.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ICodeTipFuncHighlight.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ICodeTipHighlight.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ICodeTipMultiFunc.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\language.cpp
# End Source File
# Begin Source File

SOURCE=.\langx.cpp
# End Source File
# Begin Source File

SOURCE=.\line.cpp
# End Source File
# Begin Source File

SOURCE=.\macro.cpp
# End Source File
# Begin Source File

SOURCE=.\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\msgpump.cpp
# End Source File
# Begin Source File

SOURCE=.\posx.cpp
# End Source File
# Begin Source File

SOURCE=.\precomp.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# ADD CPP /Yc"precomp.h"

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# ADD CPP /Yc"precomp.h"

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# ADD CPP /Yc"precomp.h"

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# ADD CPP /Yc"precomp.h"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

# ADD CPP /Yc"precomp.h"

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

# PROP BASE Exclude_From_Build 1
# ADD CPP /Yc"precomp.h"

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# ADD BASE CPP /Yc"precomp.h"
# ADD CPP /Yc"precomp.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\print.cpp
# End Source File
# Begin Source File

SOURCE=.\props.cpp
# End Source File
# Begin Source File

SOURCE=.\rangex.cpp
# End Source File
# Begin Source File

SOURCE=.\rectx.cpp
# End Source File
# Begin Source File

SOURCE=.\regexp.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# ADD CPP /W1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# ADD CPP /W4 /GX

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Release"

# ADD CPP /W1

!ELSEIF  "$(CFG)" == "codemax - Win32 Unicode Debug"

# ADD CPP /W1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Debug"

# ADD CPP /W1

!ELSEIF  "$(CFG)" == "codemax - Win32 ActiveX Release"

# ADD CPP /W1

!ELSEIF  "$(CFG)" == "codemax - Win32 Profile"

# ADD CPP /W1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\selftest.cpp
# End Source File
# Begin Source File

SOURCE=.\splitter.cpp
# End Source File
# Begin Source File

SOURCE=.\syntax.cpp
# End Source File
# Begin Source File

SOURCE=.\undo.cpp
# End Source File
# Begin Source File

SOURCE=.\Wnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BUFFER.H
# End Source File
# Begin Source File

SOURCE=.\CEDIT.H
# End Source File
# Begin Source File

SOURCE=.\CEDITX.H
# End Source File
# Begin Source File

SOURCE=..\include\CMAXAFX.H
# End Source File
# Begin Source File

SOURCE=.\CMIDLIDS.H
# End Source File
# Begin Source File

SOURCE=.\CodeListCtrl.h
# End Source File
# Begin Source File

SOURCE=../include\codemax.h
# End Source File
# Begin Source File

SOURCE=.\CodeTipCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CodeTipFuncHighlightCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CodeTipHighlightCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CodeTipMultiFuncCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DEBUG.H
# End Source File
# Begin Source File

SOURCE=.\DispatchWnd.h
# End Source File
# Begin Source File

SOURCE=.\DRAGDROP.H
# End Source File
# Begin Source File

SOURCE=.\EDITDLGS.H
# End Source File
# Begin Source File

SOURCE=.\EDITSEL.H
# End Source File
# Begin Source File

SOURCE=.\EDITVIEW.H
# End Source File
# Begin Source File

SOURCE=.\GLOBALSX.H
# End Source File
# Begin Source File

SOURCE=.\HKEYCTRL.H
# End Source File
# Begin Source File

SOURCE=.\hotkeyx.h
# End Source File
# Begin Source File

SOURCE=.\ICodeList.h
# End Source File
# Begin Source File

SOURCE=.\ICodeTip.h
# End Source File
# Begin Source File

SOURCE=.\ICodeTip.hxx
# End Source File
# Begin Source File

SOURCE=.\ICodeTipFuncHighlight.h
# End Source File
# Begin Source File

SOURCE=.\ICodeTipFuncHighlight.hxx
# End Source File
# Begin Source File

SOURCE=.\ICodeTipHighlight.h
# End Source File
# Begin Source File

SOURCE=.\ICodeTipMultiFunc.h
# End Source File
# Begin Source File

SOURCE=.\LANGX.H
# End Source File
# Begin Source File

SOURCE=.\MISC.H
# End Source File
# Begin Source File

SOURCE=.\POSX.H
# End Source File
# Begin Source File

SOURCE=.\PRECOMP.H
# End Source File
# Begin Source File

SOURCE=.\Rect.h
# End Source File
# Begin Source File

SOURCE=.\rectx.h
# End Source File
# Begin Source File

SOURCE=.\RegClassList.h
# End Source File
# Begin Source File

SOURCE=.\regexp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource2.h
# End Source File
# Begin Source File

SOURCE=.\UNDO.H
# End Source File
# Begin Source File

SOURCE=.\Wnd.h
# End Source File
# Begin Source File

SOURCE=.\Wnd.inl
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\btn_left.ico
# End Source File
# Begin Source File

SOURCE=.\res\btn_right.ico
# End Source File
# Begin Source File

SOURCE=.\CODEMAX.RC2
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\editx.rgs
# End Source File
# Begin Source File

SOURCE=.\res\ENDMREC.BMP
# End Source File
# Begin Source File

SOURCE=.\globalsx.rgs
# End Source File
# Begin Source File

SOURCE=.\hotkeyx.rgs
# End Source File
# Begin Source File

SOURCE=.\res\HSPLIT.CUR
# End Source File
# Begin Source File

SOURCE=.\res\HVSPLIT.CUR
# End Source File
# Begin Source File

SOURCE=.\langx.rgs
# End Source File
# Begin Source File

SOURCE=.\res\LEFTMARG.CUR
# End Source File
# Begin Source File

SOURCE=.\posx.rgs
# End Source File
# Begin Source File

SOURCE=.\rangex.rgs
# End Source File
# Begin Source File

SOURCE=.\res\RECMACRO.CUR
# End Source File
# Begin Source File

SOURCE=.\rectx.rgs
# End Source File
# Begin Source File

SOURCE=.\res\TOOLBOX.BMP
# End Source File
# Begin Source File

SOURCE=.\res\VSPLIT.CUR
# End Source File
# End Group
# End Target
# End Project
