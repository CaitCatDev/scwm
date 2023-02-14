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

void scwm_handle_map_request(scwm_t *scwm, xcb_generic_event_t *event);
void scwm_handle_configure_request(scwm_t *scwm, xcb_generic_event_t *event);
void scwm_handle_button_press(scwm_t *scwm, xcb_generic_event_t *event); 
void scwm_handle_button_release(scwm_t *scwm, xcb_generic_event_t *event);


typedef struct scwm_event_handlers {
	int event;
	void (*handler)(scwm_t *scwm, xcb_generic_event_t *event);
} scwm_event_handlers_t;

static scwm_event_handlers_t scwm_handlers[] = {
	{ XCB_MAP_REQUEST, scwm_handle_map_request },
	{ XCB_CONFIGURE_REQUEST, scwm_handle_configure_request }, 
	{ XCB_BUTTON_PRESS, scwm_handle_button_press },
	{ XCB_BUTTON_RELEASE, scwm_handle_button_release },
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

void scwm_handle_map_request(scwm_t *scwm, xcb_generic_event_t *event) {
	xcb_map_request_event_t *ev = (void *) event;

	xcb_map_window(scwm->connection, ev->window);
	xcb_flush(scwm->connection);
}

void scwm_handle_button_press(scwm_t *scwm, xcb_generic_event_t *event) {
	//Bypass unused warnings
	(void)scwm;
	(void)event;

	//TODO handle press
}

void scwm_handle_button_release(scwm_t *scwm, xcb_generic_event_t *event) {
	(void)scwm;
	(void)event;
	//TODO handle release
}

void scwm_handle_configure_request(scwm_t *scwm, xcb_generic_event_t *event) {
	uint32_t values[7];
	uint32_t i = 0;
	xcb_configure_request_event_t *ev = (void*) event;

	if (ev->value_mask & XCB_CONFIG_WINDOW_X) {
		values[i] = ev->x;
		i++;
	}

        if (ev->value_mask & XCB_CONFIG_WINDOW_Y) {
		values[i] = ev->y;
		i++;
	}

        if (ev->value_mask & XCB_CONFIG_WINDOW_WIDTH) {
		values[i] = ev->width; 
		i++;
	}

        if (ev->value_mask & XCB_CONFIG_WINDOW_HEIGHT) {
		values[i] = ev->height;
		i++;
	}
        
	if (ev->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH) {
		values[i] = ev->border_width;
		i++;
	}

        if (ev->value_mask & XCB_CONFIG_WINDOW_SIBLING)  {
		values[i] = ev->sibling;
		i++;
	}

        if (ev->value_mask & XCB_CONFIG_WINDOW_STACK_MODE) {
		values[i] = ev->stack_mode;
		i++;
	}
	printf("Cofigure Request Window: %d\n"
	       "\tX: %d\n"
	       "\tY: %d\n"
	       "\tWidth: %d\n"
	       "\tHeight: %d\n",
	       ev->window,
	       ev->x, ev->y,
	       ev->width,
	       ev->height);

        xcb_configure_window(scwm->connection, ev->window, ev->value_mask, values);
	xcb_flush(scwm->connection);
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
		
		for(size_t i = 0; i < sizeof(scwm_handlers) / sizeof(scwm_handlers[0]); ++i) {
			if((event->response_type & ~(0x80)) == scwm_handlers[i].event &&
					scwm_handlers[i].handler) {
				scwm_handlers[i].handler(scwm, event);
			}
		}

		scwm_free_xcb_event(event);
	}
}
