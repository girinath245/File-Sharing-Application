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

    auto file_list = recieve_vector_data(socket_peer);

    for (auto &v : file_list) cout << v << endl;

    closesocket(socket_peer);
    WSACleanup();

    return 0;
}