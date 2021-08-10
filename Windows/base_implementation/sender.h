#ifndef SENDER_H
#define SENDER_H

#include "network_base.h"

SOCKET make_connection(char *port);
void send_file(SOCKET socket_client);
void resume_file_send(SOCKET socket_client);

#endif