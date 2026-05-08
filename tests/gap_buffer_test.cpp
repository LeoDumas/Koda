#include <gtest/gtest.h>
#include "gap_buffer.hpp"

TEST(GapBuffer, InsertChars) {
    GapBuffer buf;
    buf.insert_char_at_cursor('H');
    buf.insert_char_at_cursor('i');
    EXPECT_EQ(buf.get_data(), "Hi");
}

TEST(GapBuffer, DeleteAtCursor) {
    GapBuffer buf;
    buf.insert_char_at_cursor('H');
    buf.insert_char_at_cursor('i');
    buf.delete_at_cursor();
    EXPECT_EQ(buf.get_data(), "H");
}

TEST(GapBuffer, DeleteAtZeroDoesNothing) {
    GapBuffer buf;
    buf.delete_at_cursor();
    EXPECT_EQ(buf.get_data(), "");
}

TEST(GapBuffer, MoveCursorAndInsert) {
    GapBuffer buf;
    buf.insert_char_at_cursor('H');
    buf.insert_char_at_cursor('l');
    buf.move_cursor_to_position(1);
    buf.insert_char_at_cursor('e');
    EXPECT_EQ(buf.get_data(), "Hel");
}

TEST(GapBuffer, MoveCursorOutOfBoundsClamped) {
    GapBuffer buf;
    buf.insert_char_at_cursor('A');
    buf.move_cursor_to_position(999);
    buf.insert_char_at_cursor('B');
    EXPECT_EQ(buf.get_data(), "AB");
}

TEST(GapBuffer, MoveCursorLeftAndRight) {
    GapBuffer buf;
    buf.insert_char_at_cursor('A');
    buf.insert_char_at_cursor('B');
    buf.insert_char_at_cursor('C');
    buf.move_cursor_to_position(0);
    buf.move_cursor_to_position(2);
    EXPECT_EQ(buf.get_data(), "ABC");
}
