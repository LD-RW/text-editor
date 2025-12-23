#ifndef COMMON_H
#define COMMON_H


#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE
#include<stdlib.h>
#include <unistd.h>
#include<termios.h>
#include<ctype.h>
#include<stdio.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/ioctl.h>
#include<string.h>
/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f)
#define B_TEXTEDITOR_VERSION "0.0.1"

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};


#endif