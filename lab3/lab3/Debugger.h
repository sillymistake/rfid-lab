#if !defined(AFX_DEBUGGER_H__CA4D19C4_FBEB_4A42_B80A_912092B412D1__INCLUDED_)
#define AFX_DEBUGGER_H__CA4D19C4_FBEB_4A42_B80A_912092B412D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Debugger.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog

class CDebugger : public CDialog
{
// Construction
public:
	CDebugger(CWnd* pParent = NULL);   // standard constructor
	unsigned char* cstring_to_unsignedchar(CString s);
	void clearView();
	long cstring_to_long(CString s);

// Dialog Data
	//{{AFX_DATA(CDebugger)
	enum { IDD = IDD_DEBUGGER };
	CEdit	m_consume;
	CEdit	m_balance;
	CComboBox	m_page;
	CComboBox	m_block;
	CEdit	m_state;
	CEdit	m_recharge;
	CEdit	m_edit2;
	CEdit	m_edit1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugger)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugger)
	afx_msg void OnButton3();
	afx_msg void OnButton2();
	afx_msg void OnInit();
	afx_msg void OnQuery();
	afx_msg void OnRecharge();
	afx_msg void OnConsume();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGGER_H__CA4D19C4_FBEB_4A42_B80A_912092B412D1__INCLUDED_)
