#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include <time.h>
#include <stdio.h>

#define BUFSIZE 11

#define TIMEZONE_OFFSET 21600

#define MY_UUID { 0xC6, 0x8F, 0x9C, 0xB4, 0x80, 0x95, 0x49, 0xA8, 0x88, 0x0B, 0xC4, 0xFA, 0x3E, 0x1A, 0x2A, 0xCD }
PBL_APP_INFO(MY_UUID,
             "Unixtime", "davecoss.com",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer text_layer;
char time_str[BUFSIZE];
time_t currtime;

void update_display() {
	snprintf(time_str, BUFSIZE, "%d", (int)currtime);
	time_str[BUFSIZE - 1] = 0;
	text_layer_set_text(&text_layer, time_str);
}

void handle_tick(AppContextRef app_ctx, PebbleTickEvent *event) {
	currtime += 1;
	update_display();
}

void handle_init(AppContextRef ctx) {
  currtime = time(NULL) + TIMEZONE_OFFSET;
  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&text_layer, GRect(0, 65, 144, 30));
  text_layer_set_text_alignment(&text_layer, GTextAlignmentCenter);
  update_display();
  text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &text_layer.layer);

}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
		.tick_handler = &handle_tick,
		.tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
