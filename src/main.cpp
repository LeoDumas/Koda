#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

enum State { NORMAL, INSERT, VISUAL, COMMAND };
State currentState = State::NORMAL;
struct termios orig_termios;

void disableRawMode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(){
  tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

 	struct termios raw = orig_termios;
 	raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
  enableRawMode();

  char c;

  while(read(STDIN_FILENO, &c, 1)){
    switch(currentState){
      case State::INSERT:
        switch(c){
          case '\x1b': // Escape key
            currentState = State::NORMAL;
            std::cout << "Entering NORMAL mode" << std::endl;
            break;
        }
        break;
      case State::COMMAND:
        switch(c){
          case '\x1b': // Escape key
            currentState = State::NORMAL;
            std::cout << "Entering NORMAL mode" << std::endl;
            break;

          case 'q':
            return 0;
        }
        break;
      case State::VISUAL:
        switch(c){
          case '\x1b': // Escape key
            currentState = State::NORMAL;
            std::cout << "Entering NORMAL mode" << std::endl;
            break;
        }
        break;
      case State::NORMAL:
        switch (c){
          case 'a':
            currentState = State::INSERT;
            std::cout << "Entering INSERT mode (insert on left)" << std::endl;
            break;
          case 'i':
            currentState = State::INSERT;
            std::cout << "Entering INSERT mode (insert on right)" << std::endl;
            break;
          case ':':
            currentState = State::COMMAND;
            std::cout << "Entering COMMAND mode" << std::endl;
            break;
          case 'v':
            currentState = State::VISUAL;
            std::cout << "Entering VISUAL mode" << std::endl;
            break;
        }
        break;
      }
    }
  return 0;
}
