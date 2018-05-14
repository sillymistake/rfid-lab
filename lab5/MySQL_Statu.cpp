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
CMySQL_Statu::CMySQL_Statu(const long& type,const CString &s){
	this->error_str = s;
	this->status_type = type;
}
void CMySQL_Statu::setString(const CString &s){
	this->error_str = s;
}
void CMySQL_Statu::settype(const long& type){
	this->status_type = type;
}
CString CMySQL_Statu::toCString(){
	return this->error_str;
}
long CMySQL_Statu::getType(){
	return this->status_type;
}
CMySQL_Statu::~CMySQL_Statu()
{

}
