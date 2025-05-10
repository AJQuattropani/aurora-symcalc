#pragma once

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <readline/history.h>

#pragma once

//#define TERMINAL

#if NDEBUG
#define __UNREACHABLE_BRANCH __builtin_unreachable();
#else
#undef TERMINAL
#define __UNREACHABLE_BRANCH fprintf(stderr, "[FATAL] Reached dangerous exception in %s at %d.\n", __func__, __LINE__); exit(1);
#endif

#ifdef TERMINAL
#define enter_alt_screen printf("\033[?1049h\033[H");
#define exit_alt_screen printf("\033[?1049l");
#else
#define enter_alt_screen
#define exit_alt_screen
#endif

static struct termios original_settings;

static void start(); 

static void end();

void reset_terminal();

void on_signal(int signum);

void move_cursor(int x);

void set_terminal();

