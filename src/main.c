#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <scwm.h>



int main(int argc, char **argv) {
	scwm_t *scwm = scwm_init();	

	scwm_event_loop(scwm);

	return 0;
}
