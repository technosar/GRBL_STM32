/*---------------------------------------------------------------------------*/
/*                                                                           */
/*  Gestion terminal compatible VT100                                        */
/*  Rev : 1.00                                                               */
/*                                                                           */
/*  Copyright (C) 2003 FOUGA Laurent                                         */
/*  e-mail : techlf@techlf.com                                               */
/*  web : http://www.techlf.com                                              */
/*                                                                           */
/*  This program is free software; you can redistribute it and/or modify it  */
/*  under the terms of the GNU General Public License as published by the    */
/*  Free Software Foundation.                                                */
/*                                                                           */
/*  This program is distributed in the hope it will be useful, but WITHOUT   */
/*  ANY WARRANTY; without even the implied warranty of MERCHANDIBILITY or    */
/*  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for */
/*  more details.                                                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*--- include files  --------------------------------------------------------*/

#include <stdlib.h>

#include "usbd_cdc_if.h"
#include "terminal.h"

#define CSI "\x1B["
#define ESC "\x1B"

char textbuf[128];

/*---------------------------------------------------------------------------*/
/* set width of VT screnn terminal                                           */
/*---------------------------------------------------------------------------*/

void term_setcol(int width)
{
 switch(width) 
   {
    case 132 : CDC_send_text(CSI"?3h");
               break;
    case 80 : CDC_send_text(CSI"?3l");
              break;  
    default:  CDC_send_text(CSI"?3l");
              break;
   }
}

/*---------------------------------------------------------------------------*/
/* set cursor position X,Y on VT terminal                                    */
/*---------------------------------------------------------------------------*/

void term_gotoxy(unsigned char x, unsigned char y )
{              
 sprintf((unsigned char *)textbuf,CSI"%d;%dH", y, x);
 CDC_send_str((char*)textbuf, strlen(textbuf));
}  

/*---------------------------------------------------------------------------*/
/* set text back ground color                                                */
/*---------------------------------------------------------------------------*/

void term_setbkcolor(int color)
{              
 sprintf((unsigned char *)textbuf,CSI"%dm", color+40);
 CDC_send_str((char*)textbuf, strlen(textbuf));
}  

/*---------------------------------------------------------------------------*/
/* set text color                                                            */
/*---------------------------------------------------------------------------*/

/*
Foreground Colours
30	Black
31	Red
32	Green
33	Yellow
34	Blue
35	Magenta
36	Cyan
37	White

	Background Colours
40	Black
41	Red
42	Green
43	Yellow
44	Blue
45	Magenta
46	Cyan
47	White
*/

void term_settextcolor(int color)
{              
 sprintf((unsigned char *)textbuf,CSI"%dm", color+30 );
 CDC_send_str((char*)textbuf, strlen(textbuf));
}  

/*---------------------------------------------------------------------------*/
/* set scroll window                                                         */
/*---------------------------------------------------------------------------*/

void term_window(int top, int bottom)
{
 sprintf((unsigned char *)textbuf,CSI"%d;%dr", top, bottom);
 CDC_send_str((char*)textbuf, strlen(textbuf));
}

/*---------------------------------------------------------------------------*/
/* set VT size                                                               */
/*---------------------------------------------------------------------------*/

void term_size(int nb)
{
  sprintf((unsigned char *)textbuf,CSI"%dt", nb);
  CDC_send_str((char*)textbuf, strlen(textbuf));
}

/*---------------------------------------------------------------------------*/
/* term command OK                                                           */
/*---------------------------------------------------------------------------*/

void term_ok(int liney)
{
 CDC_send_text(ESC"7");
 term_gotoxy(76,liney);
 term_setbkcolor(6);
 CDC_send_text("GO  ");
 CDC_send_text(ESC"8");
 term_setbkcolor(0);
}

/*---------------------------------------------------------------------------*/
/* term command ERROR                                                        */
/*---------------------------------------------------------------------------*/

void term_error(int liney)
{ 
 CDC_send_text(ESC"7");
 term_gotoxy(76,liney);
 term_setbkcolor(5);
 CDC_send_text("NOGO");
 CDC_send_text(ESC"8");
 term_setbkcolor(0);
}

/*---------------------------------------------------------------------------*/
/* init screen VT terminal                                                   */
/*---------------------------------------------------------------------------*/

void term_init(int line_count)
{      
 term_setcol(80);        
 term_gotoxy(0,0);
 CDC_send_text(CSI"0m");
 CDC_send_text(CSI"J");
 term_size(line_count);
 term_window(3, line_count-1);
 term_setbkcolor(0);
 term_settextcolor(7);
 CDC_send_text("Technosar - MC - V1.0 - (C) 2017");
 term_setbkcolor(4);
 CDC_send_text("\r\ncommand                                                                         ");
 term_setbkcolor(0);
 term_settextcolor(7);
}
