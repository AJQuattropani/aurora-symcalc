#pragma once

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

#define TERMINAL
#define enter_alt_screen puts("\033[?1049h\033[H");
#define exit_alt_screen puts("\033[?1049l");

static struct termios original_settings;

static void start(); 

static void end();

void reset_terminal();

void on_signal(int signum);

void move_cursor(int x);

void set_terminal();

