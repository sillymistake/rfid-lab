// Debugger.cpp : implementation file
//




#include "stdafx.h"
#include "lab1.h"
#include "Debugger.h"
#include "ZM124U.h"

//#include "AFX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog


CDebugger::CDebugger(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugger::IDD, pParent)
{
	DeleteFile("record.txt");
	//{{AFX_DATA_INIT(CDebugger)
	//}}AFX_DATA_INIT
}


void CDebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugger)
	DDX_Control(pDX, IDC_EDIT7, m_record);
	DDX_Control(pDX, IDC_CONMONEY, m_consume);
	DDX_Control(pDX, IDC_BALANCE, m_balance);
	DDX_Control(pDX, IDC_PAGE, m_page);
	DDX_Control(pDX, IDC_BLOCK, m_block);
	DDX_Control(pDX, IDC_STATE, m_state);
	DDX_Control(pDX, IDC_REMONEY, m_recharge);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugger, CDialog)
	//{{AFX_MSG_MAP(CDebugger)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnInit)
	ON_BN_CLICKED(IDC_BUTTON4, OnQuery)
	ON_BN_CLICKED(IDC_BUTTON5, OnRecharge)
	ON_BN_CLICKED(IDC_BUTTON6, OnConsume)
	ON_BN_CLICKED(IDC_BUTTON7, OnClearRecord)
	ON_BN_CLICKED(IDC_BUTTON8, OnQueryRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugger message handlers

void CDebugger::OnButton3() 
{
	// TODO: Add your control notification handler code here
	unsigned char uid_[10];	
	int* uid_len = new int;
	int state = find_14443(uid_, uid_len);
	CString str;
	str.Format("%d",state);
	m_edit2.SetWindowText(str);
	if(state == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid_[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		uid = str;
		m_edit1.SetWindowText("寻卡成功");
	}
	else {
		m_edit1.SetWindowText("寻卡失败");
	}
}

void CDebugger::OnButton2() 
{
	// TODO: Add your control notification handler code here
	int state = IDD_PowerOn();
	if(state == 0) {
		m_edit1.SetWindowText("设备开启成功");
	}
	else {
		m_edit1.SetWindowText("设备开启失败");
	}
}

#define page 14	
#define block 0

void CDebugger::OnInit()
{	
	if(!this->helper.r_connect_state()){
		this->helper.MYSQL_Connect();
	}
	//先获取uid
	unsigned char uid[10];
	int* uid_len = new int;
	int state1 = find_14443(uid, uid_len);
	CString str;
	str.Format("%d",state1);
	m_edit2.SetWindowText(str);
	if(state1 == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		m_edit1.SetWindowText("寻卡成功");
	}
	else {
		m_edit1.SetWindowText("寻卡失败");
		return;
	}
	//读取金额
	CString balance;
	m_balance.GetWindowText(balance);
	if(balance.IsEmpty()){
		//弹出对话框显示错误
		MessageBox("请输入初始金额");
		return;
	}
	clearView();
	unsigned char des_data[1024];
	int* des_len = new int;
	int state;
	CString rec = "初始金额: ";
	rec = rec + balance;
	unsigned char pwdtype = 0x0B;
	CString pwd="FFFFFFFFFFFF";
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
	long account = cstring_to_long(balance);
	state = write_account(page, block, pwdtype, pwd_c, account); //写卡
	if(!state){
		//弹出对话框显示写入成功
		MessageBox("写卡成功");
		CString tmp = "";
		CMySQL_Statu sql_state = helper.MYSQL_Query(uid,tmp);
		if(!sql_state.getType()){ //存在->更新
			CMySQL_Statu sql_state1 = helper.MYSQL_Update(uid,balance);
			MessageBox("钱包初始化成功");
			writeRecord(rec);
			OnQueryRecord();
			return;
		}
		else{ //不存在->插入
			CMySQL_Statu sql_state1 = helper.MYSQL_Insert(uid,balance);
			if(!sql_state1.getType()){
				MessageBox("钱包初始化成功");
				writeRecord(rec);
				OnQueryRecord();
				return;
			}else{
				MessageBox(sql_state1.toCString());
				return;
			}
		}
	}
	//弹出状态码state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);

}

void CDebugger::OnQuery() 
{
	// TODO: Add your control notification handler code here
	//先获取uid
	unsigned char uid[10];
	int* uid_len = new int;
	int state1 = find_14443(uid, uid_len);
	CString str;
	str.Format("%d",state1);
	m_edit2.SetWindowText(str);
	if(state1 == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		m_edit1.SetWindowText("寻卡成功");
	}
	else {
		m_edit1.SetWindowText("寻卡失败");
		return;
	}
	long account;
	unsigned char pwdtype = 0x0B;
	CString pwd = "FFFFFFFFFFFF";
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
	int state;
	state = read_account(page, block, pwdtype, pwd_c, &account);
	if(!state) {
		CString money;
		if(!helper.MYSQL_Query(uid,money).getType()){
			CString str_;
			str_.Format("%d", account);
			m_balance.SetWindowText(money);
			if(money==str_){
				MessageBox("余额查询成功!");	
			}else{
				MessageBox("卡上余额与数据库数据不符，恢复数据库数据!");
				account = cstring_to_long(money);
				state = write_account(page, block, pwdtype, pwd_c, account);
				if(!state){
					//弹出对话框显示写入成功
					CString rec = "卡上余额与数据库数据不符，恢复数据库数据:"+money;
					writeRecord(rec);
					OnQueryRecord();
					return;
				}else{
					MessageBox("恢复数据失败!");
					CString str_;
					str_.Format("%d",state);
					m_state.SetWindowText(str_);
				}
			}
			return;
		}
	}
	//弹出状态码state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);
}

void CDebugger::OnRecharge() 
{
	// TODO: Add your control notification handler code here

	//先获取uid
	unsigned char uid[10];
	int* uid_len = new int;
	int state1 = find_14443(uid, uid_len);
	CString str;
	str.Format("%d",state1);
	m_edit2.SetWindowText(str);
	if(state1 == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		m_edit1.SetWindowText("寻卡成功");
	}
	else {
		m_edit1.SetWindowText("寻卡失败");
		return;
	}

	CString recharge_s;
	CString rec = "充值金额: ";
	m_recharge.GetWindowText(recharge_s);
	rec = rec + recharge_s;
	if(recharge_s.IsEmpty()){
		//弹出对话框显示错误
		MessageBox("请输入充值金额");
		return;
	}
	long recharge_money = cstring_to_long(recharge_s);
	unsigned char pwdtype = 0x0B;
	CString pwd = "FFFFFFFFFFFF";
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
	long account;
	int state;
	state = read_account(page, block, pwdtype, pwd_c, &account);
	CString money;
	if(!state) {
		if(!helper.MYSQL_Query(uid,money).getType()){
			account = cstring_to_long(money);
			long sum = recharge_money + account;
			state = write_account(page, block, pwdtype, pwd_c, sum);
			if(!state) {
				money = "";
				money.Format("%d",sum);
				CMySQL_Statu mysql_statu = helper.MYSQL_Update(uid,money);
				if(!mysql_statu.getType()){
					MessageBox("充值成功!");
					writeRecord(rec);
					OnQueryRecord();
					return;
				}else{
					MessageBox(mysql_statu.toCString());
					return;
				}
				
			}
		}
		//弹出状态码state
		CString str_;
		str_.Format("%d",state);
		m_state.SetWindowText(str_);
			
	}
	//弹出状态码state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);
}

void CDebugger::OnConsume() 
{
	// TODO: Add your control notification handler code here
	//先获取uid
	unsigned char uid[10];
	int* uid_len = new int;
	int state1 = find_14443(uid, uid_len);
	CString str;
	str.Format("%d",state1);
	m_edit2.SetWindowText(str);
	if(state1 == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		m_edit1.SetWindowText("寻卡成功");
	}
	else {
		m_edit1.SetWindowText("寻卡失败");
		return;
	}

	CString consume_s;
	CString rec = "扣取金额: ";
	m_consume.GetWindowText(consume_s);
	rec = rec + consume_s;
	if(consume_s.IsEmpty()){
		//弹出对话框显示错误
		MessageBox("请输入扣取金额");
		return;
	}
	long consume_money = cstring_to_long(consume_s);
	unsigned char pwdtype = 0x0B;
	CString pwd = "FFFFFFFFFFFF";
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
	long account;
	int state;
	state = read_account(page, block, pwdtype, pwd_c, &account);
	CString money;
	if(!state) {
		if(!helper.MYSQL_Query(uid,money).getType()){
			account = cstring_to_long(money);
			long result = account - consume_money;
			if(result < 0) {
				MessageBox("余额不足，扣费失败!");
				return;
			}
			state = write_account(page, block, pwdtype, pwd_c, result);
			if(!state) {
				money = "";
				money.Format("%d",result);
				
				if(!helper.MYSQL_Update(uid,money).getType()){
					MessageBox("扣费成功!");
					writeRecord(rec);
					OnQueryRecord();
					return;
				}else{
					return;
				}
			}
			//弹出状态码state
			CString str_;
			str_.Format("%d",state);
			m_state.SetWindowText(str_);
		}
		
			
	}
	//弹出状态码state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);
	
}




unsigned char* CDebugger::cstring_to_unsignedchar(CString s) { //将字符串密码转换成字节流
	unsigned char *s_c = new unsigned char[s.GetLength()];
	s.MakeUpper();
	int j;
	int i;
	for(i = 0; i < s.GetLength(); i++){
		if(s[i] > 0x40){
			s_c[i] = (unsigned char)s[i] - 0x37;
		}else{
			char ch = s[i];
			s_c[i] = (unsigned char)atoi(&ch);
		}
	}
	unsigned char *s_c_1 = new unsigned char[s.GetLength()/2];

	for(j=0,i=0;j<s.GetLength()/2;){
		s_c_1[j] = (unsigned char)(s_c[i++]<<4);
		s_c_1[j++] |= s_c[i++];
	}
	return s_c_1;
}


long CDebugger::cstring_to_long(CString s) {//将字符串转换成数字
	long a = 0;
	for(int i = 0; i < s.GetLength(); i++){
			a = a * 10 + (long)(s[i] - '0');
	}
	return a;
}

void CDebugger::clearView()//清除界面显示的数据
{
	m_balance.SetWindowText("");
	m_consume.SetWindowText("");
	m_recharge.SetWindowText("");
	m_state.SetWindowText("");
}

void CDebugger::writeRecord(CString str) {
	str = str + "\r\n";
	CFile file("record.txt", CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	file.SeekToEnd();
	file.Write(str, str.GetLength());
	file.Close();
}


void CDebugger::OnClearRecord() 
{
	// TODO: Add your control notification handler code here
	DeleteFile("record.txt");
	m_record.SetWindowText("");
	MessageBox("记录清空成功");
}

void CDebugger::OnQueryRecord() 
{
	// TODO: Add your control notification handler code here
	m_record.SetWindowText("");
	CFile file("record.txt", CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	int nCount = file.GetLength();
	if(nCount == 0) {
		MessageBox("历史记录为空");
	}
	else {
		CString strPath;
		file.Read(strPath.GetBuffer(nCount), nCount);
		m_record.SetWindowText(strPath);
	}
	file.Close();
}


