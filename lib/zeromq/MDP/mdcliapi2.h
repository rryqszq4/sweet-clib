#ifndef __MDCLIAPI_H_INCLUDED__
#define __MDCLIAPI_H_INCLUDED__

#include "czmq.h"
#include "mdp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _mdcli_t mdcli_t;

mdcli_t *mdcli_new(char *broker, int verbose);
void mdcli_destroy(mdcli_t **self_p);
void mdcli_set_timeout(mdcli_t *self, int timeout);
void mdcli_set_retries(mdcli_t *self, int retries);
int mdcli_send(mdcli_t *self, char *service, zmsg_t **request_p);
zmsg_t *mdcli_recv(mdcli_t *self);

#ifdef __cplusplus
}
#endif

#endif