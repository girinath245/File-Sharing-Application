#include "network_base.h"

void send_data(SOCKET socket,const string &s) {
	auto _size_ = s.size();
	_size_++;

	send_data(socket,&(_size_),sizeof(_size_));    						// First send the size of the string

	send_data(socket,s.c_str(),_size_);			  						// then send the actual char array
}

void send_data(SOCKET socket,const vector<string> &vec) {
	auto _vector_size_ = vec.size();
	send_data(socket,&(_vector_size_),sizeof(_vector_size_));			// send the vector size()

	for (auto &v : vec) {
		send_data(socket,v);
	}
}

int64_t send_data(SOCKET socket,const void * buf,uint64_t size) {
	const char * now=(const char *)buf;

	uint64_t size_left_to_be_sent=size;
	int64_t bytes_sent;

	while(now < (const char *)buf+size){
		bytes_sent = send(socket,now,size_left_to_be_sent,0);
		if (bytes_sent == -1) return -1;
		
		now=now+bytes_sent;
		size_left_to_be_sent -= bytes_sent;
	}
	return (int64_t) size ; 
}

string recieve_data(SOCKET socket) {
	uint64_t _size_ = 0;
	recieve_data(socket,&_size_,sizeof(uint64_t));

	auto v = recieve_data(socket,_size_) ; 
	
	auto ret = string(v) ;

	delete[] (v);

	return ret ;
}

vector<string> recieve_vector_data(SOCKET socket) {
	auto _vector_size_ = 0;
	recieve_data(socket,&_vector_size_,sizeof(uint64_t));			// send the vector size()

	vector<string> result(_vector_size_);

	for (auto &v : result) v = recieve_data(socket);

	return result;
}

// The api has changed now recieve_data returns a const pointer to the data 
const char* recieve_data(SOCKET socket,uint64_t size) {	
	uint64_t total_recieved = 0;
	uint64_t total_needed = size;

	auto read = new char[size];
	read[size-1] = '\0';

	while(total_needed > 0) {												 
		uint64_t bytes_recieved = recv(socket,read+total_recieved,total_needed,0);
		if (bytes_recieved<1) break;
																				
		total_recieved += bytes_recieved;
		total_needed = size-total_recieved;
	}

	return read; 
}

// Auto destructible things
int64_t recieve_data(SOCKET socket,void *read,uint64_t size) {	
	uint64_t total_recieved = 0;
	uint64_t total_needed = size;

	while(total_needed > 0) {												 
		int64_t bytes_recieved = recv(socket,(char *)read+total_recieved,total_needed,0);
		if (bytes_recieved < 1) return -1;
																				
		total_recieved += bytes_recieved;
		total_needed = size-total_recieved;
	} 
	return (int64_t) size ; // return the total size sent 
}