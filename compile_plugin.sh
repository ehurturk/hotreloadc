gcc -fPIC -shared -Wall -pedantic -O2 -I./core/core.h scripts/$1.$2 -o bin/compiled_libs/lib$1.so 
