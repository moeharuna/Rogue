#include "level.h"
#include <stdio.h>
#include <stdlib.h>

int map_str_len(size_t vert)
{
  int i;
  for(i=0; this_lvl->map[vert*this_lvl->horiz_size+i]!='\n'; ++i)
  {;}
  return i;
}

int get_point (const point p){
  #ifdef DEBUG
  if(   p.y>this_lvl->vert_size
     || p.x>this_lvl->horiz_size)
  {
    fprintf(stderr, "map overflow! p.y=%d, p.x=%d, h_size=%ld, v_size=%ld\r\n",
                                   p.y, p.x, this_lvl->horiz_size, this_lvl->vert_size);
    exit(1);
  }
  else if(p.x<map_str_len(p.y))
  {
    fprintf(stderr, "map missmatch!"); //not overflow, but non-existing point
  }
  #endif
  return this_lvl->map[p.y*this_lvl->horiz_size+p.x];
}
void set_point(const point p, const char ch) {
  this_lvl->map[p.y*this_lvl->horiz_size+p.x]=ch;
}
point  plus_point(const point p, const int x, const int y)
{
  return (point){p.x+x, p.y+y};
}

int point_equals(const point p1, const point p2)
{
  return p1.x==p2.x && p1.y==p2.y;
}

point rel2abs(const point center, const point p) //relative to absolute
{
  point where = {center.x+p.x, center.y+p.y};
  return where;
}

int isfree(point p)
{
  if(p.x>=this_lvl->horiz_size ||
     p.y>=this_lvl->vert_size) return 0;
  if(get_point(p)==EMPTY_CHAR) return 1;
  return 0;
}
