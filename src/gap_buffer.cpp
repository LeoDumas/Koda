#include "gap_buffer.hpp"
#include <algorithm>

void GapBuffer::insert_char_at_cursor(char c) {
    if (this->gap_start_ == this->gap_end_) this->grow_gap();
    this->buffer_[this->gap_start_] = c;
    this->gap_start_++;
}

void GapBuffer::delete_at_cursor() {
    if (this->gap_start_ > 0) {
        this->gap_start_--;
    }
}

void GapBuffer::move_cursor_to_position(std::size_t position) {
    std::size_t len = buffer_.size() - (gap_end_ - gap_start_);
    if (position > len) position = len;

    while (position > this->gap_start_) { // shift gap right
        this->buffer_[this->gap_start_++] = this->buffer_[this->gap_end_++];
    }
    while (position < this->gap_start_) { // shift gap left
        this->buffer_[--this->gap_end_] = this->buffer_[--this->gap_start_];
    }
}

void GapBuffer::move_left() {
    if (gap_start_ > 0) {
        move_cursor_to_position(gap_start_ - 1);
    }
}

void GapBuffer::move_right() {
    std::size_t len = buffer_.size() - (gap_end_ - gap_start_);
    if (gap_start_ < len) {
        move_cursor_to_position(gap_start_ + 1);
    }
}

void GapBuffer::move_up() {
    Cursor cur = get_cursor_2d();
    if (cur.row == 0) return;

    // Find the start of the current line
    size_t pos = gap_start_;
    while (pos > 0 && get_data()[pos - 1] != '\n') {
        pos--;
    }

    // Find the start of the previous line
    size_t current_line_start = pos;
    pos--; // Move to the \n of the previous line
    while (pos > 0 && get_data()[pos - 1] != '\n') {
        pos--;
    }
    size_t prev_line_start = pos;

    // Target position is prev_line_start + col
    size_t prev_line_len = current_line_start - 1 - prev_line_start;
    size_t target_col = std::min(cur.col, prev_line_len);
    
    move_cursor_to_position(prev_line_start + target_col);
}

void GapBuffer::move_down() {
    std::string data = get_data();
    Cursor cur = get_cursor_2d();
    
    // Find the start of the next line
    size_t pos = gap_start_;
    while (pos < data.length() && data[pos] != '\n') {
        pos++;
    }
    
    if (pos >= data.length()) return; // No next line
    
    size_t next_line_start = pos + 1;
    
    // Find the end of the next line
    size_t next_line_end = next_line_start;
    while (next_line_end < data.length() && data[next_line_end] != '\n') {
        next_line_end++;
    }
    
    size_t next_line_len = next_line_end - next_line_start;
    size_t target_col = std::min(cur.col, next_line_len);
    
    move_cursor_to_position(next_line_start + target_col);
}

void GapBuffer::grow_gap() {
    std::size_t new_gap_size = 4096;
    buffer_.insert(buffer_.begin() + gap_end_, new_gap_size, '\0');
    gap_end_ += new_gap_size;
}

Cursor GapBuffer::get_cursor_2d() const {
    Cursor cur;
    std::string data = get_data();
    for (size_t i = 0; i < gap_start_; ++i) {
        if (data[i] == '\n') {
            cur.row++;
            cur.col = 0;
        } else {
            cur.col++;
        }
    }
    return cur;
}

std::string GapBuffer::get_data() const {
    std::string result;
    result.reserve(buffer_.size() - (gap_end_ - gap_start_));
    result.append(buffer_.begin(), buffer_.begin() + gap_start_);
    result.append(buffer_.begin() + gap_end_, buffer_.end());
    return result;
}
