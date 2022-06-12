#include "../base_implementation/sender.h"
#include <thread>

int main()
{

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    char port[] = "8080";
    SOCKET socket_listen = make_connection(port);

    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Client is connected... ");
    char address_buffer[100];
    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    printf("%s\n", address_buffer);

    vector<string> paths = 
    {
        "D:\\Movies and Tv\\sample\\Demon Slayer.mkv",
        "D:\\Movies and Tv\\sample\\Eternals.2021.720p.WEBRip.x264.AAC-[YTS.MX].mp4",
        "D:\\Movies and Tv\\sample\\Nobody.2021.1080p.WEBRip.x264.AAC5.1-[YTS.MX].mp4"
    };

    send_files(socket_client,paths);

    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");

    return 0;

}
