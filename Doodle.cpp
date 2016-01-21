#include "Doodle.h"

Doodle::Doodle(){}
Doodle::~Doodle(){}

bool Doodle::Initialize(Window* window, InputHandler* input)
{
    this->window = window;
    this->input = input;
}