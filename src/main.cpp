#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "editor_fsm.hpp"
#include "gap_buffer.hpp"

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_iflag &= ~(IXON | ICRNL);
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void refreshScreen(GapBuffer& buffer, const std::string& message) {
  // Test render, clear screen and show buffer
  std::cout << "\x1b[2J\x1b[H"; // Clear screen and move cursor to top left
  std::cout << "--- KODA (ESC for Normal, i for Insert, :q to Quit) ---\r\n";
  std::cout << "Buffer: " << buffer.get_data() << "\r\n";
  if (!message.empty()) {
    std::cout << "\x1b[33m" << message << "\x1b[0m\r\n";
  }
  std::cout.flush();
}

int main() {
  enableRawMode();
  
  GapBuffer buffer;
  char c;
  EditorMode current_mode = EditorMode::NORMAL;
  bool is_running = true;
  std::string last_message = "Koda";

  refreshScreen(buffer, last_message);

  // Main loop
  while(is_running && read(STDIN_FILENO, &c, 1) == 1) {
    auto [new_mode, action] = process_keypress(current_mode, c);
    current_mode = new_mode;
    last_message = "";

    std::visit([&](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      
      if constexpr (std::is_same_v<T, PrintMessage>) {
        last_message = arg.message;
      } 
      else if constexpr (std::is_same_v<T, QuitEditor>) {
        is_running = false;
      }
      else if constexpr (std::is_same_v<T, InsertChar>) {
        buffer.insert_char_at_cursor(arg.c);
      }
      else if constexpr (std::is_same_v<T, DeleteCharacter>) {
        buffer.delete_at_cursor();
      }
      // TODO: Add more handlers (MoveCursorLeft, etc.)
    }, action);

    if (is_running) {
      refreshScreen(buffer, last_message);
    }
  }
  
  return 0;
}
