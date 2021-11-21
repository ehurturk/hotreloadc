#include "bootstrapper.h"

cvector_vector_type(plugin_t) bootstrapper = NULL;

DIR *dir;
struct dirent *ent;

void load_plugin_funcs() {
  printf("loooad\n");
  if ((dir = opendir(lib_prepath)) != NULL) {
    int i = 0;
    while ((ent = readdir(dir)) != NULL) {
      i++;
      /* TODO Seperate this initializing function into a different function. */
      const char *libname = ent->d_name;
      if (i > 2) {
        char *rel_path = NULL;
        sprintf(rel_path, "%s%s", lib_prepath,
                libname); /* example: ./bin/runtime_libs/libscript1.so */
        plugin_t plugin;
        plugin.lib_name = libname;
        plugin.handle = dlopen(rel_path, RTLD_NOW);
        plugin.lib_mod_time = 0;
        if (!plugin.handle) {
          fprintf(stderr, "Failed to load library: %s\n", rel_path);
          return;
        }

        plugin.get_instance_size =
            (get_instance_size_func)dlsym(plugin.handle, "get_instance_size");
        plugin.on_plugin_reload =
            (on_plugin_reload_func)dlsym(plugin.handle, "on_plugin_reload");
        plugin.on_plugin_update =
            (on_plugin_update_func)dlsym(plugin.handle, "on_plugin_update");

        cvector_push_back(bootstrapper, plugin);
      }
    }
    closedir(dir);
  } else {
    printf("No plugin is found...\n");
    exit(1);
  }
}

void copy_plugin_libraries() {
  for (int i = 0; i < cvector_size(bootstrapper); i++) {
    plugin_t plug = bootstrapper[i];
    FILE *src, *dst;
    size_t size, read, i;
    char buffer[2048];

    char *c_path = NULL, *w_path = NULL;
    sprintf(c_path, "./bin/compiled_libs/%s", plug.lib_name);
    sprintf(w_path, "./bin/runtime_libs/%s", plug.lib_name);

    src = fopen(c_path, "rb");
    dst = fopen(w_path, "wb");

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
  printf("wtf?\n");
  for (int i = 0; i < cvector_size(bootstrapper); i++) {
    printf("heee\n");
    plugin_t plug = bootstrapper[i];
    char *libpath = NULL;
    sprintf(libpath, "./bin/compiled_libs/%s", plug.lib_name);
    printf("lib_name: %s\n", libpath);
    if (stat(libpath, &plug.lib_stat) == 0) {
      if (plug.lib_stat.st_mtimespec.tv_sec > plug.lib_mod_time) {
        if (plug.handle)
          dlclose(plug.handle);

        copy_plugin_libraries();
        load_plugin_funcs();

        size_t new_size = plug.get_instance_size();

        if (new_size != plug.instance_size) {
          plug.instance = realloc(plug.instance, new_size);

          if (new_size > plug.instance_size)
            memset((char *)plug.instance + plug.instance_size, 0,
                   new_size - plug.instance_size);

          plug.instance_size = new_size;
        }

        plug.lib_mod_time = plug.lib_stat.st_mtimespec.tv_sec;
        plug.on_plugin_reload(plug.instance);
      }
    }
    plug.on_plugin_update();
  }
}

void delete_bootstrapper() {
  for (int i = 0; i < cvector_size(bootstrapper); i++) {
    free(bootstrapper[i].instance);
  }
  cvector_free(bootstrapper);
}

int main(int argc, char **argv) {
  printf("Starting the app...\n");
  load_plugin_funcs();
  while (1) {
    update_plugins();
    sleep(1);
  }
  delete_bootstrapper();
}
