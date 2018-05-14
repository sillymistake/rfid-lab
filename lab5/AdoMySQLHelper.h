// AdoMySQLHelper.h: interface for the CAdoMySQLHelper class.
//
//////////////////////////////////////////////////////////////////////
#import "./libs/msado15.dll" no_namespace rename("EOF","ADOEOF") rename("BOF","ADOBOF")
#include "MySQL_Statu.h"

#if !defined(AFX_ADOMYSQLHELPER_H__E2464E06_628E_4375_84A3_F9468F3C76E6__INCLUDED_)
#define AFX_ADOMYSQLHELPER_H__E2464E06_628E_4375_84A3_F9468F3C76E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdoMySQLHelper  
{
public:
	CAdoMySQLHelper();
	virtual ~CAdoMySQLHelper();

	//连接数据库
	CMySQL_Statu MYSQL_Connect();
	//关闭连接
	void mySQL_close();

	//增删查改操作

	//增操作
	CMySQL_Statu MYSQL_Insert(const CString& uid,const CString& money);


	//改操作
	CMySQL_Statu MYSQL_Update(const CString& uid,const CString& money);

	//查操作
	CMySQL_Statu MYSQL_Query(const CString& uid,CString &money);


private:
	_ConnectionPtr m_pConn;
	bool connect_state;
};

#endif // !defined(AFX_ADOMYSQLHELPER_H__E2464E06_628E_4375_84A3_F9468F3C76E6__INCLUDED_)
