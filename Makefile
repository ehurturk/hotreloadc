
hotreload:
	clang -fPIC -shared -Wall -pedantic -Icore/core.h core/core.c -o bin/libcore.so -lncurses
	clang -Lbin -Wall -pedantic -O2  -Ibootstrapper/bootstrapper.h  main.c bootstrapper/bootstrapper.c -o out bin/libcore.so -ldl

