#include "common.h"
#include <stdio.h>
#include <sys/socket.h>

int writen(int fd, const void *vptr, size_t n)
{
    size_t  nleft;
    int     nwritten;
    const char  *ptr = (const char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = send(fd, ptr, nleft, NULL)) == -1) {
            return -1;
        }
        
        nleft -= nwritten;
        ptr += nwritten;
    }

    return n;
}
