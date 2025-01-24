#include "EventEngine.h"

// When creating EventEngine, set all key states to false
EventEngine::EventEngine() : running(true) {
	for (int i = 0; i < Key::LAST; ++i) {
		keys[i].pressed = false;
		keys[i].clicked = false;
		keys[i].released = false;
	} // sets all the keys to false

	for (int i = 0; i < Mouse::BTN_LAST; i++) {
		buttons[i].pressed = false;
		buttons[i].clicked = false;
		buttons[i].released = false;
	}
}

EventEngine::~EventEngine() {}

void EventEngine::pollEvents() {
	// clicked and released events are only active for one frame
	// Change - Reset click and release states when polling events
	for (int i = 0; i < Key::LAST; ++i) {
		keys[i].clicked = false;
		keys[i].released = false;
	}
	for (int i = 0; i < Mouse::BTN_LAST; i++) {
		buttons[i].clicked = false;
		buttons[i].released = false;
	}
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
			updateKeys(event.key.keysym.sym, event.type == SDL_KEYDOWN);
		} // Keys are updated on keydown and keyup events, the events are ignored if they happen multiple times in a row

		if (event.type == SDL_QUIT) {
			keys[QUIT].pressed = true;
		}
		
		// Change - getting the mouse state also updates the mouse's click and release states
		bool leftPressed = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
		bool rightPressed = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;

		// Clicked and released states are determined by comparing the pressed state of a button on two different frames
		if (leftPressed && !buttons[Mouse::BTN_LEFT].pressed) {
			buttons[Mouse::BTN_LEFT].clicked = true;
		}
		if (!leftPressed && buttons[Mouse::BTN_LEFT].pressed) {
			buttons[Mouse::BTN_LEFT].released = true;
		}
		if (rightPressed && !buttons[Mouse::BTN_RIGHT].pressed) {
			buttons[Mouse::BTN_RIGHT].clicked = true;
		}
		if (!rightPressed && buttons[Mouse::BTN_RIGHT].pressed) {
			buttons[Mouse::BTN_RIGHT].released = true;
		}

		buttons[Mouse::BTN_LEFT].pressed = leftPressed;
		buttons[Mouse::BTN_RIGHT].pressed = rightPressed;
	}
}

// The updateKeys() function only runs on individual click/release events, it doesn't trigger every frame
void EventEngine::updateKeys(const SDL_Keycode &key, bool keyDown) {
	Key index;
	switch (key) {
		case SDLK_RIGHT:	index = Key::RIGHT; break;
		case SDLK_d:		index = Key::D; break;
		case SDLK_LEFT:		index = Key::LEFT; break; 
		case SDLK_a:		index = Key::A; break;
		case SDLK_UP:		index = Key::UP; break;
		case SDLK_w:		index = Key::W; break;
		case SDLK_DOWN:		index = Key::DOWN; break;
		case SDLK_s:		index = Key::S; break;
		case SDLK_ESCAPE:	index = Key::ESC; break;
		case SDLK_SPACE:	index = Key::SPACE; break;
		default:
			return;
	}

	keys[index].pressed = keyDown;
	// Change - Update clicked/released based on keyDown
	if (keyDown) {
		keys[index].clicked = true;
	} else {
		keys[index].released = true;
	}
}

void EventEngine::setPressed(Key key) { // turns a key on // it's annoying that you can't turn a key off, You can't do on click events, only on hold
    keys[key].pressed = true;
}

void EventEngine::setPressed(Mouse btn) {
    buttons[btn].pressed = true;
}

bool EventEngine::isPressed(Key key) {
	return keys[key].pressed;
}

bool EventEngine::isPressed(Mouse btn) {
	return buttons[btn].pressed;
}

bool EventEngine::isClicked(Key key) {
	return keys[key].clicked;
}

bool EventEngine::isClicked(Mouse btn) {
	return buttons[btn].clicked;
}

bool EventEngine::isReleased(Key key) {
	return keys[key].released;
}

bool EventEngine::isReleased(Mouse btn) {
	return buttons[btn].released;
}

void EventEngine::setMouseRelative(bool b) { // relative???? to what?
	if (SDL_SetRelativeMouseMode(b ? SDL_TRUE : SDL_FALSE) < 0) {
#ifdef __DEBUG
		debug("Warning: SDL_SetRelativeMouseMode() isn't supported");
#endif
	}
}

// position where the mouse is held down
Point2 EventEngine::getMouseDPos() { 
	Point2 mouseDPos;
	SDL_GetRelativeMouseState(&mouseDPos.x, &mouseDPos.y);
	return mouseDPos;
}

// position where the mouse is
Point2 EventEngine::getMousePos() {
	Point2 pos;
	SDL_GetMouseState(&pos.x, &pos.y);
	return pos;
}