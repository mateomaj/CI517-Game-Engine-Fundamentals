#include "MyEngineSystem.h"

MyEngineSystem::MyEngineSystem() {
	//std::cout << "aaa\n";
}



// ----------------------------------------------------------------------------------------------
// 
//	Scene Class
// 
// ----------------------------------------------------------------------------------------------


// Scene constructors
// The scene has to include a root node when being created, it also has the option to set its alignment or resize the window (inspired from JavaFX)

Scene::Scene(std::shared_ptr<Parent> root) {
	this->root = root;
}

Scene::Scene(std::shared_ptr<Parent> root, Alignment alignment) {
	this->root = root;
	this->alignment = alignment;
}

Scene::Scene(std::shared_ptr<Parent> root, int windowWidth, int windowHeight, std::shared_ptr<GraphicsEngine> gfx) {
	this->root = root;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	gfx->setWindowSize(this->windowWidth, this->windowHeight);
}

Scene::Scene(std::shared_ptr<Parent> root, int windowWidth, int windowHeight, Alignment alignment, std::shared_ptr<GraphicsEngine> gfx) {
	this->root = root;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	gfx->setWindowSize(this->windowWidth, this->windowHeight);
	this->alignment = alignment;
}

// Get and set the scene's alignment
int Scene::getAlignment() {
	return this->alignment;
}

void Scene::setAlignment(Alignment alignment) {
	this->alignment = alignment;
}

// Swap root nodes
void Scene::setRoot(std::shared_ptr<Parent> root) {
	if (root != nullptr) {
		this->root = root;
	}
}

void Scene::unloadScene() {

}

// Renders the root node and starts the chain reaction for rendering the whole scene
void Scene::render(std::shared_ptr<GraphicsEngine> gfx) {
	this->root->render(gfx);
}

// Handles scene events starting from the root node
void Scene::handleSceneEvents(std::shared_ptr<EventEngine> eventsystem) {
	if (this->root != nullptr) {
		this->handleNodeEvent(eventsystem, this->root);
	}
}

// Handles mouse events for a specific node, if that node happens to be a parent, go through all of its children and handle their events as well
void Scene::handleNodeEvent(std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<Node> node) {
	if (!node->isVisible()) return;
	if (this->isMouseOverNode(eventSystem, *node->getScreenTransform())) {
		//std::cout << "Mouse is over it\n\n\n";
		if (node->onMouseHoverOn && !node->isFocused()) { 
			node->onMouseHoverOn();
		}
		if (node->onMouseClicked && eventSystem->isClicked(Mouse::BTN_LEFT)) { 
			node->setFocused(true);
			node->onMouseClicked(Mouse::BTN_LEFT); 
		}
		if (node->onMouseClicked && eventSystem->isClicked(Mouse::BTN_RIGHT)) {
			node->setFocused(true);
			node->onMouseClicked(Mouse::BTN_RIGHT);
		}
	}
	else
	{
		//std::cout << "Mouse is not over it\n\n\n";
		if (node->onMouseHoverOff && !node->isFocused()) { 
			node->onMouseHoverOff(); 
		}
	}

	// Mouse pressed and released events can happen from anywhere on the screen, if a node is focused on it should always trigger the pressed event until the mouse button is released.
	// This makes features like dragging nodes around the screen possible
	if (node->onMousePressed && node->isFocused() && eventSystem->isPressed(Mouse::BTN_LEFT)) {
		node->onMousePressed(Mouse::BTN_LEFT);
	}
	if (node->onMousePressed && node->isFocused() && eventSystem->isPressed(Mouse::BTN_RIGHT)) {
		node->onMousePressed(Mouse::BTN_RIGHT);
	}

	// Once the mouse button is released, the node should lose its focus
	if (node->onMouseReleased && node->isFocused() && eventSystem->isReleased(Mouse::BTN_LEFT)) {
		node->setFocused(false);
		node->onMouseReleased(Mouse::BTN_LEFT);
	}
	if (node->onMouseReleased && node->isFocused() && eventSystem->isReleased(Mouse::BTN_RIGHT)) {
		node->setFocused(false);
		node->onMouseReleased(Mouse::BTN_RIGHT);
	}

	// Unfocusing a node should be possible even if the mouse released event is undefined
	if (node->isFocused() && (eventSystem->isReleased(Mouse::BTN_RIGHT) || eventSystem->isReleased(Mouse::BTN_LEFT))) node->setFocused(false);

	// Attempts to cast the node into a parent, if it succeeds then the node is a parent that was downgraded to a node
	if (Parent* parent = dynamic_cast<Parent*>(node.get())) {
		//std::cout << "Is a parent\n";
		if (parent->getChildren().size() != 0) {
			for (std::shared_ptr<Node> child : parent->getChildren()) {
				this->handleNodeEvent(eventSystem, child);
			}
		}
	}
	// else {
	//	std::cout << "In not a parent\n";
	//}
}

// Returns if the cursor's position lands over a node using its screen transform
bool Scene::isMouseOverNode(std::shared_ptr<EventEngine> eventSystem, Transform& screenTransform) {
	return (Rectangle2f {
		screenTransform.x + screenTransform.offsetX,
		screenTransform.y + screenTransform.offsetY,
		screenTransform.width * screenTransform.scaleX,
		screenTransform.height * screenTransform.scaleY
	}.contains(eventSystem->getMousePos()));
}

// ----------------------------------------------------------------------------------------------
// 
//	Node Class
// 
// ----------------------------------------------------------------------------------------------

// Node constructors
// The basic constructors allow the user to define most of the node's transform data
// The base Node class isn't meant to be used on its own so it lacks most of the basic features

Node::Node() {
	
}

Node::Node(float x, float y) {
	this->transform.x = x;
	this->transform.y = y;
}

Node::Node(float x, float y, float width, float height) {
	this->transform.x = x;
	this->transform.y = y;
	this->transform.width = width;
	this->transform.height = height;
}


Node::Node(float x, float y, float width, float height, float offsetX, float offsetY) {
	this->transform.x = x;
	this->transform.y = y;
	this->transform.width = width;
	this->transform.height = height;
	this->transform.offsetX = offsetY;
	this->transform.offsetY = offsetY;
}

void Node::unload() {

}

void Node::render(std::shared_ptr<GraphicsEngine> gfx) {

}

void Node::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	this->globalTransform = transform;
}

// ----------------------------------------------------------------------------------------------
// 
//	Parent Class
// 
// ----------------------------------------------------------------------------------------------

// Parent constructor
// Just like the Node class, the basic Parent class isn't meant to do anything on its own without being derived from

Parent::Parent() {

}

// Basic functionality for adding and removing children from a parent node
// A parent can be given one or many child nodes within its child limit, when a new child is added it will have its global transform updated automatically to avoid weird rendering interactions
// Doesn't perform any special checks on the children as most cases are automatically handled during rendering
// Any more specific handling should be done by derived classes

void Parent::addChild(std::shared_ptr<Node> child) {
	if (this->childrenLimit == 0) return;
	if (this->children.size() == this->childrenLimit) return;
	this->children.push_back(child);
	this->calcScreenTransform();
	child->globalTransform = *this->getScreenTransform();
	child->parent = std::static_pointer_cast<Parent>(shared_from_this());
}

void::Parent::addChildren(std::vector<std::shared_ptr<Node>> children) {
	if (this->childrenLimit == 0) return;
	if (this->children.size() == this->childrenLimit) return;
	this->calcScreenTransform();
	for (std::shared_ptr<Node> child : children) {
		if (this->children.size() == this->childrenLimit) {
			break;
		}
		this->children.push_back(child);
		child->globalTransform = *this->getScreenTransform();
		child->parent = std::static_pointer_cast<Parent>(shared_from_this());
		if (this->children.size() == this->childrenLimit) return;
	}
}

// Needed help from: https://en.cppreference.com/w/cpp/container/vector/erase

// Removes a child node from a parent by instance // Removed children have their shared pointer reset to help with memory management
void Parent::removeChild(std::shared_ptr<Node> child) {
	std::vector<std::shared_ptr<Node>>::iterator pos = this->children.begin();
	for (std::shared_ptr<Node> node : this->children) {
		if (node == child) {
			node->parent.reset();
			node.reset();
			this->children.erase(pos);
			break;
		}
		pos++;
	}
}

// Overrides the current list of children with a new one // Removed children have their shared pointer reset to help with memory management
void Parent::setChildren(std::vector<std::shared_ptr<Node>> children) {
	for (std::shared_ptr<Node> child : this->children) { // Done just in case to prevent a memory leak
		child->parent.reset();
		child.reset();
	}
	this->children.clear();
	if (this->childrenLimit == 0) return;
	this->calcScreenTransform();
	for (std::shared_ptr<Node> child : children) {
		if (this->children.size() == this->childrenLimit) return;
		this->children.push_back(child);
		child->globalTransform = *this->getScreenTransform();
		child->parent = std::static_pointer_cast<Parent>(shared_from_this());
	}
}

// Overrides the current list of children with a single new child node // Removed children have their shared pointer reset to help with memory management
void Parent::setChild(std::shared_ptr<Node> newChild) {
	for (std::shared_ptr<Node> child : this->children) { // Done just in case to prevent a memory leak
		child->parent.reset();
		child.reset();
	}
	this->children.clear();
	if (this->childrenLimit == 0) return;
	this->calcScreenTransform();
	this->children.push_back(newChild);
	newChild->globalTransform = *this->getScreenTransform();
	newChild->parent = std::static_pointer_cast<Parent>(shared_from_this());
}

// Returns the first child of the parent, or null pointer if it can't
std::shared_ptr<Node> Parent::getChild() {
	if (this->children.size() == 0 || this->childrenLimit == 0) return nullptr;
	return this->children.at(0);
}

// Returns the child of a parent at a given index, or null pointer if it can't
std::shared_ptr<Node> Parent::getChild(int position) {
	if (this->children.size() == 0 || this->childrenLimit == 0 || position < 0 || position >= this->children.size()) return nullptr;
	return this->children.at(position);
}

// Basic parent render function, calculates its own screen transform and uses it to render each child
void Parent::render(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	this->calcScreenTransform();
	this->renderChildren(gfx, *this->getScreenTransform());
}

void Parent::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	this->globalTransform = transform;
	this->calcScreenTransform();
	this->renderChildren(gfx, *this->getScreenTransform());
}


// The render children functions are kind of redundant


// Runs through all of the parent's children and renders them, should only be ran if the parent's screen transform was already calculated
void Parent::renderChildren(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	//this->calcScreenTransform();
	for (std::shared_ptr<Node> child : children) {
		child->render(gfx, *this->getScreenTransform());
	}
}

// Runs through all of the parent's children and renders them, the input transform is expected to be the parent's screen transform
void Parent::renderChildren(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	//this->globalTransform = transform;
	//this->calcScreenTransform();
	for (std::shared_ptr<Node> child : children) {
		//child->render(gfx, *this->getScreenTransform());
		child->render(gfx, transform);
	}
}

void Parent::unload() {
	this->unloadChildren();
}

void Parent::unloadChildren() {
	for (std::shared_ptr<Node> child : children) {
		child->unload();
		child.reset();
	}
	this->children.clear();
}



// ----------------------------------------------------------------------------------------------
// 
//	Button Class
// 
// ----------------------------------------------------------------------------------------------



Button::Button() {
	
}

Button::Button(SDL_Rect rect) {
	button = rect;
	this->setX(button.x);
	this->setY(button.y);
	this->setWidth(button.w);
	this->setHeight(button.h);
}

Button::Button(Rect rect) {
	button = rect.getSDLRect();
	this->setX(button.x);
	this->setY(button.y);
	this->setWidth(button.w);
	this->setHeight(button.h);
}

Button::Button(SDL_Rect rect, SDL_Color colour) {
	button = rect;
	this->setX(button.x);
	this->setY(button.y);
	this->setWidth(button.w);
	this->setHeight(button.h);
	this->colour = colour;
}

Button::Button(Rect rect, SDL_Color colour) {
	button = rect.getSDLRect();
	this->setX(button.x);
	this->setY(button.y);
	this->setWidth(button.w);
	this->setHeight(button.h);
	this->colour = colour;
}

Button::Button(float x, float y, float width, float height) {
	this->setX(x);
	this->setY(y);
	this->setWidth(width);
	this->setHeight(height);
}

Button::Button(float x, float y, float width, float height, SDL_Color colour) {
	this->setX(x);
	this->setY(y);
	this->setWidth(width);
	this->setHeight(height);
	this->colour = colour;
}

void Button::render(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	Transform temp;
	if (this->globalTransform.alignment != Alignment::NONE) {
		temp = *this->calcScreenTransform(this->globalTransform.alignment);
	} else {
		temp = *this->calcScreenTransform();
	}
	int calcX = floor(temp.x + temp.offsetX);
	int calcY = floor(temp.y + temp.offsetY);
	this->button = { calcX, calcY, (int)(temp.width * temp.scaleX), (int)(temp.height * temp.scaleY) };
	gfx->setDrawColor(colour);
	gfx->fillRect(&this->button);

	if (this->buttonText.length() > 0) {
		gfx->setDrawColor(this->textColour);
		gfx->drawText(this->buttonText, calcX + (int)(temp.width * temp.scaleX / 2) - (int)(this->buttonText.length() * 11), calcY + (int)(temp.height * temp.scaleY / 2) - 25);
	}
}

void Button::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	this->globalTransform = transform;

	Transform temp;
	if (this->globalTransform.alignment != Alignment::NONE) {
		temp = *this->calcScreenTransform(this->globalTransform.alignment);
	} else {
		temp = *this->calcScreenTransform();
	}
	int calcX = floor(temp.x + temp.offsetX);
	int calcY = floor(temp.y + temp.offsetY);
	this->button = { calcX, calcY, (int)(temp.width * temp.scaleX), (int)(temp.height * temp.scaleY) };
	
	gfx->setDrawColor(colour);
	gfx->fillRect(&this->button);
	
	if (this->buttonText.length() > 0) {
		gfx->setDrawColor(this->textColour);
		gfx->drawText(this->buttonText, calcX + (int)(temp.width * temp.scaleX / 2) - (int)(this->buttonText.length() * 11), calcY + (int)(temp.height * temp.scaleY / 2) - 25);
	}
}



// ----------------------------------------------------------------------------------------------
// 
//	HBox Class
// 
// ----------------------------------------------------------------------------------------------

HBox::HBox() {
	
}

HBox::HBox(std::vector<std::shared_ptr<Node>> children) {
	this->setChildren(children);
}

HBox::HBox(float gap) {
	this->nodeGap = gap;
}

HBox::HBox(float gap, std::vector<std::shared_ptr<Node>> children) {
	this->nodeGap = gap;
	this->setChildren(children);
}

void HBox::render(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	
	Transform next;
	if (this->globalTransform.alignment != Alignment::NONE) {
		next = *this->calcScreenTransform(this->globalTransform.alignment);
	} else {
		next = *this->calcScreenTransform();
	}
	next.alignment = Alignment::TOP_LEFT;
	float start = next.x;
	float height = 0;
	
	for (std::shared_ptr<Node> child : this->getChildren()) {
		child->render(gfx, next);
		next.x += child->getScreenTransform()->width * child->getScreenTransform()->scaleX + nodeGap * next.scaleX;
		if (child->getScreenTransform()->height > height) {
			height = child->getScreenTransform()->height;
		}
	}
	
	this->screenTransform.width = next.x - start - (this->getChildren().size() > 0 ? nodeGap * next.scaleX : 0);
	this->screenTransform.height = height;
}

void HBox::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	this->globalTransform = transform;
	
	Transform next;
	if (this->globalTransform.alignment != Alignment::NONE) {
		next = *this->calcScreenTransform(this->globalTransform.alignment);
	} else {
		next = *this->calcScreenTransform();
	}
	next.alignment = Alignment::TOP_LEFT;
	float start = next.x;
	float height = 0;
	
	for (std::shared_ptr<Node> child : this->getChildren()) {
		child->render(gfx, next);
		next.x += child->getScreenTransform()->width * child->getScreenTransform()->scaleX + nodeGap * next.scaleX;
		if (child->getScreenTransform()->height > height) {
			height = child->getScreenTransform()->height;
		}
	}
	this->screenTransform.width = next.x - start - (this->getChildren().size() > 0 ? nodeGap * next.scaleX : 0);
	this->screenTransform.height = height;
}


// ----------------------------------------------------------------------------------------------
// 
//	VBox Class
// 
// ----------------------------------------------------------------------------------------------

VBox::VBox() {

}

VBox::VBox(std::vector<std::shared_ptr<Node>> children) {
	this->setChildren(children);
}

VBox::VBox(float gap) {
	this->nodeGap = gap;
}

VBox::VBox(float gap, std::vector<std::shared_ptr<Node>> children) {
	this->nodeGap = gap;
	this->setChildren(children);
}

void VBox::render(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	Transform next;
	if (this->globalTransform.alignment != Alignment::NONE) {
		next = *this->calcScreenTransform(this->globalTransform.alignment);
	} else {
		next = *this->calcScreenTransform();
	}
	next.alignment = Alignment::TOP_LEFT;
	float start = next.y;
	float width = 0;
	for (std::shared_ptr<Node> child : this->getChildren()) {
		child->render(gfx, next);
		next.y += child->getScreenTransform()->height * child->getScreenTransform()->scaleY + nodeGap * next.scaleY;
		if (child->getScreenTransform()->width > width) {
			width = child->getScreenTransform()->width;
		}
	}
	this->screenTransform.height = next.y - start - (this->getChildren().size() > 0 ? nodeGap * next.scaleY : 0);
	this->screenTransform.width = width;
}

void VBox::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	this->globalTransform = transform;
	Transform next;
	if (this->globalTransform.alignment != Alignment::NONE) {
		next = *this->calcScreenTransform(this->globalTransform.alignment);
	} else {
		next = *this->calcScreenTransform();
	}
	next.alignment = Alignment::TOP_LEFT;
	float start = next.y;
	float width = 0;
	for (std::shared_ptr<Node> child : this->getChildren()) {
		child->render(gfx, next);
		next.y += child->getScreenTransform()->height * child->getScreenTransform()->scaleY + nodeGap * next.scaleY;
		if (child->getScreenTransform()->width > width) {
			width = child->getScreenTransform()->width;
		}
	}
	this->screenTransform.height = next.y - start - (this->getChildren().size() > 0 ? nodeGap * next.scaleY : 0);
	this->screenTransform.width = width;
}



// ----------------------------------------------------------------------------------------------
// 
//	Frame Class
// 
// ----------------------------------------------------------------------------------------------



Frame::Frame() {

}

Frame::Frame(SDL_Color backgroundColour) {

}

Frame::Frame(int alignment) {
	this->setAlignment(alignment);
}

Frame::Frame(int alignment, SDL_Color backgroundColour) {
	this->setAlignment(alignment);
	this->setBackgroundColour(backgroundColour);
}

Frame::Frame(float width, float height) {
	this->setWidth(width);
	this->setHeight(height);
}

Frame::Frame(float width, float height, SDL_Color backgroundColour) {
	this->setWidth(width);
	this->setHeight(height);
	this->setBackgroundColour(backgroundColour);
}

Frame::Frame(float width, float height, int alignment) {
	this->setWidth(width);
	this->setHeight(height);
	this->setAlignment(alignment);
}

Frame::Frame(float width, float height, int alignment, SDL_Color backgroundColour) {
	this->setWidth(width);
	this->setHeight(height);
	this->setAlignment(alignment);
	this->setBackgroundColour(backgroundColour);
}

Frame::Frame(std::shared_ptr<Node> child) {
	this->setChild(child);
}

Frame::Frame(std::shared_ptr<Node> child, SDL_Color backgroundColour) {
	this->setChild(child);
	this->setBackgroundColour(backgroundColour);
}

Frame::Frame(std::shared_ptr<Node> child, int alignment) {
	this->setAlignment(alignment);
	this->setChild(child);
}

Frame::Frame(std::shared_ptr<Node> child, int alignment, SDL_Color backgroundColour) {
	this->setAlignment(alignment);
	this->setChild(child);
	this->setBackgroundColour(backgroundColour);
}

Frame::Frame(float width, float height, std::shared_ptr<Node> child) {
	this->setWidth(width);
	this->setHeight(height);
	this->setChild(child);
}

Frame::Frame(float width, float height, std::shared_ptr<Node> child, SDL_Color backgroundColour) {
	this->setWidth(width);
	this->setHeight(height);
	this->setChild(child);
	this->setBackgroundColour(backgroundColour);
}

Frame::Frame(float width, float height, std::shared_ptr<Node> child, int alignment) {
	this->setWidth(width);
	this->setHeight(height);
	this->setAlignment(alignment);
	this->setChild(child);
}

Frame::Frame(float width, float height, std::shared_ptr<Node> child, int alignment, SDL_Color backgroundColour) {
	this->setWidth(width);
	this->setHeight(height);
	this->setAlignment(alignment);
	this->setChild(child);
	this->setBackgroundColour(backgroundColour);
}

void Frame::render(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	this->calcScreenTransform();

	if (this->backgroundColour.a != 0) {
		gfx->setDrawColor(this->backgroundColour);
		SDL_Rect temp = { (int)this->screenTransform.x + (int)this->screenTransform.offsetX, (int)this->screenTransform.y + (int)this->screenTransform.offsetY, (int)(this->screenTransform.width * this->screenTransform.scaleX), (int)(this->screenTransform.height * this->screenTransform.scaleY) };
		gfx->fillRect(&temp);
	}
	if (this->getChildren().size() != 0) {
		this->screenTransform.alignment = this->alignment;
		this->getChildren().at(0)->render(gfx, *this->getScreenTransform());
	}
}

void Frame::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	this->globalTransform = transform;
	this->calcScreenTransform();

	if (this->backgroundColour.a != 0) {
		gfx->setDrawColor(this->backgroundColour);
		SDL_Rect temp = { (int)this->screenTransform.x + (int)this->screenTransform.offsetX, (int)this->screenTransform.y + (int)this->screenTransform.offsetY, (int)(this->screenTransform.width * this->screenTransform.scaleX), (int)(this->screenTransform.height * this->screenTransform.scaleY) };
		gfx->fillRect(&temp);
	}
	if (this->getChildren().size() != 0) {
		this->screenTransform.alignment = this->alignment;
		this->getChildren().at(0)->render(gfx, *this->getScreenTransform());
	}
}



// ----------------------------------------------------------------------------------------------
// 
//	Pane Class
// 
// ----------------------------------------------------------------------------------------------



Pane::Pane() {

}

Pane::Pane(std::vector<std::shared_ptr<Node>> children) {
	this->setChildren(children);
}

void Pane::render(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	this->calcScreenTransform();
	float width = 0;
	float height = 0;
	for (std::shared_ptr<Node> child : this->getChildren()) {
		this->screenTransform.alignment = static_cast<Alignment>(-child->getAlignment());
		child->render(gfx, this->screenTransform);
		if (child->getScreenTransform()->width + child->getScreenTransform()->x - this->getScreenTransform()->x > width) {
			width = child->getScreenTransform()->width;
		}
		if (child->getScreenTransform()->height + child->getScreenTransform()->y - this->getScreenTransform()->y > height) {
			height = child->getScreenTransform()->height;
		}
	}
	this->screenTransform.width = width;
	this->screenTransform.height = height;
}

void Pane::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	this->globalTransform = transform;
	this->calcScreenTransform();
	float width = 0;
	float height = 0;
	for (std::shared_ptr<Node> child : this->getChildren()) {
		this->screenTransform.alignment = static_cast<Alignment>(-child->getAlignment());
		child->render(gfx, this->screenTransform);
		if (child->getScreenTransform()->width + child->getScreenTransform()->x - this->getScreenTransform()->x > width) {
			width = child->getScreenTransform()->width;
		}
		if (child->getScreenTransform()->height + child->getScreenTransform()->y - this->getScreenTransform()->y > height) {
			height = child->getScreenTransform()->height;
		}
	}
	this->screenTransform.width = width;
	this->screenTransform.height = height;
}



// ----------------------------------------------------------------------------------------------
// 
//	Label Class
// 
// ----------------------------------------------------------------------------------------------



Label::Label() {

}

Label::Label(float x, float y) {
	this->setX(x);
	this->setY(y);
}

Label::Label(std::string text) {
	this->text = text;
}

Label::Label(float x, float y, std::string text) {
	this->setX(x);
	this->setY(y);
	this->text = text;
}

Label::Label(std::string text, SDL_Color colour) {
	this->text = text;
	this->colour = colour;
}

Label::Label(float x, float y, std::string text, SDL_Color colour) {
	this->setX(x);
	this->setY(y);
	this->text = text;
	this->colour = colour;
}

void Label::render(std::shared_ptr<GraphicsEngine> gfx) {
	if (!this->visible) return;
	this->calcScreenTransform();
	this->screenTransform.width = this->text.length() * 26;
	this->screenTransform.height = 50;
	if (this->text.length() == 0) return;
	gfx->setDrawColor(this->colour);
	gfx->drawText(this->text, this->screenTransform.x + this->screenTransform.offsetX, this->screenTransform.y + this->screenTransform.offsetY);
}

void Label::render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform) {
	if (!this->visible) return;
	this->globalTransform = transform;
	this->calcScreenTransform();
	this->screenTransform.width = this->text.length() * 26;
	this->screenTransform.height = 50;
	if (this->text.length() == 0) return;
	gfx->setDrawColor(this->colour);
	gfx->drawText(this->text, this->screenTransform.x + this->screenTransform.offsetX, this->screenTransform.y + this->screenTransform.offsetY);
}
