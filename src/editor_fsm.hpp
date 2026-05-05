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

using EditorAction = std::variant<PrintMessage, QuitEditor, DoNothing>;

// Return new State and new action to perform based on the current one
std::pair<EditorMode, EditorAction> process_keypress(EditorMode current_mode, char key);
