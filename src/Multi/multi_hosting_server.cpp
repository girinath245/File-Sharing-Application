#include "../base_implementation/sender.h"
#include <thread>
#include <unordered_map>
#include <queue>

using std::make_shared;
using std::queue;
using std::shared_ptr;
using std::thread;
using std::unordered_map;

/*
    Handles new files:- Loads them into memory map with the filenames as the key and file location pointer as its value .
                        Uses shared pointers since many client might be needing the same files .
*/

void load_new_files(unordered_map<string, shared_ptr<char>> &file_map)
{
    while (1)
    {
        string path;
        getline(cin, path);

        if (file_map.find(path) != file_map.end())
            continue;

        FILE *streamIn = fopen(path.c_str(), "rb");
        if (streamIn == (FILE *)0)
        {
            cout << "File could not be opened check file path/name. \n";
            continue;
        }

        fseek(streamIn, 0, SEEK_END);
        uint64_t size_of_file = ftell(streamIn);
        fseek(streamIn, 0, SEEK_SET);

        cout << "size_of_file being loaded is " << size_of_file << endl;

        shared_ptr<char> buf(new char[size_of_file]);
        assert(fread(buf.get(), sizeof(char), size_of_file, streamIn));

        file_map[path] = buf;

        fclose(streamIn);
    }
}

void handle_new_client(SOCKET socket_client, const unordered_map<string, shared_ptr<char>> &file_map)
{
    if (file_map.size())
    {
        vector<string> file_list_for_user(file_map.size());
        auto it = file_list_for_user.begin();

        for (auto &[x, y] : file_map)
        {
            *it = x;
            it++;
        }

        send_data(socket_client, file_list_for_user);
    }
    CLOSESOCKET(socket_client);
}

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

    unordered_map<string, shared_ptr<char>> file_map;
    thread t1(load_new_files, std::ref(file_map));

    printf("Waiting for connection...\nIn the meanwhile continue adding the files your server wants to host on the next line\n");

    int num_connections = 0;

    while (1)
    {
        struct sockaddr_storage client_address;
        socklen_t client_len = sizeof(client_address);
        SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);

        if (!ISVALIDSOCKET(socket_client))
        {
            fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        char address_buffer[100];
        getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
        printf("%s\n", address_buffer);

        thread t2(handle_new_client, socket_client, std::cref(file_map)); // child threads to handle individual clients
        t2.detach();

        num_connections++;

        cout << "The total number of clients connected till now are " << num_connections << endl;
    }

    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif

    return 0;
}