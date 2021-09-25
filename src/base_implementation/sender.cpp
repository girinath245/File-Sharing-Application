#include "sender.h" 

SOCKET make_connection(char *port) {
    
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

void resume_file_send(SOCKET socket_client) {
	
	string path = recieve_data(socket_client);

	FILE* streamIn = fopen(path.c_str(), "rb");
	if (streamIn == (FILE *)0) {
	    printf("File opening error ocurred. Exiting program.\n");
	    exit(0);
	}

	uint64_t position = 0;
	recieve_data(socket_client,&position,sizeof(uint64_t));

	fseek(streamIn, 0, SEEK_END);
	uint64_t size_of_file = ftell(streamIn); 
	fseek(streamIn, 0, SEEK_SET);

	fseek(streamIn,position,SEEK_SET);
	size_of_file -= position;

	send_data(socket_client,&size_of_file,sizeof(uint64_t));
	printf("The size of the file which will be sent is %llu bytes.\n",size_of_file);
	    
	unsigned char *buf=(unsigned char *)malloc(size_of_file);
	assert(fread(buf,sizeof(unsigned char),size_of_file,streamIn));
	   
	send_data(socket_client,buf,size_of_file);
	   
	free(buf);
	fclose(streamIn);
}

void send_file(SOCKET socket_client) {

	/*
	printf("Enter the file path \n");
	char path[120];
	assert(scanf("%[^\n]s",path));
	printf("%s" , path);

	send_data(socket_client,path,sizeof(path));
	*/

	string path ; 
	getline(cin,path) ;

	send_data(socket_client,path);

	FILE *streamIn = fopen(path.c_str(), "rb");
		if (streamIn == (FILE *)0) {
			printf("File opening error ocurred. Exiting program.\n");
			exit(0);
	}

	fseek(streamIn, 0, SEEK_END);
	uint64_t size_of_file = ftell(streamIn); 
	fseek(streamIn, 0, SEEK_SET);

	send_data(socket_client,&size_of_file,sizeof(uint64_t));
	printf("The size of the file which will be sent is %llu bytes.\n",size_of_file);
	
	unsigned char *buf = (unsigned char *) malloc(size_of_file);
	assert(fread(buf,sizeof(unsigned char),size_of_file,streamIn));
	
	send_data(socket_client,buf,size_of_file);
	
	free(buf);
	fclose(streamIn);
}
