#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "editor_fsm.hpp"

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();
  
  char c;
  EditorMode current_mode = EditorMode::NORMAL;
  bool is_running = true;

  // Main loop
  while(is_running && read(STDIN_FILENO, &c, 1) == 1) {
    // get new mode based on input
    auto [new_mode, action] = process_keypress(current_mode, c);
    current_mode = new_mode;

    std::visit([&is_running](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      
      if constexpr (std::is_same_v<T, PrintMessage>) {
        std::cout << "\r\n" << arg.message; 
        std::cout.flush();
      } 
      else if constexpr (std::is_same_v<T, QuitEditor>) {
        is_running = false;
      }
    }, action);
  }
  
  return 0;
}
