// AppDev.cpp : implementation file
//

#include "stdafx.h"
#include "lab1.h"
#include "AppDev.h"
#include "ZM124U.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


unsigned char pwdtype;

/////////////////////////////////////////////////////////////////////////////
// CAppDev dialog


CAppDev::CAppDev(CWnd* pParent /*=NULL*/)
	: CDialog(CAppDev::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppDev)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAppDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppDev)
	DDX_Control(pDX, IDC_COMBO2, m_block);
	DDX_Control(pDX, IDC_COMBO1, m_page);
	DDX_Control(pDX, IDC_pwd, m_pwd);
	DDX_Control(pDX, IDC_block3c, m_block3c);
	DDX_Control(pDX, IDC_block3b, m_block3b);
	DDX_Control(pDX, IDC_block3a, m_block3a);
	DDX_Control(pDX, IDC_block2, m_block2);
	DDX_Control(pDX, IDC_block1, m_block1);
	DDX_Control(pDX, IDC_block0, m_block0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAppDev, CDialog)
	//{{AFX_MSG_MAP(CAppDev)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_pwda, Onpwda)
	ON_BN_CLICKED(IDC_pwdb, Onpwdb)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonDefaultPassword)
	ON_BN_CLICKED(IDC_BUTTON2, OnReadPage)
	ON_BN_CLICKED(IDC_BUTTON4, OnWriteblock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppDev message handlers

void CAppDev::clearView()
{
	m_block0.SetWindowText("");
	m_block1.SetWindowText("");
	m_block2.SetWindowText("");
	m_block3a.SetWindowText("");
	m_block3b.SetWindowText("");
	m_block3c.SetWindowText("");
}


void CAppDev::OnButton3() 
{
	// TODO: Add your control notification handler code here
	//page, block

	CString page_s;
	CString block_s;
	CString pwd;

	clearView();

	m_pwd.GetWindowText(pwd);
	if(pwdtype != 0x0A && pwdtype != 0x0B) {
		//m_pwd.SetWindowText("请选择密钥类型!");
		MessageBox("请选择密钥类型");
		return;
	}
	if(pwd.IsEmpty()){
		//m_pwd.SetWindowText("密码不能为空!");
		MessageBox("密码不能为空");
		return;
	}

	m_page.GetWindowText(page_s);
	if(page_s.IsEmpty()){
		//m_block0.SetWindowText("扇区不能为空!");
		MessageBox("扇区不能为空");
		return;
	}
	int page = cstring_to_int(page_s);
	m_block.GetWindowText(block_s);
	if(block_s.IsEmpty()){
		//m_block0.SetWindowText("块不能为空!");
		MessageBox("块不能为空");
		return;
	}
	int block = cstring_to_int(block_s);
	
	unsigned char des_data[1024];
	int* des_len = new int;
	int state;
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
    state = read_block(page,block,pwdtype,pwd_c,des_data,des_len);
	CString str_;
	str_.Format("%d",state);
	m_block3a.SetWindowText(str_);
	if(!state){
		CString str = "";
		if(block == 3) {
			for(int k = 0; k < *des_len; k++) {
				CString s1;
				s1.Format("%02x", des_data[k]);
				str += s1;
				if(k == 5) {
					m_block3a.SetWindowText(str);
					str.Empty();
				}
				else if(k == 9) {
					m_block3b.SetWindowText(str);
					str.Empty();
				}
			}
			m_block3c.SetWindowText(str);
		} else {
			for(int k = 0; k < *des_len; k++) {
				CString s1;
				s1.Format("%02x", des_data[k]);
				str += s1;
			}
			if(block == 2) {
				m_block2.SetWindowText(str);
			}
			if(block == 1) {
				m_block1.SetWindowText(str);
			}
			if(block == 0) {
				m_block0.SetWindowText(str);
			}
		}
	}

}

void CAppDev::OnReadPage() 
{
	// TODO: Add your control notification handler code here
	CString page_s;
	CString block_s;
	CString pwd;
	
	clearView();

	m_pwd.GetWindowText(pwd);
	if(pwdtype != 0x0A && pwdtype != 0x0B) {
		//m_pwd.SetWindowText("请选择密钥类型!");
		MessageBox("请选择密钥类型");
		return;
	}
	if(pwd.IsEmpty()){
		//m_pwd.SetWindowText("密码不能为空!");
		MessageBox("密码不能为空");
		return;
	}

	m_page.GetWindowText(page_s);
	if(page_s.IsEmpty()){
		//m_block0.SetWindowText("扇区不能为空!");
		MessageBox("扇区不能为空");
		return;
	}
	int page = cstring_to_int(page_s);

	unsigned char des_data[1024];
	int* des_len = new int;
	int state;
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
	for(int block = 0 ; block < 4; block++){
		state = read_block(page,block,pwdtype,pwd_c,des_data,des_len);
		CString str_;
		str_.Format("%d",state);
		m_block3a.SetWindowText(str_);
		if(!state){
			CString str = "";
			if(block == 3) {
				for(int k = 0; k < *des_len; k++) {
					CString s1;
					s1.Format("%02x", des_data[k]);
					str += s1;
					if(k == 5) {
						m_block3a.SetWindowText(str);
						str.Empty();
					}
					else if(k == 9) {
						m_block3b.SetWindowText(str);
						str.Empty();
					}
				}
				m_block3c.SetWindowText(str);
			} else {
				for(int k = 0; k < *des_len; k++) {
					CString s1;
					s1.Format("%02x", des_data[k]);
					str += s1;
				}
				if(block == 2) {
					m_block2.SetWindowText(str);
				}
				if(block == 1) {
					m_block1.SetWindowText(str);
				}
				if(block == 0) {
					m_block0.SetWindowText(str);
				}
			}
			str.Empty();
		}
	}
	
}
void CAppDev::OnWriteblock() 
{
	// TODO: Add your control notification handler code here
	CString page_s;
	CString block_s;
	CString pwd;

	
	m_pwd.GetWindowText(pwd);
	if(pwdtype != 0x0A && pwdtype != 0x0B) {
		//m_pwd.SetWindowText("请选择密钥类型!");
		MessageBox("请选择密钥类型");
		return;
	}
	if(pwd.IsEmpty()){
		//m_pwd.SetWindowText("密码不能为空!");
		MessageBox("密码不能为空");
		return;
	}

	m_page.GetWindowText(page_s);
	if(page_s.IsEmpty()){
		//m_block0.SetWindowText("扇区不能为空!");
		MessageBox("扇区不能为空");
		return;
	}
	int page = cstring_to_int(page_s);
	m_block.GetWindowText(block_s);
	if(block_s.IsEmpty()){
		//m_block0.SetWindowText("块不能为空!");
		MessageBox("块不能为空");
		return;
	}
	int block = cstring_to_int(block_s);
	
	unsigned char des_data[1024];
	int* des_len = new int;
	int state;
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
    //state = read_block(page,block,pwdtype,pwd_c,des_data,des_len);
	state = 0;
	CString str_;
	str_.Format("%d",state);
	m_block3a.SetWindowText(str_);
	if(!state){
		if(block == 3) {
			CString msrc_data0, src_data1, src_data2;
			for(int k = 0; k < *des_len; k++) {
				CString s1;
				s1.Format("%02x", des_data[k]);
				//str += s1;
				if(k<5) msrc_data0+=s1;
				else if(k<10) src_data1+=s1;
				else src_data2+=s1;
			}
			CString src_data0;
			m_block3a.GetWindowText(src_data0);
			if(src_data0.IsEmpty()){
				src_data0 = msrc_data0;
			}
			CString data = src_data0 + src_data1 + src_data2;
			if(data.GetLength()%2!=0){
				data.Insert(data.GetLength(),"0");
			}
			unsigned char* data_c = cstring_to_unsignedchar(data);
			int data_c_length = data.GetLength()/2;


			state = write_block(page,block,pwdtype,pwd_c,data_c,data_c_length);
			if(!state){
				//对话框显示写块成功
				MessageBox("写块成功");
			}else{
				CString s = "写块失败!3"+state; 
				MessageBox(s);
			}

		} else {
			CString str = "";
			for(int k = 0; k < *des_len; k++) {
				CString s1;
				s1.Format("%02x", des_data[k]);
				str += s1;
			}
			if(block == 2) { 
				CString data;
				m_block2.GetWindowText(data);
				if(data.IsEmpty()){
					data = str;
				}
				if(data.GetLength()%2!=0){
					data.Insert(data.GetLength(),"0");
				}
				unsigned char* data_c = cstring_to_unsignedchar(data);
				int data_c_length = data.GetLength()/2;
				state = write_block(page,block,pwdtype,pwd_c,data_c,data_c_length);
				if(!state){
					//对话框显示写块成功
					MessageBox("写块成功");
				}else{
					CString s = "写块失败!2"+state; 
					MessageBox(s);
				}
			}
			if(block == 1) {
				CString data;
				m_block1.GetWindowText(data);
				if(data.IsEmpty()){
					data = str;
				}
				if(data.GetLength()%2!=0){
					data.Insert(data.GetLength(),"0");
				}
				unsigned char* data_c = cstring_to_unsignedchar(data);
				int data_c_length = data.GetLength()/2;
				state = write_block(page,block,pwdtype,pwd_c,data_c,data_c_length);
				if(!state){
					//对话框显示写块成功
					MessageBox("写块成功");
				}else{
					CString s = "写块失败!1"+state; 
					MessageBox(s);
				}
			}
			if(block == 0) {
				CString data;
				m_block0.GetWindowText(data);
				if(data.IsEmpty()){
					data = str;
				}
				if(data.GetLength()%2!=0){
					data.Insert(data.GetLength(),"0");
				}
				unsigned char* data_c = cstring_to_unsignedchar(data);
				int data_c_length = data.GetLength()/2;
				state = write_block(block,page,pwdtype,pwd_c,data_c,data_c_length);
				if(!state){
					//对话框显示写块成功
					MessageBox("写块成功0");
				}else{
					CString str_;
					str_.Format("%d",state);
					m_block3a.SetWindowText(str_);
					CString s = "写块失败!0"+state; 
					MessageBox(s);
				}
			}
		}
	}
}
void CAppDev::Onpwda() 
{
	// TODO: Add your control notification handler code here
	pwdtype = 0x0A;
}

void CAppDev::Onpwdb() 
{
	// TODO: Add your control notification handler code here
	pwdtype = 0x0B;
	
}

void CAppDev::OnButtonDefaultPassword() 
{
	// TODO: Add your control notification handler code here
	m_pwd.SetWindowText("FFFFFFFFFFFF");
}

unsigned char* CAppDev::cstring_to_unsignedchar(CString s){
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

int CAppDev::cstring_to_int(CString s){
	int a = 0;
	for(int i = 0; i < s.GetLength(); i++){
			a = a * 10 + (int)(s[i] - '0');
	}
	return a;
}





