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
  Cursor cur = buffer.get_cursor_2d();
  std::string data = buffer.get_data();

  // Clear screen and move cursor to top left
  // \x1b[2J = clear screen, \x1b[H = move to 1,1
  std::cout << "\x1b[2J\x1b[H"; 
  
  // Draw Header
  std::cout << "\x1b[7m--- KODA (Arrows/hjkl to move, i for Insert, :q to Quit) ---\x1b[m\r\n";
  
  // Draw Buffer Content
  std::string current_line;
  for (char c : data) {
    if (c == '\n') {
      std::cout << current_line << "\r\n";
      current_line.clear();
    } else {
      current_line += c;
    }
  }
  std::cout << current_line << "\r\n";

  // Draw Status Line
  std::cout << "\r\n\x1b[32mCursor: Row " << cur.row << ", Col " << cur.col << "\x1b[0m\r\n";
  if (!message.empty()) {
    std::cout << "\x1b[33m" << message << "\x1b[0m\r\n";
  }

  // Position the physical blinking cursor
  int terminal_row = cur.row + 2;
  int terminal_col = cur.col + 1;
  std::cout << "\x1b[" << terminal_row << ";" << terminal_col << "H";

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
      else if constexpr (std::is_same_v<T, MoveCursorLeft>) {
        buffer.move_left();
      }
      else if constexpr (std::is_same_v<T, MoveCursorRight>) {
        buffer.move_right();
      }
      else if constexpr (std::is_same_v<T, MoveCursorUp>) {
        buffer.move_up();
      }
      else if constexpr (std::is_same_v<T, MoveCursorDown>) {
        buffer.move_down();
      }
    }, action);

    if (is_running) {
      refreshScreen(buffer, last_message);
    }
  }
  
  return 0;
}
