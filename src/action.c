/* Copyright © 2016 Bill Allombert

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  Check the License for details. You should have received a copy of it, along
  with the package; see the file 'COPYING'. If not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "consolation.h"

static double xx=1, yy=1, x0=-1, y0=-1, x1=-1, y1=-1;
static int mode = 0;

void
set_pointer(double x, double y)
{
  xx = x+1; yy = y+1;
  if (xx < 1) xx = 1; else if (xx > screen_width)  xx = screen_width;
  if (yy < 1) yy = 1; else if (yy > screen_height) yy = screen_height;
  if (x0 >= 0 && y0 >= 0)
    select_region((int)xx,(int)yy,(int)x0,(int)y0);
  else
    draw_pointer((int)xx,(int)yy);
}

void
move_pointer(double x, double y)
{
  xx += x/20; yy += y/20;
  if (xx < 1) xx = 1; else if (xx > screen_width)  xx = screen_width;
  if (yy < 1) yy = 1; else if (yy > screen_height) yy = screen_height;
  if (x0 >= 0 && y0 >= 0)
    select_region((int)xx,(int)yy,(int)x0,(int)y0);
  else
    draw_pointer((int)xx,(int)yy);
}

void
press_left_button(void)
{
  if ((int)x1==(int)xx && (int)y1==(int)yy)
  {
    mode = (mode+1)%3;
    switch(mode)
    {
    case 0:
      select_region((int)xx,(int)yy,(int)xx,(int)yy);
      break;
    case 1:
      select_word((int)xx,(int)yy);
      break;
    case 2:
      select_line((int)xx,(int)yy);
      break;
    }
  }
  else
  {
    x0=xx; y0=yy; x1=x0; y1=y0; mode = 0;
    select_region((int)xx,(int)yy,(int)x0,(int)y0);
  }
}

void
release_left_button(void)
{
  x0=-1; y0=-1;
}

void
press_middle_button(void)
{
  paste();
}

void
press_right_button(void)
{
  if (x1>=0 && y1>=0)
    select_region((int)xx,(int)yy,(int)x1,(int)y1);
}

void
vertical_axis(double v)
{
  if (v)
    scroll(v > 0 ? 2 : -2);
}
