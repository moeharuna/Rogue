/*
  This file containts reading from file that contains all level`` information
 */

#ifndef ROGUE_INPUT
#define ROGUE_INPUT
#include "level.h"
#include "beings.h"
#include <limits.h>
enum keys {ARROW_UP=CHAR_MAX, ARROW_LEFT, ARROW_RIGHT, ARROW_DOWN};


void read_types(const char*);
void init_sizes(level * lvl, int alloc_count);
level * read_level(const char*);
void load_level(const char* filename);
enum keys key_press();
#endif
