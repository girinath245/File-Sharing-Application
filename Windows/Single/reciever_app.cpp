#include "../base_implementation/reciever.h"

int main(int argc, char *argv[]) {

    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }

    if (argc < 4) {
        fprintf(stderr, "usage: reciever hostname[ip] port download_preference \n");
        return 1;
    }
    
    SOCKET socket_peer = make_connection(argv[1] ,argv[2]);
    
    int flag = 0;
    if (argc == 4) flag = atoi(argv[3]);
    send_data(socket_peer,&flag,sizeof(flag));

    switch(flag) {
        case 0:
            recieve_file(socket_peer);
            break;
        case 1:
            printf("Enter the file path \n");
	        string path ; 
            getline(cin,path);
            resume_download(socket_peer,path);
            break;
    }

    printf("Closing socket...\n");
    CLOSESOCKET(socket_peer);

    WSACleanup();

    printf("Finished.\n");
    return 0;
}