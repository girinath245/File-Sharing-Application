#include "../base_implementation/reciever.h"
#include <filesystem>

int main(int argc, char *argv[])
{

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    SOCKET socket_peer = make_connection("::1" , "8080");

    vector<string> paths =
    {
        "D:\\Movies and Tv\\sample\\Demon Slayer.mkv",
        "D:\\Movies and Tv\\sample\\Eternals.2021.720p.WEBRip.x264.AAC-[YTS.MX].mp4",
        "D:\\Movies and Tv\\sample\\Nobody.2021.1080p.WEBRip.x264.AAC5.1-[YTS.MX].mp4"
    };

    for (auto path : paths)
    {
        if (std::filesystem::file_size(path) == std::filesystem::file_size(recieve_file(socket_peer)))
        {
            std::cout << path << "  OK\n\n" ;  
        }
    }

    printf("Closing socket...\n");
    CLOSESOCKET(socket_peer);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");
    return 0;
}