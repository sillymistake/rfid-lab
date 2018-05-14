// AdoMySQLHelper.cpp: implementation of the CAdoMySQLHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lab1.h"
#include "AdoMySQLHelper.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdoMySQLHelper::CAdoMySQLHelper()
{
	CoInitialize(NULL);
	this->connect_state = false;
}

CAdoMySQLHelper::~CAdoMySQLHelper()
{
	this->mySQL_close();
}
CMySQL_Statu CAdoMySQLHelper::MYSQL_Connect(){
	try{
		HRESULT hr = this->m_pConn.CreateInstance("ADODB.Connection");
		if(FAILED(hr)){
			CMySQL_Statu mysql_statu(1,"����_ConnectionPtr����ָ��ʧ��");
			return mysql_statu;
		}
		
		//�������ӳ�ʱʱ��  
		//this->m_pConn->ConnectionTimeout = 60000;
		//����ִ�����ʱʱ��
		//this->m_pConn->CommandTimeout = 1200;

		this->m_pConn->Open("DSN=MySQL8.0;Server=localhost;Database=rfid",  //���ݿ������ַ���
							"root",
							"",
								   adModeUnknown);
		this->connect_state = true;

	}catch(_com_error &e){
		if((this->m_pConn!=NULL) && (this->m_pConn->State)){
			this->m_pConn->Close(); //�ر�����
			this->m_pConn.Release();//�ͷ�����
			this->m_pConn=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(1,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"���ӳɹ�");
	return mysql_statu;
}
/*
*@function �Ͽ����ݿ�����
*@param void
*@retval void
*/
void CAdoMySQLHelper::mySQL_close(){
	if((this->m_pConn!=NULL) && (this->m_pConn->State)){
		this->m_pConn->Close(); //�ر�����
		this->m_pConn.Release();//�ͷ�����
		this->m_pConn=NULL;
	}
	CoUninitialize();
	//ж�����ݿ�
}
/*
*@function ���ݿ����
*@param CString,CString
*@retval fool
*/
CMySQL_Statu CAdoMySQLHelper::MYSQL_Insert(const CString& uid,const CString& money){
	_CommandPtr m_pCommand;
	try{
		HRESULT hr = m_pCommand.CreateInstance("ADODB.Command");
		if(FAILED(hr)){
			CMySQL_Statu mysql_statu(2,"����_CommandPtr����ָ��ʧ��");
			return mysql_statu;
		}
		//����Ϊ�޲���
		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		vNULL.scode = DISP_E_PARAMNOTFOUND;	
		//ע���ı���ʽ������Ҫʹ�����ţ�\'�˴�ʹ��ת�����ƴ���ַ��������ʹ�ÿո�
		//insert into table_name(�ֶ�1, �ֶ�2...) values(�ֶ�1ֵ, �ֶ�2ֵ...);
		//m_pCommand->CommandText = "delete from " + (_bstr_t)table + " where UID=\'" + (_bstr_t)uid +"\'";
		m_pCommand->CommandText = "insert into card(uid,money) values(\'"+(_bstr_t)uid+"\',\'"+(_bstr_t)money+"\');";
		//�ǳ��ؼ���һ�䣬����������������������							
		m_pCommand->ActiveConnection = this->m_pConn;
	  
		//ִ��ɾ������
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}catch(_com_error &e){
		if((m_pCommand!=NULL) && (m_pCommand->State)){
			m_pCommand.Release();//�ͷ�����
			m_pCommand=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(2,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"����ɹ�");
	return mysql_statu;
}

/*
*@function ���ݿ����
*@param CString,CString
*@retval fool
*/
CMySQL_Statu CAdoMySQLHelper::MYSQL_Update(const CString& uid,const CString& money){
	_RecordsetPtr m_pRecordset;
	try{
		HRESULT hr = m_pRecordset.CreateInstance("ADODB.Recordset");
		if(FAILED(hr)){
			CMySQL_Statu mysql_statu(3,"����recordsetָ��ʧ�ܣ�����!");
			return mysql_statu;
		}
		_variant_t table_name = "card"; 

		m_pRecordset->Open( table_name,
							_variant_t((IDispatch*)(this->m_pConn),true),
							adOpenForwardOnly,
							adLockOptimistic,
							adCmdTableDirect);
		
		if(!m_pRecordset->ADOEOF){
			//�ƶ��α굽��ǰ����ADOBOF
			m_pRecordset->MoveFirst();
			bool flag = false;
			//ѭ���������ݼ������Ի�ȡÿ�м�¼ÿ���ֶε�����ֵ
			while(!m_pRecordset->ADOEOF){
				//Ѱ������ĳ�������ļ�¼
				CString str = (LPCSTR)_bstr_t(m_pRecordset->Fields->GetItem("uid")->Value);
				if(uid == str){
					//_variant_t var1;
					//var1.ChangeType(VT_INT);
					//var1.intVal = 123;
					//m_pRecordset->Fields->GetItem("�ֶ���")->Value = var1;
					
					//��ʽ��
					//_variant_t var2;
					//var2.ChangeType(VT_BOOL);
					//var2.boolVal == VARIANT_TRUE;
					//m_pRecordset->Fields->GetItem("�ֶ���")->PutValue(var2);
					
					//��ʽ��
					_variant_t var3 = (LPCTSTR)money;
					m_pRecordset->PutCollect("money", var3);
					flag = true;
					//�������ƶ��α�ǰִ�и���!!
					m_pRecordset->Update();
					break;
				}
				m_pRecordset->MoveNext();
			}// end of while(!m_pRecordset->ADOEOF)
			//m_pRecordset->Close(); //�ر�����
			m_pRecordset.Release();//�ͷ�����
			m_pRecordset=NULL;
			if(!flag){
				CMySQL_Statu mysql_statu(3,"�޸�uid��Ӧ��ֵ");
				return mysql_statu;
			}
		}else{
			CMySQL_Statu mysql_statu(3,"�޸�uid��Ӧ��ֵ");
			return mysql_statu;
		}

	}catch(_com_error &e){
		if((m_pRecordset!=NULL) && (m_pRecordset->State)){
			//m_pRecordset->Close(); //�ر�����
			m_pRecordset.Release();//�ͷ�����
			m_pRecordset=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(3,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"�޸ĳɹ�");
	return mysql_statu;
}
/*
*@function ���ݿ��ѯ
*@param CString,CString
*@retval fool
*/
CMySQL_Statu CAdoMySQLHelper::MYSQL_Query(const CString& uid,CString& money){
	_RecordsetPtr m_pRecordset;
	try{
		HRESULT hr = m_pRecordset.CreateInstance("ADODB.Recordset");
		if(FAILED(hr)){
			CMySQL_Statu mysql_statu(4,"����recordsetָ��ʧ�ܣ�����!");
			return mysql_statu;
		}

		CString strsql = "select * from card where uid = \'"+uid+"\';";
		_variant_t sql=(LPCTSTR)strsql; 

		m_pRecordset->Open( sql,
							_variant_t((IDispatch*)(this->m_pConn),true),
							adOpenForwardOnly,
							adLockReadOnly,
							adCmdText);
		//�����ӣ���ȡ���ݼ�
		if(!m_pRecordset->ADOEOF){
			//�ƶ��α굽��ǰ����ADOBOF
			m_pRecordset->MoveFirst();

			//ѭ���������ݼ������Ի�ȡÿ�м�¼ÿ���ֶε�����ֵ
			bool flag = false;
			while(!m_pRecordset->ADOEOF){
				//��ʽһ
				if(flag){
					if((m_pRecordset!=NULL) && (m_pRecordset->State)){
						m_pRecordset->Close(); //�ر�����
						m_pRecordset.Release();//�ͷ�����
						m_pRecordset=NULL;
					}
					CMySQL_Statu mysql_statu(4,"�����ظ��ֶΣ�����!");
					return mysql_statu;
				}else{
					flag = true;
				}	
				_variant_t var1 = m_pRecordset->Fields->GetItem("money")->Value;
				//ȡint����
				//var1.ChangeType(VT_INT);
				//int varInt = var1.intVal;

				//��ʽ��
				//_variant_t var2 = m_pRecordset->Fields->GetItem("�ֶ���")->GetValue();
				//ȡbool����
				//var2.ChangeType(VT_BOOL);
				//BOOL varBool = (var2.boolVal == VARIANT_FALSE);

				//��ʽ��
				//_variant_t var3 = m_pRecordset->GetCollect("�ֶ���");
				//ȡCString����

				money = (LPCSTR)_bstr_t(var1);
				//�α���ǰ�ƶ�

				m_pRecordset->MoveNext();
			} // end of while(!m_pRecordset->ADOEOF)


			m_pRecordset->Close(); //�ر�����
			m_pRecordset.Release();//�ͷ�����
			m_pRecordset=NULL;

			if(!flag){
				CMySQL_Statu mysql_statu(4,"�޸�uid��Ӧ�������¶�ȡ��Ƭ!");
				return mysql_statu;
			}

		}else{// end of if(!m_pRecordset->ADOEOF)
			if((m_pRecordset!=NULL) && (m_pRecordset->State)){
				m_pRecordset->Close(); //�ر�����
				m_pRecordset.Release();//�ͷ�����
				m_pRecordset=NULL;
			}
			CMySQL_Statu mysql_statu(4,"�޸�uid��Ӧ�������¶�ȡ��Ƭ!");
			return mysql_statu;
		}
	}catch(_com_error &e){
		if((m_pRecordset!=NULL) && (m_pRecordset->State)){
			m_pRecordset->Close(); //�ر�����
			m_pRecordset.Release();//�ͷ�����
			m_pRecordset=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(4,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"��ѯ�ɹ�");
	return mysql_statu;
}