#include "level.h"
#include "beings.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#define BUFFER_SIZE 512



void remove_whitespace(char * str, size_t size)
{
  for(size_t i=0; i<size; ++i)
  {
    if(isspace(str[i]) && i!=size-1)
    {
      for(size_t j=i; j<size-1; ++j)
      {
        str[j] = str[j+1];
      }
    }
    else if(isspace(str[i]) && i==size-1)
    {
      str[i]='\0';
    }
  }
}

void read_types(const char* filename)
{
  int read_ln(char ** key, size_t * ks, char ** value, size_t * vs, FILE * file)
  {

    if(getdelim(key, ks, ':', file)==-1)
      return 0;
    getline(value, vs, file);
    remove_whitespace(*key, strlen(*key));
    remove_whitespace(*value, strlen(*value));
    #ifdef DEBUG
    fprintf(stderr, "%ld, %ld", *ks, *vs);
    fprintf(stderr, "key=\'%s\', val=\'%s\'\n", *key, *value);
    #endif
    return 1; //not empty
  }
  FILE * file= fopen(filename, "r");
  if(file==NULL)
  {
    perror("types fopen error!");
    exit(1);
  }
  being_types = malloc(sizeof(being));
  being_types_s=-1;
  char * key = NULL;
  char * val = NULL;
  size_t key_size=0, val_size=0;
#ifdef DEBUG
  puts("start file parsing");
#endif
  while(read_ln(&key, &key_size, &val, &val_size, file)) //while not empty
  {
    if(strcmp(key, "sym:")==0) //this part set being.sym and creates new being_type for each sym
    {
      being_types_s++;
      if(isalpha(val[0]))
        being_types[being_types_s].sym=val[0];
      else
        fprintf(stderr, "error! wrong sym, it's =%s", val);

      if(realloc(being_types, (being_types_s+1)*sizeof(being))==NULL)
      {
        perror("read_types realloc");
        exit(1);
      }
    }
    else if(strcmp(key, "speed:")==0)
      being_types[being_types_s].speed   = atoi(val);
    else if(strcmp(key, "opinion:")==0)
      being_types[being_types_s].opinion = atoi(val); //must be set as number
    else if(strcmp(key, "hp:")==0)
      being_types[being_types_s].hp      = atoi(val);
    else if(strcmp(key, "dmg:")==0)
      being_types[being_types_s].dmg     = atoi(val);
  }
  being_types_s++;
  free(key);
  free(val);
  fclose(file);
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
    perror("level fopen error!");
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
  fclose(file);
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


enum keys key_press() //this function return
                //1. ascii_key value if input in ascii(and not in escape sequence as '[')
                //2. value from key enum if it's sequence
                //3. -1 otherwise
{
  char ch;
  if(read(STDIN_FILENO, &ch, 1)==1)
  {
    switch(ch)
    {
    case '[':
      {
        if(read(STDIN_FILENO, &ch, 1)==1)
        {
          switch(ch)
          {
          case 'A':
            return ARROW_UP;
          case 'D':
            return ARROW_LEFT;
          case 'C':
            return ARROW_RIGHT;
          case 'B':
            return ARROW_DOWN;
          default:
            return -1;
          }
        }
        else
          return '[';
        break;
      }
    default:
      if(ch<CHAR_MAX) return ch;
      else return -1;
    }
  }
  return -1;
}
