; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLogin
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ServerDemo.h"

ClassCount=4
Class1=CServerDemoApp
Class2=CServerDemoDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SERVERDEMO_DIALOG
Class4=CLogin
Resource4=IDD_DIALOG_Login

[CLS:CServerDemoApp]
Type=0
HeaderFile=ServerDemo.h
ImplementationFile=ServerDemo.cpp
Filter=N

[CLS:CServerDemoDlg]
Type=0
HeaderFile=ServerDemoDlg.h
ImplementationFile=ServerDemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CServerDemoDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=ServerDemoDlg.h
ImplementationFile=ServerDemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERVERDEMO_DIALOG]
Type=1
Class=CServerDemoDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_BUTTON_StartServer,button,1342242816
Control3=IDC_BUTTON_StopServer,button,1342242816
Control4=IDC_BUTTON_SendData,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC_Status,static,1342312961
Control7=IDC_STATIC,button,1342177287
Control8=IDC_EDIT_InPut,edit,1350631552
Control9=IDC_PIC,static,1342308352

[DLG:IDD_DIALOG_Login]
Type=1
Class=CLogin
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_UserName,edit,1350631552
Control4=IDC_EDIT_PassWord,edit,1350631584
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BUTTON_Reset,button,1342242816
Control8=IDC_STATIC,button,1342177287

[CLS:CLogin]
Type=0
HeaderFile=Login.h
ImplementationFile=Login.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_UserName
VirtualFilter=dWC

