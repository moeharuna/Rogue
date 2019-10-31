#include "beings.h"
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
void beings_init()
{
  point iter;
  for(iter.x=0; iter.x<HORIZ_SIZE; ++iter.x)
    for(iter.y = 0; iter.y<VERT_SIZE; ++iter.y)
      if(being_exist(get_char(iter)))
        beings_size++;
  beings = malloc(beings_size*sizeof(being));
  int being_count=0;
  for(iter.x=0; iter.x<HORIZ_SIZE; ++iter.x)
    for(iter.y = 0; iter.y<VERT_SIZE; ++iter.y)
      for(size_t i=0; i<being_types_s; ++i)
        if(get_char(iter)==being_types[i].sym)
          beings[being_count++] =being_types[i];
}
