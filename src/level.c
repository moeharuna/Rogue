#include "level.h"
char map[VERT_SIZE*HORIZ_SIZE]  =    {'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      ' ', 'x', 'x', ' ', 'x',
                                      ' ', ' ', ' ', ' ', 'x',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', 'e', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', 'e', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', ' ', ' ', 'x', ' ',
                                      'x', 'x', 'x', 'x', 'x'};
int get_char (const point p){
  return map[p.y*HORIZ_SIZE+p.x];
}
void set_char(const point p, const char ch) {
  map[p.y*HORIZ_SIZE+p.x]=ch;
}
