#ifndef __RTP_H__
#define __RTP_H__

#include <sys/socket.h>

// Invoke rdpSocketIntervalAction() periodically.
#define RDP_SOCKET_CHECK_TIMEOUT_DEFAULT 500
#define RDP_SOCKET_CHECK_TIMEOUT_MIN 50
#define RDP_SOCKET_CHECK_TIMEOUT_MAX 1000

// errno to which rdpReadPoll() and rdpWrite() might set. It indicates to invoke
// these functions again.
#define EAGAINCONTINUE 199

// Flags that rdpReadPoll() might returns.
#define RDP_CONTINUE (1 << 0)
#define RDP_AGAIN (1 << 1)      // Not drained fd.
#define RDP_ACCEPT (1 << 2)     // Have passive new connection established.
#define RDP_CONNECTED (1 << 3)  // Actively established connection connected.
#define RDP_DATA (1 << 4)       // Data in the buffer to be read.
#define RDP_POLLOUT (1 << 5)    // Have spare sapce for writing.
#define RDP_CONN_ERROR (1 << 6) // The connection failed.
#define RDP_ERROR (1 << 9)      // Invoke params error, or system call error.

enum { RDP_PROP_FD, RDP_PROP_SNDBUF, RDP_PROP_RCVBUF };

typedef struct rdpConn rdpConn;
typedef struct rdpSocket rdpSocket;

struct rdpVec {
  const void *base;
  size_t len;
};

// Current rdp version: 1.
rdpSocket *rdpSocketCreate(int version, const char *node, const char *service);
int rdpSocketDestroy(rdpSocket *s);
rdpConn *rdpConnCreate(rdpSocket *s);
int rdpConnClose(rdpConn *c);
int rdpConnect(rdpConn *c, const struct sockaddr *addr, socklen_t addrlen);
rdpConn *rdpNetConnect(rdpSocket *s, const char *host, const char *service);
ssize_t rdpWrite(rdpConn *c, const void *buf, size_t len);
ssize_t rdpReadPoll(rdpSocket *s, void *buf, size_t len, rdpConn **c,
                    int *flag);
int rdpSocketIntervalAction(rdpSocket *s);
int rdpConnGetAddr(rdpConn *c, struct sockaddr *addr, socklen_t *addrlen);
int rdpSocketGetProp(rdpSocket *s, int opt);
int rdpSocketSetProp(rdpSocket *s, int opt, int val);
void *rdpConnGetUserData(rdpConn *c);
int rdpConnSetUserData(rdpConn *c, void *userData);

#endif // __RTP_H__
