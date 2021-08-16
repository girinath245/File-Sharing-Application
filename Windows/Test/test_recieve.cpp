#include "../base_implementation/reciever.h"

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
    
    auto ans = recieve_data(socket_peer,(uint64_t)120);
    
    cout << ans.get() << endl;
 
    return 0;
}