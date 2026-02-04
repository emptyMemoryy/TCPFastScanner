#include "banner.h"

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

void grab_banner(const char *ip, int port, char* out,size_t out_size){
 int sock;
 struct sockaddr_in addr;
 char buf[512];
 if(!out||out_size == 0) return;
 out[0]='\0';
 sock = socket(AF_INET,SOCK_STREAM,0);
 if(sock <0) return;
 memset(&addr,0,sizeof(addr));
 addr.sin_family=AF_INET;
 addr.sin_port=htons(port);
 inet_pton(AF_INET,ip,&addr.sin_addr);
 if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0) return;

 // http req (;
 if(port==80){send(sock,"HEAD / HTTP/ 1.0\r\n\r\n",19,0);} 
 int n = recv(sock,buf,sizeof(buf)-1,0);
 if(n>0){buf[n]='\0';strncpy(out,buf,out_size - 1); out[out_size-1]='\0';}
 close(sock);
}
