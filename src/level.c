#include "level.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512


#ifdef DEBUG
int map_str_len(size_t vert)
{
  int i;
  for(i=0; this_lvl->map[vert*this_lvl->horiz_size+i]!='\n'; ++i)
  {;}
  return i;
}
#endif

int get_point (const point p){
  #ifdef DEBUG
  if(   p.y>this_lvl->vert_size
     || p.x>this_lvl->horiz_size)
  {
    fprintf(stderr, "map overflow! p.y=%d, p.x=%d, h_size=%ld, v_size=%ld\r\n",
                                   p.y, p.x, this_lvl->horiz_size, this_lvl->vert_size);
    exit(1);
  }
  #ifdef DEEP_DEBUG
  else if(p.x<map_str_len(p.y))
  {
    fprintf(stderr, "map missmatch!"); //not overflow, but non-existing point
  }
  #endif
  #endif
  return this_lvl->map[p.y*this_lvl->horiz_size+p.x];
}
void set_point(const point p, const char ch) {
  this_lvl->map[p.y*this_lvl->horiz_size+p.x]=ch;
}

void init_sizes(level * lvl,int alloc_count)
{
  int size = BUFFER_SIZE*alloc_count;
  int longest_str =0;
  int this_str    =0;
  for(int i=0; i<size; ++i)
  {
    this_str++;
    if(lvl->map[i]=='\n')
    {
      lvl->vert_size++;
      if(this_str>longest_str)
      {
        longest_str = this_str;
      }
      this_str=0;
    }
  }
  lvl->horiz_size = longest_str;
}

level * read_level(const char* filename)
{
  FILE * file= fopen(filename, "r");
  if(file==NULL)
  {
    perror("fopen error!");
    exit(1);
  }
  level * result = malloc(sizeof(level));
  int nread =0;
  result->map = malloc(BUFFER_SIZE*sizeof(char));
  int times=1;
  while( (nread = fread(result->map, sizeof(char), BUFFER_SIZE, file))!=0)
  {
    #ifdef DEBUG
    fprintf(stderr, "%d\r\n", nread);
    #endif
    if(nread==BUFFER_SIZE) //if size of file 2much 2read it in one buffer
    {
      times++;
      if(realloc(result->map, BUFFER_SIZE*sizeof(char)*times)==NULL)
      {
        perror("read_level realloc:");
        exit(1);
      }
    }
  }
  init_sizes(result, times);
  return result;
}

void load_level(const char* filename)
{
  if(this_lvl!=NULL)
  {
    free(this_lvl->map);
    free(this_lvl);
  }
  this_lvl=read_level(filename);
}
