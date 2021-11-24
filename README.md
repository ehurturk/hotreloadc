# C Hotreloader for multiple plugins.
I have used @veridisquot's [tutorial](https://github.com/veridisquot/tutorials) on hot reloading with c on single file, and I have improved it to be able to be used with multiple scripts, or plugins. The program now only supports UNIX systems (Mac and Linux), and I am not considering supporting Windows right now since this is such a small project.

Please check [@veridisquot's tutorial](https://github.com/veridisquot/tutorials) since this really explains the logic behind it.

## Quick Start
To build the project, simply type the following into your terminal:
```sh
sh init_project.sh # to init the necessary project files
sh compile_plugin.sh player_movement c # to compile a plugin the parameters are [1] the name of the script and [2] the name of the language (can be c or cpp)
make hotreload
./out
```
Once you change the update code in `scripts/player_movement.c` and compile it, you will see the change being hot-reloaded.
