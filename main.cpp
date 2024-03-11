#include "SnakeLogic.h"

int main() {
	using namespace sf;
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(600, 650), "Snake", Style::Default, settings);
	GameManagement(window);
}
