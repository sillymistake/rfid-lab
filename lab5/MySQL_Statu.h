// MySQL_Statu.h: interface for the CMySQL_Statu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQL_STATU_H__B95C71CF_D248_40A0_90A0_C367AA160F40__INCLUDED_)
#define AFX_MYSQL_STATU_H__B95C71CF_D248_40A0_90A0_C367AA160F40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySQL_Statu  
{
public:
	CMySQL_Statu();
	CMySQL_Statu(const CString &s,const long& type);
	void setString(const CString &s);
	void settype(const long& type);
	CString toString();
	long getType();
	virtual ~CMySQL_Statu();
private:
	CString error_str;
	long status_type;

};

#endif // !defined(AFX_MYSQL_STATU_H__B95C71CF_D248_40A0_90A0_C367AA160F40__INCLUDED_)
