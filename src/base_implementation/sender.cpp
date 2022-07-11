#include "sender.h" 

SOCKET make_connection(const char *port) {
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0,port, &hints, &bind_address);
 
    SOCKET socket_listen = socket(bind_address->ai_family,bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    int option = 0;
    if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,(const char*)&option, sizeof(option))) {
    fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
    }

    
    if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
     
    freeaddrinfo(bind_address);
    
    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    return socket_listen;
}

static void send_file_internal(SOCKET socket_client , const uint64_t position , const string &path) {
	try {
        file streamIn(path , position) ;
        
        auto size_left = streamIn.size_left() ;
        
        if (streamIn.size_left() > 0) send_data(socket_client, &(size_left), sizeof(uint64_t));
        else return ;
        
        cout << "The size of the file which will be sent is " << streamIn.size_left() << " bytes.\n" ;

        while (streamIn.size_left() > 0) 
        {
            auto chunk = streamIn.load_region() ; 
            
            if (send_data(socket_client, chunk.current_region ,chunk.amount_last_retrieved ) == -1) 
                std::cerr << "Error occured while sending\n";
        }
    }
    
    catch( const std::exception & ex ) {
       std::cerr << ex.what() << endl;
    }
}

void send_file_formatted(SOCKET socket_client, string &path)
{
    string path_proper;

    for (auto c : path)
    {
        if (c != '\"' && c != '\'')
            path_proper += c;
    }

    int i = path_proper.size() - 1;

    while (i >= 0 && ((path_proper[i] != '\\') && (path_proper[i] != '/')))
    {
        i--;
    }

    string final_filename = path_proper.substr(i + 1, path_proper.length() - i);
    
    send_data(socket_client, final_filename);    
    send_file_internal(socket_client, 0, path_proper);
}

void resume_file_send(SOCKET socket_client) {
	string path = recieve_data(socket_client);

	uint64_t position = 0;
	recieve_data(socket_client,&position,sizeof(uint64_t));
	   
	send_file_internal(socket_client,position,path);
}



void send_file(SOCKET socket_client) {
	string path ; 
	getline(cin,path) ;

    send_file_formatted(socket_client , path) ;
}

void send_files(SOCKET socket_client , const vector<string> &file_lists)
{
    for (auto file : file_lists)
    {
        send_file_formatted(socket_client , file) ;
    }
}