#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

void recieve_data(SOCKET socket_peer,void *read,int size)
 {
 		int total_bytes=0;
 		int total_needed=size-total_bytes;

 		while(total_needed>0) {														//Main Reciever handles with 

	 		int bytes_recieved=recv(socket_peer,(unsigned char *)read+total_bytes,total_needed,0);	
	 		if (bytes_recieved<1) break;
	 																				//buffer and recv()
	 		total_bytes=total_bytes+bytes_recieved;
	 		total_needed=size-total_bytes;

    	} 
 }

 void send_data(SOCKET socket_client,void *buf,int size)
    {
    	void *now=buf;

    	int size_left_to_be_sent=size;
   		
   		while(now<buf+size){
    		int bytes_sent=send(socket_client,(unsigned char *)now,size_left_to_be_sent,0);
    		if (bytes_sent==-1) break;
    		now=now+bytes_sent;
    		size_left_to_be_sent -=bytes_sent;
    	}
    }

 void recieve_file(SOCKET socket_peer,char filename[60])
	{
		long long int size;
		recieve_data(socket_peer,&size,sizeof(size));
		printf("The size of the file which will be recieved is %lld bytes.\n",size);

 		unsigned char *read=(unsigned char *)malloc(size);
 		recieve_data(socket_peer,read,size);
    	
		FILE *fo = fopen(filename,"wb");
		fwrite(read,sizeof(unsigned char),size, fo);
		fclose(fo);
		free(read);
	}

 void send_file(SOCKET socket_client)
 	{

 	printf("Enter the file path \n");
    char path[60];
    assert(scanf("%[^\n]s",path));
    FILE *streamIn = fopen(path, "rb");
        if (streamIn == (FILE *)0) {
            printf("File opening error ocurred. Exiting program.\n");
            exit(0);
    }

    fseek(streamIn, 0, SEEK_END);
    long long int size_of_file = ftell(streamIn); 
    fseek(streamIn, 0, SEEK_SET);

    send_data(socket_client,&size_of_file,sizeof(size_of_file));
    printf("The size of the file which will be sent is %lld bytes.\n",size_of_file);
    
    unsigned char *buf=(unsigned char *)malloc(size_of_file);
    assert(fread(buf,sizeof(unsigned char),size_of_file,streamIn));
   
    send_data(socket_client,buf,size_of_file);
   
    free(buf);
   	fclose(streamIn);

 	}
