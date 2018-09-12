# Microsoft Developer Studio Project File - Name="etCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=etCore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "etCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "etCore.mak" CFG="etCore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "etCore - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "etCore - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "etCore - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ETCORE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../SDK/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ETCORE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../../../SDK/Lib"

!ELSEIF  "$(CFG)" == "etCore - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ETCORE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../SDK/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ETCORE_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../SDK/Lib"

!ENDIF 

# Begin Target

# Name "etCore - Win32 Release"
# Name "etCore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\etAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\etBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\etCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\etConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\etCoreManager.cpp
# End Source File
# Begin Source File

SOURCE=.\etDetailLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\etEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\etEntityManager.cpp
# End Source File
# Begin Source File

SOURCE=.\etFloatRect.cpp
# End Source File
# Begin Source File

SOURCE=.\etLightLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\etLiquidObject.cpp
# End Source File
# Begin Source File

SOURCE=.\etMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\etParticle.cpp
# End Source File
# Begin Source File

SOURCE=.\etPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\etSceneManager.cpp
# End Source File
# Begin Source File

SOURCE=.\etSelectionBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\etSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\etSpriteObject.cpp
# End Source File
# Begin Source File

SOURCE=.\etTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\etTile.cpp
# End Source File
# Begin Source File

SOURCE=.\etVegetation.cpp
# End Source File
# Begin Source File

SOURCE=.\etVegetationObject.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\etAnimation.h
# End Source File
# Begin Source File

SOURCE=.\etBrush.h
# End Source File
# Begin Source File

SOURCE=.\etCollision.h
# End Source File
# Begin Source File

SOURCE=.\etCommon.h
# End Source File
# Begin Source File

SOURCE=.\etConfig.h
# End Source File
# Begin Source File

SOURCE=.\etCoreManager.h
# End Source File
# Begin Source File

SOURCE=.\etDetailLayer.h
# End Source File
# Begin Source File

SOURCE=.\etEntity.h
# End Source File
# Begin Source File

SOURCE=.\etEntityManager.h
# End Source File
# Begin Source File

SOURCE=.\etFloatRect.h
# End Source File
# Begin Source File

SOURCE=.\etLightLayer.h
# End Source File
# Begin Source File

SOURCE=.\etLiquidObject.h
# End Source File
# Begin Source File

SOURCE=.\etMesh.h
# End Source File
# Begin Source File

SOURCE=.\etParticle.h
# End Source File
# Begin Source File

SOURCE=.\etPlugin.h
# End Source File
# Begin Source File

SOURCE=.\etSceneManager.h
# End Source File
# Begin Source File

SOURCE=.\etSelectionBuffer.h
# End Source File
# Begin Source File

SOURCE=.\etSprite.h
# End Source File
# Begin Source File

SOURCE=.\etSpriteObject.h
# End Source File
# Begin Source File

SOURCE=.\etTerrain.h
# End Source File
# Begin Source File

SOURCE=.\etTile.h
# End Source File
# Begin Source File

SOURCE=.\etVegetation.h
# End Source File
# Begin Source File

SOURCE=.\etVegetationObject.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\res_etCore.rc
# End Source File
# Begin Source File

SOURCE=.\Res\resource.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\Res\et-Liquid.xml"
# End Source File
# Begin Source File

SOURCE=".\Res\et-Terrain.xml"
# End Source File
# Begin Source File

SOURCE=".\Res\et-Vegetation.xml"
# End Source File
# End Target
# End Project
