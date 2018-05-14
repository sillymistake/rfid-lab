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
			CMySQL_Statu mysql_statu(1,"创建_ConnectionPtr智能指针失败");
			return mysql_statu;
		}
		
		//设置连接超时时间  
		//this->m_pConn->ConnectionTimeout = 60000;
		//设置执行命令超时时间
		//this->m_pConn->CommandTimeout = 1200;

		this->m_pConn->Open("DSN=MySQL8.0;Server=localhost;Database=rfid",  //数据库连接字符串
							"root",
							"",
								   adModeUnknown);
		this->connect_state = true;

	}catch(_com_error &e){
		if((this->m_pConn!=NULL) && (this->m_pConn->State)){
			this->m_pConn->Close(); //关闭连接
			this->m_pConn.Release();//释放连接
			this->m_pConn=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(1,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"连接成功");
	return mysql_statu;
}
/*
*@function 断开数据库连接
*@param void
*@retval void
*/
void CAdoMySQLHelper::mySQL_close(){
	if((this->m_pConn!=NULL) && (this->m_pConn->State)){
		this->m_pConn->Close(); //关闭连接
		this->m_pConn.Release();//释放连接
		this->m_pConn=NULL;
	}
	CoUninitialize();
	//卸载数据库
}
/*
*@function 数据库插入
*@param CString,CString
*@retval fool
*/
CMySQL_Statu CAdoMySQLHelper::MYSQL_Insert(const CString& uid,const CString& money){
	_CommandPtr m_pCommand;
	try{
		HRESULT hr = m_pCommand.CreateInstance("ADODB.Command");
		if(FAILED(hr)){
			CMySQL_Statu mysql_statu(2,"创建_CommandPtr智能指针失败");
			return mysql_statu;
		}
		//定义为无参数
		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		vNULL.scode = DISP_E_PARAMNOTFOUND;	
		//注意文本格式数据需要使用引号，\'此处使用转义符；拼接字符串间合理使用空格！
		//insert into table_name(字段1, 字段2...) values(字段1值, 字段2值...);
		//m_pCommand->CommandText = "delete from " + (_bstr_t)table + " where UID=\'" + (_bstr_t)uid +"\'";
		m_pCommand->CommandText = "insert into card(uid,money) values(\'"+(_bstr_t)uid+"\',\'"+(_bstr_t)money+"\');";
		//非常关键的一句，将建立的连接与命令对象绑定							
		m_pCommand->ActiveConnection = this->m_pConn;
	  
		//执行删除命令
		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
	}catch(_com_error &e){
		if((m_pCommand!=NULL) && (m_pCommand->State)){
			m_pCommand.Release();//释放连接
			m_pCommand=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(2,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"插入成功");
	return mysql_statu;
}

/*
*@function 数据库更改
*@param CString,CString
*@retval fool
*/
CMySQL_Statu CAdoMySQLHelper::MYSQL_Update(const CString& uid,const CString& money){
	_RecordsetPtr m_pRecordset;
	try{
		HRESULT hr = m_pRecordset.CreateInstance("ADODB.Recordset");
		if(FAILED(hr)){
			CMySQL_Statu mysql_statu(3,"创建recordset指针失败，请检查!");
			return mysql_statu;
		}
		_variant_t table_name = "card"; 

		m_pRecordset->Open( table_name,
							_variant_t((IDispatch*)(this->m_pConn),true),
							adOpenForwardOnly,
							adLockOptimistic,
							adCmdTableDirect);
		
		if(!m_pRecordset->ADOEOF){
			//移动游标到最前，即ADOBOF
			m_pRecordset->MoveFirst();
			bool flag = false;
			//循环遍历数据集，可以获取每行记录每列字段的属性值
			while(!m_pRecordset->ADOEOF){
				//寻找满足某个条件的记录
				CString str = (LPCSTR)_bstr_t(m_pRecordset->Fields->GetItem("uid")->Value);
				if(uid == str){
					//_variant_t var1;
					//var1.ChangeType(VT_INT);
					//var1.intVal = 123;
					//m_pRecordset->Fields->GetItem("字段名")->Value = var1;
					
					//方式二
					//_variant_t var2;
					//var2.ChangeType(VT_BOOL);
					//var2.boolVal == VARIANT_TRUE;
					//m_pRecordset->Fields->GetItem("字段名")->PutValue(var2);
					
					//方式三
					_variant_t var3 = (LPCTSTR)money;
					m_pRecordset->PutCollect("money", var3);
					flag = true;
					//必须在移动游标前执行更新!!
					m_pRecordset->Update();
					break;
				}
				m_pRecordset->MoveNext();
			}// end of while(!m_pRecordset->ADOEOF)
			//m_pRecordset->Close(); //关闭连接
			m_pRecordset.Release();//释放连接
			m_pRecordset=NULL;
			if(!flag){
				CMySQL_Statu mysql_statu(3,"无该uid对应的值");
				return mysql_statu;
			}
		}else{
			CMySQL_Statu mysql_statu(3,"无该uid对应的值");
			return mysql_statu;
		}

	}catch(_com_error &e){
		if((m_pRecordset!=NULL) && (m_pRecordset->State)){
			//m_pRecordset->Close(); //关闭连接
			m_pRecordset.Release();//释放连接
			m_pRecordset=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(3,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"修改成功");
	return mysql_statu;
}
/*
*@function 数据库查询
*@param CString,CString
*@retval fool
*/
CMySQL_Statu CAdoMySQLHelper::MYSQL_Query(const CString& uid,CString& money){
	_RecordsetPtr m_pRecordset;
	try{
		HRESULT hr = m_pRecordset.CreateInstance("ADODB.Recordset");
		if(FAILED(hr)){
			CMySQL_Statu mysql_statu(4,"创建recordset指针失败，请检查!");
			return mysql_statu;
		}

		CString strsql = "select * from card where uid = \'"+uid+"\';";
		_variant_t sql=(LPCTSTR)strsql; 

		m_pRecordset->Open( sql,
							_variant_t((IDispatch*)(this->m_pConn),true),
							adOpenForwardOnly,
							adLockReadOnly,
							adCmdText);
		//打开连接，获取数据集
		if(!m_pRecordset->ADOEOF){
			//移动游标到最前，即ADOBOF
			m_pRecordset->MoveFirst();

			//循环遍历数据集，可以获取每行记录每列字段的属性值
			bool flag = false;
			while(!m_pRecordset->ADOEOF){
				//方式一
				if(flag){
					if((m_pRecordset!=NULL) && (m_pRecordset->State)){
						m_pRecordset->Close(); //关闭连接
						m_pRecordset.Release();//释放连接
						m_pRecordset=NULL;
					}
					CMySQL_Statu mysql_statu(4,"出现重复字段，请检查!");
					return mysql_statu;
				}else{
					flag = true;
				}	
				_variant_t var1 = m_pRecordset->Fields->GetItem("money")->Value;
				//取int类型
				//var1.ChangeType(VT_INT);
				//int varInt = var1.intVal;

				//方式二
				//_variant_t var2 = m_pRecordset->Fields->GetItem("字段名")->GetValue();
				//取bool类型
				//var2.ChangeType(VT_BOOL);
				//BOOL varBool = (var2.boolVal == VARIANT_FALSE);

				//方式三
				//_variant_t var3 = m_pRecordset->GetCollect("字段名");
				//取CString类型

				money = (LPCSTR)_bstr_t(var1);
				//游标向前移动

				m_pRecordset->MoveNext();
			} // end of while(!m_pRecordset->ADOEOF)


			m_pRecordset->Close(); //关闭连接
			m_pRecordset.Release();//释放连接
			m_pRecordset=NULL;

			if(!flag){
				CMySQL_Statu mysql_statu(4,"无该uid对应金额，请重新读取卡片!");
				return mysql_statu;
			}

		}else{// end of if(!m_pRecordset->ADOEOF)
			if((m_pRecordset!=NULL) && (m_pRecordset->State)){
				m_pRecordset->Close(); //关闭连接
				m_pRecordset.Release();//释放连接
				m_pRecordset=NULL;
			}
			CMySQL_Statu mysql_statu(4,"无该uid对应金额，请重新读取卡片!");
			return mysql_statu;
		}
	}catch(_com_error &e){
		if((m_pRecordset!=NULL) && (m_pRecordset->State)){
			m_pRecordset->Close(); //关闭连接
			m_pRecordset.Release();//释放连接
			m_pRecordset=NULL;
		}
		CString msg = (LPCSTR)e.Description();
		CMySQL_Statu mysql_statu(4,msg);
		return mysql_statu;
	}
	CMySQL_Statu mysql_statu(0,"查询成功");
	return mysql_statu;
}