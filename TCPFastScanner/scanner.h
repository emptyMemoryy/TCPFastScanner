#ifndef SCANNER_H
#define SCANNER_H
#include <time.h>
typedef enum{
 CONN_CONNECTING,CONN_OPEN,CONN_CLOSED,
 CONN_FILTRED,CONN_DONE
}conn_state;
typedef struct {
 int sockfd;
 int port;
 conn_state state;
 clock_t start;
 char banner[512];
}conn;
typedef struct {
 char target[64];
 int start_port;
 int end_port;
 int timeout;
 int batch;
 int grab_bnr;
 int js_out;
}sett;
int scan_ports(sett* cfg);
#endif
