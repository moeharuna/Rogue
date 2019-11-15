#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include "level.h"
/* #include "beings.h"              */

#define MAIN_CHAR  '@'
#define EMPTY_CHAR ' '





point rel2abs(const point center, const point p) //relative to absolute
{
  point where = {center.x+p.x, center.y+p.y};
  return where;
}

/* void print_status(const being h) */
/* { */
/*   printf("hp=%d, dmg=%d, speed=%d\r\n", h.hp, h.dmg, h.speed); */
/* } */
void print_map()
{
  point std;
  for(std.y=0; std.y<this_lvl->vert_size; ++std.y)
  {
    for(std.x=0; std.x<this_lvl->horiz_size; ++std.x)
    {
      putchar(get_point(std));
    }
    //printf("\r\n"); //\n don't makes newline in raw_mode
  }
}

/* being* search_being(point pos) */
/* { */
/*   for(size_t i=0; i<beings_count; ++i) */
/*   { */
/*     if(beings[i].pos.x==pos.x && beings[i].pos.y ==pos.y) */
/*       return &beings[i]; */
/*   } */
/*   return NULL; */
/* } */

/* int isenemy(point pos) */
/* { */
/*   if(search_being(pos)!=NULL) */
/*     return 1; */
/*   return 0; */
/* } */


int isfree(point p)
{
  if(p.x>=this_lvl->horiz_size ||
     p.y>=this_lvl->vert_size) return 0;
  if(get_point(p)==EMPTY_CHAR) return 1;
  return 0;
}

void clear()
{
  char seq[] = {27, '[', '2', 'J'};
  puts(seq);
}

void game_over()
{
  clear();
  printf("You are dead");
  exit(0);
}


/* void being_die(being *body) */
/* { */
/*   set_point(body->pos, EMPTY_CHAR); */
/*   if(body->sym==MAIN_CHAR) */
/*   { */
/*     game_over(); */
/*   } */
/* } */

/* void attack(const being atk, being* def)  */
/* { */
/*   #ifdef DEBUG */
/*   printf("def->hp= %d\n", def->hp); */
/*   #endif */
/*   def->hp-=atk.dmg; */
/*   if(def->hp<=0) */
/*   { */
/*     being_die(def); */
/*   } */
/* } */


/* void move(being* b, point distance) */
/* { */
/* #ifdef DEBUG */
/*   printf("Move from (%d, %d) on (%d %d) pixels\n", b->pos.x, b->pos.y, */
/*                                                    distance.x, distance.y ); */
/* #endif */
/*   if(isfree(rel2abs(b->pos, distance))) */
/*   { */
/*     set_point(rel2abs(b->pos, distance), b->sym); */
/*     set_point(b->pos, EMPTY_CHAR); */
/*     b->pos = rel2abs(b->pos, distance); */
/*   } */
/*   else if(isenemy(rel2abs(b->pos, distance))) */
/*   { */
/*     attack(*b, search_being(rel2abs(b->pos, distance))); */
/*   } */
/* } */



/* void move_left(being* b)  {move(b, (point){-b->speed, 0});} */
/* void move_right(being* b) {move(b, (point){b->speed,  0});} */
/* void move_up(being* b)    {move(b, (point){0, -b->speed});} */
/* void move_down(being* b)  {move(b, (point){0,  b->speed});} */




/* int hero_actions(being *hero) */
/* { */
/*   char ch; */
/*   if(read(STDIN_FILENO, &ch, 1)==1 && ch!='q') */
/*   { */
/*     switch(ch) */
/*     { */
/*     case '[': */
/*       { */
/*         if(read(STDIN_FILENO, &ch, 1)==1) */
/*         { */
/*           switch(ch) */
/*           { */
/*           case 'A': */
/*             move_up(hero); */
/*             return 1; */
/*           case 'D': */
/*             move_left(hero); */
/*             return 1; */
/*           case 'C': */
/*             move_right(hero); */
/*             return 1; */
/*           case 'B': */
/*             move_down(hero); */
/*             return 1; */
/*           } */
/*         } */
/*         break; */
/*       } */
/*     } */
/*   } */
/*   else */
/*   { */
/*     perror("read"); */
/*   } */
/*   return 1; */
/* } */

/* int is_nearby(point where, char ch) */
/* { */
/*   point pos = where; */
/*   pos.y+=1; */
/*   if(get_point(pos)==ch) return 1; //TODO */
/*   else return 0; */
/* } */


/* void hostile_actions(being e) */
/* { */
/*   if(is_nearby(e.pos, main_hero->sym)) */
/*   { */
/*     attack(e, main_hero); */
/*   } */
/* } */

struct termios orig_termios;
void disable_raw_mode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(void)
{
  struct termios raw;
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);
  raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* void tick(const being hero) */
/* { */
/*   clear(); */
/*   print_status(hero); */
/*   print_map(); */
/* } */

/* void beings_actions(void) */
/* { */
/*   for(size_t i=0; i<beings_count; ++i) */
/*   { */
/*     if(beings[i].opinion==hostile) */
/*     { */
/*       hostile_actions(beings[i]); *\/ */
/* /\*     *\/ } */
/*   } */
/* } */

int main()
{
  enable_raw_mode();
  /* beings_init(); */
  load_level("/home/lain/programming/rogue/levels/test.lvl");
  /* set_point(main_hero->pos, main_hero->sym); */
  perror("error!");
  print_map();
  perror("print_map!");
  while(0)
  {
    /* beings_actions(); */
    print_map();

  }
  return 0;
}
