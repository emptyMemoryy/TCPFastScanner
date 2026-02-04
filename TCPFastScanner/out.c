#include "out.h"
#include <stdio.h>

static const char *state_to_string(conn_state s) {
    if (s == CONN_OPEN) return "OPEN";
    if (s == CONN_CLOSED) return "CLOSED";
    if (s == CONN_FILTRED) return "FILTERED";
    return "UNKNOWN";
}

void print_result(conn *c, const char *service, int json) {
    if (!c)
        return;

    const char *state = state_to_string(c->state);

    if (json) {
        printf("{\"port\":%d,\"state\":\"%s\",\"service\":\"%s\",\"banner\":\"%s\"}\n",
               c->port,
               state,
               service ? service : "UNKNOWN",
               c->banner[0] ? c->banner : "");
    } else {
        if (c->state == CONN_OPEN) {
            printf("%d/tcp %-8s %-12s %s\n",
                   c->port,
                   state,
                   service ? service : "UNKNOWN",
                   c->banner[0] ? c->banner : "");
        } else {
            printf("%d/tcp %-8s\n", c->port, state);
        }
    }
}
