#include "InputHandler.h"

InputHandler::InputHandler()
{
    this->keystate = SDL_GetKeyboardState(NULL);
}
InputHandler::~InputHandler(){}

void InputHandler::OnEvent(SDL_Event* event)
{
    CEvent::OnEvent(event);
}

void InputHandler::OnMouseFocus(){}

void InputHandler::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{
    this->mX = mX;
    this->mY = mY;
    this->relX = relX;
    this->relY = relY;
    this->mL = Left;
    this->mR = Right;
    this->mM = Middle;
}

void InputHandler::OnLButtonDown(int mX, int mY)
{
    this->mX = mX;
    this->mY = mY;

    this->mL = true;
}

void InputHandler::OnLButtonUp(int mX, int mY)
{
    this->mX = mX;
    this->mY = mY;

    this->mL = false;
}

void InputHandler::OnKeyDown(SDL_Keycode sym, Uint16 mod)
{

}

void InputHandler::OnKeyUp(SDL_Keycode sym, Uint16 mod)
{

}