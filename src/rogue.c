#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include "level.h"

#define MAIN_CHAR  'o'
#define EMPTY_CHAR ' '


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


being * enemies;
size_t   enemies_size =0;


point rel2abs(const point center, const point p) //relative to absolute
{
  point where = {center.x+p.x, center.y+p.y};
  return where;
}

void print_status(const being h)
{
  printf("hp=%d, dmg=%d, speed=%d\r\n", h.hp, h.dmg, h.speed);
}
void print_map()
{
  point std;
  for(std.y=0; std.y<VERT_SIZE; ++std.y)
  {
    for(std.x=0; std.x<HORIZ_SIZE; ++std.x)
    {
      putchar(get_char(std));
    }
    printf("\r\n");
  }
}

being* search_being(point pos)
{
  for(size_t i=0; i<enemies_size; ++i)
  {
    if(enemies[i].pos.x==pos.x && enemies[i].pos.y ==pos.y)
      return &enemies[i];
  }
  return NULL;
}

int isenemy(point pos)
{
  if(search_being(pos)!=NULL)
    return 1;
  return 0;
}


int isfree(point p)
{
  if(p.x>=HORIZ_SIZE ||
     p.y>=VERT_SIZE) return 0;
  if(get_char(p)==EMPTY_CHAR) return 1;
  return 0;
}


void game_over()
{
  clear();
  printf("You are dead");
  exit(0);
}


void being_die(being *body)
{
  set_char(body->pos, EMPTY_CHAR);
  if(body->sym==MAIN_CHAR)
  {
    game_over();
  }
}

void attack(const being atk, being* def)
{
  #ifdef DEBUG
  printf("def->hp= %d\n", def->hp);
  #endif
  def->hp-=atk.dmg;
  if(def->hp<=0)
  {
    being_die(def);
  }
}


void move(being* b, point distance)
{
#ifdef DEBUG
  printf("Move from (%d, %d) on (%d %d) pixels\n", b->pos.x, b->pos.y,
                                                   distance.x, distance.y );
#endif
  if(isfree(rel2abs(b->pos, distance)))
  {
    set_char(rel2abs(b->pos, distance), b->sym);
    set_char(b->pos, EMPTY_CHAR);
    b->pos = rel2abs(b->pos, distance);
  }
  else if(isenemy(rel2abs(b->pos, distance)))
  {
    attack(*b, search_being(rel2abs(b->pos, distance)));
  }
}

void move_left(being* b)  {move(b, (point){-b->speed, 0});}
void move_right(being* b) {move(b, (point){b->speed,  0});}
void move_up(being* b)    {move(b, (point){0, -b->speed});}
void move_down(being* b)  {move(b, (point){0,  b->speed});}

void enemies_init()
{
  point iter;
  for(iter.x=0; iter.x<HORIZ_SIZE; ++iter.x)
    for(iter.y = 0; iter.y<VERT_SIZE; ++iter.y)
      if(get_char(iter)=='e')
        enemies_size++;
  enemies = malloc(enemies_size*sizeof(being));
  int i=0;
  for(iter.x=0; iter.x<HORIZ_SIZE; ++iter.x)
    for(iter.y = 0; iter.y<VERT_SIZE; ++iter.y)
      if(get_char(iter)=='e')
        enemies[i++] =(being) {.pos=iter, .sym='e', .speed=1, .dmg=5, .hp=10};
}


void clear()
{
  char seq[] = {27, '[', '2', 'J'};
  puts(seq);
}

int hero_actions(being *hero)
{
  char ch;
  if(read(STDIN_FILENO, &ch, 1)==1 && ch!='q')
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
            move_up(hero);
            return 1;
          case 'D':
            move_left(hero);
            return 1;
          case 'C':
            move_right(hero);
            return 1;
          case 'B':
            move_down(hero);
            return 1;
          }
        }
        break;
      }
    }
  }
  else
  {
    perror("read");
  }
  return 1;
}

int is_nearby(point where, char ch)
{
  point pos = where;
  pos.y+=1;
  if(get_char(pos)==ch) return 1; //TODO
  else return 0;
}


void evil_actions(being e, being *hero)
{
  if(is_nearby(e.pos, hero->sym))
  {
    attack(e, hero);
  }
}

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

void tick(const being hero)
{
  clear();
  print_status(hero);
  print_map();
}

int main()
{
  enable_raw_mode();
  enemies_init();

  being hero = {.pos={2, 10},.sym=MAIN_CHAR, .speed=1, .dmg=5, .hp=10};
  set_char(hero.pos, hero.sym);
  print_map();
  while(1)
  {
    //while(action not consume time) take_input;
    //ai_do_somethnigs
    //print_state
    while(hero_actions(&hero)!=1);
    //evil_actions(enemies[0], &hero);
    tick(hero);
  }
  return 0;
}
