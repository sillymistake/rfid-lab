// Debugger.cpp : implementation file
//

#include "stdafx.h"
#include "lab1.h"
#include "Debugger.h"
#include "ZM124U.h"
//#include "AFX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog


CDebugger::CDebugger(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugger::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugger)
	//}}AFX_DATA_INIT
}


void CDebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugger)
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugger, CDialog)
	//{{AFX_MSG_MAP(CDebugger)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugger message handlers

void CDebugger::OnButton3() 
{
	// TODO: Add your control notification handler code here
	unsigned char uid[10];	
	int* uid_len = new int;
	int state = find_14443(uid, uid_len);
	CString str;
	str.Format("%d",state);
	m_edit2.SetWindowText(str);
	if(state == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		m_edit1.SetWindowText("寻卡成功");
	}
	else {
		m_edit1.SetWindowText("寻卡失败");
	}
}

void CDebugger::OnButton2() 
{
	// TODO: Add your control notification handler code here
	int state = IDD_PowerOn();
	if(state == 0) {
		m_edit1.SetWindowText("设备开启成功");
	}
	else {
		m_edit1.SetWindowText("设备开启失败");
	}
}
