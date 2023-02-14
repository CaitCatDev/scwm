#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <scwm.h>

struct scwm {
	xcb_connection_t *connection;
	xcb_screen_t *screen;
	xcb_window_t root;

};

const char *xcb_get_event_name(int event) {
	switch(event) {
		case XCB_KEY_PRESS:
			return "XCB_KEY_PRESS";
		case XCB_KEY_RELEASE:
			return "XCB_KEY_RELEASE";
		case XCB_BUTTON_PRESS:
			return "XCB_BUTTON_PRESS";
		case XCB_BUTTON_RELEASE:
			return "XCB_BUTTON_RELEASE";
		case XCB_MOTION_NOTIFY:
			return "XCB_MOTION_NOTIFY";
		case XCB_ENTER_NOTIFY:
			return "XCB_ENTER_NOTIFY";
		case XCB_LEAVE_NOTIFY:
			return "XCB_LEAVE_NOTIFY";
		case XCB_FOCUS_IN:
			return "XCB_FOCUS_IN";
		case XCB_FOCUS_OUT:
			return "XCB_FOCUS_OUT";
		case XCB_KEYMAP_NOTIFY:
			return "XCB_KEYMAP_NOTIFY";
		case XCB_EXPOSE:
			return "XCB_EXPOSE";
		case XCB_GRAPHICS_EXPOSURE:
			return "XCB_GRAPHICS_EXPOSURE";
		case XCB_NO_EXPOSURE:
			return "XCB_NO_EXPOSURE";
		case XCB_VISIBILITY_NOTIFY:
			return "XCB_VISIBILITY_NOTIFY";
		case XCB_CREATE_NOTIFY:
			return "XCB_CREATE_NOTIFY";
		case XCB_DESTROY_NOTIFY:
			return "XCB_DESTROY_NOTIFY";
		case XCB_UNMAP_NOTIFY:
			return "XCB_UNMAP_NOTIFY";
		case XCB_MAP_NOTIFY:
			return "XCB_MAP_NOTIFY";
		case XCB_MAP_REQUEST:
			return "XCB_MAP_REQUEST";
		case XCB_REPARENT_NOTIFY:
			return "XCB_REPARENT_NOTIFY";
		case XCB_CONFIGURE_NOTIFY:
			return "XCB_CONFIGURE_NOTIFY";
		case XCB_CONFIGURE_REQUEST:
			return "XCB_CONFIGURE_REQUEST";
		case XCB_GRAVITY_NOTIFY:
			return "XCB_GRAVITY_NOTIFY";
		case XCB_RESIZE_REQUEST:
			return "XCB_RESIZE_REQUEST";
		case XCB_CIRCULATE_NOTIFY:
			return "XCB_CIRCULATE_NOTIFY";
		case XCB_CIRCULATE_REQUEST:
			return "XCB_CIRCULATE_REQUEST";
		case XCB_PROPERTY_NOTIFY:
			return "XCB_PROPERTY_NORIFY";
		case XCB_SELECTION_CLEAR:
			return "XCB_SELECTION_CLEAR";
		case XCB_SELECTION_NOTIFY:
			return "XCB_SELECTION_NOTIFY";
		case XCB_SELECTION_REQUEST:
			return "XCB_SELECTION_REQUEST";
		case XCB_COLORMAP_NOTIFY:
			return "XCB_COLORMAP_NOTIFY";
		case XCB_CLIENT_MESSAGE:
			return "XCB_CLIENT_MESSAGE";
		case XCB_MAPPING_NOTIFY:
			return "XCB_MAPPING_NOTIFY";
		case XCB_GE_GENERIC:
			return "XCB_GE_GENERIC";
	}
		return "UNKNOWN";
}

scwm_t *scwm_init() {
	uint32_t values[1];

	values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
			| XCB_EVENT_MASK_STRUCTURE_NOTIFY
			| XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
			| XCB_EVENT_MASK_PROPERTY_CHANGE;
	
	scwm_t *scwm = calloc(1, sizeof(scwm_t));
	if(scwm == NULL) {
		return scwm;
	}

	scwm->connection = xcb_connect(NULL, NULL);
	if(xcb_connection_has_error(scwm->connection) != 0) {
		xcb_disconnect(scwm->connection);
		return NULL;
	}

	scwm->screen = xcb_setup_roots_iterator(xcb_get_setup(scwm->connection)).data;

	scwm->root = scwm->screen->root;
	//Register ourselves to listen out for events and 
	//for the server to redirect events to us
	xcb_change_window_attributes(scwm->connection, scwm->root, 
								XCB_CW_EVENT_MASK, values);

	xcb_flush(scwm->connection);	
	return scwm;
}

void scwm_free_xcb_event(void *event) {
	free(event);
}

void scwm_event_loop(scwm_t *scwm) {
	xcb_generic_event_t *event = NULL;
	while(1) {
		event = xcb_wait_for_event(scwm->connection);

		printf("Event: %s\n", xcb_get_event_name(event->response_type & ~(0x80)));
	
		scwm_free_xcb_event(event);
	}
}
