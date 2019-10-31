#include "level.h" //2 include point
#include <stdlib.h>
#ifndef BEING
#define BEING
typedef enum {frendly, hostile, netural, hero} attitude;
typedef struct
{
  point    pos;
  char     sym;
  unsigned speed;
  attitude opinon;
  int      hp;
  unsigned dmg;
} being;
void beings_init();


being * beings;
size_t   beings_size =0;
#endif
