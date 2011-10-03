#ifndef _SOCKET_H
#define _SOCKET_H

typedef enum
{
    TCPClient,
    TCPServer,
    InvalidSocket
} SocketType;

typedef enum
{
    Socket_Blocking,
    Socket_NonBlocking
} SocketBlocking;

typedef struct _Socket
{
    unsigned int   sock_fd;
    SocketType     type;
    SocketBlocking blocktype;
} Socket;

Socket *socket_new(SocketBlocking blocktype);
void socket_free(Socket *socket);

int socket_connect(Socket *socket, char *addr, char *port);
int socket_listen(Socket *socket, char *port, int backlog);
Socket *socket_accept(Socket *socket, SocketBlocking blocktype);

int socket_read(Socket *socket, char *dest_buf, int len);
char *socket_readline(Socket *osocket, char *lineterm);
int socket_write(Socket *socket, char *data, int len);

int socket_enable_blocktype(int sock_fd, SocketBlocking blocktype);

unsigned int socket_select(Socket *socket, long seconds, long usec);
void wait_until_data_available (Socket *socket);

#endif /* _SOCKET_H_ */
