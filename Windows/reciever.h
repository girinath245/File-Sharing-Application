#ifndef NETWORKING_HANDLER_H
#define NETWORKING_HANDLER_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#include <conio.h>
#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#define max_memory 10240000


#ifdef __cpluscplus
extern "C" {
#endif

SOCKET make_connection(char *ip,char *port);
void recieve_data(SOCKET socket_peer,void *read,uint64_t size);
void recieve_file(SOCKET socket_peer,char filename[100]);
void resume_download(SOCKET socket_peer,char filename[100]);

#ifdef __cpluscplus
}

#endif

#endif	// Networking_Handler_h