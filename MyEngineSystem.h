#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include <map>
#include <vector>
#include <functional>
#include "../EngineCommon.h"
#include "../GraphicsEngine.h"
#include "../EventEngine.h"
//#include "../XCube2d.h" // Either include XCube or specific engine parts that you need
// including specific engine parts instead of XCube is probably better practice
// Yep, You can't include XCube because it would make a cyclical hierarchy and cause a lot of problems, you can only include classes on the same level as you

enum Alignment {
	NONE,
	TOP_LEFT,		TOP,			TOP_RIGHT,
	CENTER_LEFT,	CENTER,			CENTER_RIGHT,
	BOTTOM_LEFT,	BOTTOM,			BOTTOM_RIGHT
}; // No point in adding a LAST enum for this // Could add INLINE but this is enough
// Left and Right are used in eventEngine and apparently that causes the program to break.
// Enumerator variable names can be referenced on their own for some reason without using the enum, so because of namespace wackyness I need to change their names

class Node;
class Parent;
class Scene;
struct Transform;

class MyEngineSystem { // effective name - Scene graph manager
	friend class XCube2Engine; // The friend relationship means that XCube has access to my private functions, I can't get anything from XCube unless it friends me // XCube is friended so it can instanciate MyEngineSystem, that's why it's private
	private:
		MyEngineSystem();
		//XCube2Engine baseEngine = nullptr;
		
	protected:
		// With how XCube and all the other subsystems are set up, only one instance of a subsystem can exist at a time, and they can only be set up within XCube. Since there's only one instance of the GFX, we don't need to pass it into every single render function as if it's going to change, the user should not be forced to do that.
		//std::shared_ptr<GraphicsEngine> gfx;

	public:
		//void test(GraphicsEngine*);
		//~MyEngineSystem();


};

class Scene {
	friend class MyEngineSystem;
	private:
		std::shared_ptr<Parent> root;
		Alignment alignment = TOP_LEFT;
		int windowWidth = DEFAULT_WINDOW_WIDTH, windowHeight = DEFAULT_WINDOW_HEIGHT;
		void handleNodeEvent(std::shared_ptr<EventEngine> eventSystem, std::shared_ptr<Node> node);
		bool isMouseOverNode(std::shared_ptr<EventEngine> eventSystem, Transform& screenTransform);
	public:
		Scene(std::shared_ptr<Parent> root);
		Scene(std::shared_ptr<Parent> root, Alignment alignment);
		Scene(std::shared_ptr<Parent> root, int windowWidth, int windowHeight, std::shared_ptr<GraphicsEngine> gfx);
		Scene(std::shared_ptr<Parent> root, int windowWidth, int windowHeight, Alignment allignment, std::shared_ptr<GraphicsEngine> gfx);

		int getWindowWidth() { return windowWidth; }
		int getWindowHeight() { return windowHeight; }

		int getAlignment();
		void setAlignment(Alignment alignment);

		std::shared_ptr<Parent> getRoot() { return root; }
		void setRoot(std::shared_ptr<Parent> Root);

		void unloadScene(); // or Destroy scene

		void render(std::shared_ptr<GraphicsEngine> gfx); // Renders the whole scene (starts the chain reaction for rendering)
		void handleSceneEvents(std::shared_ptr<EventEngine> eventSystem);
		
};

// Contains transformation values relative to the screen. This shows the actual position, rotation, and scale of a node, that can then be passed onto its children and added onto for rendering
struct Transform {
public:
	float x = 0, y = 0, width = 0, height = 0, angle = 0, scaleX = 1, scaleY = 1, offsetX = 0, offsetY = 0;
	Alignment alignment = Alignment::NONE;
	Transform() : Transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, Alignment::NONE) {}
	Transform(float x, float y, float width, float height, float angle, float scaleX, float scaleY, float offsetX, float offsetY, Alignment alignment) : x(x), y(y), width(width), height(height), angle(angle), scaleX(scaleX), scaleY(scaleY), offsetX(offsetX), offsetY(offsetY), alignment(alignment) {}

	// = operation -> replaces all of this Transform's data with the other
	// Used for replacing one transform with another
	Transform& operator=(const Transform& t) {
		if (this != &t) {
			x = t.x;
			y = t.y;
			width = t.width;
			height = t.height;
			angle = t.angle;
			scaleX = t.scaleX;
			scaleY = t.scaleY;
			offsetX = t.offsetX;
			offsetY = t.offsetY;
			alignment = t.alignment;
		}
		return *this;
	}

	// + operation -> combines all of this Transform's data with another
	// Not really practical for rendering because of how width, height and scale work
	// Might be useful in some way but its function was completely replaced by constrictWithParentTransform()
	Transform operator+(const Transform& t) const {
		return Transform(
			x + t.x,
			y + t.y,
			width + t.width,
			height + t.height,
			angle + t.angle,
			scaleX,
			scaleY,
			offsetX + t.offsetX,
			offsetY + t.offsetY,
			alignment
		);
	}

	// += operation -> same as the +, might be useful but was made redundant by constrictWithParentTransform()
	Transform& operator+=(const Transform& t) {
		x += t.x;
		y += t.y;
		width += t.width;
		height += t.height;
		angle += t.angle;
		//scaleX += t.scaleX;
		//scaleY += t.scaleY;
		offsetX += t.offsetX;
		offsetY += t.offsetY;
		return *this;
	}

	// Combines two transformations together like the +/+= operations but it makes sure the first transform (local) falls within the bounds defined the by second operand (parent)
	// If it has width/height of 0 or less, it won't use width/height to restrict the local transform.
	// Stats like position offsets don't get used in the calculations // Parity with JavaFX
	// Static because it saves memory and doesn't need to be a part of every Transform instance.
	
	// Takes the local transformation of a node and makes it relative to its parent's transform, constricting it within the bounds of the parent node
	// With no alignment input, this version asumes that the node can freely placed anywhere within the parent.
	static Transform constrictWithParentTransform(const Transform& local, const Transform& parent) {
		float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;
		float scaleX = local.scaleX * parent.scaleX;
		float scaleY = local.scaleY * parent.scaleY;
		// The sizes of the child and parent nodes after being scaled
		// The parent node only uses its own scale because the child can't affect it
		// If a node doesn't have a specific alignment and can move freely within its parent node, its position will be affected by the parent's scale
		float localX = local.x * parent.scaleX;
		float localY = local.y * parent.scaleY;
		float localWidth = local.width * scaleX;
		float localHeight = local.height * scaleY;
		float parentWidth = parent.width * parent.scaleX;
		float parentHeight = parent.height * parent.scaleY;
		if (parentWidth < 0) parentWidth = 0;
		if (parentHeight < 0) parentHeight = 0;
		
		// Calculate the node's width and position on the X axis
		if (parentWidth > 0) {
			if (localWidth > 0) {
				// If both the parent and child have a width, use whichever is smaller.
				// The node can't become bigger than its parent without clipping out of the parent's bounds
				width = (localWidth < parentWidth ? local.width : parent.width);
			} else {
				// If the childe node has no width, use the parent's instead, this effectively tells the node that it can fill the same area as the parent if it needs to
				width = parent.width;
			}
			// localX + parent.x gives the node's screen position
			x = localX + parent.x;
			// Because the parent has a width, the node's local position is clamped between 0 and the parent's width // A negative local position doesn't necessarily mean that the node is off screen
			if (localX < 0) {
				x = parent.x;
				localX = 0;
			}
			// Check if the node clips past the parent 
			if (localX + width * scaleX > parentWidth) {
				// If it does then shift its local position back until it's back in bounds
				float tempX = localX - (localX + width * scaleX - parentWidth);
				// If the local position gets pushed back into the negatives, shrink the child node's size.
				// This can only happen if the parent's width is used for the node and the node's local scale is greater than the parents // This forces the node to be shrunk so that its scale cancels itself out
				if (tempX < 0) {
					tempX = 0;
					width = parentWidth / scaleX; // parentWidth because it is the only case that can get here
				}
				x = parent.x + tempX;
			}
		}
		else
		{
			// If the parent has no width, the child node can be positioned anywhere and at any size
			x = localX + parent.x;
			width = local.width;
			if (width < 0) {
				width = 0;
			}
		}
		
		// Calculate the node's height and position on the Y axis
		if (parentHeight > 0) {
			if (localHeight > 0) {
				height = (localHeight < parentHeight ? local.height : parent.height);
			} else {
				height = parent.height;
			}
			y = localY + parent.y;
			if (localY < 0) {
				y = parent.y;
				localY = 0;
			}
			if (localY + height * scaleY > parentHeight) {
				float tempY = localY - (localY + height * scaleY - parentHeight);
				if (tempY < 0) {
					tempY = 0;
					height = parentHeight / scaleY;
				}
				y = parent.y + tempY;
			}
		}
		else
		{
			y = localY + parent.y;
			height = local.height;
			if (height < 0) {
				height = 0;
			}
		}
		
		// Return the result as a new transform
		return Transform(
			x, y, width, height,
			local.angle + parent.angle,
			scaleX,
			scaleY,
			local.offsetX + parent.offsetX,
			local.offsetY + parent.offsetY,
			parent.alignment
		);
	}

	// constrictWithParentTransform() with an alignment input that dictates where the child node should be placed within the parent.
	// With an alignment input, this version asumes that the parent node has fixed bounds for its children to be placed in. // If it doesn't, the function will still try to work with it in some way
	static Transform constrictWithParentTransform(const Transform& local, const Transform& parent, int alignment) {
		float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;
		float scaleX = local.scaleX * parent.scaleX;
		float scaleY = local.scaleY * parent.scaleY;
		// The sizes of the child and parent nodes after being scaled
		// The parent node only uses its own scale because the child can't affect it
		// If a node doesn't have a specific alignment and can move freely within its parent node, its position will be affected by the parent's scale
		float localX = local.x * parent.scaleX;
		float localY = local.y * parent.scaleY;
		float localWidth = local.width * scaleX;
		float localHeight = local.height * scaleY;
		float parentWidth = parent.width * parent.scaleX;
		float parentHeight = parent.height * parent.scaleY;
		if (parentWidth < 0) parentWidth = 0;
		if (parentHeight < 0) parentHeight = 0;
		
		// Calculate the node's width and position on the X axis
		if (parentWidth > 0) {
			if (localWidth > 0) {
				width = (localWidth < parentWidth ? local.width : parent.width);
			} else {
				width = parent.width;
			}
			x = localX + parent.x;
			if (localX < 0) {
				x = parent.x;
				localX = 0;
			}
			if (width * scaleX > parentWidth) {
				width = parentWidth / scaleX;
			}
			if (localX + width * scaleX > parentWidth) {
				x = parent.x + localX - (localX + width * scaleX - parentWidth);
			}
		}
		else
		{
			x = localX + parent.x;
			width = local.width;
			if (width < 0) {
				width = 0;
			}
		}
		
		// Calculate the node's height and position on the Y axis
		if (parentHeight > 0) {
			if (localHeight > 0) {
				height = (localHeight < parentHeight ? local.height : parent.height);
			} else {
				height = parent.height;
			}
			y = localY + parent.y;
			if (localY < 0) {
				y = parent.y;
				localY = 0;
			}
			if (height * scaleY > parentHeight) {
				height = parentHeight / scaleY;
			}
			if (localY + height * scaleY > parentHeight) {
				y = parent.y + localY - (localY + height * scaleY - parentHeight);
			}
		}
		else
		{
			y = localY + parent.y;
			height = local.height;
			if (height < 0) {
				height = 0;
			}
		}

		// There are three types of alignment with their own unique behaviour: positive (>0), none (0), negative (<0)
		// No alignment will have the same effect as running the normal version of this function. Nothing will happen past this point
		if (alignment == 0 || alignment == -1) {
			return Transform(
				x, y, width, height,
				local.angle + parent.angle,
				scaleX,
				scaleY,
				local.offsetX + parent.offsetX,
				local.offsetY + parent.offsetY,
				parent.alignment
			);
		}
		// Positive alignments require the parent to have a fixed size, the child will be placed within the parent at fixed points and their local X/Y values will be ignored
		// - If the parent has no fixed size, local X/Y values will still be ignored but the parent's given position will be treated as an anchor point for the node
		// Negative alignments don't need the parent to have a fixed size, the child node's position within the parent will be used as an anchor point.
		// - If the parent has a fixed size, the child node's position will be adjusted so that it doesn't end up going out of bounds
		if (alignment > 0) {
			// Horizontal alignment
			switch (alignment) {
				// Left
				case 1:
				case 4:
				case 7:
					x = parent.x;
					break;
				// Center
				case 2:
				case 5:
				case 8:
					localX = parentWidth / 2 - (width * scaleX) / 2;
					x = localX + parent.x;
					if (localX + width * scaleX > parentWidth) {
						x = parent.x + localX - (localX + width * scaleX - parentWidth);
					}
					break;
				// Right
				case 3:
				case 6:
				case 9:
					x = parent.x + parentWidth - width * scaleX;
					break;
			}
			// Vertical alignment
			switch (alignment) {
				// Top
				case 1:
				case 2:
				case 3:
					y = parent.y;
					break;
				// Center
				case 4:
				case 5:
				case 6:
					localY = parentHeight / 2 - (height * scaleY) / 2;
					y = localY + parent.y;
					if (localY + height * scaleY > parentHeight) {
						y = parent.y + localY - (localY + height * scaleY - parentHeight);
					}
					break;
				// Bottom
				case 7:
				case 8:
				case 9:
					y = parent.y + parentHeight - height * scaleY;
					break;
			}
		}
		else
		{
			// Horizontal alignment
			switch (-alignment) {
				// Left - Nothing needs to be done
				// Center
				case 2:
				case 5:
				case 8:
					localX -= (width * scaleX) / 2;
					x = parent.x + localX;
					if (parentWidth > 0 && localX > parentWidth) {
						x = parent.x + localX - (localX - parentWidth);
					}
					if (parentWidth > 0 && x < parent.x) {
						x = parent.x;
					}
					break;
				// Right
				case 3:
				case 6:
				case 9:
					x = parent.x + localX - width * scaleX;
					if (parentWidth > 0 && x < parent.x) {
						x = parent.x;
					}
					break;
			}

			// Vertical alignment
			switch (-alignment) {
				// Top - Nothing needs to be done
				// Center
				case 4:
				case 5:
				case 6:
					localY -= (height * scaleY) / 2;
					y = parent.y + localY;
					if (parentHeight > 0 && localY > parentHeight) {
						y = parent.y + localY - (localY - parentHeight);
					}
					if (parentHeight > 0 && y < parent.y) {
						y = parent.y;
					}
					break;
				// Bottom
				case 7:
				case 8:
				case 9:
					y = parent.y + localY - height * scaleY;
					if (parentHeight > 0 && y < parent.y) {
						y = parent.y;
					}
					break;
			}
		}

		// Return the result as a new transform
		return Transform(
			x, y, width, height,
			local.angle + parent.angle,
			scaleX,
			scaleY,
			local.offsetX + parent.offsetX,
			local.offsetY + parent.offsetY,
			parent.alignment
		);
	}
};


class Node : public std::enable_shared_from_this<Node> {
	friend class Parent;
	private:
		// A pointer to the node's parent, if it has one.
		std::shared_ptr<Parent> parent = nullptr;
	protected:
		Alignment alignment = TOP_LEFT;
		Transform transform; // local transformation of the node - How the node positions itself
		Transform globalTransform; // global transformation of the node - How the world/parent position the node
		// When combinding the local and global transformations, you get the transformation used for the node and its children, what they get placed relative to
		// The globalTransform variable doesn't need to exist but it might be useful to have it as an option for other developers
		Transform screenTransform; // The actual position and size of the node on screen, mostly tracked for scene events but it can also be used by the node's parent for various purposes

		// Combines the local transform of the node with the global transform of the parent to get the node's position on the screen. For more details check the Transform class
		Transform* calcScreenTransform() { return &(this->screenTransform = Transform::constrictWithParentTransform(this->transform, this->globalTransform)); }
		Transform* calcScreenTransform(Alignment alignment) { return &(this->screenTransform = Transform::constrictWithParentTransform(this->transform, this->globalTransform, alignment)); }

		// Tracks if a node is being focused on. Mainly used for tracking if the node was clicked and dragged on.
		bool focused = false; 

		// Tracks if the node is visible // Invisible nodes can't render themselves or their children, this also applies for running events
		bool visible = true;
	public:
		// Basic costructors for the node class
		Node();
		Node(float x, float y);
		Node(float x, float y, float width, float height);
		Node(float x, float y, float width, float height, float offsetX, float offsetY);

		int getAlignment() { return this->alignment; }
		void setAlignment(Alignment alignment) { this->alignment = alignment; }
		void setAlignment(int alignment) { this->alignment = static_cast<Alignment>(alignment); }

		// Local transformation of the node
		Transform* getLocalTransform() { return &this->transform; }
		Transform* getGlobalTransform() { return &this->globalTransform; }
		Transform* getScreenTransform() { return &this->screenTransform; }

		float getX() { return this->transform.x; }
		float getY() { return this->transform.y; }
		void setX(float x) { this->transform.x = x; }
		void setY(float y) { this->transform.y = y; }

		float getOffsetX() { return this->transform.offsetX; }
		float getOffsetY() { return this->transform.offsetY; }
		void setOffsetX(float offsetX) { this->transform.offsetX = offsetX; }
		void setOffsetY(float offsetY) { this->transform.offsetY = offsetY; }

		float getWidth() { return this->transform.width; }
		float getHeight() { return this->transform.height; }
		void setWidth(float width) { this->transform.width = width; }
		void setHeight(float height) { this->transform.height = height; }

		float getScaleX() { return this->transform.scaleX; }
		float getScaleY() { return this->transform.scaleY; }
		void setScaleX(float scaleX) { this->transform.scaleX = scaleX; }
		void setScaleY(float scaleY) { this->transform.scaleY = scaleY; }
		void setScale(float scale) {
			this->transform.scaleX = scale;
			this->transform.scaleY = scale;
		}
		void setScale(float scaleX, float scaleY) {
			this->transform.scaleX = scaleX;
			this->transform.scaleY = scaleY;
		}

		float getAngle() { return this->transform.angle; }
		void setAngle(float angle) { this->transform.angle = angle; }
		// Rotate the node clockwise in degrees (increments and returns the angle)
		float rotate(float angle) { return (this->transform.angle += angle); }

		bool isFocused() { return this->focused; }
		void setFocused(bool focused) { this->focused = focused; }

		bool isVisible() { return this->visible; }
		void setVisible(bool visible) { this->visible = visible; if (!visible) this->focused = false; }

		Point2 getPos() { return Point2(this->transform.x, this->transform.y); }
		void setPos(Point2 position) { this->transform.x = position.x; this->transform.y = position.y; };
		
		std::shared_ptr<Parent> getParent() { return parent; }

		// Render relative to the screen
		virtual void render(std::shared_ptr<GraphicsEngine> gfx);
		// Render relative to (parent) transform
		virtual void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
		virtual void unload(); // Delete yourself from memory and the scene
		
		// Events related to the mouse
		// No keyboard input events were added for the demo but the updated EventEngine has the needed support for it.

		std::function<void()> onMouseHoverOn;
		std::function<void()> onMouseHoverOff;
		std::function<void(Mouse)> onMouseClicked;
		std::function<void(Mouse)> onMouseReleased;
		std::function<void(Mouse)> onMousePressed;
};
// Node note - Rendering is handled within the node subclasses instead of the rendering engine so the way that each node is rendered is completely customisable for the person defining a node for the scene graph. // We don't need to give nodes a built in SDL_Rect or anything else since they can be given to the subclasses if they are needed.

class Parent : public Node {
	private:
		std::vector<std::shared_ptr<Node>> children;
	protected:
		// The total number of children the parent is intended to support
		// 0 - None
		// -1 - Unlimited
		int childrenLimit = -1;

		// Renders each child within the parent, should only be ran if the parent's screen transform was already calculated
		void renderChildren(std::shared_ptr<GraphicsEngine> gfx);
		void renderChildren(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
	public:
		Parent();
		
		std::vector<std::shared_ptr<Node>> getChildren() { return children; }
		virtual void addChild(std::shared_ptr<Node> child);
		virtual void addChildren(std::vector<std::shared_ptr<Node>> children);
		virtual void removeChild(std::shared_ptr<Node> child);
		virtual void setChildren(std::vector<std::shared_ptr<Node>> children);
		virtual void setChild(std::shared_ptr<Node> child);
		virtual std::shared_ptr<Node> getChild();
		virtual std::shared_ptr<Node> getChild(int position);

		virtual void render(std::shared_ptr<GraphicsEngine> gfx);
		virtual void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
		

		void unload();
		void unloadChildren();
};



// Rectangle specifically designed to support mouse events
class Button : public Node {
	private:
		SDL_Rect button = {0, 0, 200, 100};
		SDL_Color colour = SDL_COLOR_BLUE;
		std::string buttonText = "";
		SDL_Color textColour = SDL_COLOR_BLACK;
	public:
		Button();
		Button(SDL_Rect rect);
		Button(Rect rect);
		Button(SDL_Rect rect, SDL_Color colur);
		Button(Rect rect, SDL_Color colur);
		Button(float x, float y, float width, float height);
		Button(float x, float y, float width, float height, SDL_Color colur);

		SDL_Color getButtonColour() { return this->colour; }
		void setButtonColour(SDL_Color colour) { this->colour = colour; }

		std::string getText() { return this->buttonText; }
		void setText(std::string text) { this->buttonText = text; }

		SDL_Color getTextColour() { return this->textColour; }
		void setTextColour(SDL_Color colour) { this->textColour = colour; }

		void render(std::shared_ptr<GraphicsEngine> gfx);
		void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
};

// Stores its children horizontally side by side, with a set gap in between them
class HBox : public Parent {
	private:
		float nodeGap = 10; // Distance between child nodes
		
	public:
		HBox();
		HBox(std::vector<std::shared_ptr<Node>> children);
		HBox(float gap);
		HBox(float gap, std::vector<std::shared_ptr<Node>> children);

		void render(std::shared_ptr<GraphicsEngine> gfx);
		void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
};

// Stores its children vertically top to bottom, with a set gap in between them
class VBox : public Parent {
	private:
		float nodeGap = 10; // Distance between child nodes

	public:
		VBox();
		VBox(std::vector<std::shared_ptr<Node>> children);
		VBox(float gap);
		VBox(float gap, std::vector<std::shared_ptr<Node>> children);

		void render(std::shared_ptr<GraphicsEngine> gfx);
		void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
};

// A node designed to store a single child and align it within its own bounds
// It is required to have a set width/height
class Frame : public Parent {
	private:
	protected:
		Transform transform = Transform(0, 0, 100, 100, 0, 1, 1, 0, 0, alignment);
		SDL_Color backgroundColour = { 0, 0, 0, 0 };
		int childrenLimit = 1;
	public:
		Frame();
		Frame(SDL_Color backgroundColour);
		Frame(int alignment);
		Frame(int alignment, SDL_Color backgroundColour);
		Frame(float width, float height);
		Frame(float width, float height, SDL_Color backgroundColour);
		Frame(float width, float height, int alignment);
		Frame(float width, float height, int alignment, SDL_Color backgroundColour);
		Frame(std::shared_ptr<Node> child);
		Frame(std::shared_ptr<Node> child, SDL_Color backgroundColour);
		Frame(std::shared_ptr<Node> child, int alignment);
		Frame(std::shared_ptr<Node> child, int alignment, SDL_Color backgroundColour);
		Frame(float width, float height, std::shared_ptr<Node> child);
		Frame(float width, float height, std::shared_ptr<Node> child, SDL_Color backgroundColour);
		Frame(float width, float height, std::shared_ptr<Node> child, int alignment);
		Frame(float width, float height, std::shared_ptr<Node> child, int alignment, SDL_Color backgroundColour);

		void setChildren(std::vector<std::shared_ptr<Node>> children) { this->setChild(children.at(0)); }

		void setBackgroundColour(SDL_Color colour) { this->backgroundColour = colour; this->backgroundColour.a = 255; }
		SDL_Color getBackgroundColour() { return this->backgroundColour; }
		void removeBackgroundColour() { this->backgroundColour = { 0, 0, 0, 0 }; }

		void render(std::shared_ptr<GraphicsEngine> gfx);
		void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
};

class Pane : public Parent {
	private:
	protected:
	public:
		Pane();
		Pane(std::vector<std::shared_ptr<Node>> children);

		void render(std::shared_ptr<GraphicsEngine> gfx);
		void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
};

class Label : public Node {
	private:
		std::string text = "";
		SDL_Color colour = SDL_COLOR_BLACK;
	protected:
	public:
		Label();
		Label(float x, float y);
		Label(std::string text);
		Label(float x, float y, std::string text);
		Label(std::string text, SDL_Color colour);
		Label(float x, float y, std::string text, SDL_Color colour);

		std::string getText() { return this->text; }
		void setText(std::string text) { this->text = text; }

		SDL_Color getTextColour() { return this->colour; }
		void setTextColour(SDL_Color colour) { this->colour = colour; }

		void render(std::shared_ptr<GraphicsEngine> gfx);
		void render(std::shared_ptr<GraphicsEngine> gfx, Transform& transform);
};

#endif