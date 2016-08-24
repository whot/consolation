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

/* global state */

unsigned int screen_width;
unsigned int screen_height;

/* selection.c */

void set_screen_size(void);
void draw_pointer(int x, int y);
void select_region(int x, int y, int x2, int y2);
void select_word(int x, int y);
void select_line(int x, int y);
void paste(void);
void scroll(int sc);

/* action.c */

void set_pointer(double x, double y);
void move_pointer(double x, double y);
void press_left_button(void);
void release_left_button(void);
void press_middle_button(void);
void press_right_button(void);
void vertical_axis(double v);

/* input.c */

int event_main(int argc, char **argv);
