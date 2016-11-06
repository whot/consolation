/* Adpated from test/event-debug.c from the libinput distribution */
/* test/event-debug.c 1.3.3:
 * Copyright © 2014 Red Hat, Inc.
 * Modifications: Copyright © 2016 Bill Allombert <ballombe@debian.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#define _GNU_SOURCE
#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>

#include <libinput.h>
#include "shared.h"
#include "consolation.h"

struct tools_context context;
static unsigned int stop = 0;

static void
handle_motion_event(struct libinput_event *ev)
{
  struct libinput_event_pointer *p = libinput_event_get_pointer_event(ev);
  double x = libinput_event_pointer_get_dx(p);
  double y = libinput_event_pointer_get_dy(p);
  move_pointer(x, y);
}

static void
handle_absmotion_event(struct libinput_event *ev)
{
  struct libinput_event_pointer *p = libinput_event_get_pointer_event(ev);
  double x = libinput_event_pointer_get_absolute_x_transformed(p, screen_width);
  double y = libinput_event_pointer_get_absolute_y_transformed(p, screen_height);
  set_pointer(x, y);
}

static void
handle_pointer_button_event(struct libinput_event *ev)
{
  struct libinput_event_pointer *p = libinput_event_get_pointer_event(ev);
  enum libinput_button_state state;
  int button;
  button = libinput_event_pointer_get_button(p);
  state = libinput_event_pointer_get_button_state(p);
  switch(button)
  {
  case BTN_LEFT:
    if (state==LIBINPUT_BUTTON_STATE_PRESSED)
      press_left_button();
    else
      release_left_button();
    break;
  case BTN_MIDDLE:
    if (state==LIBINPUT_BUTTON_STATE_PRESSED)
      press_middle_button();
    break;
  case BTN_RIGHT:
    if (state==LIBINPUT_BUTTON_STATE_PRESSED)
      press_right_button();
    break;
  }
}

static void
handle_pointer_axis_event(struct libinput_event *ev)
{
  struct libinput_event_pointer *p = libinput_event_get_pointer_event(ev);
  double v = 0;
  if (libinput_event_pointer_has_axis(p,
        LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL))
  {
    v = libinput_event_pointer_get_axis_value(p,
        LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL);
    vertical_axis(v);
  }
}

static void
handle_touch_event_down(struct libinput_event *ev)
{
  struct libinput_event_touch *t = libinput_event_get_touch_event(ev);
  double x = libinput_event_touch_get_x_transformed(t, screen_width);
  double y = libinput_event_touch_get_y_transformed(t, screen_height);
  set_pointer(x, y);
  press_left_button();
}

static void
handle_touch_event_motion(struct libinput_event *ev)
{
  struct libinput_event_touch *t = libinput_event_get_touch_event(ev);
  double x = libinput_event_touch_get_x_transformed(t, screen_width);
  double y = libinput_event_touch_get_y_transformed(t, screen_height);
  set_pointer(x, y);
}

static void
handle_touch_event_up(struct libinput_event *ev)
{
  release_left_button();
}

static int
handle_events(struct libinput *li)
{
  int rc = -1;
  struct libinput_event *ev;

  libinput_dispatch(li);
  set_screen_size();
  while ((ev = libinput_get_event(li))) {

    switch (libinput_event_get_type(ev)) {
    case LIBINPUT_EVENT_NONE:
      abort();
    case LIBINPUT_EVENT_DEVICE_ADDED:
    case LIBINPUT_EVENT_DEVICE_REMOVED:
      tools_device_apply_config(libinput_event_get_device(ev),
          &context.options);
      break;
    case LIBINPUT_EVENT_POINTER_MOTION:
      handle_motion_event(ev);
      break;
    case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
      handle_absmotion_event(ev);
      break;
    case LIBINPUT_EVENT_POINTER_BUTTON:
      handle_pointer_button_event(ev);
      break;
    case LIBINPUT_EVENT_POINTER_AXIS:
      handle_pointer_axis_event(ev);
      break;
    case LIBINPUT_EVENT_TOUCH_DOWN:
      handle_touch_event_down(ev);
      break;
    case LIBINPUT_EVENT_TOUCH_MOTION:
      handle_touch_event_motion(ev);
      break;
    case LIBINPUT_EVENT_TOUCH_UP:
      handle_touch_event_up(ev);
      break;
    default:
      break;
    }
    libinput_event_destroy(ev);
    libinput_dispatch(li);
    rc = 0;
  }
  return rc;
}

static void
sighandler(int signal, siginfo_t *siginfo, void *userdata)
{
  stop = 1;
}

static void
mainloop(struct libinput *li)
{
  struct pollfd fds;
  struct sigaction act;

  fds.fd = libinput_get_fd(li);
  fds.events = POLLIN;
  fds.revents = 0;

  memset(&act, 0, sizeof(act));
  act.sa_sigaction = sighandler;
  act.sa_flags = SA_SIGINFO;

  if (sigaction(SIGINT, &act, NULL) == -1) {
    fprintf(stderr, "Failed to set up signal handling (%s)\n",
        strerror(errno));
    return;
  }

  /* Handle already-pending device added events */
  if (handle_events(li))
    fprintf(stderr, "Expected device added events on startup but got none. "
        "Maybe you don't have the right permissions?\n");

  while (!stop && poll(&fds, 1, -1) > -1)
    handle_events(li);
}

int
event_init(int argc, char **argv)
{
  tools_init_context(&context);
  return tools_parse_args(argc, argv, &context);
}

int
event_main(void)
{
  struct libinput *li;

  li = tools_open_backend(&context);
  if (!li)
    return 1;

  mainloop(li);

  libinput_unref(li);

  return 0;
}
