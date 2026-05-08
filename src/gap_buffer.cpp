#include "gap_buffer.hpp"

void GapBuffer::insert_char_at_cursor(char c)
{
  if (this->gap_start_ == this->gap_end_) this->grow_gap();
  this->buffer_[this->gap_start_] = c;
  this->gap_start_ ++;

}

void GapBuffer::delete_at_cursor()
{
  if(this->gap_start_ > 0){
    this->gap_start_--; // The deleted char will end up in the gap
  }
}

void GapBuffer::move_cursor_to_position(std::size_t position)
  {
    std::size_t len = buffer_.size() - (gap_end_ - gap_start_);
    if (position > len) position = len;
    while (position > this->gap_start_) {  // shift gap right
      this->buffer_[this->gap_start_++] = this->buffer_[this->gap_end_++];
    }
    while (position < this->gap_start_) {  // shift gap left
      this->buffer_[--this->gap_end_] = this->buffer_[--this->gap_start_];
    }
  }

  void GapBuffer::grow_gap()
  {
    std::size_t new_gap_size = 4096;
    buffer_.insert(buffer_.begin() + gap_end_, new_gap_size, '\0');
    gap_end_ += new_gap_size;
  }

std::string GapBuffer::get_data()
{
  // Content before the gap
  std::string result(this->buffer_.begin(), this->buffer_.begin() + this->gap_start_);
  // Add to result the content after the gap
  result.insert(result.end(), this->buffer_.begin() + this->gap_end_, this->buffer_.end());

  return result;
}
