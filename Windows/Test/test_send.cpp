#include "../base_implementation/sender.h"

int main()
{
    WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
	    fprintf(stderr, "Failed to initialize.\n");
	    return 1;
	}

    char port[] = "8080";
    SOCKET socket_listen = make_connection(port);

    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen,(struct sockaddr*) &client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client)) {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Client is connected... ");
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_address,client_len, address_buffer, sizeof(address_buffer), 0, 0,NI_NUMERICHOST);
    printf("%s\n", address_buffer);
         
    // Your code 

    vector<string> vec;
    vec.push_back("giri nath");
    vec.push_back("sarnavo sarkar");
    vec.push_back("ayush pattnayak");

    send_data(socket_client,vec);
    
    return 0;
}