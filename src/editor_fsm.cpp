#include "editor_fsm.hpp"
#include <unistd.h>

std::pair<EditorMode, EditorAction> process_keypress(EditorMode current_mode, char key) {
  // Handle escape sequences
  if (key == '\x1b') {
    char seq[3];
    // Set a short timeout or check if more chars are available
    // For simplicity in this raw read environment, we check if [ follows
    if (read(STDIN_FILENO, &seq[0], 1) == 0) return {EditorMode::NORMAL, PrintMessage{"Entering NORMAL mode"}};
    if (read(STDIN_FILENO, &seq[1], 1) == 0) return {current_mode, DoNothing{}};

    if (seq[0] == '[') {
      switch (seq[1]) {
        case 'A': return {current_mode, MoveCursorUp{}};
        case 'B': return {current_mode, MoveCursorDown{}};
        case 'C': return {current_mode, MoveCursorRight{}};
        case 'D': return {current_mode, MoveCursorLeft{}};
      }
    }
    return {EditorMode::NORMAL, PrintMessage{"Entering NORMAL mode"}};
  }

  switch(current_mode) {
    case EditorMode::NORMAL:
      switch(key) {
        case 'h': return {current_mode, MoveCursorLeft{}};
        case 'j': return {current_mode, MoveCursorDown{}};
        case 'k': return {current_mode, MoveCursorUp{}};
        case 'l': return {current_mode, MoveCursorRight{}};
        case 'a': return {EditorMode::INSERT, PrintMessage{"Entering INSERT mode (insert on right)"}};
        case 'i': return {EditorMode::INSERT, PrintMessage{"Entering INSERT mode (insert on left)"}};
        case ':': return {EditorMode::COMMAND, PrintMessage{"Entering COMMAND mode"}};
        case 'v': return {EditorMode::VISUAL, PrintMessage{"Entering VISUAL mode"}};
        default:  return {current_mode, DoNothing{}};
      }

    case EditorMode::INSERT:
      // Handle backspace
      if (key == '\b' || key == 127) { // Backspace key
        return {current_mode, DeleteCharacter{}};
      }
      else if (key == '\r') { // Enter key in raw mode
        return {current_mode, InsertChar{'\n'}};
      }
      else {
        return {current_mode, InsertChar{key}}; 
      }

    case EditorMode::VISUAL:
      if (key == '\x1b') { // Escape key
        return {EditorMode::NORMAL, PrintMessage{"Entering NORMAL mode"}};
      }
      return {current_mode, DoNothing{}};

    case EditorMode::COMMAND:
      if (key == '\x1b') { // Escape key
        return {EditorMode::NORMAL, PrintMessage{"Entering NORMAL mode"}};
      }
      if (key == 'q') {
        return {current_mode, QuitEditor{}};
      }
      return {current_mode, DoNothing{}};
    }
    
    return {current_mode, DoNothing{}};
}
