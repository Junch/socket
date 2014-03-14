#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <libgen.h>

#define    MAXLINE        1024

void usage(char *command)
{
    printf("usage :%s ipaddr portnum filename\n", command);
    exit(0);
}

ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char  *ptr;
    
    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;       /* and call write() again */
            else
                return(-1);         /* error */
        }
        
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}

int sendFileName(int fd, char* filename){
    int flen = strlen(filename);
    int flenSend = htonl(flen);
    writen(fd, &flenSend, 4);
    return writen(fd, filename, flen);
}

int main(int argc,char **argv)
{
    struct sockaddr_in     serv_addr;
    char                   buf[MAXLINE];
    int                    sock_id;
    int                    read_len;
    int                    send_len;
    FILE                   *fp;
    int                    i_ret;
   
    if (argc != 4) {
        usage(argv[0]);
    }
    
    if ((fp = fopen(argv[3],"rb")) == NULL) {
        perror("Open file failed\n");
        exit(0);
    }
    
    if ((sock_id = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("Create socket failed\n");
        exit(0);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    //inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
   
    /* connect the server commented by guoqingbo*/
    i_ret = connect(sock_id, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if (-1 == i_ret) {
        printf("Connect socket failed\n");
        return -1;
    }

    char *fname = basename(argv[3]);
    if (sendFileName(sock_id, fname) < 0){
        perror("Send file name failed\n");
        return -1;
    }
    
    /* transported the file commented by guoqingbo*/
    bzero(buf, MAXLINE);
    while ((read_len = fread(buf, sizeof(char), MAXLINE, fp)) >0 ) {
        send_len = send(sock_id, buf, read_len, 0);
        if ( send_len < 0 ) {
            perror("Send file failed\n");
            exit(0);
        }
        bzero(buf, MAXLINE);
    }

    fclose(fp);
    close(sock_id);
    printf("Send Finish\n");
    return 0;
}
