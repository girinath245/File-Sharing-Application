#ifndef SENDER_H
#define SENDER_H

#include "network_base.h"
#include "file_impl.h"

SOCKET make_connection(const char *port);
void send_file(SOCKET socket_client);
void resume_file_send(SOCKET socket_client);
void send_file_formatted(SOCKET socket_client, string &path);
void send_files(SOCKET socket_client, const vector<string>& file_lists);

#endif