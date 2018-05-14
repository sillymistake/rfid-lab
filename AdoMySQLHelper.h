// AdoMySQLHelper.h: interface for the CAdoMySQLHelper class.
//
//////////////////////////////////////////////////////////////////////

#import "./libs/msado15.dll" no_namespace rename("EOF","ADOEOF") rename("BOF","ADOBOF")

#if !defined(AFX_ADOMYSQLHELPER_H__9F5E4FE3_1E01_4201_A4E6_B5B8081A8F9E__INCLUDED_)
#define AFX_ADOMYSQLHELPER_H__9F5E4FE3_1E01_4201_A4E6_B5B8081A8F9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CAdoMySQLHelper  
{
public:
	CAdoMySQLHelper();
	virtual ~CAdoMySQLHelper();

	//�������ݿ�
	bool MYSQL_Connect();
	//�ر�����
	void mySQL_close();

	//��ɾ��Ĳ���

	//������
	bool MYSQL_Insert(const CString& uid,const CString& money);


	//�Ĳ���
	bool MYSQL_Update(const CString& uid,const CString& money);

	//�����
	bool MYSQL_Query(const CString& uid,CString &money);


private:
	_ConnectionPtr m_pConn;
};

#endif // !defined(AFX_ADOMYSQLHELPER_H__9F5E4FE3_1E01_4201_A4E6_B5B8081A8F9E__INCLUDED_)
