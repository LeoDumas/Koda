#pragma once

#include <vector>
#include <string>

class GapBuffer{

private: 
  std::vector<char> buffer_;
  std::size_t gap_start_; // gap start index & the cursor position
  std::size_t gap_end_; // gap end index

  void grow_gap();

public:
  GapBuffer(std::size_t initial_gap_size = 4096)
    : buffer_(initial_gap_size, '\0'),
      gap_start_(0),
      gap_end_(initial_gap_size)
    {}

  void insert_char_at_cursor(char c);

  void delete_at_cursor(); // Backspace equivalent

  void move_cursor_to_position(std::size_t position);

  std::string get_data();

};
