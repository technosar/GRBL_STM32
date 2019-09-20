#ifndef _terminal_h
#define _terminal_h

/*---------------------------------------------------------------------------*/
/*  constantes & variables                                                   */
/*___________________________________________________________________________*/



void term_setcol(int width);
void term_gotoxy(unsigned char x, unsigned char y );
void term_setbkcolor(int color);
void term_settextcolor(int color);
void term_init(int line_count);
void term_window(int top, int bottom);
void term_error(int liney);
void term_ok(int liney);

#endif
