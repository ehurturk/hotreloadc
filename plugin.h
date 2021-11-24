#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef size_t (*get_instance_size_func)();
typedef void (*on_plugin_reload_func)(void *);
typedef void (*on_plugin_update_func)();

struct plugin {
  void *handle; /* dll handle */
  void *instance;
  const char *w_lib_path, *s_lib_path;
  uint64_t lib_mod_time; /* last library modification time */
  size_t instance_size;  /* instance size */

  struct stat lib_stat;

  get_instance_size_func get_instance_size;
  on_plugin_reload_func on_plugin_reload;
  on_plugin_update_func on_plugin_update;
};

typedef struct plugin plugin_t;

#endif
