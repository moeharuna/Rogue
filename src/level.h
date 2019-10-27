#ifndef ROGUE_LEVEL
#define ROGUE_LEVEL
extern char map[];
#define VERT_SIZE   20
#define HORIZ_SIZE  5
typedef struct
{
  unsigned int x, y;
} point;
int get_char (const point p)               ;
void set_char(const point p, const char ch);
#endif //ROGUE_LEVEL
