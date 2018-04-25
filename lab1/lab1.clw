; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAppDev
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "lab1.h"

ClassCount=5
Class1=CLab1App
Class2=CLab1Dlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_LAB1_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CAppDev
Resource4=IDD_APPDEV
Class5=CDebugger
Resource5=IDD_DEBUGGER

[CLS:CLab1App]
Type=0
HeaderFile=lab1.h
ImplementationFile=lab1.cpp
Filter=N

[CLS:CLab1Dlg]
Type=0
HeaderFile=lab1Dlg.h
ImplementationFile=lab1Dlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CLab1Dlg

[CLS:CAboutDlg]
Type=0
HeaderFile=lab1Dlg.h
ImplementationFile=lab1Dlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LAB1_DIALOG]
Type=1
Class=CLab1Dlg
ControlCount=1
Control1=IDC_TAB1,SysTabControl32,1342177280

[CLS:CAppDev]
Type=0
HeaderFile=AppDev.h
ImplementationFile=AppDev.cpp
BaseClass=CDialog
Filter=D
LastObject=CAppDev

[CLS:CDebugger]
Type=0
HeaderFile=Debugger.h
ImplementationFile=Debugger.cpp
BaseClass=CDialog
Filter=D
LastObject=CDebugger

[DLG:IDD_APPDEV]
Type=1
Class=CAppDev
ControlCount=0

[DLG:IDD_DEBUGGER]
Type=1
Class=CDebugger
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

