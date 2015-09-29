// attackDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "attack.h"
#include "attackDlg.h"
#include "ProcessAttackDlg.h"

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CattackDlg �Ի���




CattackDlg::CattackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CattackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CattackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_interfaceLst);
}

BEGIN_MESSAGE_MAP(CattackDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CattackDlg::OnNMDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CattackDlg ��Ϣ�������

BOOL CattackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	vector<MyNetCard> interfaceArray;
	this->GetInterfaceArray(interfaceArray);
	DWORD style = m_interfaceLst.GetExtendedStyle();
	style |= LVS_EX_FULLROWSELECT;
	style |= LVS_EX_GRIDLINES;
	//style |= LVS_EX_CHECKBOXES;
	m_interfaceLst.SetExtendedStyle(style);
	m_interfaceLst.InsertColumn(0,"���",LVCFMT_LEFT,40);
	m_interfaceLst.InsertColumn(1,"Name",LVCFMT_LEFT, 200);
	m_interfaceLst.InsertColumn(2,"����",LVCFMT_LEFT, 250);
	m_interfaceLst.InsertColumn(3,"IP��ַ",LVCFMT_LEFT, 100);
	m_interfaceLst.InsertColumn(4,"��������",LVCFMT_LEFT,100);
	m_interfaceLst.InsertColumn(5,"����ID",LVCFMT_CENTER, 0);
	InitListData(interfaceArray);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CattackDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CattackDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CattackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CattackDlg::GetInterfaceArray(vector<MyNetCard> &interfaceArray)
{
	interfaceArray.clear();
	pcap_if_t	*alldevs;
	pcap_if_t	*dev;
	char		errbuf[PCAP_ERRBUF_SIZE] = {0};
	HKEY		key;
	char		subKey[1024] = {0};
	char		lpData[512] = {0};
	int         i = 0;
	DWORD		lpType;
	DWORD		cbData;
	char		ipAddr[20] = {0};
	char		netmask[20] = {0};

	if(pcap_findalldevs_ex(PCAP_SRC_IF_STRING,NULL,&alldevs,errbuf)==-1)
	{
		return;
	}
	MyNetCard  interface1;
	for(dev = alldevs; dev!=NULL; dev = dev->next)
	{
		interface1.netCardName = dev->name;
		interface1.description = "";
		if(dev->description)
		{
			char *start = strstr(dev->description,"\'");
			char *end = strstr(start+1,"\'");
			*end = '\0';
			interface1.description = start+1;
		}
		GetIp4Str(dev->addresses, ipAddr, netmask);
		interface1.ipaddr = ipAddr;
		interface1.netmask = netmask;
		//interface1.description = dev->description;
		//interface1.ipaddr      = inet_ntoa((in_addr)*(dev->addresses->addr));
		//interface1.netmask	  = inet_ntoa((in_addr)*(dev->addresses->netmask));
		interface1.friendName = "";
		sprintf(subKey,
			"SYSTEM\\CurrentControlSet\\Control\\Network\\"
			"{4D36E972-E325-11CE-BFC1-08002BE10318}\\%s\\Connection",
			dev->name+20);
		if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey,0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS)
		{
			cbData = 512;
			if(::RegQueryValueEx(key, "Name",NULL,&lpType,(LPBYTE)lpData,&cbData)==ERROR_SUCCESS)
			{
				lpData[cbData] = '\0';
				interface1.friendName = lpData;
			}
			::RegCloseKey(key);
		}
		interfaceArray.push_back(interface1);
	}
	pcap_freealldevs(alldevs);
}

void CattackDlg::InitListData(vector<MyNetCard> &interfaceArray)
{
	char num[25] = {0};
	for(unsigned int i=0; i<interfaceArray.size(); i++)
	{
		sprintf(num,"%d",i+1);
		m_interfaceLst.InsertItem(i, num);
		m_interfaceLst.SetItemText(i, 1, interfaceArray[i].friendName.c_str());
		m_interfaceLst.SetItemText(i, 2, interfaceArray[i].description.c_str());
		m_interfaceLst.SetItemText(i, 3, interfaceArray[i].ipaddr.c_str());
		m_interfaceLst.SetItemText(i, 4, interfaceArray[i].netmask.c_str());
		m_interfaceLst.SetItemText(i, 5, interfaceArray[i].netCardName.c_str());
	}
}

void CattackDlg::GetIp4Str(pcap_addr *address,char *ipAddr, char *netmask)
{
	//char result[20] = {0};
	memset(ipAddr,0, 20);
	memset(netmask,0, 20);
	pcap_addr *curAddr = address;
	while(curAddr)
	{
		if(curAddr->addr->sa_family == AF_INET)
		{
			char *temp = inet_ntoa(((struct sockaddr_in *)curAddr->addr)->sin_addr);
			strcpy(ipAddr,temp);
			temp = inet_ntoa((in_addr)((struct sockaddr_in*)curAddr->netmask)->sin_addr);
			strcpy(netmask, temp);
			break;
		}
		curAddr = curAddr->next;
	}
}

void CattackDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char	netId[MAX_PATH] = {0};
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int nItem = pNMListView->iItem;
	if(nItem>=0 && nItem < m_interfaceLst.GetItemCount())
	{
		m_interfaceLst.GetItemText(nItem,5,netId, MAX_PATH);
		CProcessAttackDlg dlg(netId, this);
		dlg.DoModal();
	}
	*pResult = 0;
}