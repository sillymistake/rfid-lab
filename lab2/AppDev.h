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

// Dialog Data
	//{{AFX_DATA(CAppDev)
	enum { IDD = IDD_APPDEV};
		// NOTE: the ClassWizard will add data members here
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
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDEV_H__21D6E7F2_589F_472F_B8F3_53C2D32CF55B__INCLUDED_)
