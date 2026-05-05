#include "editor_fsm.hpp"

std::pair<EditorMode, EditorAction> process_keypress(EditorMode current_mode, char key) {
  switch(current_mode) {
    case EditorMode::NORMAL:
      switch(key) {
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
      else if (key == '\x1b') { // Escpae key
        return {EditorMode::NORMAL, PrintMessage{"Entering NORMAL mode"}};
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
