#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <scwm.h>



int main(void) {
	scwm_t *scwm = scwm_init();	

	scwm_event_loop(scwm);

	return 0;
}
