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
	xcb_window_t window;
	int detail;
};

void scwm_handle_map_request(scwm_t *scwm, xcb_generic_event_t *event);
void scwm_handle_configure_request(scwm_t *scwm, xcb_generic_event_t *event);
void scwm_handle_button_press(scwm_t *scwm, xcb_generic_event_t *event); 
void scwm_handle_button_release(scwm_t *scwm, xcb_generic_event_t *event);
void scwm_handle_motion_notify(scwm_t *scwm, xcb_generic_event_t *event);

typedef struct scwm_event_handlers {
	int event;
	void (*handler)(scwm_t *scwm, xcb_generic_event_t *event);
} scwm_event_handlers_t;

static scwm_event_handlers_t scwm_handlers[] = {
	{ XCB_MAP_REQUEST, scwm_handle_map_request },
	{ XCB_CONFIGURE_REQUEST, scwm_handle_configure_request }, 
	{ XCB_BUTTON_PRESS, scwm_handle_button_press },
	{ XCB_BUTTON_RELEASE, scwm_handle_button_release },
	{ XCB_MOTION_NOTIFY, scwm_handle_motion_notify },
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

}

void scwm_handle_button_press(scwm_t *scwm, xcb_generic_event_t *event) {
	xcb_button_press_event_t *ev = (void *)event;
	//Bypass unused warnings
	printf("%d, %d\n", ev->child, scwm->root);
	scwm->window = ev->child;
	scwm->detail = ev->detail;
	//Grab root window pointer
	//Now we should get motion events with this window
	xcb_grab_pointer(scwm->connection, 0, scwm->root, XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_BUTTON_MOTION | XCB_EVENT_MASK_POINTER_MOTION_HINT,
			XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, scwm->root, XCB_NONE,
			XCB_CURRENT_TIME);
	
	//Flush to server
}

void scwm_handle_button_release(scwm_t *scwm, xcb_generic_event_t *event) {
	xcb_button_release_event_t *ev = (void *)event;
	
	xcb_ungrab_pointer(scwm->connection, ev->time);
}

void scwm_handle_motion_notify(scwm_t *scwm, xcb_generic_event_t *event) {
	uint32_t values[2];
	xcb_query_pointer_cookie_t ptr_cookie = xcb_query_pointer(scwm->connection, scwm->root);
	xcb_query_pointer_reply_t *pointer = xcb_query_pointer_reply(scwm->connection, ptr_cookie, 0); 
	xcb_motion_notify_event_t *ev = (void *)event;
	
	printf("%d %d\n%dx%d\n%d\n", ev->root_x, ev->root_y, ev->event_x, ev->event_y, scwm->window);

	values[0] = ev->root_x;
	values[1] = ev->root_y;
	printf("%d:%d\n", values[0],values[1]);
	if(scwm->detail == 1) {
		xcb_configure_window(scwm->connection, scwm->window, XCB_CONFIG_WINDOW_X | 
			XCB_CONFIG_WINDOW_Y, values);
	} else if(scwm->detail == 3) {
		xcb_configure_window(scwm->connection, scwm->window, 
				XCB_CONFIG_WINDOW_HEIGHT | XCB_CONFIG_WINDOW_WIDTH,
				values);
	}
	

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

	xcb_ungrab_key(scwm->connection, XCB_GRAB_ANY, scwm->root, XCB_MOD_MASK_ANY);
	xcb_flush(scwm->connection);
	
	xcb_grab_button(scwm->connection, 0, scwm->root,
			XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
			XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, scwm->root, XCB_NONE,
			XCB_BUTTON_INDEX_1, XCB_MOD_MASK_1);
	xcb_grab_button(scwm->connection, 0, scwm->root,
			XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
			XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, scwm->root, XCB_NONE,
			XCB_BUTTON_INDEX_3, XCB_MOD_MASK_1);
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
		xcb_flush(scwm->connection);
	}
}
