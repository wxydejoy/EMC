// TCPSocket.cpp: implementation of the CTCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerDemo.h"
#include "TCPSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPSocket::CTCPSocket(int nType)
{
	m_nType=nType;
	m_sSocket=NULL;
	m_bAvailable=Initwinsock();
	m_bCreated=FALSE;
	m_bAuto=FALSE;
	m_dwUserData=0;
	error=0;

	m_nPort=-1;
	m_hServerThread=NULL;
	for(int i=0;i<MAX_CONNECTION;i++)
	{
		m_hServerDataThread[i]=NULL;
		m_sServer[i]=NULL;
		ZeroMemory(m_cIp[i],16);
		m_bConnected[i]=FALSE;
	}
	m_nConnections=0;
	m_nCurrent=0;
	m_lpServerStatusProc=NULL;
	m_lpServerDataArriveProc=NULL;

	m_hClientThread=NULL;
	m_lpClientDataArriveProc=NULL;
	m_lpClientStatusProc=NULL;
}

CTCPSocket::~CTCPSocket()
{
	Close();
}

int CTCPSocket::GetError()
{
	return error;
}

SOCKET CTCPSocket::GetSocket()
{
	return m_sSocket;
}

int CTCPSocket::GetType()
{
	return m_nType;
}

BOOL CTCPSocket::IsConnected(SOCKET s)
{
	int nRet=0;
	struct fd_set Fd_Recv;
	struct timeval Time_Recv;

	memset(&Fd_Recv,0,sizeof(struct fd_set));
	FD_CLR(s,&Fd_Recv);
	FD_SET(s,&Fd_Recv);
	Time_Recv.tv_sec=0;
	Time_Recv.tv_usec=0;

	nRet=select(s,&Fd_Recv,NULL,NULL,&Time_Recv);

	return (nRet==0);
}

BOOL CTCPSocket::CreateServer(int nPort,int backlog)
{
	m_bAvailable=Initwinsock();
	if(!m_bAvailable)
	{
		return FALSE;
	}
	if(m_nType!=TCP_SOCKET_SERVER)
	{
		return FALSE;
	}
	if(m_bCreated)
	{
		return FALSE;
	}

	struct sockaddr_in local;

	m_sSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	if(m_sSocket==SOCKET_ERROR)
	{
		error=WSAGetLastError();
		return FALSE;
	}

	local.sin_addr.s_addr=htonl(INADDR_ANY);
	local.sin_family=AF_INET;
	local.sin_port=htons(nPort);

	if(bind(m_sSocket,(struct sockaddr*)&local,sizeof(local))==SOCKET_ERROR)
	{
		error=WSAGetLastError();
		closesocket(m_sSocket);
		return FALSE;
	}

	if(listen(m_sSocket,backlog)!=0)
	{
		error=WSAGetLastError();
		closesocket(m_sSocket);
		return FALSE;
	}

	m_nPort=nPort;
	m_bCreated=TRUE;

	return TRUE;
}

BOOL CTCPSocket::StartServer(LPStatusProc proc1,LPDataArriveProc proc2,DWORD userdata)
{
	if(!m_bAvailable)
	{
		return FALSE;
	}
	if(m_nType!=TCP_SOCKET_SERVER)
	{
		return FALSE;
	}
	if(!m_bCreated)
	{
		return FALSE;
	}
	if(m_bAuto)
	{
		return FALSE;
	}

	m_lpServerStatusProc=proc1;
	m_lpServerDataArriveProc=proc2;
	m_dwUserData=userdata;

	DWORD dwThreadId;

	m_bAuto=TRUE;
	m_hServerThread=CreateThread(NULL,0,ServerThread,this,0,&dwThreadId);

	if(m_hServerThread==NULL)
	{
		m_bAuto=FALSE;
		error=WSAGetLastError();
		return FALSE;
	}
	
	return TRUE;
}

BOOL CTCPSocket::StopServer()
{
	if(!m_bAvailable)
	{
		return FALSE;
	}
	if(m_nType!=TCP_SOCKET_SERVER)
	{
		return FALSE;
	}
	if(!m_bCreated)
	{
		return FALSE;
	}
	if(!m_bAuto)
	{
		return FALSE;
	}

	DWORD exitcode;
	m_bAuto=FALSE;
	//ֹͣ�����߳�
	WaitForSingleObject(m_hServerThread,500);
	if(!GetExitCodeThread(m_hServerThread,&exitcode))
	{
		TerminateThread(m_hServerThread,exitcode);
	}
	CloseHandle(m_hServerThread);
	m_hServerThread=NULL;
	shutdown(m_sSocket,SD_RECEIVE);
	closesocket(m_sSocket);
	m_sSocket=NULL;

	//ֹͣ�����շ������߳�
	int i;
		
	for(i=0;i<MAX_CONNECTION;i++)
	{
		if(m_bConnected[i])
		{
			m_bConnected[i]=FALSE;
			WaitForSingleObject(m_hServerDataThread[i],50);
			if(!GetExitCodeThread(m_hServerDataThread[i],&exitcode))
			{
				TerminateThread(m_hServerDataThread[i],exitcode);
			}
			shutdown(m_sServer[i],SD_RECEIVE);
			closesocket(m_sServer[i]);
			m_sServer[i]=NULL;
			CloseHandle(m_hServerDataThread[i]);
			m_hServerDataThread[i]=NULL;
		}
	}
	m_nConnections=0;
	
	return TRUE;
}

SOCKET CTCPSocket::Listen(char* ClientIP)
{
	if(!m_bAvailable)
	{
		return -1;
	}
	if(m_nType!=TCP_SOCKET_SERVER)
	{
		return -1;
	}
	if(!m_bCreated)
	{
		return -1;
	}

	SOCKET sClient;
	int iAddrSize;
	struct sockaddr_in addr;

	iAddrSize=sizeof(addr);

	sClient=accept(m_sSocket,(struct sockaddr*)&addr,&iAddrSize);

	if(sClient==SOCKET_ERROR)
	{
		error=WSAGetLastError();
		closesocket(sClient);
		return SOCKET_ERROR;
	}

	if(ClientIP!=NULL)
	{
		sprintf(ClientIP,"%3d.%3d.%3d.%3d",addr.sin_addr.S_un.S_un_b.s_b1,addr.sin_addr.S_un.S_un_b.s_b2,addr.sin_addr.S_un.S_un_b.s_b3,addr.sin_addr.S_un.S_un_b.s_b4);
	}

	return sClient;
}

int CTCPSocket::ReceiveServer(int nNo,char* data,int length,int timeout)
{
	if(!m_bConnected[nNo])
	{
		return -2;
	}

	HANDLE hThread;
	DWORD dwThreadId;

	TimeOutParameter TimeOut;

	TimeOut.bExit=FALSE;
	TimeOut.bFinished=FALSE;
	TimeOut.EndTime=timeout;
	TimeOut.nNo=nNo;
	TimeOut.pbConnected=&(m_bConnected[nNo]);
	TimeOut.phDataThread=&(m_hServerDataThread[nNo]);
	TimeOut.pnConnections=&m_nConnections;
	TimeOut.s=m_sServer[nNo];

	hThread=CreateThread(NULL,0,TimeOutControl,(LPVOID)&TimeOut,0,&dwThreadId);

	if(hThread==NULL)
	{
		return -3;
	}

	int nRet=recv(m_sServer[nNo],data,length,0);
	if(nRet==SOCKET_ERROR)
	{
		error=WSAGetLastError();
	}
	TimeOut.bFinished=TRUE;

	while(!TimeOut.bExit)
	{
		Sleep(1);
	}

	return nRet;
}

int CTCPSocket::SendServer(int nNo,char *data,int length)
{
	if(!m_bConnected[nNo])
	{
		return -2;
	}

	int nRet=send(m_sServer[nNo],data,length,0);
	if(nRet==SOCKET_ERROR)
	{
		error=WSAGetLastError();
	}

	return nRet;
}

void CTCPSocket::Disconnect(int nNo)
{
	if(!m_bConnected[nNo])
	{
		return;
	}

	//�Ͽ��������ϵ�nNo������
	DWORD exitcode;
	m_bConnected[nNo]=FALSE;
	WaitForSingleObject(m_hServerDataThread[nNo],50);
	if(!GetExitCodeThread(m_hServerDataThread[nNo],&exitcode))
	{
		TerminateThread(m_hServerDataThread[nNo],exitcode);
	}
	shutdown(m_sServer[nNo],SD_RECEIVE);
	closesocket(m_sServer[nNo]);
	m_sServer[nNo]=NULL;
	CloseHandle(m_hServerDataThread[nNo]);
	m_hServerDataThread[nNo]=NULL;
	m_nConnections--;

}

BOOL CTCPSocket::Connect(LPCTSTR pstrHost,int nPort)
{
	m_bAvailable=Initwinsock();
	if(!m_bAvailable)
	{
		return FALSE;
	}
	if(m_nType==TCP_SOCKET_SERVER)
	{
		return FALSE;
	}
	if(m_bCreated)
	{
		return FALSE;
	}

	LPHOSTENT lpHost;
	struct sockaddr_in server;

	//��������
	lpHost=gethostbyname(pstrHost);
	if(lpHost==NULL)
	{
		return FALSE;
	}

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=*((u_long FAR*)(lpHost->h_addr));
	server.sin_port=htons(nPort);

	m_sSocket=socket(AF_INET,SOCK_STREAM,0);

	if(m_sSocket<=0)
	{
		error=WSAGetLastError();
		return FALSE;
	}
	
	if(connect(m_sSocket,(LPSOCKADDR)&server,sizeof(SOCKADDR))==SOCKET_ERROR) 
	{
		error=WSAGetLastError();
		closesocket(m_sSocket);
		m_sSocket=NULL;
		return FALSE;
	}

	m_bCreated=TRUE;

	return TRUE;
}

BOOL CTCPSocket::StartReceiving(LPStatusProc proc1,LPDataArriveProc proc2,DWORD userdata)
{
	if(!m_bAvailable)
	{
		return FALSE;
	}
	if(m_nType==TCP_SOCKET_SERVER)
	{
		return FALSE;
	}
	if(!m_bCreated)
	{
		return FALSE;
	}
	if(m_bAuto)
	{
		return FALSE;
	}

	//��ʼ�Զ�����
	m_lpClientStatusProc=proc1;
	m_lpClientDataArriveProc=proc2;
	m_dwUserData=userdata;
	m_bAuto=TRUE;

	DWORD dwThreadId;

	m_hServerThread=CreateThread(NULL,0,ClientThread,this,0,&dwThreadId);

	if(m_hServerThread==NULL)
	{
		m_bAuto=FALSE;
		error=WSAGetLastError();
		return FALSE;
	}
	
	return TRUE;
}

BOOL CTCPSocket::StopReceiving()
{
	if(!m_bAvailable)
	{
		return FALSE;
	}
	if(m_nType==TCP_SOCKET_SERVER)
	{
		return FALSE;
	}
	if(!m_bCreated)
	{
		return FALSE;
	}
	if(!m_bAuto)
	{
		return FALSE;
	}

	DWORD exitcode;
	m_bAuto=FALSE;
	//ֹͣ�����߳�
	WaitForSingleObject(m_hClientThread,500);
	if(!GetExitCodeThread(m_hClientThread,&exitcode))
	{
		TerminateThread(m_hClientThread,exitcode);
	}
	CloseHandle(m_hClientThread);
	m_hClientThread=NULL;

	return TRUE;
}

int CTCPSocket::ReceiveClient(char* data, int length,int timeout)
{
	if(m_nType==TCP_SOCKET_SERVER)
	{
		return -3;
	}
	if(!m_bCreated)
	{
		return -2;
	}

	HANDLE hThread;
	DWORD dwThreadId;

	TimeOutParameter TimeOut;

	TimeOut.bExit=FALSE;
	TimeOut.bFinished=FALSE;
	TimeOut.EndTime=timeout;
	TimeOut.nNo=-1;
	TimeOut.pbConnected=&(m_bAuto);
	TimeOut.phDataThread=&(m_hClientThread);
	TimeOut.pnConnections=&(m_bCreated);
	TimeOut.s=m_sSocket;

	hThread=CreateThread(NULL,0,TimeOutControl,(LPVOID)&TimeOut,0,&dwThreadId);

	if(hThread==NULL)
	{
		return -3;
	}

	int nRet=recv(m_sSocket,data,length,0);
	if(nRet==SOCKET_ERROR)
	{
		error=WSAGetLastError();
	}
	TimeOut.bFinished=TRUE;

	while(!TimeOut.bExit)
	{
		Sleep(1);
	}

	return nRet;
}

int CTCPSocket::SendClient(char* data,int length)
{
	if(m_nType==TCP_SOCKET_SERVER)
	{
		return -3;
	}
	if(!m_bCreated)
	{
		return -2;
	}

	int nRet=send(m_sSocket,data,length,0);
	if(nRet==SOCKET_ERROR)
	{
		error=WSAGetLastError();
	}

	return nRet;
}

void CTCPSocket::Close()
{
	if(m_nType==TCP_SOCKET_SERVER)
	{
		StopServer();
		m_bCreated=FALSE;
	}
	else
	{
		StopReceiving();
		shutdown(m_sSocket,SD_RECEIVE);
		closesocket(m_sSocket);
		m_sSocket=NULL;
		m_bCreated=FALSE;
	}
	WSACleanup();
}

/////////////////////////////////////////////////////////////////////////////
//Protected Functions

BOOL CTCPSocket::Initwinsock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	
	wVersionRequested=MAKEWORD(2,2);
	
	if(WSAStartup(wVersionRequested,&wsaData)==0)
	{
		return TRUE;
	}
	else
	{
		WSACleanup();
		return FALSE;
	}
}

BOOL CTCPSocket::NewConnect(int nNo)
{
	//����һ���������ݵ��߳�
	m_bConnected[nNo]=TRUE;
	m_hServerDataThread[nNo]=CreateThread(NULL,0,DataThread,this,0,NULL);
	if(m_hServerDataThread==NULL)
	{
		m_bConnected[nNo]=FALSE;
		closesocket(m_sServer[nNo]);
		return FALSE;
	}
	m_nConnections++;

	return TRUE;
}

DWORD WINAPI CTCPSocket::ServerThread(LPVOID lpParameter)
{
	CTCPSocket* m_pTCP=(CTCPSocket*)lpParameter;

	SOCKET sClient;
	int iAddrSize;
	struct sockaddr_in addr;
	int i;

	iAddrSize=sizeof(addr);

	//���ϼ�������
	while(m_pTCP->m_bAuto)
	{
		sClient=accept(m_pTCP->m_sSocket,(struct sockaddr*)&addr,&iAddrSize);
		if(sClient==SOCKET_ERROR)
		{
			continue;
		}
		//�ж��Ƿ�ﵽ���������
		if(m_pTCP->m_nConnections>=MAX_CONNECTION)
		{
			closesocket(sClient);
			continue;
		}

		//û����ʼһ���̴߳���������շ�
		for(i=0;i<MAX_CONNECTION;i++)
		{
			if(!(m_pTCP->m_bConnected[i]))
			{
				break;
			}
		}
		sprintf(m_pTCP->m_cIp[i],"%3d.%3d.%3d.%3d",addr.sin_addr.S_un.S_un_b.s_b1,addr.sin_addr.S_un.S_un_b.s_b2,addr.sin_addr.S_un.S_un_b.s_b3,addr.sin_addr.S_un.S_un_b.s_b4);
		TRACE("%s�Ѿ�����! ���%d\n",m_pTCP->m_cIp[i],i);

		//�ص�����
		if(m_pTCP->m_lpServerStatusProc!=NULL)
		{
			char* inf;
			inf=new char[22];
			sprintf(inf,"S%sC%3d",m_pTCP->m_cIp[i],i);
			m_pTCP->m_lpServerStatusProc(inf,22,m_pTCP->m_dwUserData);
			delete inf;
		}

		m_pTCP->m_sServer[i]=sClient;
		m_pTCP->m_nCurrent=i;
		m_pTCP->NewConnect(i);
	}

	return 0;
}

DWORD WINAPI CTCPSocket::DataThread(LPVOID lpParameter)
{
	CTCPSocket* m_pTCP=(CTCPSocket*)lpParameter;
	int MyNumber=m_pTCP->m_nCurrent;

	int nRet;
	char buf[512];
	
	timeval tv={0,5000};
	fd_set fs;

	//���Ͻ���ÿ���Ѿ����ӵĿͻ�������
	while(m_pTCP->m_bConnected[MyNumber])
	{
		FD_ZERO(&fs);
		FD_SET(m_pTCP->m_sServer[MyNumber],&fs);
		if(select(1,&fs,NULL,NULL,&tv)==1)
		{
			nRet=recv(m_pTCP->m_sServer[MyNumber],buf,512,0);
			
			if(nRet==SOCKET_ERROR)
			{
				//����Ͽ�(����ͻ���ִ���˳�ʱ���������Լ��Ͽ�)
				m_pTCP->error=WSAGetLastError();
				closesocket(m_pTCP->m_sServer[MyNumber]);
				m_pTCP->m_bConnected[MyNumber]=FALSE;
				m_pTCP->m_nConnections--;
				TRACE("%s����Ͽ�! ���%d\n",m_pTCP->m_cIp[MyNumber],MyNumber);

				//�ص�����
				if(m_pTCP->m_lpServerStatusProc!=NULL)
				{
					char* inf;
					inf=new char[22];
					sprintf(inf,"S%sD%3d",m_pTCP->m_cIp[MyNumber],MyNumber);
					m_pTCP->m_lpServerStatusProc(inf,22,m_pTCP->m_dwUserData);
					delete inf;
				}

				break;
			}
			
			if(nRet>0)
			{
				//�յ��µ�����
				TRACE("�յ�%s����%d�ֽ�! ���%d\n",m_pTCP->m_cIp[MyNumber],nRet,MyNumber);

				//���ݻص�����
				if(m_pTCP->m_lpServerDataArriveProc!=NULL)
				{
					char* inf;
					inf=new char[nRet+21];
					sprintf(inf,"S%s%3d",m_pTCP->m_cIp[MyNumber],MyNumber);
					memcpy(inf+21,buf,nRet);
					m_pTCP->m_lpServerDataArriveProc(inf,nRet+21,m_pTCP->m_dwUserData);
					delete inf;
				}

				continue;
			}
			
			if(nRet==0)
			{
				//�ͻ��Ͽ�
				TRACE("%s�����Ͽ�! ���%d\n",m_pTCP->m_cIp[MyNumber],MyNumber);

				//�ص�����
				if(m_pTCP->m_lpServerStatusProc!=NULL)
				{
					char* inf;
					inf=new char[22];
					sprintf(inf,"S%sD%3d",m_pTCP->m_cIp[MyNumber],MyNumber);
					m_pTCP->m_lpServerStatusProc(inf,22,m_pTCP->m_dwUserData);
					delete inf;
				}

				closesocket(m_pTCP->m_sServer[MyNumber]);
				m_pTCP->m_bConnected[MyNumber]=FALSE;
				m_pTCP->m_nConnections--;
				break;
			}
		}
	}

	return 0;
}

DWORD WINAPI CTCPSocket::ClientThread(LPVOID lpParameter)
{
	CTCPSocket* m_pTCP=(CTCPSocket*)lpParameter;

	int nRet;
	char buf[256];
	
	timeval tv={0,5000};
	fd_set fs;

	//���Ͻ��շ�������������
	while(m_pTCP->m_bAuto)
	{
		FD_ZERO(&fs);
		FD_SET(m_pTCP->m_sSocket,&fs);
		if(select(1,&fs,NULL,NULL,&tv)==1)
		{
			nRet=recv(m_pTCP->m_sSocket,buf,256,0);
			
			if(nRet==SOCKET_ERROR)
			{
				//����Ͽ�(����������ر�)
				m_pTCP->error=WSAGetLastError();
				closesocket(m_pTCP->m_sSocket);
				m_pTCP->m_bAuto=FALSE;
				TRACE("�ͻ��˳���Ͽ�! \n");

				//�ص�����
				if(m_pTCP->m_lpClientStatusProc!=NULL)
				{
					char* inf;
					inf=new char[22];					
					inf[0]='C';
					inf[1]='D';
					m_pTCP->m_lpClientStatusProc(inf,22,m_pTCP->m_dwUserData);
					delete inf;
				}

				break;
			}
			
			if(nRet>0)
			{
				//�յ��µ�����
				TRACE("�ͻ����յ�����%d�ֽ�! \n",nRet);

				//���ݻص�����
				if(m_pTCP->m_lpClientDataArriveProc!=NULL)
				{
					char* inf;
					inf=new char[nRet+1];
					inf[0]='C';
					memcpy(inf+1,buf,nRet);
					m_pTCP->m_lpClientDataArriveProc(inf,nRet+1,m_pTCP->m_dwUserData);
					delete inf;
				}

				continue;
			}
			
			if(nRet==0)
			{
				//�����������Ͽ�
				TRACE("�ͻ��������Ͽ�! \n");

				//�ص�����
				if(m_pTCP->m_lpClientStatusProc!=NULL)
				{
					char* inf;
					inf=new char[22];					
					inf[0]='C';
					inf[1]='D';				
					m_pTCP->m_lpClientStatusProc(inf,22,m_pTCP->m_dwUserData);
					delete inf;
				}
				closesocket(m_pTCP->m_sSocket);
				m_pTCP->m_bAuto=FALSE;

				break;
			}
		}
	}

	return 0;
}

DWORD WINAPI CTCPSocket::TimeOutControl(LPVOID lpParameter)
{
	TimeOutParameter* m_pTimeOut=(TimeOutParameter*)lpParameter;

	time_t starttime,endtime;
	BOOL bTimeOut=FALSE;

	starttime=time(NULL);
	while(!bTimeOut)
	{
		if(m_pTimeOut->bFinished)
		{
			m_pTimeOut->bExit=TRUE;
			return 1;
		}
		Sleep(1);
		endtime=time(NULL);
		if((endtime-starttime)>m_pTimeOut->EndTime)
		{
			//��ʱ
			bTimeOut=TRUE;
		}
	}

	//�Ͽ���Ӧ����
	DWORD exitcode;
	if(m_pTimeOut->bFinished)
	{
		return 1;
	}
	if(m_pTimeOut->s!=NULL)
	{
		if(m_pTimeOut->nNo>=0)
		{
			//������socket
			//ֹͣ�ý����߳�
			*(m_pTimeOut->pbConnected)=FALSE;
			WaitForSingleObject(*(m_pTimeOut->phDataThread),50);
			if(!GetExitCodeThread(*(m_pTimeOut->phDataThread),&exitcode))
			{
				TerminateThread(*(m_pTimeOut->phDataThread),exitcode);
			}
			shutdown(m_pTimeOut->s,SD_RECEIVE);
			closesocket(m_pTimeOut->s);
			m_pTimeOut->s=NULL;
			CloseHandle(*(m_pTimeOut->phDataThread));
			*(m_pTimeOut->phDataThread)=NULL;
			*(m_pTimeOut->pnConnections)--;
			
		}
		else
		{
			//ֹͣ�ͻ��˽����߳�
			if(*(m_pTimeOut->pbConnected))
			{
				*(m_pTimeOut->pbConnected)=FALSE;
				WaitForSingleObject(*(m_pTimeOut->phDataThread),50);
				if(!GetExitCodeThread(*(m_pTimeOut->phDataThread),&exitcode))
				{
					TerminateThread(*(m_pTimeOut->phDataThread),exitcode);
				}
			}
			shutdown(m_pTimeOut->s,SD_RECEIVE);
			closesocket(m_pTimeOut->s);
			m_pTimeOut->s=NULL;
			CloseHandle(*(m_pTimeOut->phDataThread));
			*(m_pTimeOut->phDataThread)=NULL;
			*(m_pTimeOut->pnConnections)=FALSE;
		}
	}

	m_pTimeOut->bExit=TRUE;
	return 0;
}
