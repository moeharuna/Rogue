#include <stdlib.h>
#ifndef ROGUE_LEVEL
#define ROGUE_LEVEL
typedef struct
{
  char * map;
  size_t vert_size, horiz_size;
}level;
typedef struct
{
  unsigned int x, y;
} point;
level * this_lvl;
int get_point (const point p)               ;
void set_point(const point p, const char ch);
void load_level(const char* filename);
#endif //ROGUE_LEVEL
