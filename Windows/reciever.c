#include "reciever.h"

int main(int argc, char *argv[]) {

    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }

    if (argc < 4) {
        fprintf(stderr, "usage: reciever hostname[ip] port filename.filetype \n");
        return 1;
    }
    
    SOCKET socket_peer = make_connection(argv[1] ,argv[2]);

    int8_t flag=0;
    send_data(socket_peer,&flag,sizeof(int8_t));

    switch(flag){
        case 0:
            recieve_file(socket_peer,argv[3]);
            break;
        case 1:
            resume_download(socket_peer,argv[3]);
            break;
    }

    printf("Closing socket...\n");
    CLOSESOCKET(socket_peer);

    WSACleanup();

    printf("Finished.\n");
    return 0;
}