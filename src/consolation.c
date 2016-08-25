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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "consolation.h"

static int csl_daemon(void)
{
  pid_t pid = fork();
  switch(pid) {
      case -1: return 1; /* father, fork failed */
      case 0:
        (void)setsid(); /* son becomes process group leader */
        if (fork()) exit(0); /* now son exits, also when fork fails */
        break; /* grandson: its father is the son, which exited,
                * hence father becomes 'init', that'll take care of it */
      default: /* father, fork succeeded */
        (void)waitpid(pid,NULL,0); /* wait for son to exit, immediate */
        return 1;
  }
  /* grandson */
  return 0;
}

int main(int argc, char **argv)
{
  if (event_init(argc, argv))
    return 1;
  if (!csl_daemon()) event_main();
  return 0;
}
