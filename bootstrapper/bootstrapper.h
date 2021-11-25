#ifndef BOOTSTRAPPER_H_
#define BOOTSTRAPPER_H_

// #include "../core/core.h"
#include "../plugin.h"
#include "../utils/cvector.h"

#include <dirent.h> /* filesystem */
#include <dlfcn.h>  /* library */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* also filesystem */
#include <sys/types.h>
#include <unistd.h>

#define lib_prepath "./bin/runtime_libs/"

void init_plugins();
void load_plugin_funcs();
void copy_plugin_libraries();
void update_plugins();
void delete_bootstrapper();

#endif
