#ifndef CORE_H_
#define CORE_H_

#define IS_UPPER(x) (64 < (int)x && (int)x < 90)
#define KEY(x) IS_UPPER(x) ? (int)x - 65 : (int)x - 97

void init_core();
void end_core();
void print_time();
void print_window(const char *msg);
int get_async_key();

#endif
