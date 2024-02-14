/*
* 不用管报错，直接复制到modulemanager.cpp
建议放到Modulemanager.cpp里面执行一次就行了
*/

#include <winsock2.h>  
#include <stdio.h>  
#include <string>
using namespace std;
#pragma comment(lib,"ws2_32.lib")	//把ws2_32.lib加到Link页的连接库  
#define IP "1.1.1.1"			//在两台计算机上测试，IP为Server端的IP地址 记得设置！！！
#define PORT 19132					//注意：客户端设置通信的端口 = 服务端的端口
#define BUFFER_SIZE 1024			//数据发送缓冲区大小
HW_PROFILE_INFO hwProfileInfo;
bool Go = false;
bool first = false;

void Veri() {

	if (g_Data.getLocalPlayer() == nullptr)
		return;

	auto guiData = g_Data.getClientInstance()->getGuiData();

	std::wstring hardware_guid;
	GetCurrentHwProfile(&hwProfileInfo);
	hardware_guid = hwProfileInfo.szHwProfileGuid;
	std::string hwid(hardware_guid.begin(), hardware_guid.end());//这一步是获取hwid
	//转为字符串

	char buf[1024];//buf数组存放客户端发送的消息  
	char Rec[1024];
	int i;				
	for (i = 0; i < hwid.length(); i++)
		buf[i] = hwid[i];
	buf[i] = '\0';						//把string转录到char,不知道为什么用c_str乱码

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) == SOCKET_ERROR)  //WSAStartup()函数对Winsock DLL进行初始化
	{GameData::terminate();//终止游戏
		return;
	}
	SOCKET sock;											//客户端进程创建套接字
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)  //创建流套接字（与服务端保持一致）
	{
		GameData::terminate();
		WSACleanup();
		return;
	}

	struct sockaddr_in ClientAddr;				//sockaddr_in结构用来标识TCP/IP协议下的地址，可强制转换为sockaddr结构
	ClientAddr.sin_family = AF_INET;				//指Internet域
	ClientAddr.sin_port = htons(PORT);			//指定服务端所预留的端口
	ClientAddr.sin_addr.s_addr = inet_addr(IP);	//指定服务端所绑定的IP地址
	if (connect(sock, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)  //调用connect()函数，向服务器进程发出连接请求  
	{
		GameData::terminate();
		closesocket(sock);
		WSACleanup();
		return;
	}

	if (!Go && !first) {
		send(sock, buf, 1024, 0);				 //向服务器发送hwid
		Go = true;
	}

	/*接收数据*/
	if (Go && !first) {
		memset(Rec, 0, sizeof(Rec));//初始化
		recv(sock, Rec, 1024, 0);//接收来自服务器的信号



		if (!strncmp("114", Rec, 4))//123是服务端的成功信号，自己加密
		{
			if (g_Data.getLocalPlayer() != nullptr)
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sVerify!", WHITE);
			//first = true;
		}

		else {
			if (g_Data.getLocalPlayer() != nullptr) {
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYou are not Verified", WHITE);
				GameData::terminate();
				Go = false;
				first = false;
			}
		}
	}
	closesocket(sock);							 //关闭套接字
	WSACleanup();//终止对Winsock DLL的使用，并释放资源，以备下一次使用

}
