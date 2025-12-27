#ifndef COMMON_H
#define COMMON_H
/**
 * @file common.h
 * @brief Global definitions, macros, and library includes for B-textEditor.
 */

/* FEATURE TEST MACROS: These unlock modern terminal features and system 
   functions in the standard C headers. */
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

/* Standard C Libraries */
#include<stdlib.h>
#include <unistd.h>
#include<termios.h>
#include<ctype.h>
#include<stdio.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/ioctl.h>
#include<string.h>


/**
 * @brief Bitwise macro to mimic the CTRL key behavior.
 * @details The CTRL key in a terminal works by stripping the 3 most 
 * significant bits (MSBs) from a character. For example, 'q' (01110001) 
 * becomes Ctrl-Q (00010001). This macro performs a bitwise AND with 0x1F.
 */
#define CTRL_KEY(k) ((k) & 0x1f)
#define TAB_STOP 8  



#define B_TEXTEDITOR_VERSION "0.0.1"

/**
 * @brief Enumeration for mapping special keys to integer constants.
 * @details To prevent collision with standard ASCII characters (0-255), we 
 * start this sequence at 1000. Each subsequent entry automatically 
 * increments (e.g., ARROW_RIGHT = 1001).
 */
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