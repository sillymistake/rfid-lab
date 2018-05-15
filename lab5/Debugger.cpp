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
	DDX_Control(pDX, IDC_STOP_CLOCK, m_stop_clock);
	DDX_Control(pDX, IDC_RESET_TIME, m_reset_time);
	DDX_Control(pDX, IDC_START_CLOCK, m_start_clock);
	DDX_Control(pDX, IDC_SEC, m_second);
	DDX_Control(pDX, IDC_MINUTE, m_minute);
	DDX_Control(pDX, IDC_HOUR, m_hour);
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
	ON_BN_CLICKED(IDC_START_CLOCK, OnStartClock)
	ON_BN_CLICKED(IDC_STOP_CLOCK, OnStopClock)
	ON_BN_CLICKED(IDC_RESET_TIME, OnResetTime)
	ON_BN_CLICKED(IDC_QUERY_TIME, OnQueryTime)
	ON_WM_TIMER()
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
		m_edit1.SetWindowText("Ѱ���ɹ�");
	}
	else {
		m_edit1.SetWindowText("Ѱ��ʧ��");
	}
}

void CDebugger::OnButton2() 
{
	// TODO: Add your control notification handler code here
	int state = IDD_PowerOn();
	if(state == 0) {
		m_edit1.SetWindowText("�豸�����ɹ�");
	}
	else {
		m_edit1.SetWindowText("�豸����ʧ��");
	}
}

#define page 14	
#define block 0

void CDebugger::OnInit()
{	
	if(!this->helper.r_connect_state()){
		this->helper.MYSQL_Connect();
	}
	//�Ȼ�ȡuid
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
		m_edit1.SetWindowText("Ѱ���ɹ�");
	}
	else {
		m_edit1.SetWindowText("Ѱ��ʧ��");
		return;
	}
	//��ȡ���
	CString balance;
	m_balance.GetWindowText(balance);
	if(balance.IsEmpty()){
		//�����Ի�����ʾ����
		MessageBox("�������ʼ���");
		return;
	}
	clearView();
	unsigned char des_data[1024];
	int* des_len = new int;
	int state;
	CString rec = "��ʼ���: ";
	rec = rec + balance;
	unsigned char pwdtype = 0x0B;
	CString pwd="FFFFFFFFFFFF";
	unsigned char* pwd_c = cstring_to_unsignedchar(pwd);
	long account = cstring_to_long(balance);
	state = write_account(page, block, pwdtype, pwd_c, account); //д��
	if(!state){
		//�����Ի�����ʾд��ɹ�
		MessageBox("д���ɹ�");
		CString tmp = "";
		CMySQL_Statu sql_state = helper.MYSQL_Query(uid,tmp);
		if(!sql_state.getType()){ //����->����
			CMySQL_Statu sql_state1 = helper.MYSQL_Update(uid,balance);
			MessageBox("Ǯ����ʼ���ɹ�");
			writeRecord(rec);
			OnQueryRecord();
			return;
		}
		else{ //������->����
			CMySQL_Statu sql_state1 = helper.MYSQL_Insert(uid,balance);
			if(!sql_state1.getType()){
				MessageBox("Ǯ����ʼ���ɹ�");
				writeRecord(rec);
				OnQueryRecord();
				return;
			}else{
				MessageBox(sql_state1.toCString());
				return;
			}
		}
	}
	//����״̬��state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);

}

void CDebugger::OnQuery() 
{
	// TODO: Add your control notification handler code here
	//�Ȼ�ȡuid
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
		m_edit1.SetWindowText("Ѱ���ɹ�");
	}
	else {
		m_edit1.SetWindowText("Ѱ��ʧ��");
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
				MessageBox("����ѯ�ɹ�!");	
			}else{
				MessageBox("������������ݿ����ݲ������ָ����ݿ�����!");
				account = cstring_to_long(money);
				state = write_account(page, block, pwdtype, pwd_c, account);
				if(!state){
					//�����Ի�����ʾд��ɹ�
					CString rec = "������������ݿ����ݲ������ָ����ݿ�����:"+money;
					writeRecord(rec);
					OnQueryRecord();
					return;
				}else{
					MessageBox("�ָ�����ʧ��!");
					CString str_;
					str_.Format("%d",state);
					m_state.SetWindowText(str_);
				}
			}
			return;
		}
	}
	//����״̬��state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);
}

void CDebugger::OnRecharge() 
{
	// TODO: Add your control notification handler code here

	//�Ȼ�ȡuid
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
		m_edit1.SetWindowText("Ѱ���ɹ�");
	}
	else {
		m_edit1.SetWindowText("Ѱ��ʧ��");
		return;
	}

	CString recharge_s;
	CString rec = "��ֵ���: ";
	m_recharge.GetWindowText(recharge_s);
	rec = rec + recharge_s;
	if(recharge_s.IsEmpty()){
		//�����Ի�����ʾ����
		MessageBox("�������ֵ���");
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
					MessageBox("��ֵ�ɹ�!");
					writeRecord(rec);
					OnQueryRecord();
					return;
				}else{
					MessageBox(mysql_statu.toCString());
					return;
				}
				
			}
		}
		//����״̬��state
		CString str_;
		str_.Format("%d",state);
		m_state.SetWindowText(str_);
			
	}
	//����״̬��state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);
}

void CDebugger::OnConsume() 
{
	// TODO: Add your control notification handler code here
	//�Ȼ�ȡuid
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
		m_edit1.SetWindowText("Ѱ���ɹ�");
	}
	else {
		m_edit1.SetWindowText("Ѱ��ʧ��");
		return;
	}

	CString consume_s;
	CString rec = "��ȡ���: ";
	m_consume.GetWindowText(consume_s);
	rec = rec + consume_s;
	if(consume_s.IsEmpty()){
		//�����Ի�����ʾ����
		MessageBox("�������ȡ���");
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
				MessageBox("���㣬�۷�ʧ��!");
				return;
			}
			state = write_account(page, block, pwdtype, pwd_c, result);
			if(!state) {
				money = "";
				money.Format("%d",result);
				
				if(!helper.MYSQL_Update(uid,money).getType()){
					MessageBox("�۷ѳɹ�!");
					writeRecord(rec);
					OnQueryRecord();
					return;
				}else{
					return;
				}
			}
			//����״̬��state
			CString str_;
			str_.Format("%d",state);
			m_state.SetWindowText(str_);
		}
		
			
	}
	//����״̬��state
	CString str_;
	str_.Format("%d",state);
	m_state.SetWindowText(str_);
	
}
void CDebugger::OnResetTime() 
{
	// TODO: Add your control notification handler code here
	
	long time = 0;
	CString str;
	m_hour.GetWindowText(str);
	time+=cstring_to_long(str);
	
	time*=60;
	m_minute.GetWindowText(str);
	long time_ = cstring_to_long(str);
	if(time_>=60){
		MessageBox("���벻�淶��������");
		return;
	}
	time+=time_;
	time*=60;
	m_second.GetWindowText(str);
	time_ = cstring_to_long(str);
	if(time_>=60){
		MessageBox("���벻�淶��������");
		return;
	}
	time+=time_;
	//��ó�ʼ��ʱ��
	
	//�Ȼ�ȡuid
	/*unsigned char uid[10];
	int* uid_len = new int;
	int state1 = find_14443(uid, uid_len);
	str="";
	str.Format("%d",state1);
	m_edit2.SetWindowText(str);
	if(state1 == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		m_edit1.SetWindowText("Ѱ���ɹ�");
	}
	else {
		m_edit1.SetWindowText("Ѱ��ʧ��");
		return;
	}*/
	uid="ffffffff";
	
	str = "";
	str.Format("%d",time);
	CString rec = "��ʼ����ʣ��: ";
	rec = rec + str;

	//������д�����ݿ�
	CString tmp = "";
	CMySQL_Statu sql_state = helper.MYSQL_net_Query(uid,tmp);
	if(!sql_state.getType()){ //����->����
		sql_state = helper.MYSQL_net_Update(uid,str);
		if(!sql_state.getType()){
			MessageBox("������ʼ���ɹ�");
			writeRecord(rec);
			OnQueryRecord();
			m_start_clock.EnableWindow(1);
			return;
		}else{
			MessageBox(sql_state.toCString());
			return;
		}
	}
	else{ //������->����
		sql_state = helper.MYSQL_net_Insert(uid,str);
		if(!sql_state.getType()){
			MessageBox("������ʼ���ɹ�");
			writeRecord(rec);
			OnQueryRecord();
			m_stop_clock.EnableWindow(1);
			m_start_clock.EnableWindow(1);
			return;
		}else{
			MessageBox(sql_state.toCString());
			return;
		}
	}
	
	
}
void CDebugger::OnQueryTime() 
{
	// TODO: Add your control notification handler code here
	CString tmp;

	/*unsigned char uid[10];
	int* uid_len = new int;
	int state1 = find_14443(uid, uid_len);
	str="";
	str.Format("%d",state1);
	m_edit2.SetWindowText(str);
	if(state1 == 0) {
		int b = 0;
		for(int i=0; i<*uid_len; i++){
			b=b*256+(int)uid[i];
		}
		str.Format(("%x"),b);
		m_edit2.SetWindowText(str);
		m_edit1.SetWindowText("Ѱ���ɹ�");
	}
	else {
		m_edit1.SetWindowText("Ѱ��ʧ��");
		return;
	}*/

	uid = "ffffffff";
	CMySQL_Statu sql_state = helper.MYSQL_net_Query(uid,tmp);
	if(!sql_state.getType()){
		m_start_clock.EnableWindow(1);
		long time = cstring_to_long(tmp);
		long second = time % 60;
		time/=60;
		CString str = "";
		str.Format("%d",second);
		m_second.SetWindowText(str);	
		second = time % 60;
		time/=60;
		CString str1 = "";
		str1.Format("%d",second);
		m_minute.SetWindowText(str1);
		CString str2 = "";
		str2.Format("%d",time);
		m_hour.SetWindowText(str2);
		return;
	}else{
		MessageBox(sql_state.toCString());
		return;
	}
}

#define SCANTIMER_ID 1
#define SCANTIMER 1//1ms

void CDebugger::OnStartClock() 
{
	// TODO: Add your control notification handler code here
	CString tmp = "";
	CMySQL_Statu sql_state = helper.MYSQL_net_Query(uid,tmp);
	if(!sql_state.getType()){
		m_ActiveTimer = SetTimer(SCANTIMER_ID, SCANTIMER * 1000, NULL);
		m_reset_time.EnableWindow(0);
		m_start_clock.EnableWindow(0);
		m_stop_clock.EnableWindow(1);
		CString rec = "������ʼ��ʱ��: ";
		CTime m_time;  
		m_time=CTime::GetCurrentTime();
		CString time_str = m_time.Format(_T("%Y-%m-%d %H:%M:%S %A"));
		rec = rec + time_str;
		writeRecord(rec);
		rec = "ʣ��ʱ��Ϊ:";
		long time = cstring_to_long(tmp);
		long second = time % 60;
		time/=60;
		CString str = "";
		str.Format("%d",second);
		m_second.SetWindowText(str);	
		second = time % 60;
		time/=60;
		CString str1 = "";
		str1.Format("%d",second);
		m_minute.SetWindowText(str1);
		CString str2 = "";
		str2.Format("%d",time);
		m_hour.SetWindowText(str2);
		rec = rec + str2 + "Сʱ" + str1 + "��" + str + "��";
		writeRecord(rec);
		OnQueryRecord();
		return;
	}else{
		MessageBox(sql_state.toCString());
		return;
	}
}	

void CDebugger::OnStopClock() 
{
	// TODO: Add your control notification handler code here
	KillTimer(m_ActiveTimer);
	m_reset_time.EnableWindow(1);
	m_start_clock.EnableWindow(1);
	m_stop_clock.EnableWindow(0);
	CString tmp = "";
	CMySQL_Statu sql_state = helper.MYSQL_net_Query(uid,tmp);
	if(!sql_state.getType()){
		CString rec = "����������ʱ��: ";
		CTime m_time;  
		m_time=CTime::GetCurrentTime();
		CString time_str = m_time.Format(_T("%Y-%m-%d %H:%M:%S %A"));
		rec = rec + time_str;
		writeRecord(rec);
		rec = "ʣ��ʱ��Ϊ:";
		long time = cstring_to_long(tmp);
		long second = time % 60;
		time/=60;
		CString str = "";
		str.Format("%d",second);
		m_second.SetWindowText(str);	
		second = time % 60;
		time/=60;
		CString str1 = "";
		str1.Format("%d",second);
		m_minute.SetWindowText(str1);
		CString str2 = "";
		str2.Format("%d",time);
		m_hour.SetWindowText(str2);
		rec = rec + str2 + "Сʱ" + str1 + "��" + str + "��";
		writeRecord(rec);
		OnQueryRecord();
		return;
	}else{
		MessageBox(sql_state.toCString());
		return;
	}
}






unsigned char* CDebugger::cstring_to_unsignedchar(CString s) { //���ַ�������ת�����ֽ���
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


long CDebugger::cstring_to_long(CString s) {//���ַ���ת��������
	long a = 0;
	for(int i = 0; i < s.GetLength(); i++){
			a = a * 10 + (long)(s[i] - '0');
	}
	return a;
}

void CDebugger::clearView()//���������ʾ������
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
	MessageBox("��¼��ճɹ�");
}

void CDebugger::OnQueryRecord() 
{
	// TODO: Add your control notification handler code here
	m_record.SetWindowText("");
	CFile file("record.txt", CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	int nCount = file.GetLength();
	if(nCount == 0) {
		MessageBox("��ʷ��¼Ϊ��");
	}
	else {
		CString strPath;
		file.Read(strPath.GetBuffer(nCount), nCount);
		m_record.SetWindowText(strPath);
	}
	file.Close();
}





void CDebugger::OnTimer(UINT nIDEvent) 
{
    // �ڴ������Ϣ�����������/�����Ĭ��ֵ
    switch (nIDEvent){
	case SCANTIMER_ID:{
            // do something here
			CString time_str;
			CMySQL_Statu sql_state = this->helper.MYSQL_net_Query(uid,time_str);
			if(!sql_state.getType()){
				long time = cstring_to_long(time_str);
				if(time==0){
					KillTimer(m_ActiveTimer);
					MessageBox("���������ֵ");
					CString rec = "����������ʱ��: ";
					CTime m_time;  
					m_time=CTime::GetCurrentTime();
					time_str = m_time.Format(_T("%Y-%m-%d %H:%M:%S %A"));
					rec = rec + time_str;
					writeRecord(rec);
					rec = "ʣ��ʱ��Ϊ:0";
					writeRecord(rec);
					OnQueryRecord();
					m_reset_time.EnableWindow(1);
					m_start_clock.EnableWindow(1);
					m_stop_clock.EnableWindow(0);
				}else{
					time--;
					time_str = "";
					time_str.Format("%d",time);
					CMySQL_Statu sql_state1 = helper.MYSQL_net_Update(uid,time_str);
					if(sql_state.getType()){
						MessageBox(sql_state.toCString());
						KillTimer(m_ActiveTimer);
						break;
					}
				}
				long second = time % 60;
				time/=60;
				CString str = "";
				str.Format("%d",second);
				m_second.SetWindowText(str);
				second = time % 60;
				time/=60;
				str = "";
				str.Format("%d",second);
				m_minute.SetWindowText(str);
				str = "";
				str.Format("%d",time);
				m_hour.SetWindowText(str);
			}else{
				MessageBox(sql_state.toCString());
				KillTimer(m_ActiveTimer);
			}

            break;}
        default:
            break;
    }

    CDialog::OnTimer(nIDEvent);

}
