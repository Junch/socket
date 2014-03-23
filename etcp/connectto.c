#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "etcp.h"

/* client - place holder for the client code */
static void client( SOCKET s, struct sockaddr_in *peerp )
{
	char buf[ 128 ];

	readline( s, buf, sizeof( buf ) );
	puts( buf );
}

/* alarm_hndlr - catch the alarm signal */
void alarm_hndlr( int sig )
{
	return;
}

/* main - connect to the server */
/* include main_head */
int main( int argc, char **argv )
{
/* end main_head */
	struct sockaddr_in peer;
	SOCKET s;
	int rc;

	INIT();

	set_address( argv[ 1 ], argv[ 2 ], &peer, "tcp" );

	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( !isvalidsock( s ) )
		error( 1, errno, "socket call failed" );

/* include connect */
	signal( SIGALRM, alarm_hndlr );
	alarm( 5 );
	rc = connect( s, ( struct sockaddr * )&peer, sizeof( peer ) );
	alarm( 0 );
/*@.bp*/
	if ( rc < 0 )
	{
		if ( errno == EINTR )
			error( 1, 0, "connect timed out\n" );
/* end connect */
		error( 1, errno, "connect failed" );
	}
	client( s, &peer );
	EXIT( 0 );
/* include main_tail */
}
/* end main_tail */
