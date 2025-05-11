#include "terminal.h"

void reset_terminal() { 
  tcsetattr(STDIN_FILENO, TCSANOW, &original_settings); 
  exit_alt_screen;
}

void set_terminal() {
  tcgetattr(STDIN_FILENO, &original_settings);

  atexit(end);
  signal(SIGINT, on_signal);
  signal(SIGTERM, on_signal);
  signal(SIGABRT, on_signal);
  signal(SIGCHLD, on_signal);
  signal(SIGKILL, on_signal);

  struct termios new_termios = original_settings;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
  
  enter_alt_screen;
}

void on_signal(int signum) {
  end();
  exit(signum);
}

__attribute__((constructor)) void start() { 
  set_terminal();
  using_history();
}

__attribute__((destructor)) void end() { 
  reset_terminal(); 
}

void move_cursor(int x) { fprintf(stdout, "\033[0;%dH", x); }
