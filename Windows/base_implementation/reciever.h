#ifndef RECIEVER_H
#define RECIEVER_H

#include "network_base.h"

SOCKET make_connection(char *ip,char *port);
void recieve_file(SOCKET socket_peer);
void resume_download(SOCKET socket_peer,const string &filename);

#endif