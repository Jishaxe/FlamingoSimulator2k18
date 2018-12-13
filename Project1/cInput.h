#pragma once
#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "XInput.lib")


struct JumpOrDuckInputState {
	bool duckHeldDown = false;
	bool jumpHeldDown = false;
	bool exit = false;
};

// This is whether the duck/jump keys are held down on the keyboard, as we only get keydown/keyup events here
struct KeyboardInputState {
	bool duckHeldDown = false;
	bool jumpHeldDown = false;
};

class cInput
{
private:
	DWORD xResult;
	XINPUT_STATE state;

public:
	cInput();
	~cInput();

	// The current input state only from the keyboard, 
	// this is persisted as we only get events from keyboards, rather than checking once a tick
	KeyboardInputState keyboardInputState;

	JumpOrDuckInputState getInput();
};

