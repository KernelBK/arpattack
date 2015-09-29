// attackDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "common.h"
#include <pcap.h>
#include<vector>
using namespace std;


// CattackDlg �Ի���
class CattackDlg : public CDialog
{
// ����
public:
	CattackDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ATTACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_interfaceLst;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void GetInterfaceArray(vector<MyNetCard> &interfaceArray);
	void InitListData(vector<MyNetCard> &interfaceArray);
	void GetIp4Str(pcap_addr *address,char *ipAddr, char *netmask);
};
