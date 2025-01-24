#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct GameKey {
	Point2 pos;
	bool isAlive;
};

class MyGame : public AbstractGame {
	private:
		Scene* mainScene;
		Rect background;

		// For the scene graph, none of the gameplay variables are needed

		//Vector2i velocity;

		//std::vector<std::shared_ptr<GameKey>> gameKeys;

		/* GAMEPLAY */
		//int score, numKeys, lives;
		//bool gameWon;
		

		// Button combination to reveal the easter egg
		int buttonPattern[10] = { 1, 4, 3, 2, 2, 3, 4, 1, 5, 5 };
		int patternStage = 0;
		bool patternMatched = false;

		// Main nodes used for each popup menu
		std::shared_ptr<HBox> menu1;
		std::shared_ptr<VBox> menu2;
		std::shared_ptr<Button> menu3;
		std::shared_ptr<Pane> menu4;

		void onLeftMouseButton();
		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
        MyGame();
		~MyGame();
};

#endif