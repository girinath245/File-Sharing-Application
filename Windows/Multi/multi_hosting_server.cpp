#include "../base_implementation/network_base.h"
#include <thread>
#include <conio.h>
#include <set>

using std::thread;
using std::set;

void load_new_files() {
    while(1) {
        if (_kbhit()) {
            string path;
            getline(cin,path);

            FILE* streamIn = fopen(path.c_str(), "rb");
	        if (streamIn == (FILE *)0) {
	            printf("File opening error ocurred. Exiting program.\n");
	        }



            char *buf = (char *) malloc(size_of_file);
	        assert(fread(buf,sizeof(char),size_of_file,streamIn));
        }
    }
}

void handle_new_client(const set<string,void*> &s) {

}

int main()
{
    WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
	    fprintf(stderr, "Failed to initialize.\n");
	    return 1;
	}

    char port[] = "8080";
    SOCKET socket_listen = make_connection(port);

    set<string,void*> s;
    thread t1(load_new_files,std::ref(s));
    
    printf("Waiting for connection...\n");

    int num_connections = 0;

    while(1) {
       
        struct sockaddr_storage client_address;
        socklen_t client_len = sizeof(client_address);
        SOCKET socket_client = accept(socket_listen,(struct sockaddr*) &client_address, &client_len);
        if (!ISVALIDSOCKET(socket_client)) {
            fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        char address_buffer[100];
        getnameinfo((struct sockaddr*)&client_address,client_len, address_buffer, sizeof(address_buffer), 0, 0,NI_NUMERICHOST);
        printf("%s\n", address_buffer);

        thread t2(handle_new_client,std::cref(s));          // child threads to handle individual clients

        num_connections++;

        cout << "The total number of clients connected till now are " << num_connections << endl;
    }

    return 0;
}