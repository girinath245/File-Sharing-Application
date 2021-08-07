#include "reciever.h" 

SOCKET make_connection(char *ip,char *port) {
	
	struct addrinfo hints;
 	memset(&hints, 0, sizeof(hints));
 	hints.ai_socktype = SOCK_STREAM;
 	struct addrinfo *peer_address;
	
	if (getaddrinfo(ip, port, &hints, &peer_address)) {
		fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	printf("Remote address is: ");
	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,address_buffer, sizeof(address_buffer),service_buffer, sizeof(service_buffer),NI_NUMERICHOST);
	printf("%s %s\n", address_buffer, service_buffer);

	SOCKET socket_peer = socket(peer_address->ai_family,peer_address->ai_socktype, peer_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_peer)) {
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	if (connect(socket_peer,peer_address->ai_addr, peer_address->ai_addrlen)) {
		fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
	freeaddrinfo(peer_address);

	printf("Connected \n");

	return socket_peer;

}

void recieve_data(SOCKET socket_peer,void *read,uint64_t size) {
 		
 		uint64_t total_recieved=0;
 		uint64_t total_needed=size-total_recieved;

 		while(total_needed>0) {												 

	 		uint64_t bytes_recieved = recv(socket_peer,(char *)read+total_recieved,total_needed,0);
	 		if (bytes_recieved<1) break;
	 																				
	 		total_recieved += bytes_recieved;
	 		total_needed = size-total_recieved;

    	} 
	}

void download_file(SOCKET socket_peer,void *read,int size,FILE *fo) {

		uint64_t total_recieved=0;
 		uint64_t total_needed=size-total_recieved;

 		while(total_needed>0) {												 

	 		uint64_t bytes_recieved = recv(socket_peer,(char *)read,max_memory,0);
	 		if (bytes_recieved<1) break;

	 		fwrite(read,sizeof(char),bytes_recieved,fo);
	 		fseek(fo,0L,SEEK_END);	
	 			 			
	 		free(read);
	 		read = (char *)malloc(max_memory);
	 		
	 		total_recieved+= bytes_recieved;
	 		total_needed = size-total_recieved;
    	}
}

void recieve_file(SOCKET socket_peer) {
		
		char filename[120];
		recieve_data(socket_peer,filename,sizeof(filename));

		uint64_t expected_size=0;
		recieve_data(socket_peer,&expected_size,sizeof(uint64_t));
		
 		char *read=(char *)malloc(max_memory);
 		FILE *fo = fopen(filename,"wb");
 		
 		download_file(socket_peer,read,expected_size,fo);

 		fseek(fo, 0, SEEK_END);
	    uint64_t actual_file_size = ftell(fo); 

	    if (actual_file_size==expected_size)	printf("The size of the file recieved is %I64d bytes.\n",expected_size);
	    else printf("Error\n");
		
		fclose(fo);
		free(read);
	}

void send_data(SOCKET socket_peer,void *buf,uint64_t size) {
	char * now=(char *)buf;

	uint64_t size_left_to_be_sent=size;
	uint64_t bytes_sent;

	while(now < (char *)buf+size){
		bytes_sent=send(socket_peer,(char *)now,size_left_to_be_sent,0);
		if (bytes_sent==-1) break;
		
		now=now+bytes_sent;
		size_left_to_be_sent -=bytes_sent;
	}
}

void resume_download(SOCKET socket_peer,char filename[120]) {

	char *read = (char *)malloc(max_memory);
 	FILE *fo = fopen(filename,"ab");
	
	send_data(socket_peer,filename,100);
	fseek(fo, 0, SEEK_END);
	uint64_t actual_file_size = ftell(fo);

	send_data(socket_peer,&actual_file_size,sizeof(actual_file_size));

	uint64_t expected_size=0;
	recieve_data(socket_peer,&expected_size,sizeof(uint64_t));
		
 	download_file(socket_peer,read,expected_size,fo);

	actual_file_size = ftell(fo); 
	fseek(fo, 0, SEEK_SET);

	printf("The size of the file [%s] recieved is %I64d bytes.\n",filename,expected_size);
		
	fclose(fo);
	free(read);
}