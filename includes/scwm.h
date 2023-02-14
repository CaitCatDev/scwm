#pragma once 

/*opaque structure*/
typedef struct scwm scwm_t;

scwm_t *scwm_init();
void scwm_event_loop(scwm_t *scwm);
