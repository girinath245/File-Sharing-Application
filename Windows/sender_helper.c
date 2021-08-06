#include "sender.h" 

SOCKET make_connection(char *port){
    
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
    if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,(void*)&option, sizeof(option))) {
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

void send_data(SOCKET socket_client,void *buf,uint64_t size){
	unsigned char * now=(unsigned char *)buf;

	uint64_t size_left_to_be_sent=size;
	uint64_t bytes_sent;

	while(now<(unsigned char *)buf+size){
		bytes_sent=send(socket_client,(char *)now,size_left_to_be_sent,0);
		if (bytes_sent==-1) break;
		
		now=now+bytes_sent;
		size_left_to_be_sent -=bytes_sent;
	}
}

void resume_file_send(SOCKET socket_client){
	uint64_t position = 0L;
	recieve_data(socket_client,&position,sizeof(uint64_t));

	printf("Enter the file path \n");
	char path[100];
	assert(scanf("%[^\n]s",path));
	FILE *streamIn = fopen(path, "rb");
	if (streamIn == (FILE *)0) {
	    printf("File opening error ocurred. Exiting program.\n");
	    exit(0);
	}

	fseek(streamIn, 0, SEEK_END);
	uint64_t size_of_file = ftell(streamIn); 
	fseek(streamIn, 0, SEEK_SET);

	fseek(streamIn,position,SEEK_SET);
	size_of_file-= position;

	send_data(socket_client,&size_of_file,sizeof(uint64_t));
	printf("The size of the file which will be sent is %I64d bytes.\n",size_of_file);
	    
	unsigned char *buf=(unsigned char *)malloc(size_of_file);
	assert(fread(buf,sizeof(unsigned char),size_of_file,streamIn));
	   
	send_data(socket_client,buf,size_of_file);
	   
	free(buf);
	fclose(streamIn);
}

void send_file(SOCKET socket_client){

	printf("Enter the file path \n");
	char path[120];
	assert(scanf("%[^\n]s",path));
	printf("%s" , path);
	FILE *streamIn = fopen(path, "rb");
		if (streamIn == (FILE *)0) {
			printf("File opening error ocurred. Exiting program.\n");
			exit(0);
	}

	fseek(streamIn, 0, SEEK_END);
	uint64_t size_of_file = ftell(streamIn); 
	fseek(streamIn, 0, SEEK_SET);

	send_data(socket_client,&size_of_file,sizeof(uint64_t));
	printf("The size of the file which will be sent is %I64d bytes.\n",size_of_file);
	
	unsigned char *buf=(unsigned char *)malloc(size_of_file);
	assert(fread(buf,sizeof(unsigned char),size_of_file,streamIn));
	
	send_data(socket_client,buf,size_of_file);
	
	free(buf);
	fclose(streamIn);
}

void recieve_data(SOCKET socket_peer,void *read,uint64_t size) {
 		
 		uint64_t total_recieved=0;
 		uint64_t total_needed=size-total_recieved;

 		while(total_needed>0) {												 

	 		uint64_t bytes_recieved=recv(socket_peer,(char *)read+total_recieved,total_needed,0);
	 		if (bytes_recieved<1) break;
	 																				
	 		total_recieved+= bytes_recieved;
	 		total_needed=size-total_recieved;

    	} 
	}