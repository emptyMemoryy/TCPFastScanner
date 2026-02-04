#include "scanner.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(char *prog)
{
printf("usage: %s -t <target> [-p start-end] [-b N] [-T N] [--json]\n", prog);
 printf("--examples--:\n");
 printf("  %s -t 127.0.0.1 -p 1-1000\n", prog);
 printf("  %s -t 10.10.10.10 -p 1-1024 --json\n", prog);
}
int main(int argc, char **argv)
{
printf("========================================\n");
printf(" Simple Port Scanner v1.0\n");
printf(" Author: @emptyMemoryy\n");
printf("========================================\n\n");

if (argc < 2) {
usage(argv[0]);
return 1;
 }
sett cfg;
memset(&cfg, 0, sizeof(cfg));

 strncpy(cfg.target, "127.0.0.1", sizeof(cfg.target) - 1);
cfg.start_port = 1;
cfg.end_port   = 1024;
cfg.batch      = 100;
cfg.timeout    = 3;
cfg.grab_bnr   = 0;
cfg.js_out     = 0;

static struct option long_options[] = {
{"json",    no_argument,       0, 'j'},
{"timeout", required_argument, 0, 'T'},
{"batch",   required_argument, 0, 'B'},
{0, 0, 0, 0}
};

int opt;
while ((opt = getopt_long(argc, argv, "t:p:b:T:Bj", long_options, NULL)) != -1) {
switch (opt) {
case 't':
strncpy(cfg.target, optarg, sizeof(cfg.target) - 1);
cfg.target[sizeof(cfg.target) - 1] = '\0';
break;

case 'p':
if (sscanf(optarg, "%d-%d", &cfg.start_port, &cfg.end_port) != 2) {
if (sscanf(optarg, "%d", &cfg.start_port) == 1) {
cfg.end_port = cfg.start_port;
} else {
printf("invalid port format: %s\n", optarg);
return 1;
}
}
break;

case 'b':
cfg.batch = atoi(optarg);
break;

case 'T':
cfg.timeout = atoi(optarg);
break;

case 'B':
cfg.grab_bnr = 1;
break;

case 'j':
cfg.js_out = 1;
break;

default:
usage(argv[0]);
return 1;
  }
}
if (cfg.start_port < 1 || cfg.end_port > 65535 || cfg.start_port > cfg.end_port) {
printf("invalid port range :(\n");
return 1;
}
if (cfg.timeout <= 0) cfg.timeout = 3;
if (cfg.batch <= 0) cfg.batch = 100;
scan_ports(&cfg);
return 0;
}

