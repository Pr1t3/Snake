#include "SnakeLogic.h"

FallingBox::FallingBox(int x):timer(0), delay(0.01), box(sf::Vector2f(25,75)) {
    box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color::Black);
	box.setFillColor(sf::Color::White);
	box.setPosition(x, 100);
}
sf::RectangleShape FallingBox::GetBox() {
	return box;
}
sf::Vector2f FallingBox::GetPosition() {
	return box.getPosition();
}
sf::Vector2f FallingBox::GetSize() {
	return box.getSize();
}
void FallingBox::Fall(float y) {
	box.move(0, y);
}
float FallingBox::GetTimer() {
	return timer;
}
float FallingBox::GetDelay() {
	return delay;
}
void FallingBox::SetTimer(float time) {
	timer = time;
}

float RandomPosFallingBox(std::vector<FallingBox>& falling_boxes) {
	int pos = 50 + (rand() % (width / 25)) * width / 20;
	bool ok = true;
	for (int i = 0; i < falling_boxes.size(); i++) {
		if (falling_boxes[i].GetPosition().x == pos) {
			ok = false;
		}
	}
	if (falling_boxes.size() == 0 || ok == true) {
		return pos;
	}
	return RandomPosFallingBox(falling_boxes);
}

void MoveSnake(float speedX, float speedY, double& time_move_snake, const double delay_move_snake, std::vector<sf::RectangleShape>& snake, bool& is_game_over, sf::RenderWindow& window, bool& is_game_started, sf::CircleShape &circle, sf::RectangleShape &food, int &score) {
	if (speedX != 0 or speedY != 0) {
		if (is_game_started == false) {
			is_game_started = true;
		}
		if (time_move_snake > delay_move_snake) {
			std::vector<sf::RectangleShape> temp;
			for (int i = 0; i < snake.size(); i++) {
				if (snake[0].getPosition().x < 50 || snake[0].getPosition().x > 49 + width || snake[0].getPosition().y < 100 || snake[0].getPosition().y > 99 + height) {
					is_game_over = true;
				}
				if (snake[0].getPosition().x == snake[i].getPosition().x && snake[0].getPosition().y == snake[i].getPosition().y && i != 0) {
					is_game_over = true;
				}
				temp.push_back(snake[i]);
				if (i == 0) {
					snake[i].move(speedX, speedY);
				}
				else {
					snake[i] = temp[i - 1];

				}
			}
			time_move_snake = 0;
		}
	}
	for (int i = 0; i < snake.size(); i++) {
		window.draw(snake[i]);
	}

}

void MoveCircle(double& timerMovecircle, const double delayMovecircle, sf::CircleShape& circle, std::vector<sf::RectangleShape>& snake, float& circle_speed_x, float& circle_speed_y, float& prevcircleSpeedX, float& prevcircleSpeedY, bool is_game_started, bool &is_game_over) {
	if (is_game_started) {
		sf::Vector2f circle_center(circle.getPosition().x + circle.getRadius() / 2, circle.getPosition().y + circle.getRadius() / 2);
		if (timerMovecircle > delayMovecircle) {
			if (circle_speed_x == 0 && circle_speed_y == 0) {
				circle_speed_x = RandomSpeedCircle();
				circle_speed_y = RandomSpeedCircle();
			}
			if (circle_center.x <= 50 + circle.getRadius() / 2) {
				float new_speed = RandomSpeedCircle(circle_speed_x, circle_speed_y, false);
				circle_speed_x = -circle_speed_x;
				circle_speed_y = new_speed;
			}
			if (circle_center.x >= 30 + width - circle.getRadius() ) {
				float new_speed = RandomSpeedCircle(circle_speed_x, circle_speed_y, false);
				circle_speed_x = -circle_speed_x;
				circle_speed_y = new_speed;
			}
			if (circle_center.y <= 100 + circle.getRadius() / 2) {
				float new_speed = RandomSpeedCircle(circle_speed_x, circle_speed_y, true);
				circle_speed_y = -circle_speed_y;
				circle_speed_x = new_speed;
			}
			if (circle_center.y >= 80 + height - circle.getRadius()) {
				float new_speed = RandomSpeedCircle(circle_speed_x, circle_speed_y, true);
				circle_speed_y = -circle_speed_y;
				circle_speed_x = new_speed;
			}
			circle.move(circle_speed_x, circle_speed_y);
			timerMovecircle = 0;
		}
	}
}

void FallBoxes(std::vector<FallingBox> &falling_boxes, sf::RenderWindow& window, double &timer_between_falling, double delay_between_falling, bool &is_falling_first, bool is_game_over) {
	if (!is_game_over) {
		if (timer_between_falling > delay_between_falling) {
			if (is_falling_first) {
				for (int i = 0; i < 3; i++) {
					float pos = RandomPosFallingBox(falling_boxes);
					FallingBox box(pos);
					falling_boxes.push_back(box);
				}
				is_falling_first = false;
			}
			for (int i = 0; i < falling_boxes.size(); i++) {
				if (falling_boxes[i].GetTimer() > falling_boxes[i].GetDelay()) {
					falling_boxes[i].Fall(3);
					falling_boxes[i].SetTimer(0);
				}
				if (falling_boxes[i].GetPosition().y - (falling_boxes[i].GetBox().getSize().y - 25)/2.0 >= height) {
					falling_boxes.erase(falling_boxes.begin() + i);
				}
			}
			if (falling_boxes.size() == 0) {
				timer_between_falling = 0;
				is_falling_first = true;
			}
		}
		for (int i = 0; i < falling_boxes.size(); i++) {
			window.draw(falling_boxes[i].GetBox());
		}
	}
}

float RandomSpeedCircle() {
	float speed = (rand() % 400) / 100.0 - 2;
	if ((speed > 1.6 && speed < 1.8) || (speed < -1.6 && speed > -1.8)) {
		return speed;
	}
	return RandomSpeedCircle();

}

float RandomSpeedCircle(float circle_speed_x, float circle_speed_y, bool is_x) {
	float speed = (rand() % 400) / 100.0 - 2;
	if ((speed > 1.5 && speed < 2) || (speed < -1.5 && speed > -2)) {
		if (is_x == true) {
			if (speed > 0 && circle_speed_x > 0 || speed < 0 && circle_speed_x < 0) {
				return speed;
			}
		}
		else {
			if (speed > 0 && circle_speed_y > 0 || speed < 0 && circle_speed_y < 0) {
				return speed;
			}
		}
	}
	return RandomSpeedCircle(circle_speed_x, circle_speed_y, is_x);
}

sf::Vector2f RandomPosCircle(std::vector<sf::RectangleShape>& snake, const int size_circle) {
	float pos_x = (rand() % width);
	float pos_y = (rand() % height);
	for (int i = 0; i < snake.size(); i++) {
		sf::Vector2f segment_pos(snake[i].getPosition().x + static_cast<float>(25) / 2, snake[i].getPosition().y + static_cast<float>(25) / 2);
		float difference_x = (pos_x - segment_pos.x) < 0 ? (pos_x - segment_pos.x) * -1 : (pos_x - segment_pos.x);
		float difference_y = (pos_y - segment_pos.y) < 0 ? (pos_y - segment_pos.y) * -1 : (pos_y - segment_pos.y);
		if (difference_x > static_cast<float>(25) / 2 + size_circle) {
			if (difference_y > static_cast<float>(25) / 2 + size_circle) {
				if (pos_x > 50 + size_circle && pos_x < 50 + width - size_circle*2 && pos_y > 100 + size_circle && pos_y < 100 + height - size_circle*2) {
					return sf::Vector2f(pos_x, pos_y);
				}
			}
		}
	}
	return RandomPosCircle(snake, size_circle);
}

sf::Vector2f RandomPosFood(std::vector<sf::RectangleShape>& snake, const int size) {
	int pos_x = (rand() % (width/25)) * width/20;
	int pos_y = (rand() % (height / 25)) * height/20;
	for (int i = 0; i < snake.size(); i++) {
		sf::Vector2f segment_pos(snake[i].getPosition().x + static_cast<float>(25) / 2, snake[i].getPosition().y + static_cast<float>(25) / 2);
		float difference_x = (pos_x - segment_pos.x) < 0 ? (pos_x - segment_pos.x) * -1 : (pos_x - segment_pos.x);
		float difference_y = (pos_y - segment_pos.y) < 0 ? (pos_y - segment_pos.y) * -1 : (pos_y - segment_pos.y);
		if (difference_x > static_cast<float>(25) / 2 + size) {
			if (difference_y > static_cast<float>(25) / 2 + size) {
				if (pos_x > 50 && pos_x < width-size && pos_y > 100 && pos_y < height-size) {
					return sf::Vector2f(pos_x, pos_y);
				}
			}
		}
	}
	return RandomPosFood(snake, size);
}

void EatingFood(std::vector<sf::RectangleShape> &snake, sf::RectangleShape &food, int &score) {
	if (snake[0].getPosition().x == food.getPosition().x && snake[0].getPosition().y == food.getPosition().y) {
		score += 100;
		food.setPosition(RandomPosFood(snake, food.getSize().x));
	}
}

void CheckCollisions(sf::CircleShape &circle, std::vector<sf::RectangleShape> &snake, std::vector<FallingBox> falling_boxes, bool &is_game_over) {
	sf::Vector2f circle_center(circle.getPosition().x + circle.getRadius(), circle.getPosition().y + circle.getRadius());
	for (int i = 0; i < snake.size(); i++) {
		sf::Vector2f snake_center(snake[i].getPosition().x + snake[i].getSize().x / 2, snake[i].getPosition().y + snake[i].getSize().y / 2);
		float difference_x_circle = (snake_center.x - circle_center.x) < 0 ? (snake_center.x - circle_center.x)*-1 : (snake_center.x - circle_center.x);
		float difference_y_circle = (snake_center.y - circle_center.y) < 0 ? (snake_center.y - circle_center.y)*-1 : (snake_center.y - circle_center.y);
		if (difference_x_circle <= circle.getRadius() && difference_y_circle <= circle.getRadius()) {
			is_game_over = true;
		}
		for (int j = 0; j < falling_boxes.size(); j++) {
			sf::Vector2f boxCenter(falling_boxes[j].GetPosition().x + falling_boxes[j].GetSize().x / 2, falling_boxes[j].GetPosition().y + falling_boxes[j].GetSize().y / 2);
			float difference_x_box = (snake_center.x - boxCenter.x) < 0 ? (snake_center.x - boxCenter.x)*-1 : (snake_center.x - boxCenter.x);
			float difference_y_box = (snake_center.y - boxCenter.y) < 0 ? (snake_center.y - boxCenter.y)*-1 : (snake_center.y - boxCenter.y);
			if (difference_x_box <= falling_boxes[j].GetSize().x / 2 + snake[i].getSize().x / 2 - 5 && difference_y_box <= falling_boxes[j].GetSize().y / 2 + snake[i].getSize().y / 2 - 5) {
				is_game_over = true;
			}
		}
	}
}

void TimersUpdate(sf::Clock& clock, double& time_move_snake, double& timer_pause, double& timer_use_buttons, double& timerMovecircle, std::vector<FallingBox> &falling_boxes, double &timer_between_falling, bool is_game_started, bool is_game_over) {
	float time = clock.getElapsedTime().asSeconds();
	clock.restart();
	timer_use_buttons += time;
	if (is_game_started == true && is_game_over == false) {
		time_move_snake += time;
		timer_pause += time;
		timerMovecircle += time;
		timer_between_falling += time;
		for (int i = 0; i < falling_boxes.size(); i++) {
			falling_boxes[i].SetTimer(time + falling_boxes[i].GetTimer());
		}
	}
}

void DrawGrid(sf::RenderWindow &window) {
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

void KeyboardProcessing(sf::Event event, sf::RenderWindow &window, float &snake_speed_x, float& snake_speed_y, float snake_speed, double &timer_use_buttons, const float delay_use_buttons, double &timer_pause, const int delay_pause, bool &pause) {
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	if (event.key.code == sf::Keyboard::Up && snake_speed_y != snake_speed && timer_use_buttons > delay_use_buttons) {
		snake_speed_x = 0;
		snake_speed_y = -snake_speed;
		timer_use_buttons = 0;
	}
	if (event.key.code == sf::Keyboard::Down && snake_speed_y != -snake_speed && timer_use_buttons > delay_use_buttons) {
		snake_speed_x = 0;
		snake_speed_y = snake_speed;
		timer_use_buttons = 0;
	}
	if (event.key.code == sf::Keyboard::Right && snake_speed_x != -snake_speed && timer_use_buttons > delay_use_buttons && (snake_speed_x != 0 or snake_speed_y != 0)) {
		snake_speed_x = snake_speed;
		snake_speed_y = 0;
		timer_use_buttons = 0;
	}
	if (event.key.code == sf::Keyboard::Left && snake_speed_x != snake_speed && timer_use_buttons > delay_use_buttons) {
		snake_speed_x = -snake_speed;
		snake_speed_y = 0;
		timer_use_buttons = 0;
	}
	if (event.key.code == sf::Keyboard::Escape && timer_pause > delay_pause) {
		if (pause == false) {
			pause = true;
		}
		else {
			pause = false;
		}
		timer_pause = 0;
	}
}

void InitText(sf::Text &text, float pos_x, float pos_y, int sizeFont, sf::String string, sf::Color text_color, sf::RenderWindow& window, sf::Font& font) {
	text.setFont(font);
	text.setCharacterSize(sizeFont);
	text.setPosition(pos_x, pos_y);
	text.setString(string);
	text.setFillColor(text_color);
	window.draw(text);
}

void DisplayText(int score, float score_pos_x, float score_pos_y, int score_size, sf::Text& text_score, double timer_between_falling, double delay_between_falling, float timer_boxes_pos_x, float timer_boxes_pos_y, float timer_boxes_size, sf::Text &text_timer_falling_boxes, bool is_game_over, sf::Text &text_game_over, sf::Text &text_play_again, sf::Text& text_yes, sf::Text& text_no, sf::RenderWindow &window, sf::Font& font) {
	std::ostringstream score_string;
	score_string << score;
	if (is_game_over) {
		InitText(text_game_over, 115, 175.5, 35, "Game Over", sf::Color::Red, window, font);
		InitText(text_play_again, 140, 260, 25, "Play again?", sf::Color::White, window, font);
		InitText(text_yes, 175, 335, 20, "Yes", sf::Color::White, window, font);
		InitText(text_no, 375, 335, 20, "No", sf::Color::White, window, font);
		InitText(text_score, 185, 600, 25, "Score: " + score_string.str(), sf::Color::Yellow, window, font);
	} else {
		InitText(text_score, score_pos_x, score_pos_y, score_size, "Score: " + score_string.str(), sf::Color::Green, window, font);
		std::ostringstream timer_falling_boxes_string;
		delay_between_falling - timer_between_falling < 0 ? timer_falling_boxes_string << 0 : timer_falling_boxes_string << static_cast<int>(delay_between_falling - timer_between_falling);
		InitText(text_timer_falling_boxes, timer_boxes_pos_x, timer_boxes_pos_y, timer_boxes_size, timer_falling_boxes_string.str() + " Left", sf::Color::Red, window, font);
		window.draw(text_timer_falling_boxes);
		window.draw(text_score);
	}

}

int CheckRestart(sf::RenderWindow &window, bool is_game_over) {
	if (is_game_over) {
		if (sf::IntRect(170, 335, 75, 25).contains(sf::Mouse::getPosition(window)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			return 1;
		}
		if (sf::IntRect(370, 335, 50, 25).contains(sf::Mouse::getPosition(window)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			return -1;
		}
	}
	return 0;
}

void GameManagement(sf::RenderWindow &window) {
	using namespace sf;

	Clock clock;
	srand(time(0));

	Font font;
	font.loadFromFile("../resources/QuinqueFive.ttf");
	Text text_score;
	Text text_timer_falling_boxes;
	Text text_game_over;
	Text text_play_again;
	Text text_yes;
	Text text_no;

	int snake_size = 25;
	float snake_speed = 25;
	float snake_speed_x = 0;
	float snake_speed_y = 0;
	RectangleShape s1(Vector2f(snake_size, snake_size));
	RectangleShape s2(Vector2f(snake_size, snake_size));
	RectangleShape s3(Vector2f(snake_size, snake_size));
	std::vector<RectangleShape> snake = {s1, s2, s3};
	for (int i = 0; i < snake.size(); i++) {
		snake[i].setPosition(150 + i * snake_size, 150);
		snake[i].setOutlineThickness(0.5);
		snake[i].setOutlineColor(sf::Color::Black);
		snake[i].setFillColor(sf::Color(0, 175, 0));
	}

	const int size_circle = 48;
	float circle_speed_x = 0;
	float circle_speed_y = 0;
	float prev_circle_speed_x;
	float prev_circle_speed_y;
	CircleShape circle(size_circle);
	Vector2f circle_pos = RandomPosCircle(snake, size_circle);
	circle.setPosition(circle_pos.x, circle_pos.y);
	circle.setFillColor(Color::White);

	std::vector<FallingBox> falling_boxes;

	int food_size = 25;
	RectangleShape food(Vector2f(food_size, food_size));
	food.setFillColor(Color::Red);
	food.setOutlineColor(Color::Black);
	food.setOutlineThickness(0.5);
	food.setPosition(RandomPosFood(snake, food_size));
	int score = 0;

	double time_move_snake = 0;
	double delay_move_snake = 0.15;
	double timer_pause = 0;
	int delay_pause = 3;
	double timer_use_buttons = 0;
	double delay_use_buttons = 0.16;
	double timer_move_circle = 0;
	double delay_move_circle = 0.01;
	double delay_between_falling = 4;
	double timer_between_falling = 0;

	bool is_game_over = false;
	bool pause = false;
	bool is_game_started = false;
	bool is_falling_first = true;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			KeyboardProcessing(event, window, snake_speed_x, snake_speed_y, snake_speed, timer_use_buttons, delay_use_buttons, timer_pause, delay_pause, pause);
		}
		window.clear(Color::Black);
		if (!is_game_over && !pause) {
			DrawGrid(window);
			window.draw(food);
			MoveCircle(timer_move_circle, delay_move_circle, circle, snake, circle_speed_x, circle_speed_y, prev_circle_speed_x, prev_circle_speed_y, is_game_started, is_game_over);
			MoveSnake(snake_speed_x, snake_speed_y, time_move_snake, delay_move_snake, snake, is_game_over, window, is_game_started, circle, food, score);
			FallBoxes(falling_boxes, window, timer_between_falling, delay_between_falling, is_falling_first, is_game_over);
			CheckCollisions(circle, snake, falling_boxes, is_game_over);
			EatingFood(snake, food, score);
			window.draw(circle);
		} else if (is_game_over) {
			if (CheckRestart(window, is_game_over) == 1) {
				GameManagement(window);
			} else if(CheckRestart(window, is_game_over) == -1) {
				window.close();
			}
		}
		DisplayText(score, 400, 50, 15, text_score, timer_between_falling, delay_between_falling, 50, 50, 15, text_timer_falling_boxes, is_game_over, text_game_over, text_play_again, text_yes, text_no, window, font);
		TimersUpdate(clock, time_move_snake, timer_pause, timer_use_buttons, timer_move_circle, falling_boxes, timer_between_falling, is_game_started, is_game_over);
		window.display();
	}
}
