// TCPSocket.h: interface for the CTCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(TCPSOCKET)
#define TCPSOCKET

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"
#ifdef UNDER_CE
# pragma comment(lib,"winsock.lib")
#else
# pragma comment(lib,"ws2_32.lib")
#endif

#define SD_RECEIVE 0x00
#define SD_SEND 0x01
#define SD_BOTH 0x02

#define MAX_CONNECTION 100

enum TCP_SOCKET_TYPE
{
	TCP_SOCKET_SERVER=0,
		TCP_SOCKET_CLIENT
};

typedef void (*LPStatusProc)(char *data,int length,DWORD userdata); //������״̬�Ļص�����
typedef void (*LPDataArriveProc)(char *data,int length,DWORD userdata); //���ݵ���Ļص�����

struct TimeOutParameter
{
	int EndTime;
	SOCKET s;
	int nNo;
	BOOL bFinished;
	BOOL bExit;
	BOOL* pbConnected;
	HANDLE* phDataThread;
	int* pnConnections;
};

class CTCPSocket  
{
public:
	CTCPSocket(int nType=TCP_SOCKET_CLIENT);
	virtual ~CTCPSocket();

	//����
	int error; //��������

	//����
	int GetError(); //ȡ�ô���
	SOCKET GetSocket(); //ȡ���׽���
	int GetType(); //ȡ������
	BOOL IsConnected(SOCKET s); //�ж�һ��socket�Ƿ�����

	BOOL CreateServer(int nPort,int backlog=5); //����������
	BOOL StartServer(LPStatusProc proc1=NULL,LPDataArriveProc proc2=NULL,DWORD userdata=NULL); //��ʼ����
	BOOL StopServer(); //ֹͣ����
	SOCKET Listen(char* ClientIP=NULL); //��������IP������
	int ReceiveServer(int nNo,char* data, int length,int timeout); //����ָ���ֽڵ�����
	int SendServer(int nNo,char* data, int length); //����ָ���ֽڵ�����
	void Disconnect(int nNo);

	BOOL Connect(LPCSTR pstrHost, int nPort); //����һ��IP
	BOOL StartReceiving(LPStatusProc proc1=NULL,LPDataArriveProc proc2=NULL,DWORD userdata=NULL); //��ʼ�Զ�����
	BOOL StopReceiving(); //ֹͣ�Զ�����	
	int ReceiveClient(char* data, int length,int timeout); //����ָ���ֽڵ�����
	int SendClient(char* data, int length); //����ָ���ֽڵ�����

	void Close(); //�ر�	

protected:

	//����
	int m_nType; //����
	SOCKET m_sSocket; //�׽���
	BOOL m_bAvailable; //�ܷ�ʹ��
	BOOL m_bCreated; //�Ƿ���,����CreateServer��Connect֮���״̬
	BOOL m_bAuto; //�Ƿ��Զ��շ�,����StartServer��StartReceiving֮���״̬
	DWORD m_dwUserData; //�û�����

	int m_nPort; //�������˿�
	HANDLE m_hServerThread; //�������������ӵ��߳�
	HANDLE m_hServerDataThread[MAX_CONNECTION]; //�����������շ����߳�
	SOCKET m_sServer[MAX_CONNECTION]; //ÿ���ͻ�������,����Ϊ�˷�������ӻ��Ͳ���������
	char m_cIp[MAX_CONNECTION][16]; //ÿ�����ӵ�IP
	BOOL m_bConnected[MAX_CONNECTION]; //ÿ���ܹ�ʹ�õ����ӵ�״̬
	int m_nConnections; //��������
	int m_nCurrent; //��ǰ��Ҫ����������
	LPDataArriveProc m_lpServerDataArriveProc; //�����������շ��ص�
	LPStatusProc m_lpServerStatusProc; //������״̬�ظ��ص�

	HANDLE m_hClientThread; //�ͻ��������շ����߳�
	LPDataArriveProc m_lpClientDataArriveProc; //�ͻ��������շ��ص�
	LPStatusProc m_lpClientStatusProc; //�ͻ���״̬�ظ��ص�

	//����
	BOOL Initwinsock();
	BOOL NewConnect(int nNo);
	static DWORD WINAPI ServerThread(LPVOID lpParmameter); //�����������߳�
	static DWORD WINAPI DataThread(LPVOID lpParameter); //�����շ��߳�

	static DWORD WINAPI ClientThread(LPVOID lpParameter); //�ͻ��˽����߳�

	static DWORD WINAPI TimeOutControl(LPVOID lpParameter); //��ʱ�����߳�

};

#endif
