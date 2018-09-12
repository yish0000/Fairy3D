# Microsoft Developer Studio Project File - Name="EffectCompiler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=EffectCompiler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EffectCompiler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EffectCompiler.mak" CFG="EffectCompiler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EffectCompiler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "EffectCompiler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EffectCompiler - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "EffectCompiler - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EffectCompiler - Win32 Release"
# Name "EffectCompiler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FlyAabb.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyObb.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyQuat.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyRay.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyRenderAutoParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyRenderEffectCompiler.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyRenderState.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyRenderVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\FlySphere.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyStringUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyVector.cpp
# End Source File
# Begin Source File

SOURCE=.\FlyXmlReader.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Fly.h
# End Source File
# Begin Source File

SOURCE=.\FlyAabb.h
# End Source File
# Begin Source File

SOURCE=.\FlyColor.h
# End Source File
# Begin Source File

SOURCE=.\FlyMath.h
# End Source File
# Begin Source File

SOURCE=.\FlyMathCommon.h
# End Source File
# Begin Source File

SOURCE=.\FlyMatrix.h
# End Source File
# Begin Source File

SOURCE=.\FlyObb.h
# End Source File
# Begin Source File

SOURCE=.\FlyPlane.h
# End Source File
# Begin Source File

SOURCE=.\FlyPolygon.h
# End Source File
# Begin Source File

SOURCE=.\FlyQuat.h
# End Source File
# Begin Source File

SOURCE=.\FlyRay.h
# End Source File
# Begin Source File

SOURCE=.\FlyRenderAutoParameter.h
# End Source File
# Begin Source File

SOURCE=.\FlyRenderEffectCompiler.h
# End Source File
# Begin Source File

SOURCE=.\FlyRenderState.h
# End Source File
# Begin Source File

SOURCE=.\FlyRenderVariable.h
# End Source File
# Begin Source File

SOURCE=.\FlyShaderObject.h
# End Source File
# Begin Source File

SOURCE=.\FlySphere.h
# End Source File
# Begin Source File

SOURCE=.\FlyStringUtil.h
# End Source File
# Begin Source File

SOURCE=.\FlyTypes.h
# End Source File
# Begin Source File

SOURCE=.\FlyUtil.h
# End Source File
# Begin Source File

SOURCE=.\FlyVector.h
# End Source File
# Begin Source File

SOURCE=.\FlyXmlReader.h
# End Source File
# Begin Source File

SOURCE=.\stl_user_config.h
# End Source File
# End Group
# End Target
# End Project
