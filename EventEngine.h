#ifndef __EVENT_ENGINE_H__
#define __EVENT_ENGINE_H__

#include <string>
#include <thread>

#include <SDL.h>

#include "EngineCommon.h"
#include "GameMath.h"

enum Key { // Enums for keys
	W, S, A, D, ESC, SPACE, UP, DOWN, LEFT, RIGHT, QUIT, LAST
};

enum Mouse { // Mouse enums
	BTN_LEFT, BTN_RIGHT, BTN_LAST
};

// Addition:
// Tracks click, press, and release states of buttons/keys
struct ButtonState {
	bool clicked = false, pressed = false, released = false;
};

class EventEngine {
	friend class XCube2Engine;
	private:
		bool running;
		SDL_Event event;
		// Change - arrays were swapped from bool to ButtonState
		ButtonState keys[Key::LAST];
		ButtonState buttons[Mouse::BTN_LAST];

		void updateKeys(const SDL_Keycode &, bool);

		EventEngine();
	public:
		~EventEngine();

		/**
		* Equivalent to calling SDL_PollEvent()
		*/
		void pollEvents();
		
		// Change - Extra functions were added for click and release states
		bool isPressed(Key);
		bool isPressed(Mouse);
		bool isClicked(Key);
		bool isClicked(Mouse);
		bool isReleased(Key);
		bool isReleased(Mouse);
    
        /**
         * Software emulation of keypresses
         */
        void setPressed(Key);
        void setPressed(Mouse);
	
		void setMouseRelative(bool);

		/**
		* Returns mouse's delta position
		* It's the difference between current and
		* previous mouse positions
		*
		*/
		Point2 getMouseDPos();

		/**
		* Returns current mouse position relative to the window
		*/
		Point2 getMousePos();
};

#endif