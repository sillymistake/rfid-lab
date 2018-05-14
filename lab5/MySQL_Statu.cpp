// MySQL_Statu.cpp: implementation of the CMySQL_Statu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lab1.h"
#include "MySQL_Statu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySQL_Statu::CMySQL_Statu(){
	this->status_type = 0;
	this->error_str = "";
}
CMySQL_Statu::CMySQL_Statu(const CString &s,const long& type){
	this->error_str = s;
	this->status_type = type;
}

CMySQL_Statu::~CMySQL_Statu()
{

}
