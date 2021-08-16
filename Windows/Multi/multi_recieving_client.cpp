#include "../base_implementation/network_base.h"

int main(int argc, char *argv[]) {
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }

    if (argc < 3) {
        fprintf(stderr, "usage: reciever hostname[ip] port\n");
        return 1;
    }
    
    SOCKET socket_peer = make_connection(argv[1] ,argv[2]);

    




    return 0;
}