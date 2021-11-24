#include "bootstrapper.h"
#include <string.h>

cvector_vector_type(plugin_t *) bootstrapper = NULL;

DIR *dir;
struct dirent *ent;

void init_plugins() {
  /* get the name of all plugins and then initialize them and add to the vector
   */
  if ((dir = opendir("./bin/compiled_libs/")) != NULL) {
    int i = 0;
    while ((ent = readdir(dir)) != NULL) {
      i++;
      const char *libname = ent->d_name;
      if (i > 2) {
        char *w_rel_path = malloc(sizeof(char) * 60),
             *s_rel_path = malloc(sizeof(char) * 60);
        strcpy(w_rel_path, "./bin/runtime_libs/");
        strcpy(s_rel_path, "./bin/compiled_libs/");
        strcat(w_rel_path, libname);
        strcat(s_rel_path, libname);
        plugin_t *plugin = malloc(sizeof(plugin_t));
        plugin->w_lib_path = w_rel_path;
        plugin->s_lib_path = s_rel_path;
        plugin->handle = NULL;
        plugin->lib_mod_time = 0;
        plugin->get_instance_size = NULL;
        plugin->on_plugin_reload = NULL;
        plugin->on_plugin_update = NULL;
        cvector_push_back(bootstrapper, plugin);
      }
    }
    closedir(dir);
  } else {
    printf("No plugin is found...\n");
    exit(1);
  }
}

void load_plugin_funcs() {
  for (int i = 0; i < cvector_size(bootstrapper); i++) {
    plugin_t *plugin = bootstrapper[i];
    plugin->handle = dlopen(plugin->w_lib_path, RTLD_NOW);
    if (!plugin->handle) {
      fprintf(stderr, "Failed to load library: %s\n", plugin->w_lib_path);
      return;
    }

    plugin->get_instance_size =
        (get_instance_size_func)dlsym(plugin->handle, "get_instance_size");
    plugin->on_plugin_reload =
        (on_plugin_reload_func)dlsym(plugin->handle, "on_plugin_reload");
    plugin->on_plugin_update =
        (on_plugin_update_func)dlsym(plugin->handle, "on_plugin_update");
  }
}

void copy_plugin_libraries() {
  for (int i = 0; i < cvector_size(bootstrapper); i++) {
    plugin_t *plug = bootstrapper[i];
    FILE *src, *dst;
    size_t size, read, i;
    char buffer[2048];

    src = fopen(plug->s_lib_path, "rb");
    dst = fopen(plug->w_lib_path, "wb");

    fseek(src, 0, SEEK_END);
    size = ftell(src);
    fseek(src, 0, SEEK_SET);

    for (i = 0; i < size; i += 2048) {
      read = fread(buffer, 1, 2048, src);
      fwrite(buffer, read, 1, dst);
    }

    fclose(src);
    fclose(dst);
  }
}

void update_plugins() {
  for (int i = 0; i < cvector_size(bootstrapper); i++) {
    plugin_t *plug = bootstrapper[i];
    if (stat(plug->s_lib_path, &plug->lib_stat) == 0) {
      if (plug->lib_stat.st_mtime > plug->lib_mod_time) {
        if (plug->handle)
          dlclose(plug->handle);

        copy_plugin_libraries();
        load_plugin_funcs();
        size_t new_size = plug->get_instance_size();

        if (new_size != plug->instance_size) {
          plug->instance = realloc(plug->instance, new_size);

          if (new_size > plug->instance_size)
            memset((char *)plug->instance + plug->instance_size, 0,
                   new_size - plug->instance_size);

          plug->instance_size = new_size;
        }

        plug->lib_mod_time = plug->lib_stat.st_mtime;
        plug->on_plugin_reload(plug->instance);
      }
    }
    plug->on_plugin_update();
  }
}

void delete_bootstrapper() {
  for (int i = 0; i < cvector_size(bootstrapper); i++) {
    free((void *)bootstrapper[i]->w_lib_path);
    free((void *)bootstrapper[i]->s_lib_path);
    free(bootstrapper[i]->instance);
    free(bootstrapper[i]);
  }
  cvector_free(bootstrapper);
}

int main(int argc, char **argv) {
  printf("Starting the app...\n");
  init_plugins();
  for (int i = 0; i < 10; i++) {
    update_plugins();
    sleep(1);
  }
  delete_bootstrapper();
}
