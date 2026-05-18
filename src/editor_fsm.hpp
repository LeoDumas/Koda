#pragma once
#include <variant>
#include <string>
#include <utility>

// Available state
enum class EditorMode { NORMAL, INSERT, VISUAL, COMMAND };

// Possible actions
struct PrintMessage { std::string message; };
struct QuitEditor {};
struct DoNothing {};

// Content manipulation
struct InsertChar { char c; };
struct MoveCursorLeft {};
struct MoveCursorRight {};
struct MoveCursorUp {};
struct MoveCursorDown {};
struct DeleteCharacter {};

using EditorAction = std::variant<PrintMessage, QuitEditor, DoNothing, InsertChar, 
      MoveCursorLeft, MoveCursorRight, MoveCursorUp, MoveCursorDown, DeleteCharacter>;

// Return new State and new action to perform based on the current one
std::pair<EditorMode, EditorAction> process_keypress(EditorMode current_mode, char key);
