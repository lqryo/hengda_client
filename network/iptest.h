#ifndef IPTEST_H
#define IPTEST_H

// 需要的头文件
#include <Winsock2.h>
#include "iphlpapi.h"
#include "icmpapi.h"

bool ICMPPing(const char* szIP)
{
	HANDLE iHwnd = IcmpCreateFile();
	char* SendData = "Data Buffer";
	DWORD dwRet;
	LPVOID  ReplyBuffer;
	ReplyBuffer = (VOID*)malloc(sizeof(ICMP_ECHO_REPLY) + strlen(SendData));
	if ((dwRet = IcmpSendEcho(iHwnd, inet_addr(szIP), SendData, strlen(SendData), NULL, ReplyBuffer,
		sizeof(ICMP_ECHO_REPLY) + strlen(SendData) + sizeof(ICMP_ECHO_REPLY), 1000)) != 0)
	{
		IcmpCloseHandle(iHwnd);
		return true;
	}
	else
	{
		IcmpCloseHandle(iHwnd);
		return false;
	}
}

#endif // !IPTEST_H