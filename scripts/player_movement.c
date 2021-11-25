#include "../core/core.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct player_motor {
  int velocity;
};

typedef struct player_motor player_motor_t;

player_motor_t *m_instance;

size_t get_instance_size() { return sizeof(player_motor_t); }

/*
 * would be called once the plugin is modified+compiled runtime
 * it is advised to initialize necessary variables in here (such as malloc'ing
 * pointers)
 */
void on_plugin_reload(void *ptr) { m_instance = (player_motor_t *)ptr; }

/* would be called in the update loop */
void on_plugin_update() {
  // int ch = get_async_key();
  // if (ch == 65) {
  //   char *msg = NULL;
  //   sprintf(msg, "Player velocity: %i\n", m_instance->velocity);
  //   print_window(msg);
  // }
  printf("Player velocity: %i\n", m_instance->velocity);
}
