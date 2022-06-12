#include "reciever.h"

SOCKET make_connection(const char *ip, const char *port)
{

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo *peer_address;

	if (getaddrinfo(ip, port, &hints, &peer_address))
	{
		fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	printf("Remote address is: ");
	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
	printf("%s %s\n", address_buffer, service_buffer);

	SOCKET socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_peer))
	{
		fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

	if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen))
	{
		fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
	freeaddrinfo(peer_address);

	printf("Connected \n");

	return socket_peer;
}

/*
	Modern c++ api
	Optimized as well
*/

static size_t recieve_file_internal(SOCKET socket_peer, size_t size, FILE *fo)
{
	size_t total_recieved = 0;
	uint64_t total_needed = size - total_recieved;

	while (total_needed > 0)
	{
		uint64_t chunk_size = std::min(total_needed, MAX_MEMORY);
		unique_ptr<char> chunk(new char[chunk_size]);

		if (recieve_data(socket_peer, chunk.get(), chunk_size) == -1)
			std::cerr << "Error occured while recieving\n";

		fwrite(chunk.get(), sizeof(char), chunk_size, fo);

		total_recieved += chunk_size;
		total_needed = size - total_recieved;
	}
	return total_recieved ;
}

string recieve_file(SOCKET socket_peer)
{
	string filename = recieve_data(socket_peer);
	cout << "File to be downloaded is " << filename << endl;

	uint64_t expected_size = 0;
	recieve_data(socket_peer, &expected_size, sizeof(uint64_t));

	FILE *fo = fopen(filename.c_str(), "wb");

	if (recieve_file_internal(socket_peer, expected_size, fo) == expected_size)
		cout << "The size of the file recieved is " << expected_size << " bytes.\n";
	else
		std::cerr << "Error\n";

	fclose(fo);
	return filename;
}

void resume_download(SOCKET socket_peer, const string &filename)
{
	FILE *fo = fopen(filename.c_str(), "ab");

	send_data(socket_peer, filename);
	fseek(fo, 0, SEEK_END);
	uint64_t actual_file_size = ftell(fo);

	send_data(socket_peer, &actual_file_size, sizeof(actual_file_size));

	uint64_t expected_size = 0;
	recieve_data(socket_peer, &expected_size, sizeof(uint64_t));

	recieve_file_internal(socket_peer, expected_size, fo);

	actual_file_size = ftell(fo);
	fseek(fo, 0, SEEK_SET);

	cout << "The size of the file << filename.c_str() "
		 << " recieved is " << expected_size << " bytes.\n";

	fclose(fo);
}