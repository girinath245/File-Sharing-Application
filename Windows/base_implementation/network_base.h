#ifndef NETWORK_BASE_H
#define NETWORK_BASE_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#include <windows.h>

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <memory>

#define max_memory 10240000

using std::string ; using  std::vector ;  using std::cout ; using std::cin ; using std::endl;
using std::unique_ptr;

void send_data(SOCKET socket,const void *buf,uint64_t size);
void send_data(SOCKET socket,const string &s);
void send_data(SOCKET socket,const vector<string> &vec);
void send_data(SOCKET socket,const vector<int> &vec);

// Use string version for variable sized arrays otherwise use fixed values with the pointer return types only when 
// the caller and the vendor both know the sizes 

void recieve_data(SOCKET socket,void *read,uint64_t size);                     // 1                            
unique_ptr<const char[]> recieve_data(SOCKET socket,uint64_t size);              // 2
string recieve_data(SOCKET socket);                                            // 3
vector<string> recieve_vector_data(SOCKET socket);                             // 4     


#endif