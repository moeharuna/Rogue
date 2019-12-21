#include <stdlib.h>
#ifndef ROGUE_LEVEL
#define ROGUE_LEVEL
#define EMPTY_CHAR ' '
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
point plus_point(const point, const int, const int);
int point_equals(const point p1, const point p2);
#endif //ROGUE_LEVEL
