// lab1Dlg.h : header file
//

#if !defined(AFX_LAB1DLG_H__20611F76_5155_47A1_A5EC_5C27018A02E3__INCLUDED_)
#define AFX_LAB1DLG_H__20611F76_5155_47A1_A5EC_5C27018A02E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLab1Dlg dialog

class CLab1Dlg : public CDialog
{
// Construction
public:
	CDialog m_MenuAppDev;
	CDialog m_MenuDebugger;
	CLab1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLab1Dlg)
	enum { IDD = IDD_LAB1_DIALOG };
	CTabCtrl	m_MainMenu;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLab1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLab1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAB1DLG_H__20611F76_5155_47A1_A5EC_5C27018A02E3__INCLUDED_)
