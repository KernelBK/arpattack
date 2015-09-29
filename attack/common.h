#ifndef COMMON_INCLUDE_H
#define COMMON_INCLUDE_H
#include <string>
#include <WinSock2.h>
using namespace std;

enum PROTOCOL_TYPE{MXL_UNKNOWN_PROTOCOL,MXL_ARP,MXL_RARP,MXL_IP,
					MXL_TCP,MXL_UDP,
					MXL_ICMP};



#define WM_CAPTURE_PACKET WM_USER+110

struct MyNetCard
{
	string	netCardName;//������
	string	description;//��������
	string  ipaddr;//����ip��ַ
	string  netmask;//����
	string	friendName;//�Ѻ�����
};

struct DataPacket
{
	int					packetNum;//���ݰ����
	PROTOCOL_TYPE		packetType;//���ݰ�����
	int					packetLen;//��ǰ���ݰ�����
	//u_char				*data;//��������ȡ��������
};


#endif