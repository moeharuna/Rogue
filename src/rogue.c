#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <time.h>
#include "level.h"
#include "beings.h"
#include "input.h"

being * hero = NULL;
struct termios orig_termios;
void disable_terminal_mode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_terminal_mode(void)
{
  struct termios raw;
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_terminal_mode);
  raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


void move(being* b, point distance)
{
#ifdef DEBUG
  printf("Move from (%d, %d) on (%d %d) pixels\n", b->pos.x, b->pos.y,
                                                   distance.x, distance.y );
#endif
  point pos = rel2abs(b->pos, distance);
  if(isfree(pos))
  {
    set_point(pos, b->sym);
    set_point(b->pos, EMPTY_CHAR);
    b->pos = rel2abs(b->pos, distance);
  }
}


void move_left(being* b)  {move(b, (point){-b->speed, 0});}
void move_right(being* b) {move(b, (point){b->speed,  0});}
void move_up(being* b)    {move(b, (point){0, -b->speed});}
void move_down(being* b)  {move(b, (point){0,  b->speed});}


void hostile_move(being * subject)
{
  random()%2 ? move(subject, (point) {random()%2, random()%2})
    :move(subject, (point) {-random()%2, -random()%2});
}


void attack(const being atk, being* def) // be careful! there is no check for dying
{
#ifdef DEBUG
  fprintf(stderr, "def->hp= %d\n", def->hp);
#endif
  int damage2deal=0;
  damage2deal = atk.dmg;
  if(atk.opinion==HERO)
    printf("You hit the %s. %s lost %d hp.\n", "enemy", "enemy", damage2deal);
  else if(def->opinion==HERO)
    printf("You have been hit. You lost %d hp.\n", damage2deal);
  def->hp-=damage2deal;
}

void clear();
void game_over()
{
  clear();
  printf("You are dead");
  exit(0);
}


void being_die(being *body)
{
#ifdef DEBUG
  fprintf(stderr, "Warning! beings_die is called. This broke all pointers to beings[]\n");
#endif
  if(body->opinion==HERO)
  {
    game_over();
  }
  set_point(body->pos, EMPTY_CHAR);
  size_t i=0;
  while(!point_equals(beings[i].pos, body->pos))i++;
#ifdef DEBUG
  fprintf(stderr, "%ld is killing...", i);
#endif
  for(size_t j=i; j<beings_s-1; ++j){
    beings[j]=beings[j+1];
  }
  beings_s--;
}


int start_engage(being * attacker, being * defender) //battle start
                                                     //0 - attacker win, 1 - defender win, 2 - nothing happened -1 if bug
{
  int engage_action(being * subj, being * obj)
  {
    if(subj->opinion==HERO)
    {

      switch(key_press())
      {
      case ARROW_UP:
        attack(*subj, obj);
        break;
      case ARROW_DOWN:
      case ARROW_LEFT:
        printf("not implemented! sorry!\n");
        return engage_action(subj, obj);
        break;
      case ARROW_RIGHT:
      {
        printf("You trying to flee...\n");
        point pos;
        pos.x = 2 * (random()%4-1);
        pos.y = 2 * (random()%4-1);
        printf("sub.x=%d, sub.y=%d, x=%d, y=%d\n",subj->pos.x, subj->pos.y, pos.x, pos.y);
        if(isfree(pos))
          move(subj, pos);
        else
        {
          printf("You fail the flee\n");
          return 0;
        }
        return 1;
      }
      default:
        return engage_action(subj, obj);
        break;
      }
    }
    else
      attack(*subj, obj);
    return 0;
  }
  if(!is_enemy(*attacker, *defender))
    return -1;
  puts("engage started!");
  if(attacker->opinion==HERO || defender->opinion==HERO)
    printf("Select a action: att=↑ def=← mag=↓ esc=→\n");
  int ischiken=0;
  while(attacker->hp >0) //defender hp checked inside
  {
    ischiken += engage_action(attacker, defender);
    if(defender->hp<=0)
    {
      being_die(defender);
      return 0;
    }
   ischiken += engage_action(defender, attacker);
   if(ischiken) return 2;
  }
  being_die(attacker);
  return 1;
}


being * enemy_nearby(const being subject)
{
  const int dir_size = 4;
  point directons[dir_size];
  directons[0] = plus_point(subject.pos, 0, 1); //up
  directons[1] = plus_point(subject.pos, 0, -1);//down
  directons[2] = plus_point(subject.pos, 1, 0); //right
  directons[3] = plus_point(subject.pos, -1, 0);//left

  for(int i=0; i<dir_size; ++i)
  {
    being * b = search_being(directons[i]);
    if(b!=NULL && is_enemy(subject, *b))
      return b;
  }
  return NULL;
}


int hero_action(being *hero)
{
  switch(key_press())
  {
  case ARROW_UP:
    move_up(hero);
    break;
  case ARROW_DOWN:
    move_down(hero);
    break;
  case ARROW_RIGHT:
    move_right(hero);
    break;
  case ARROW_LEFT:
    move_left(hero);
    break;
  }
  being * enemy = enemy_nearby(*hero);
  if(enemy!=NULL) start_engage(hero, enemy);

  return 1;
}

int hostile_action(being *hostile)
{
  hostile_move(hostile);
  being * enemy = NULL;
  if( (enemy = enemy_nearby(*hostile)))
    start_engage(hostile, enemy);
  return 1;
}


void clear()
{
  char seq[] = {27, '[', '2', 'J'};
  puts(seq);
}
void print_status(const being h)
{
  printf("hp=%d, dmg=%d, speed=%d\r\n", h.hp, h.dmg, h.speed);
}
void print_map()
{
  point std;
  for(std.y=0; std.y<this_lvl->vert_size; ++std.y)
  {
    for(std.x=0; std.x<this_lvl->horiz_size; ++std.x)
      putchar(get_point(std));
    //printf("\r\n"); //\n don't makes newline in raw_mode
  }
}

void tick(const being hero)
{
  //clear();
  print_map();
  print_status(hero);
}


int init()
{
  srandom((unsigned long)time);
  enable_terminal_mode();
  load_level("levels/test.txt");
  beings_init("types/test.txt");

  for(size_t i=0; i<beings_s; ++i)
  {
    if(beings[i].opinion==HERO)
      hero = &beings[i];
  }
  if(hero==NULL)
  {
    fprintf(stderr, "ERORR! player character not found.");
    exit(1);
  }
  tick(*hero);
  return 0;
}


int main()
{
  init();
  for(;;) //main loop
  {
    while(hero_action(hero)!=1); //do player turn until its take no time
    for(size_t i=0; i<beings_s; ++i) //make turn for non player
      switch(beings[i].opinion)
      {
      case HOSTILE:
        hostile_action(&beings[i]);
        break;
      default:
        if(beings[i].opinion!=HERO)
          puts("not yet implemented! Sorry!");
        break;
      }
    tick(*hero);
  }
  return 0;
}
