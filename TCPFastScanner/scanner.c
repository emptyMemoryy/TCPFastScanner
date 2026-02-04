#include "scanner.h"

#include "banner.h"
#include "srvc.h"
#include "out.h"

#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>


int makeNonblocking(int fd)
{
 int flags = fcntl(fd, F_GETFL, 0);
 if (flags == -1) {
   return -1;
 }
   if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
  return -1;
 }
 return 0;
}

static void connection(conn*c,const char *ip,int port){
 struct sockaddr_in addr;
 c->sockfd = socket(AF_INET,SOCK_STREAM,0);
 c->port = port;
 c->state = CONN_CONNECTING;
 c->banner[0] = '\0';
 makeNonblocking(c->sockfd);
 memset(&addr,0,sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_port = htons(port);
 inet_pton(AF_INET,ip,&addr.sin_addr);
 connect(c->sockfd,(struct sockaddr*)&addr,sizeof(addr));
}

int scan_ports(sett* cfg) {
int total_ports = cfg->end_port - cfg->start_port + 1;

for (int base = cfg->start_port; base <= cfg->end_port; base += cfg->batch) {
int batch_end = base + cfg->batch - 1;
if (batch_end > cfg->end_port) batch_end = cfg->end_port;
int batch_count = batch_end - base + 1;

conn *conns = malloc(batch_count * sizeof(conn));
if (!conns) { perror("malloc"); exit(1); }

for (int i = 0; i < batch_count; i++) {
connection(&conns[i], cfg->target, base + i);
}

int active = batch_count;

while (active > 0) {
fd_set wfds;
FD_ZERO(&wfds);

int maxfd = 0;
for (int i = 0; i < batch_count; i++) {
if (conns[i].state == CONN_CONNECTING) {
FD_SET(conns[i].sockfd, &wfds);
if (conns[i].sockfd > maxfd) maxfd = conns[i].sockfd;
}
}

struct timeval tv = {0, 200000};
select(maxfd + 1, NULL, &wfds, NULL, &tv);

clock_t now = clock();

for (int i = 0; i < batch_count; i++) {
if (conns[i].state != CONN_CONNECTING) continue;

double elapsed = (double)(now - conns[i].start) / CLOCKS_PER_SEC;
if (elapsed > cfg->timeout) {
conns[i].state = CONN_FILTRED;
close(conns[i].sockfd);
active--;
continue;
}
if (FD_ISSET(conns[i].sockfd, &wfds)) {
int err = 0;
socklen_t len = sizeof(err);
 getsockopt(conns[i].sockfd, SOL_SOCKET, SO_ERROR, &err, &len);
 conns[i].state = (err == 0) ? CONN_OPEN : CONN_CLOSED;
 close(conns[i].sockfd);
 active--;
   }
  }
 }
 if (cfg->grab_bnr) {
 for (int i = 0; i < batch_count; i++) {
 if (conns[i].state == CONN_OPEN) {
 grab_banner(cfg->target, conns[i].port, conns[i].banner, sizeof(conns[i].banner));
   }
  }
 }
for (int i = 0; i < batch_count; i++) {
const char *service = detect_service(conns[i].port, conns[i].banner);
print_result(&conns[i], service, cfg->js_out);
 }
free(conns);
 }
return total_ports;
}

