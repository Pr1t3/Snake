#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <vector>
#include <array>
#include <cmath>
#include <sstream>

const int width = 20*25;
const int height = 20*25;

class FallingBox : public sf::RectangleShape {
private:
	sf::RectangleShape box;
	float timer;
	float delay;
public:
	FallingBox(int x):timer(0), delay(0.01), box(sf::Vector2f(25,75)) {
		box.setOutlineThickness(1);
		box.setOutlineColor(sf::Color::Black);
		box.setFillColor(sf::Color::White);
		box.setPosition(x, 100);
	}
	sf::RectangleShape getBox() {
		return box;
	}
	sf::Vector2f getPosition() {
		return box.getPosition();
	}
	sf::Vector2f getSize() {
		return box.getSize();
	}
	void fall(float y) {
		box.move(0, y);
	}
	float getTimer() {
		return timer;
	}
	float getDelay() {
		return delay;
	}
	void setTimer(float time) {
		timer = time;
	}
	sf::FloatRect getGlobalBounds() {
		return box.getGlobalBounds();
	}
};
float randomSpeedcircle();
float randomSpeedcircle(float circleSpeedX, float circleSpeedY, bool isX);
sf::Vector2f randomPosFood(std::vector<sf::RectangleShape>& snake, const int size);
sf::Vector2f randomPosCircle(std::vector<sf::RectangleShape>& snake, const int sizeCircle);
void eatingFood(std::vector<sf::RectangleShape>& snake, sf::RectangleShape& food, int& score);
float randomPosFallingBox(std::vector<FallingBox>& fallingBoxes) {
	int pos = 50 + (rand() % (width / 25)) * width / 20;
	bool ok = true;
	for (int i = 0; i < fallingBoxes.size(); i++) {
		if (fallingBoxes[i].getPosition().x == pos) {
			ok = false;
		}
	}
	if (fallingBoxes.size() == 0 || ok == true) {
		return pos;
	}
	return randomPosFallingBox(fallingBoxes);
}
void moveSnake(float speedX, float speedY, double& timerMoveSnake, const double delayMoveSnake, std::vector<sf::RectangleShape>& snake, bool& gameOver, sf::RenderWindow& window, bool& isGameStarted, sf::CircleShape &circle, sf::RectangleShape &food, int &score) {
	if (speedX != 0 or speedY != 0) {
		if (isGameStarted == false) {
			isGameStarted = true;
		}
		if (timerMoveSnake > delayMoveSnake) {
			std::vector<sf::RectangleShape> helpSnake;
			for (int i = 0; i < snake.size(); i++) {
				if (snake[0].getPosition().x < 50 || snake[0].getPosition().x > 49 + width || snake[0].getPosition().y < 100 || snake[0].getPosition().y > 99 + height) {
					gameOver = true;
				}
				if (snake[0].getPosition().x == snake[i].getPosition().x && snake[0].getPosition().y == snake[i].getPosition().y && i != 0) {
					gameOver = true;
				}
				helpSnake.push_back(snake[i]);
				if (i == 0) {
					snake[i].move(speedX, speedY);
				}
				else {
					snake[i] = helpSnake[i - 1];

				}
			}
			timerMoveSnake = 0;
		}
	}
	for (int i = 0; i < snake.size(); i++) {
		window.draw(snake[i]);
	}
	
}
void moveCircle(double& timerMovecircle, const double delayMovecircle, sf::CircleShape& circle, std::vector<sf::RectangleShape>& snake, float& circleSpeedX, float& circleSpeedY, float& prevcircleSpeedX, float& prevcircleSpeedY, bool isGameStarted, bool &gameOver) {
	if (isGameStarted == true) {
		sf::Vector2f circleCenter(circle.getPosition().x + circle.getRadius() / 2, circle.getPosition().y + circle.getRadius() / 2);
		if (timerMovecircle > delayMovecircle) {			
			if (circleSpeedX == 0 && circleSpeedY == 0) {
				circleSpeedX = randomSpeedcircle();
				circleSpeedY = randomSpeedcircle();
			}
			if (circleCenter.x <= 50 + circle.getRadius() / 2) {
				float newSpeed = randomSpeedcircle(circleSpeedX, circleSpeedY, false);
				circleSpeedX = -circleSpeedX;
				circleSpeedY = newSpeed;
			}
			if (circleCenter.x >= 30 + width - circle.getRadius() ) {
				float newSpeed = randomSpeedcircle(circleSpeedX, circleSpeedY, false);
				circleSpeedX = -circleSpeedX;
				circleSpeedY = newSpeed;
			}
			if (circleCenter.y <= 100 + circle.getRadius() / 2) {
				float newSpeed = randomSpeedcircle(circleSpeedX, circleSpeedY, true);
				circleSpeedY = -circleSpeedY;
				circleSpeedX = newSpeed;
			}
			if (circleCenter.y >= 80 + height - circle.getRadius()) {
				float newSpeed = randomSpeedcircle(circleSpeedX, circleSpeedY, true);
				circleSpeedY = -circleSpeedY;
				circleSpeedX = newSpeed;
			}
			circle.move(circleSpeedX, circleSpeedY);
			timerMovecircle = 0;
		}
	}
}
void fallBoxes(std::vector<FallingBox> &fallingBoxes, sf::RenderWindow& window, double &timerBetweenFalling, double delayBetweenFalling, bool &isFallingFirst, bool gameOver) {
	if (gameOver == false) {
		if (timerBetweenFalling > delayBetweenFalling) {
			if (isFallingFirst == true) {
				for (int i = 0; i < 3; i++) {
					float pos = randomPosFallingBox(fallingBoxes);
					FallingBox box(pos);
					fallingBoxes.push_back(box);
				}
				isFallingFirst = false;
			}
			for (int i = 0; i < fallingBoxes.size(); i++) {
				if (fallingBoxes[i].getTimer() > fallingBoxes[i].getDelay()) {
					fallingBoxes[i].fall(3);
					fallingBoxes[i].setTimer(0);
				}
				if (fallingBoxes[i].getPosition().y >= height - static_cast<float>(1) / 2 * fallingBoxes[i].getBox().getSize().y + 25) {
					fallingBoxes.erase(fallingBoxes.begin() + i);
				}
			}
			if (fallingBoxes.size() == 0) {
				timerBetweenFalling = 0;
				isFallingFirst = true;
			}
		}
		for (int i = 0; i < fallingBoxes.size(); i++) {
			window.draw(fallingBoxes[i].getBox());
		}
	}
}
float randomSpeedcircle() {
	float speed = (rand() % 400) / 100.0 - 2;
	if ((speed > 1.6 && speed < 1.8) || (speed < -1.6 && speed > -1.8)) {
		return speed;
	}
	return randomSpeedcircle();

}
float randomSpeedcircle(float circleSpeedX, float circleSpeedY, bool isX) {
	float speed = (rand() % 400) / 100.0 - 2;
	if ((speed > 1.5 && speed < 2) || (speed < -1.5 && speed > -2)) {
		if (isX == true) {
			if (speed > 0 && circleSpeedX > 0 || speed < 0 && circleSpeedX < 0) {
				return speed;
			}
		}
		else {
			if (speed > 0 && circleSpeedY > 0 || speed < 0 && circleSpeedY < 0) {
				return speed;
			}
		}
	}
	return randomSpeedcircle(circleSpeedX, circleSpeedY, isX);
}
sf::Vector2f randomPosCircle(std::vector<sf::RectangleShape>& snake, const int sizeCircle) {
	float posX = (rand() % width);
	float posY = (rand() % height);
	for (int i = 0; i < snake.size(); i++) {
		sf::Vector2f posSnakeI(snake[i].getPosition().x + static_cast<float>(25) / 2, snake[i].getPosition().y + static_cast<float>(25) / 2);
		float differenceX = (posX - posSnakeI.x) < 0 ? (posX - posSnakeI.x) * -1 : (posX - posSnakeI.x);
		float differenceY = (posY - posSnakeI.y) < 0 ? (posY - posSnakeI.y) * -1 : (posY - posSnakeI.y);
		if (differenceX > static_cast<float>(25) / 2 + sizeCircle) {
			if (differenceY > static_cast<float>(25) / 2 + sizeCircle) {
				if (posX > 50 + sizeCircle && posX < 50 + width - sizeCircle*2 && posY > 100 + sizeCircle && posY < 100 + height - sizeCircle*2) {
					return sf::Vector2f(posX, posY);
				}
			}
		}
	}
	return randomPosCircle(snake, sizeCircle);
}
sf::Vector2f randomPosFood(std::vector<sf::RectangleShape>& snake, const int size) {
	int posX = (rand() % (width/25)) * width/20;
	int posY = (rand() % (height / 25)) * height/20;
	for (int i = 0; i < snake.size(); i++) {
		sf::Vector2f posSnakeI(snake[i].getPosition().x + static_cast<float>(25) / 2, snake[i].getPosition().y + static_cast<float>(25) / 2);
		float differenceX = (posX - posSnakeI.x) < 0 ? (posX - posSnakeI.x) * -1 : (posX - posSnakeI.x);
		float differenceY = (posY - posSnakeI.y) < 0 ? (posY - posSnakeI.y) * -1 : (posY - posSnakeI.y);
		if (differenceX > static_cast<float>(25) / 2 + size) {
			if (differenceY > static_cast<float>(25) / 2 + size) {
				if (posX > 50 && posX < width-size && posY > 100 && posY < height-size) {
					return sf::Vector2f(posX, posY);
				}
			}
		}
	}
	return randomPosFood(snake, size);
}
void eatingFood(std::vector<sf::RectangleShape> &snake, sf::RectangleShape &food, int &score) {
	if (snake[0].getPosition().x == food.getPosition().x && snake[0].getPosition().y == food.getPosition().y) {
		score += 100;
		food.setPosition(randomPosFood(snake, food.getSize().x));
	}	
}
void checkCollisions(sf::CircleShape &circle, std::vector<sf::RectangleShape> &snake, std::vector<FallingBox> fallingBoxes, bool &isGameOver) {
	sf::Vector2f circleCenter(circle.getPosition().x + circle.getRadius(), circle.getPosition().y + circle.getRadius());
	for (int i = 0; i < snake.size(); i++) {
		sf::Vector2f snakeCenter(snake[i].getPosition().x + snake[i].getSize().x / 2, snake[i].getPosition().y + snake[i].getSize().y / 2);
		float differenceXCircle = (snakeCenter.x - circleCenter.x) < 0 ? (snakeCenter.x - circleCenter.x)*-1 : (snakeCenter.x - circleCenter.x);
		float differenceYCircle = (snakeCenter.y - circleCenter.y) < 0 ? (snakeCenter.y - circleCenter.y)*-1 : (snakeCenter.y - circleCenter.y);
		if (differenceXCircle <= circle.getRadius() && differenceYCircle <= circle.getRadius()) {
			isGameOver = true;
		}
		for (int j = 0; j < fallingBoxes.size(); j++) {
			sf::Vector2f boxCenter(fallingBoxes[j].getPosition().x + fallingBoxes[j].getSize().x / 2, fallingBoxes[j].getPosition().y + fallingBoxes[j].getSize().y / 2);
			float differenceXBox = (snakeCenter.x - boxCenter.x) < 0 ? (snakeCenter.x - boxCenter.x)*-1 : (snakeCenter.x - boxCenter.x);
			float differenceYBox = (snakeCenter.y - boxCenter.y) < 0 ? (snakeCenter.y - boxCenter.y)*-1 : (snakeCenter.y - boxCenter.y);
			if (differenceXBox <= fallingBoxes[j].getSize().x / 2 + snake[i].getSize().x / 2 - 5 && differenceYBox <= fallingBoxes[j].getSize().y / 2 + snake[i].getSize().y / 2 - 5) {
				isGameOver = true;
			}
		}
	}
}
void timers(sf::Clock& clock, double& timerMoveSnake, double& timerPause, double& timerUseButtons, double& timerMovecircle, std::vector<FallingBox> &fallingBoxes, double &timerBetweenFalling, bool isGameStarted, bool isGameOver) {
	float time = clock.getElapsedTime().asSeconds();
	clock.restart();
	timerUseButtons += time;
	if (isGameStarted == true && isGameOver == false) {		
		timerMoveSnake += time;
		timerPause += time;		
		timerMovecircle += time;
		timerBetweenFalling += time;
		for (int i = 0; i < fallingBoxes.size(); i++) {
			fallingBoxes[i].setTimer(time + fallingBoxes[i].getTimer());
		}
	}	
}
void drawGrid(sf::RenderWindow &window) {
	for (int i = 0; i < width/25; i++) {
		for (int j = 0; j < height/25; j++) {
			sf::RectangleShape rect(sf::Vector2f(25.0f, 25.0f));
			rect.setFillColor(sf::Color(0, 0, 0, 0));
			rect.setOutlineThickness(1.0f);
			rect.setOutlineColor(sf::Color::White);
			rect.setPosition(50 + 25 * i, 100 + 25 * j);
			window.draw(rect);
		}
	}
}
void keyboardProcessing(sf::Event event, sf::RenderWindow &window, float &snakeSpeedX, float& snakeSpeedY, float snakeSpeed, double &timerUseButtons, const float delayUseButtons, double &timerPause, const int delayPause, bool &pause) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	if (event.key.code == sf::Keyboard::Up && snakeSpeedY != snakeSpeed && timerUseButtons > delayUseButtons) {
		snakeSpeedX = 0;
		snakeSpeedY = -snakeSpeed;
		timerUseButtons = 0;
	}
	if (event.key.code == sf::Keyboard::Down && snakeSpeedY != -snakeSpeed && timerUseButtons > delayUseButtons) {
		snakeSpeedX = 0;
		snakeSpeedY = snakeSpeed;
		timerUseButtons = 0;
	}
	if (event.key.code == sf::Keyboard::Right && snakeSpeedX != -snakeSpeed && timerUseButtons > delayUseButtons && (snakeSpeedX != 0 or snakeSpeedY != 0)) {
		snakeSpeedX = snakeSpeed;
		snakeSpeedY = 0;
		timerUseButtons = 0;
	}
	if (event.key.code == sf::Keyboard::Left && snakeSpeedX != snakeSpeed && timerUseButtons > delayUseButtons) {
		snakeSpeedX = -snakeSpeed;
		snakeSpeedY = 0;
		timerUseButtons = 0;
	}
	if (event.key.code == sf::Keyboard::Escape && timerPause > delayPause) {
		if (pause == false) {
			pause = true;
		}
		else {
			pause = false;
		}
		timerPause = 0;
	}
}
void initText(sf::Text &text, float xPos, float yPos, int sizeFont, sf::String string, sf::Color textColor, sf::RenderWindow& window, sf::Font& font) {
	text.setFont(font);
	text.setCharacterSize(sizeFont);
	text.setPosition(xPos, yPos);
	text.setString(string);
	text.setFillColor(textColor);
	window.draw(text);
}
void displayText(int score, float xPosScore, float yPosScore, int sizeScore, sf::Text& textScore, double timerBetweenFalling, double delayBetweenFalling, float xPosTimerFallingBoxes, float yPosTimerFallingBoxes, float sizeTimerFallingBoxes, sf::Text &textTimerFallingBoxes, bool isGameOver, sf::Text &textGameOver, sf::Text &textPlayAgain, sf::Text& textYes, sf::Text& textNo, sf::RenderWindow &window, sf::Font& font) {
	std::ostringstream scoreString;
	scoreString << score;
	if (isGameOver == true) {
		initText(textGameOver, 115, 175.5, 35, "Game Over", sf::Color::Red, window, font);
		initText(textPlayAgain, 140, 260, 25, "Play again?", sf::Color::White, window, font);
		initText(textYes, 175, 335, 20, "Yes", sf::Color::White, window, font);
		initText(textNo, 375, 335, 20, "No", sf::Color::White, window, font);
		initText(textScore, 185, 600, 25, "Score: " + scoreString.str(), sf::Color::Yellow, window, font);
	} else {
		initText(textScore, xPosScore, yPosScore, sizeScore, "Score: " + scoreString.str(), sf::Color::Green, window, font);
		std::ostringstream timerFallingBoxesString;
		delayBetweenFalling - timerBetweenFalling < 0 ? timerFallingBoxesString << 0 : timerFallingBoxesString << static_cast<int>(delayBetweenFalling - timerBetweenFalling);
		initText(textTimerFallingBoxes, xPosTimerFallingBoxes, yPosTimerFallingBoxes, sizeTimerFallingBoxes, timerFallingBoxesString.str() + " Left", sf::Color::Red, window, font);
		window.draw(textTimerFallingBoxes);
		window.draw(textScore);
	}
	
}
int checkAgainGame(sf::RenderWindow &window, bool isGameOver) {
	if (isGameOver == true) {
		if (sf::IntRect(170, 335, 75, 25).contains(sf::Mouse::getPosition(window)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			return 1;
		}
		if (sf::IntRect(370, 335, 50, 25).contains(sf::Mouse::getPosition(window)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			return -1;
		}
	}
	return 0;
}

void game(sf::RenderWindow &window) {
	using namespace sf;

	Clock clock;
	srand(time(0));

	Font font;
	font.loadFromFile("../resources/QuinqueFive.ttf");
	Text textScore;
	Text textTimerFallingBoxes;
	Text textGameOver;
	Text textPlayAgain;
	Text textYes;
	Text textNo;

	int snakeSize = 25;
	float snakeSpeed = 25;
	float snakeSpeedX = 0;
	float snakeSpeedY = 0;
	RectangleShape s1(Vector2f(snakeSize, snakeSize));
	RectangleShape s2(Vector2f(snakeSize, snakeSize));
	RectangleShape s3(Vector2f(snakeSize, snakeSize));
	std::vector<RectangleShape> snake = {s1, s2, s3};
	for (int i = 0; i < snake.size(); i++) {
		snake[i].setPosition(150 + i * snakeSize, 150);
		snake[i].setOutlineThickness(0.5);
		snake[i].setOutlineColor(sf::Color::Black);
		snake[i].setFillColor(sf::Color(0, 175, 0));
	}

	const int sizeCircle = 48;
	float circleSpeedX = 0;
	float circleSpeedY = 0;
	float prevCircleSpeedX;
	float prevCircleSpeedY;
	CircleShape circle(sizeCircle);
	Vector2f circlePos = randomPosCircle(snake, sizeCircle);
	circle.setPosition(circlePos.x, circlePos.y);
	circle.setFillColor(Color::White);

	std::vector<FallingBox> fallingBoxes;

	int foodSize = 25;
	RectangleShape food(Vector2f(foodSize, foodSize));
	food.setFillColor(Color::Red);
	food.setOutlineColor(Color::Black);
	food.setOutlineThickness(0.5);
	food.setPosition(randomPosFood(snake, foodSize));
	int score = 0;

	double timerMoveSnake = 0;
	double delayMoveSnake = 0.15;
	double timerPause = 0;
	int delayPause = 3;
	double timerUseButtons = 0;
	double delayUseButtons = 0.16;
	double timerMoveCircle = 0;
	double delayMoveCircle = 0.01;
	double delayBetweenFalling = 4;
	double timerBetweenFalling = 0;

	bool isGameOver = false;
	bool pause = false;
	bool isGameStarted = false;
	bool isFallingFirst = true;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			keyboardProcessing(event, window, snakeSpeedX, snakeSpeedY, snakeSpeed, timerUseButtons, delayUseButtons, timerPause, delayPause, pause);
		}
		window.clear(Color::Black);
		if (isGameOver == false && pause == false) {
			drawGrid(window);
			window.draw(food);
			moveCircle(timerMoveCircle, delayMoveCircle, circle, snake, circleSpeedX, circleSpeedY, prevCircleSpeedX, prevCircleSpeedY, isGameStarted, isGameOver);
			moveSnake(snakeSpeedX, snakeSpeedY, timerMoveSnake, delayMoveSnake, snake, isGameOver, window, isGameStarted, circle, food, score);
			fallBoxes(fallingBoxes, window, timerBetweenFalling, delayBetweenFalling, isFallingFirst, isGameOver);
			checkCollisions(circle, snake, fallingBoxes, isGameOver);
			eatingFood(snake, food, score);
			window.draw(circle);			
		} else if (isGameOver == true) {
			if (checkAgainGame(window, isGameOver) == 1) {
				game(window);
			} else if(checkAgainGame(window, isGameOver) == -1) {
				window.close();
			}
		}
		displayText(score, 400, 50, 15, textScore, timerBetweenFalling, delayBetweenFalling, 50, 50, 15, textTimerFallingBoxes, isGameOver, textGameOver, textPlayAgain, textYes, textNo, window, font);
		timers(clock, timerMoveSnake, timerPause, timerUseButtons, timerMoveCircle, fallingBoxes, timerBetweenFalling, isGameStarted, isGameOver);
		window.display();
	}
}

int main() {
	using namespace sf;
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(600, 650), "Snake", Style::Default, settings);
	game(window);
}
