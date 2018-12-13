#include "cInput.h"



cInput::cInput()
{
}


cInput::~cInput()
{
}

JumpOrDuckInputState cInput::getInput()
{
	// Results defaults to false
	JumpOrDuckInputState result = { false, false, false };


	SDL_Event event;

	// Create the state to hold the result
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	xResult = XInputGetState(0, &state);

	// If there was no error trying to get the first controller, then it is plugged in
	if (xResult == ERROR_SUCCESS) {
		// Jump buttons
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A || state.Gamepad.wButtons & XINPUT_GAMEPAD_Y || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			result.jumpHeldDown = true;
		}

		// Duck buttons
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X || state.Gamepad.wButtons & XINPUT_GAMEPAD_B || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			result.duckHeldDown = true;
		}
	}

	while (SDL_PollEvent(&event))
	{
		// Pass the exit back to the game
		if (event.type == SDL_QUIT) result.exit = true;

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
			case SDLK_SPACE:
			case SDLK_UP:
				// Make the player jump here
				keyboardInputState.jumpHeldDown = true;
				break;
			case SDLK_s:
			case SDLK_LCTRL:
			case SDLK_RCTRL:
			case SDLK_DOWN:
				keyboardInputState.duckHeldDown = true;
				// Make the player duck here
				break;
			default:
				break;
			}
		}

		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			case SDLK_w:
			case SDLK_SPACE:
			case SDLK_UP:
				// Make the player not jump here
				keyboardInputState.jumpHeldDown = false;
				break;
			case SDLK_s:
			case SDLK_LCTRL:
			case SDLK_RCTRL:
			case SDLK_DOWN:
				// Make the player not sduck here
				keyboardInputState.duckHeldDown = false;
				break;
			default:
				break;
			}
		}
	}

	// Now see if the keyboard buttons are held down and apply this to the final result
	if (keyboardInputState.duckHeldDown) result.duckHeldDown = true;
	if (keyboardInputState.jumpHeldDown) result.jumpHeldDown = true;
	return result;
}
