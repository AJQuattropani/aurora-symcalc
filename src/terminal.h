#pragma once

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <readline/history.h>

#define TERMINAL
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

