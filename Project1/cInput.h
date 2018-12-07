#pragma once
#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "XInput.lib")


struct JumpOrDuckInputState {
	bool duckHeldDown;
	bool jumpHeldDown;
};

class cInput
{
private:
	DWORD xResult;
	XINPUT_STATE state;

public:
	cInput();
	~cInput();

	JumpOrDuckInputState getInput();
};

