#include "beings.h"
#include "level.h"
being * being_types; //must be readed from file
size_t being_types_s;


int being_exist(int ch)
{
  for(size_t i=0; i<being_types_s; ++i)
    {
      if(ch==being_types[i].sym) return 1;
    }
  return 0;
}
void read_types(const char* filename)
{
  being_types = malloc(sizeof(being));
  being_types[0] = (being) {.sym='e', .speed=1, .opinion=hostile, .hp=1, .dmg=0};
  being_types_s = 1;
}
void beings_init(void)
{
  point iter;
  for(iter.x=0; iter.x<this_lvl->horiz_size; ++iter.x)
    for(iter.y = 0; iter.y<this_lvl->vert_size; ++iter.y)
      if(being_exist(get_point(iter)))
        beings_count++;
  beings = malloc(beings_count*sizeof(being));
  int beings_counter=0;
  for(iter.x=0; iter.x<this_lvl->horiz_size; ++iter.x)
    for(iter.y = 0; iter.y<this_lvl->vert_size; ++iter.y)
      for(size_t i=0; i<being_types_s; ++i)
        if(get_point(iter)==being_types[i].sym)
        {
          beings[beings_counter].pos = iter;
          beings[beings_counter++] =being_types[i];
        }

  main_hero  = (&(being) {.sym='h', .speed=1, .opinion=hero, .hp=10, .dmg=1, .pos={5, 5}});
}
