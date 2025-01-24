#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false), background(0, 0, 1920, 1080) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 45);
	gfx->useFont(font);
	gfx->setVerticalSync(true);
	
	gfx->setWindowIcon("res/icon.png");
	gfx->setWindowTitle("X-CUBE 2D Scene Graph System - Mateusz Maj - mm2490");
	
	// Initialise menu nodes

	menu1 = std::make_shared<HBox>();
	menu2 = std::make_shared<VBox>(0);
	menu3 = std::make_shared<Button>(700, 400, 200, 100, SDL_Color{ 255, 150, 255 });
	menu4 = std::make_shared<Pane>();

	menu1->setVisible(false);
	menu2->setVisible(false);
	menu3->setVisible(false);
	menu4->setVisible(false);

	// Main five buttons for switching between menus
	std::shared_ptr<Button> button = std::make_shared<Button>(SDL_Rect{ 0, 0, 100, 100 }, SDL_Color{ 255, 255, 0 });
	std::shared_ptr<Button> button2 = std::make_shared<Button>(SDL_Rect{ 0, 0, 100, 100 }, SDL_Color{ 0, 255, 0 });
	std::shared_ptr<Button> button3 = std::make_shared<Button>(SDL_Rect{ 0, 0, 100, 100 }, SDL_Color{ 0, 255, 255 });
	std::shared_ptr<Button> button4 = std::make_shared<Button>(SDL_Rect{ 0, 0, 100, 100 }, SDL_Color{ 0, 0, 255 });
	std::shared_ptr<Button> button5 = std::make_shared<Button>(SDL_Rect{ 0, 0, 210, 50 }, SDL_Color{ 255, 255, 255 });
	button->setText("1");
	button2->setText("2");
	button3->setText("3");
	button4->setText("4");
	button5->setText("Enter");
	
	button->onMouseHoverOn = [button]() {
		button->setWidth(button->getWidth() + 3);
		button->setHeight(button->getHeight() + 3);
		if (button->getWidth() > 110) button->setWidth(110);
		if (button->getHeight() > 110) button->setHeight(110);
	};
	button->onMouseHoverOff = [button]() {
		button->setWidth(button->getWidth() - 3);
		button->setHeight(button->getHeight() - 3);
		if (button->getWidth() < 100) button->setWidth(100);
		if (button->getHeight() < 100) button->setHeight(100);
	};
	button->onMouseClicked = [button, this](Mouse mouseButton) {
		button->setWidth(95);
		button->setHeight(95);
		if (patternStage < 10) {
			if (buttonPattern[patternStage] != 1) {
				patternStage = 0;
			}
			if (buttonPattern[patternStage] == 1) {
				patternStage++;
			}
		}
		menu1->setVisible(true);
		menu2->setVisible(false);
		menu3->setVisible(false);
		menu4->setVisible(false);
	};
	button->onMouseReleased = [button](Mouse mouseButton) {
		button->setWidth(100);
		button->setHeight(100);
	};

	button2->onMouseHoverOn = [button2]() {
		//std::cout << "I'm running!\n";
		button2->setWidth(button2->getWidth() + 3);
		button2->setHeight(button2->getHeight() + 3);
		if (button2->getWidth() > 110) button2->setWidth(110);
		if (button2->getHeight() > 110) button2->setHeight(110);
	};
	button2->onMouseHoverOff = [button2]() {
		button2->setWidth(button2->getWidth() - 3);
		button2->setHeight(button2->getHeight() - 3);
		if (button2->getWidth() < 100) button2->setWidth(100);
		if (button2->getHeight() < 100) button2->setHeight(100);
	};
	button2->onMouseClicked = [button2, this](Mouse button) {
		button2->setWidth(95);
		button2->setHeight(95);
		if (patternStage < 10) {
			if (buttonPattern[patternStage] != 2) {
				patternStage = 0;
			}
			if (buttonPattern[patternStage] == 2) {
				patternStage++;
			}
		}
		menu1->setVisible(false);
		menu2->setVisible(true);
		menu3->setVisible(false);
		menu4->setVisible(false);
	};
	button2->onMousePressed = [button2](Mouse button) {
		/*
		if (button == Mouse::BTN_LEFT) {
			std::cout << "Left Held\n";
		}
		else if (button == Mouse::BTN_RIGHT) {
			std::cout << "Right Held\n";
		}
		*/
	};
	button2->onMouseReleased = [button2](Mouse button) {
		button2->setWidth(100);
		button2->setHeight(100);
	};

	button3->onMouseHoverOn = [button3]() {
		button3->setWidth(button3->getWidth() + 3);
		button3->setHeight(button3->getHeight() + 3);
		if (button3->getWidth() > 110) button3->setWidth(110);
		if (button3->getHeight() > 110) button3->setHeight(110);
	};
	button3->onMouseHoverOff = [button3]() {
		button3->setWidth(button3->getWidth() - 3);
		button3->setHeight(button3->getHeight() - 3);
		if (button3->getWidth() < 100) button3->setWidth(100);
		if (button3->getHeight() < 100) button3->setHeight(100);
	};
	button3->onMouseClicked = [button3, this](Mouse button) {
		button3->setWidth(95);
		button3->setHeight(95);
		if (patternStage < 10) {
			if (buttonPattern[patternStage] != 3) {
				patternStage = 0;
			}
			if (buttonPattern[patternStage] == 3) {
				patternStage++;
			}
		}
		menu1->setVisible(false);
		menu2->setVisible(false);
		menu3->setVisible(true);
		menu4->setVisible(false);
	};
	button3->onMouseReleased = [button3](Mouse button) {
		button3->setWidth(100);
		button3->setHeight(100);
	};

	button4->onMouseHoverOn = [button4]() {
		button4->setWidth(button4->getWidth() + 3);
		button4->setHeight(button4->getHeight() + 3);
		if (button4->getWidth() > 110) button4->setWidth(110);
		if (button4->getHeight() > 110) button4->setHeight(110);
	};
	button4->onMouseHoverOff = [button4]() {
		button4->setWidth(button4->getWidth() - 3);
		button4->setHeight(button4->getHeight() - 3);
		if (button4->getWidth() < 100) button4->setWidth(100);
		if (button4->getHeight() < 100) button4->setHeight(100);
	};
	button4->onMouseClicked = [button4, this](Mouse button) {
		button4->setWidth(95);
		button4->setHeight(95);
		if (patternStage < 10) {
			if (buttonPattern[patternStage] != 4) {
				patternStage = 0;
			}
			if (buttonPattern[patternStage] == 4) {
				patternStage++;
			}
		}
		menu1->setVisible(false);
		menu2->setVisible(false);
		menu3->setVisible(false);
		menu4->setVisible(true);
	};
	button4->onMouseReleased = [button4](Mouse button) {
		button4->setWidth(100);
		button4->setHeight(100);
	};

	button5->onMouseHoverOn = [button5]() {
		button5->setWidth(button5->getWidth() + 3);
		button5->setHeight(button5->getHeight() + 3);
		if (button5->getWidth() > 220) button5->setWidth(220);
		if (button5->getHeight() > 60) button5->setHeight(60);
	};
	button5->onMouseHoverOff = [button5]() {
		button5->setWidth(button5->getWidth() - 3);
		button5->setHeight(button5->getHeight() - 3);
		if (button5->getWidth() < 210) button5->setWidth(210);
		if (button5->getHeight() < 50) button5->setHeight(50);
	};
	button5->onMouseClicked = [button5, this](Mouse button) {
		button5->setWidth(205);
		button5->setHeight(45);
		if (patternStage < 10) {
			if (buttonPattern[patternStage] != 5) {
				patternStage = 0;
			}
			if (buttonPattern[patternStage] == 5) {
				patternStage++;
			}
		}
		menu1->setVisible(false);
		menu2->setVisible(false);
		menu3->setVisible(false);
		menu4->setVisible(false);
	};
	button5->onMouseReleased = [button5](Mouse button) {
		button5->setWidth(210);
		button5->setHeight(50);
	};
	
	std::shared_ptr<VBox> parent = std::make_shared<VBox>();
	std::shared_ptr<HBox> parent2 = std::make_shared<HBox>();
	std::shared_ptr<HBox> parent3 = std::make_shared<HBox>();

	parent->setX(28);
	parent->setY(350);
	parent->setScale(1.5);

	parent->addChild(parent2);
	parent->addChild(parent3);

	std::shared_ptr<Frame> frame1 = std::make_shared<Frame>(110, 110);
	frame1->setChild(button);
	frame1->setAlignment(Alignment::CENTER);
	std::shared_ptr<Frame> frame2 = std::make_shared<Frame>(110, 110);
	frame2->setChild(button2);
	frame2->setAlignment(Alignment::CENTER);
	std::shared_ptr<Frame> frame3 = std::make_shared<Frame>(110, 110);
	frame3->setChild(button3);
	frame3->setAlignment(Alignment::CENTER);
	std::shared_ptr<Frame> frame4 = std::make_shared<Frame>(110, 110);
	frame4->setChild(button4);
	frame4->setAlignment(Alignment::CENTER);
	std::shared_ptr<Frame> frame5 = std::make_shared<Frame>(230, 60);
	frame5->setChild(button5);
	frame5->setAlignment(Alignment::CENTER);
	
	parent2->addChild(frame1);
	parent2->addChild(frame2);
	parent3->addChild(frame3);
	parent3->addChild(frame4);
	parent->addChild(frame5);

	std::shared_ptr<Frame> backFrame1 = std::make_shared<Frame>(1600, 70);
	backFrame1->setChild(std::make_shared<Button>(SDL_Rect{ 0, 0, 1600, 70 }, SDL_Color{ 100, 100, 100 }));
	std::shared_ptr<Frame> backFrame2 = std::make_shared<Frame>(400, 760);
	backFrame2->setChild(std::make_shared<Button>(SDL_Rect{ 0, 0, 400, 860 }, SDL_Color{ 150, 150, 150 }));
	std::shared_ptr<Frame> backFrame3 = std::make_shared<Frame>(1600, 70);
	backFrame3->setChild(std::make_shared<Button>(SDL_Rect{ 0, 0, 1600, 70 }, SDL_Color{ 100, 100, 100 }));
	
	std::shared_ptr<VBox> backgroundVBox = std::make_shared<VBox>(0);
	backgroundVBox->addChild(backFrame1);
	backgroundVBox->addChild(backFrame2);
	backgroundVBox->addChild(backFrame3);

	std::shared_ptr<Label> gameLogoLabel = std::make_shared<Label>("Game Logo", SDL_COLOR_WHITE);
	gameLogoLabel->onMouseClicked = [gameLogoLabel](Mouse button) {
		gameLogoLabel->setTextColour(getRandomColor(100, 255));
	};

	std::shared_ptr<HBox> topHBox = std::make_shared<HBox>();
	topHBox->addChild(gameLogoLabel);

	topHBox->setX(10);
	topHBox->setY(10);



	// Menu 1 - Mission select



	menu1->setX(475);
	menu1->setY(225);

	std::shared_ptr<VBox> tempVBox = std::make_shared<VBox>(0);

	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 350 }, SDL_Color{ 0, 150, 0 });
	std::shared_ptr<Frame> frame = std::make_shared<Frame>(260, 360, Alignment::BOTTOM);
	frame->setChild(button);
	tempVBox->addChild(frame);
	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 75 }, SDL_Color{ 200, 50, 200 });
	button->setText("Mission 1");
	frame = std::make_shared<Frame>(260, 85, Alignment::TOP);
	frame->setChild(button);
	tempVBox->addChild(frame);
	menu1->addChild(tempVBox);
	tempVBox->onMouseHoverOn = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() + 3);
		button1->setHeight(button1->getHeight() + 3);
		if (button1->getWidth() > 260) button1->setWidth(260);
		if (button1->getHeight() > 360) button1->setHeight(360);
		button2->setWidth(button2->getWidth() + 3);
		button2->setHeight(button2->getHeight() + 3);
		if (button2->getWidth() > 260) button2->setWidth(260);
		if (button2->getHeight() > 85) button2->setHeight(85);
	};
	tempVBox->onMouseHoverOff = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() - 3);
		button1->setHeight(button1->getHeight() - 3);
		if (button1->getWidth() < 250) button1->setWidth(250);
		if (button1->getHeight() < 350) button1->setHeight(350);
		button2->setWidth(button2->getWidth() - 3);
		button2->setHeight(button2->getHeight() - 3);
		if (button2->getWidth() < 250) button2->setWidth(250);
		if (button2->getHeight() < 75) button2->setHeight(75);
	};
	tempVBox->onMouseClicked = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(245);
		button1->setHeight(345);
		button2->setWidth(245);
		button2->setHeight(70);
	};
	tempVBox->onMouseReleased = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(250);
		button1->setHeight(350);
		button2->setWidth(250);
		button2->setHeight(75);
	};

	tempVBox = std::make_shared<VBox>(0);

	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 350 }, SDL_Color{ 0, 150, 0 });
	frame = std::make_shared<Frame>(260, 360, Alignment::BOTTOM);
	frame->setChild(button);
	tempVBox->addChild(frame);
	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 75 }, SDL_Color{ 200, 50, 200 });
	button->setText("Mission 2");
	frame = std::make_shared<Frame>(260, 85, Alignment::TOP);
	frame->setChild(button);
	tempVBox->addChild(frame);
	menu1->addChild(tempVBox);
	tempVBox->onMouseHoverOn = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() + 3);
		button1->setHeight(button1->getHeight() + 3);
		if (button1->getWidth() > 260) button1->setWidth(260);
		if (button1->getHeight() > 360) button1->setHeight(360);
		button2->setWidth(button2->getWidth() + 3);
		button2->setHeight(button2->getHeight() + 3);
		if (button2->getWidth() > 260) button2->setWidth(260);
		if (button2->getHeight() > 85) button2->setHeight(85);
	};
	tempVBox->onMouseHoverOff = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() - 3);
		button1->setHeight(button1->getHeight() - 3);
		if (button1->getWidth() < 250) button1->setWidth(250);
		if (button1->getHeight() < 350) button1->setHeight(350);
		button2->setWidth(button2->getWidth() - 3);
		button2->setHeight(button2->getHeight() - 3);
		if (button2->getWidth() < 250) button2->setWidth(250);
		if (button2->getHeight() < 75) button2->setHeight(75);
	};
	tempVBox->onMouseClicked = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(245);
		button1->setHeight(345);
		button2->setWidth(245);
		button2->setHeight(70);
	};
	tempVBox->onMouseReleased = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(250);
		button1->setHeight(350);
		button2->setWidth(250);
		button2->setHeight(75);
	};

	tempVBox = std::make_shared<VBox>(0);

	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 350 }, SDL_Color{ 0, 150, 0 });
	frame = std::make_shared<Frame>(260, 360, Alignment::BOTTOM);
	frame->setChild(button);
	tempVBox->addChild(frame);
	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 75 }, SDL_Color{ 200, 50, 200 });
	button->setText("Mission 3");
	frame = std::make_shared<Frame>(260, 85, Alignment::TOP);
	frame->setChild(button);
	tempVBox->addChild(frame);
	menu1->addChild(tempVBox);
	tempVBox->onMouseHoverOn = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() + 3);
		button1->setHeight(button1->getHeight() + 3);
		if (button1->getWidth() > 260) button1->setWidth(260);
		if (button1->getHeight() > 360) button1->setHeight(360);
		button2->setWidth(button2->getWidth() + 3);
		button2->setHeight(button2->getHeight() + 3);
		if (button2->getWidth() > 260) button2->setWidth(260);
		if (button2->getHeight() > 85) button2->setHeight(85);
	};
	tempVBox->onMouseHoverOff = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() - 3);
		button1->setHeight(button1->getHeight() - 3);
		if (button1->getWidth() < 250) button1->setWidth(250);
		if (button1->getHeight() < 350) button1->setHeight(350);
		button2->setWidth(button2->getWidth() - 3);
		button2->setHeight(button2->getHeight() - 3);
		if (button2->getWidth() < 250) button2->setWidth(250);
		if (button2->getHeight() < 75) button2->setHeight(75);
	};
	tempVBox->onMouseClicked = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(245);
		button1->setHeight(345);
		button2->setWidth(245);
		button2->setHeight(70);
	};
	tempVBox->onMouseReleased = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(250);
		button1->setHeight(350);
		button2->setWidth(250);
		button2->setHeight(75);
	};

	tempVBox = std::make_shared<VBox>(0);

	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 350 }, SDL_Color{ 0, 150, 0 });
	frame = std::make_shared<Frame>(260, 360, Alignment::BOTTOM);
	frame->setChild(button);
	tempVBox->addChild(frame);
	button = std::make_shared<Button>(SDL_Rect{ 0, 0, 250, 75 }, SDL_Color{ 200, 50, 200 });
	button->setText("Mission 4");
	frame = std::make_shared<Frame>(260, 85, Alignment::TOP);
	frame->setChild(button);
	tempVBox->addChild(frame);
	menu1->addChild(tempVBox);
	tempVBox->onMouseHoverOn = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() + 3);
		button1->setHeight(button1->getHeight() + 3);
		if (button1->getWidth() > 260) button1->setWidth(260);
		if (button1->getHeight() > 360) button1->setHeight(360);
		button2->setWidth(button2->getWidth() + 3);
		button2->setHeight(button2->getHeight() + 3);
		if (button2->getWidth() > 260) button2->setWidth(260);
		if (button2->getHeight() > 85) button2->setHeight(85);
	};
	tempVBox->onMouseHoverOff = [tempVBox]() {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(button1->getWidth() - 3);
		button1->setHeight(button1->getHeight() - 3);
		if (button1->getWidth() < 250) button1->setWidth(250);
		if (button1->getHeight() < 350) button1->setHeight(350);
		button2->setWidth(button2->getWidth() - 3);
		button2->setHeight(button2->getHeight() - 3);
		if (button2->getWidth() < 250) button2->setWidth(250);
		if (button2->getHeight() < 75) button2->setHeight(75);
	};
	tempVBox->onMouseClicked = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(245);
		button1->setHeight(345);
		button2->setWidth(245);
		button2->setHeight(70);
	};
	tempVBox->onMouseReleased = [tempVBox](Mouse button) {
		std::shared_ptr<Node> button1 = dynamic_cast<Parent*>(tempVBox->getChild(0).get())->getChild();
		std::shared_ptr<Node> button2 = dynamic_cast<Parent*>(tempVBox->getChild(1).get())->getChild();
		button1->setWidth(250);
		button1->setHeight(350);
		button2->setWidth(250);
		button2->setHeight(75);
	};



	// Menu 2 - Mock-up Settings menu



	menu2->setX(475);
	menu2->setY(200);

	//Top half
	frame = std::make_shared<Frame>(1075, 75, SDL_Color{ 100, 150, 150 });
	std::shared_ptr<HBox> tempHBox = std::make_shared<HBox>(20);
	tempHBox->setOffsetX(25);
	tempHBox->setOffsetY(10);
	frame->setChild(tempHBox);
	menu2->addChild(frame);

	tempHBox->addChild(std::make_shared<Label>("Settings"));
	tempHBox->addChild(std::make_shared<Label>("Graphics"));
	tempHBox->addChild(std::make_shared<Label>("Gameplay"));
	tempHBox->addChild(std::make_shared<Label>("Account"));
	
	// Bottom half

	frame = std::make_shared<Frame>(1075, 425, SDL_Color{ 150, 250, 250 });
	tempVBox = std::make_shared<VBox>(25);
	tempVBox->setOffsetX(25);
	tempVBox->setOffsetY(25);
	frame->setChild(tempVBox);
	menu2->addChild(frame);

	frame = std::make_shared<Frame>(1025, 50, SDL_Color{ 100, 225, 225 });
	tempVBox->addChild(frame);
	tempHBox = std::make_shared<HBox>(400);
	frame->setChild(tempHBox);
	frame = std::make_shared<Frame>(300, 50);
	frame->setOffsetX(10);
	frame->setChild(std::make_shared<Label>("Option1"));
	tempHBox->addChild(frame);
	frame = std::make_shared<Frame>(300, 50, Alignment::CENTER);
	button = std::make_shared<Button>(0, 0, 300, 50, SDL_Color{ 100, 200, 175});
	button->setText("Selection");
	frame->setChild(button);
	tempHBox->addChild(frame);

	frame = std::make_shared<Frame>(1025, 50, SDL_Color{ 100, 225, 225 });
	tempVBox->addChild(frame);
	tempHBox = std::make_shared<HBox>(400);
	frame->setChild(tempHBox);
	frame = std::make_shared<Frame>(300, 50);
	frame->setOffsetX(10);
	frame->setChild(std::make_shared<Label>("Option2"));
	tempHBox->addChild(frame);
	frame = std::make_shared<Frame>(300, 50, Alignment::CENTER);
	button = std::make_shared<Button>(0, 0, 300, 50, SDL_Color{ 100, 200, 175 });
	button->setText("Selection");
	frame->setChild(button);
	tempHBox->addChild(frame);

	frame = std::make_shared<Frame>(1025, 50, SDL_Color{ 100, 225, 225 });
	tempVBox->addChild(frame);
	tempHBox = std::make_shared<HBox>(400);
	frame->setChild(tempHBox);
	frame = std::make_shared<Frame>(300, 50);
	frame->setOffsetX(10);
	frame->setChild(std::make_shared<Label>("Option3"));
	tempHBox->addChild(frame);
	frame = std::make_shared<Frame>(300, 50, Alignment::CENTER);
	button = std::make_shared<Button>(0, 0, 300, 50, SDL_Color{ 100, 200, 175 });
	button->setText("Selection");
	frame->setChild(button);
	tempHBox->addChild(frame);

	frame = std::make_shared<Frame>(1025, 50, SDL_Color{ 100, 225, 225 });
	tempVBox->addChild(frame);
	tempHBox = std::make_shared<HBox>(400);
	frame->setChild(tempHBox);
	frame = std::make_shared<Frame>(300, 50);
	frame->setOffsetX(10);
	frame->setChild(std::make_shared<Label>("Option4"));
	tempHBox->addChild(frame);
	frame = std::make_shared<Frame>(300, 50, Alignment::CENTER);
	button = std::make_shared<Button>(0, 0, 300, 50, SDL_Color{ 100, 200, 175 });
	button->setText("Selection");
	frame->setChild(button);
	tempHBox->addChild(frame);

	frame = std::make_shared<Frame>(1025, 50, SDL_Color{ 100, 225, 225 });
	tempVBox->addChild(frame);
	tempHBox = std::make_shared<HBox>(400);
	frame->setChild(tempHBox);
	frame = std::make_shared<Frame>(300, 50);
	frame->setOffsetX(10);
	frame->setChild(std::make_shared<Label>("Option5"));
	tempHBox->addChild(frame);
	frame = std::make_shared<Frame>(300, 50, Alignment::CENTER);
	button = std::make_shared<Button>(0, 0, 300, 50, SDL_Color{ 100, 200, 175 });
	button->setText("Selection");
	frame->setChild(button);
	tempHBox->addChild(frame);


	// Menu 3 - Draggable node
	
	
	menu3->setText("Drag Me!!");
	menu3->onMouseClicked = [](Mouse button) {};
	menu3->onMousePressed = [this](Mouse button) {
		Point2 position = eventSystem->getMousePos();
		menu3->setX(position.x - menu3->getWidth() / 2);
		menu3->setY(position.y - menu3->getHeight() / 2);
	};
	menu3->onMouseReleased = [](Mouse button) {};


	// Menu 4 - Draggable node limited within its parent

	menu4->setX(475);
	menu4->setY(125);
	menu4->setWidth(1050);
	menu4->setHeight(650);
	frame = std::make_shared<Frame>(0, 0, SDL_Color{ 150, 150, 75 });
	menu4->addChild(frame);
	button = std::make_shared<Button>(0, 0, 300, 150, SDL_Color{ 200, 150, 200 });
	button->setText("Drag Me Too!!");
	button->onMouseClicked = [](Mouse mouseButton) {};
	button->onMousePressed = [button, this](Mouse mouseButton) {
		Point2 position = eventSystem->getMousePos();
		button->setX(position.x - button->getWidth() / 2 - menu4->getX());
		button->setY(position.y - button->getHeight() / 2 - menu4->getY());
	};
	button->onMouseReleased = [](Mouse mouseButton) {};
	menu4->addChild(button);


	std::shared_ptr<Pane> pane = std::make_shared<Pane>();
	pane->addChild(backgroundVBox);
	pane->addChild(topHBox);
	pane->addChild(parent);
	pane->addChild(menu1);
	pane->addChild(menu2);
	pane->addChild(menu3);
	pane->addChild(menu4);

	mainScene = new Scene(pane, 1600, 900, gfx);
}

MyGame::~MyGame() { // Destructor - Runs when the object is being deleted
	
}

void MyGame::onLeftMouseButton() {
	Point2 point = eventSystem->getMousePos();
	// Technically could have been used for specific mouse events but it won't be able to support events like hover
}

void MyGame::handleKeyEvents() {
	// Not neded for the demo
}

void MyGame::update() {
	// Handle any mouse input events on the scene
	mainScene->handleSceneEvents(eventSystem);
	
	if (patternStage == 10) {
		patternMatched = true;
	}
}

void MyGame::render() {
	// Draw background
	gfx->setDrawColor(SDL_Color { 100, 150, 255});
	gfx->fillRect(&background.getSDLRect());
	gfx->drawRect(background);
}

void MyGame::renderUI() {
	// Render scene graph
	mainScene->render(gfx);

	// If the right pattern of buttons is hit, display easter egg text
	gfx->setDrawColor(SDL_COLOR_BLACK);
	if (patternMatched) gfx->drawText("Mateusz_Maj-CI517-2024", 0, 850);
}