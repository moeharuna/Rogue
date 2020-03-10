#include "beings.h"
#include "level.h"
#include "input.h"
#include <stdio.h>

int is_enemy(const being subject, const being object)
{
  switch(subject.opinion)
  {
  case HERO:
    if(object.opinion==HOSTILE)
      return 1;
    else return 0;
    break;
  case HOSTILE:
    if(object.opinion==HERO &&
       object.opinion==FRENDLY)
      return 1;
    else return 0;
    break;
  case FRENDLY:
    if(object.opinion==HOSTILE)
      return 1;
    else return 0;
  default:
    return 0;
    break;
  }
}

int being_exist(int ch)
{
  for(size_t i=0; i<being_types_s; ++i)
    {
      if(ch==being_types[i].sym) return 1;
    }
  return 0;
}


being* search_being(point pos)
{
  for(size_t i=0; i<beings_s; ++i)
  {
    if(beings[i].pos.x==pos.x && beings[i].pos.y==pos.y)
      return &beings[i];
  }
  return NULL;
}

void being_from_type(being * dest, const being* type )
{
  dest->sym =     type->sym;
  dest->hp  =     type->hp;
  dest->dmg =     type->dmg;
  dest->opinion = type->opinion;
  dest->speed =   type->speed;
}
void beings_init(char * filename)
{
  read_types(filename);
  point iter;
  for(iter.x=0; iter.x<this_lvl->horiz_size; ++iter.x)
    for(iter.y = 0; iter.y<this_lvl->vert_size; ++iter.y)
      if(being_exist(get_point(iter)))
      {
        beings_s++;
      }
  beings = malloc((beings_s+1)*sizeof(being)); //+1 for main_hero
  int beings_count=0;
  for(iter.x=0; iter.x<this_lvl->horiz_size; ++iter.x)
    for(iter.y = 0; iter.y<this_lvl->vert_size; ++iter.y)
      for(size_t i=0; i<being_types_s; ++i)
        if(get_point(iter)==being_types[i].sym)
        {
#ifdef DEUBG
          fprintf(stderr, "point =%c", get_point(iter));
#endif
          beings[beings_count].pos = iter;
          being_from_type(&beings[beings_count++], &being_types[i]);
        }
#ifdef DEBUG
  puts("main_hero init start");
#endif
  being * main_hero =malloc(sizeof(being));
  main_hero  = (&(being) {.sym='h', .speed=1, .opinion=HERO, .hp=10, .dmg=1, .pos={5, 5}});
  beings[beings_s++] = *main_hero;
  set_point(main_hero->pos, main_hero->sym);
#ifdef DEBUG
  puts("main_hero init end");
#endif

}
