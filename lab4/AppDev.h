#if !defined(AFX_APPDEV_H__21D6E7F2_589F_472F_B8F3_53C2D32CF55B__INCLUDED_)
#define AFX_APPDEV_H__21D6E7F2_589F_472F_B8F3_53C2D32CF55B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppDev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAppDev dialog

class CAppDev : public CDialog
{
// Construction
public:
	CAppDev(CWnd* pParent = NULL);   // standard constructor
	unsigned char* cstring_to_unsignedchar(CString s);
	void clearView();
	int cstring_to_int(CString s);

// Dialog Data
	//{{AFX_DATA(CAppDev)
	enum { IDD = IDD_APPDEV };
	CComboBox	m_block;
	CComboBox	m_page;
	CEdit	m_pwd;
	CEdit	m_block3c;
	CEdit	m_block3b;
	CEdit	m_block3a;
	CEdit	m_block2;
	CEdit	m_block1;
	CEdit	m_block0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppDev)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAppDev)
	afx_msg void OnButton3();
	afx_msg void Onpwda();
	afx_msg void Onpwdb();
	afx_msg void OnButtonDefaultPassword();
	afx_msg void OnReadPage();
	afx_msg void OnWriteblock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDEV_H__21D6E7F2_589F_472F_B8F3_53C2D32CF55B__INCLUDED_)
