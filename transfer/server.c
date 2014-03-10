#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define    MAXLINE        1024

void usage(char *command);
int  createSocket(int portid);
void receiveFile(int sock_id);
ssize_t readn(int fd, void *vptr, size_t n);
void getFileName(int fd, char buf[], int bufsize);

int main(int argc,char **argv)
{
    if (argc != 2) {
        usage(argv[0]);
    }

    int sock_id = createSocket(atoi(argv[1]));
    
    receiveFile(sock_id);
    close(sock_id);
    return 0;
}

void usage(char *command)
{
    printf("usage :%s portnum\n", command);
    exit(0);
}

int createSocket(int portid)
{
    int sock_id;
    if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Create socket failed\n");
        exit(0);
    }
    
    struct sockaddr_in  serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portid);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sock_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) {
        perror("Bind socket failed\n");
        exit(0);
    }
    
    if (-1 == listen(sock_id, 10)) {
        perror("Listen socket failed\n");
        exit(0);
    }
    
    return sock_id;
}

void receiveFile(int sock_id)
{
    char    buf[MAXLINE];
    struct  sockaddr_in clie_addr;
    
    while (1) {
        socklen_t clie_addr_len = sizeof(clie_addr);
        int link_id = accept(sock_id, (struct sockaddr *)&clie_addr, &clie_addr_len);
        if (-1 == link_id) {
            perror("Accept socket failed\n");
            exit(0);
        }
        bzero(buf, MAXLINE);
        
        getFileName(link_id, buf, MAXLINE);
        FILE *fp;
        if ((fp = fopen(buf, "w")) == NULL) {
            perror("Open file failed\n");
            exit(0);
        }
        
        bzero(buf, MAXLINE);
        ssize_t recv_len;
        while ((recv_len = recv(link_id, buf, MAXLINE, 0)) != 0) {
            if(recv_len < 0) {
                printf("Recieve Data From Server Failed!\n");
                break;
            }
            printf("#");
            size_t write_leng = fwrite(buf, sizeof(char), recv_len, fp);
            if (write_leng < recv_len) {
                printf("Write file failed\n");
                break;
            }
            bzero(buf,MAXLINE);
        }
        
        printf("\nFinish Recieve\n");
        fclose(fp);
        close(link_id);
    }
}

void getFileName(int fd, char buf[], int bufsize)
{
    int flen = 0;
    if (readn(fd, &flen, 4) < 0) {
        perror("Failed to get file name length");
        return;
    }
    
    flen = ntohl(flen);
    printf("file name length = %d\n", flen);

    if (flen > bufsize) {
        perror("Failed to get the file name");
        return;
    }

    if (readn(fd, buf, flen) < 0) {
        perror("Failed to get the file name");
        return;
    }
    
    printf("file name: %s\n", buf);
}

ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;
    ptr = vptr;
    nleft = n;
    
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0; /* and call read() again */
            else
                return (-1);
        } else if (nread == 0)
            break; /* EOF */
        
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft); /* return >= 0 */
}

