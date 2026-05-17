#include <gtest/gtest.h>
#include "editor_fsm.hpp"

TEST(EditorFSM, NormalToInsert) {
    auto [mode, action] = process_keypress(EditorMode::NORMAL, 'i');
    EXPECT_EQ(mode, EditorMode::INSERT);
    EXPECT_TRUE(std::holds_alternative<PrintMessage>(action));
}

TEST(EditorFSM, NormalToCommand) {
    auto [mode, action] = process_keypress(EditorMode::NORMAL, ':');
    EXPECT_EQ(mode, EditorMode::COMMAND);
    EXPECT_TRUE(std::holds_alternative<PrintMessage>(action));
}

TEST(EditorFSM, NormalToVisual) {
    auto [mode, action] = process_keypress(EditorMode::NORMAL, 'v');
    EXPECT_EQ(mode, EditorMode::VISUAL);
    EXPECT_TRUE(std::holds_alternative<PrintMessage>(action));
}

TEST(EditorFSM, InsertCharInInsertMode) {
    auto [mode, action] = process_keypress(EditorMode::INSERT, 'x');
    EXPECT_EQ(mode, EditorMode::INSERT);
    ASSERT_TRUE(std::holds_alternative<InsertChar>(action));
    EXPECT_EQ(std::get<InsertChar>(action).c, 'x');
}

TEST(EditorFSM, BackspaceInInsertMode) {
    auto [mode, action] = process_keypress(EditorMode::INSERT, 127);
    EXPECT_EQ(mode, EditorMode::INSERT);
    EXPECT_TRUE(std::holds_alternative<DeleteCharacter>(action));
}

TEST(EditorFSM, EscapeToNormalMode) {
    // From INSERT
    auto res1 = process_keypress(EditorMode::INSERT, '\x1b');
    EXPECT_EQ(res1.first, EditorMode::NORMAL);

    // From VISUAL
    auto res2 = process_keypress(EditorMode::VISUAL, '\x1b');
    EXPECT_EQ(res2.first, EditorMode::NORMAL);

    // From COMMAND
    auto res3 = process_keypress(EditorMode::COMMAND, '\x1b');
    EXPECT_EQ(res3.first, EditorMode::NORMAL);
}

TEST(EditorFSM, QuitFromCommandMode) {
    auto [mode, action] = process_keypress(EditorMode::COMMAND, 'q');
    EXPECT_EQ(mode, EditorMode::COMMAND);
    EXPECT_TRUE(std::holds_alternative<QuitEditor>(action));
}

TEST(EditorFSM, DoNothingOnUnknownKey) {
    auto [mode, action] = process_keypress(EditorMode::NORMAL, 'z');
    EXPECT_EQ(mode, EditorMode::NORMAL);
    EXPECT_TRUE(std::holds_alternative<DoNothing>(action));
}
