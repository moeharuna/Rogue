#include "level.h" //2 include point
#include <stdlib.h>
#ifndef BEING
#define BEING
typedef enum {FRENDLY=0, HOSTILE=1, NETURAl=2, HERO=3} Attitude;
typedef struct
{
  point    pos;
  char     sym;
  unsigned speed;
  Attitude opinion;
  int      hp;
  unsigned dmg;

} being;
being* search_being(point pos);
void beings_init();


being * beings;
size_t  beings_s;
being * being_types; //must be readed from file
size_t being_types_s;


#endif //BEING
