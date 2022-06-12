#ifndef NETWORK_BASE_H
#define NETWORK_BASE_H

#if defined(_WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>

    #define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
    #define CLOSESOCKET(s) closesocket(s)
    #define GETSOCKETERRNO() (WSAGetLastError())

#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>

    #define ISVALIDSOCKET(s) ((s) >= 0)
    #define CLOSESOCKET(s) close(s)
    #define SOCKET int
    #define GETSOCKETERRNO() (errno)
#endif

#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <memory>


const uint64_t MAX_MEMORY = 10 * 1024 * 1024 ; 

using std::string ; using  std::vector ;  using std::cout ; using std::cin ; using std::endl;
using std::unique_ptr; using std::make_unique;

int64_t send_data(SOCKET socket,const void *buf,uint64_t size);
void send_data(SOCKET socket,const string &s);
void send_data(SOCKET socket,const vector<string> &vec);
void send_data(SOCKET socket,const vector<int> &vec);

// Use string version for variable sized arrays otherwise use fixed values with the pointer return types only when 
// the caller and the vendor both know the sizes 

int64_t recieve_data(SOCKET socket,void *read,uint64_t size);                  // 1             
const char* recieve_data(SOCKET socket,uint64_t size);               
// unique_ptr<const char> recieve_data(SOCKET socket,uint64_t size);              // 2
string recieve_data(SOCKET socket);                                            // 3
vector<string> recieve_vector_data(SOCKET socket);                             // 4     

#endif