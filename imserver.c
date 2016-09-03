#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/errno.h>

#include <unistd.h>


int PtTcpRcv(int sockfd, char *buffer, int length)
{
	  int  len=0;
	  int  totalcnt;
	  totalcnt = 0;
	
	  while(totalcnt < length) {
	    len = read(sockfd, buffer + totalcnt, length - totalcnt);
	    if (len < 0) {
	      if ((errno == EINTR)||(errno == EAGAIN)) {
	        len = 0;
	        break;
	      }
	      else
	        {
	          return(-1);
	        }
	    }
	    else if (len == 0)
	    {
	      break;
	    }
	
	    totalcnt = totalcnt + len;
	
	  }
	
   return(totalcnt);
}

int main()
{   
	int count = 0;
	int sfp,nfp,size;
	struct sockaddr_in s_add,c_add;
	socklen_t sin_size;
	unsigned short portnum=12306;
	char rcvbuf[1024*1024];
	printf("Hello,welcome to my server !\n");
	sfp = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfp)
	{
	    printf("socket fail ! \n");
	    return -1;
	}
	printf("socket ok !\n");
	
	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family=AF_INET;
	s_add.sin_addr.s_addr=htonl(INADDR_ANY);
	s_add.sin_port=htons(portnum);
	
	if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
	    printf("bind fail !\n");
	    return -1;
	}
	printf("bind ok !\r\n");
	
	if(-1 == listen(sfp,5))
	{
	    printf("listen fail !\n");
	    return -1;
	}
	printf("listen ok\n");
	
	while(1)
	{
	sin_size = sizeof(struct sockaddr_in);
	
	nfp = accept(sfp, (struct sockaddr *)(&c_add), &sin_size);
	if(-1 == nfp)
	{
	    printf("accept fail !\n");
	    return -1;
	}
	printf("accept ok!\r\nServer start get connect from %#x : %d\n",ntohl(c_add.sin_addr.s_addr),ntohs(c_add.sin_port));
	size = 0;
	memset(rcvbuf, 0x00, sizeof(rcvbuf));
	size = PtTcpRcv(nfp, rcvbuf , sizeof(rcvbuf));
	if(-1 == size)
	{
	    printf("write fail!\n");
	    return -1;
	}
	
	FILE * wfp;
	
	char filename[20] = {0};
	sprintf(filename,"buf%d.bmp",count);
	wfp = fopen(filename,"wb");
	fwrite(rcvbuf,1,size,wfp);
    printf("write ok!\n");
    count++;
	}
	close(sfp);
	return 0;
}
	
