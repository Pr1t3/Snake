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
	FallingBox(int x);
	sf::RectangleShape GetBox();
	sf::Vector2f GetPosition();
	sf::Vector2f GetSize();
	void Fall(float y);
	float GetTimer();
	float GetDelay();
	void SetTimer(float time);
};

float RandomPosFallingBox(std::vector<FallingBox>& falling_boxes);

void MoveSnake(float speedX, float speedY, double& time_move_snake, const double delay_move_snake, std::vector<sf::RectangleShape>& snake, bool& is_game_over, sf::RenderWindow& window, bool& is_game_started, sf::CircleShape &circle, sf::RectangleShape &food, int &score);

void MoveCircle(double& timerMovecircle, const double delayMovecircle, sf::CircleShape& circle, std::vector<sf::RectangleShape>& snake, float& circle_speed_x, float& circle_speed_y, float& prevcircleSpeedX, float& prevcircleSpeedY, bool is_game_started, bool &is_game_over);

void FallBoxes(std::vector<FallingBox> &falling_boxes, sf::RenderWindow& window, double &timer_between_falling, double delay_between_falling, bool &is_falling_first, bool is_game_over);

float RandomSpeedCircle();

float RandomSpeedCircle(float circle_speed_x, float circle_speed_y, bool is_x);

sf::Vector2f RandomPosCircle(std::vector<sf::RectangleShape>& snake, const int size_circle);

sf::Vector2f RandomPosFood(std::vector<sf::RectangleShape>& snake, const int size);

void EatingFood(std::vector<sf::RectangleShape> &snake, sf::RectangleShape &food, int &score);

void CheckCollisions(sf::CircleShape &circle, std::vector<sf::RectangleShape> &snake, std::vector<FallingBox> falling_boxes, bool &is_game_over);

void TimersUpdate(sf::Clock& clock, double& time_move_snake, double& timer_pause, double& timer_use_buttons, double& timerMovecircle, std::vector<FallingBox> &falling_boxes, double &timer_between_falling, bool is_game_started, bool is_game_over);

void DrawGrid(sf::RenderWindow &window);

void KeyboardProcessing(sf::Event event, sf::RenderWindow &window, float &snake_speed_x, float& snake_speed_y, float snake_speed, double &timer_use_buttons, const float delay_use_buttons, double &timer_pause, const int delay_pause, bool &pause);

void InitText(sf::Text &text, float pos_x, float pos_y, int sizeFont, sf::String string, sf::Color text_color, sf::RenderWindow& window, sf::Font& font);

void displayText(int score, float score_pos_x, float score_pos_y, int score_size, sf::Text& text_score, double timer_between_falling, double delay_between_falling, float timer_boxes_pos_x, float timer_boxes_pos_y, float timer_boxes_size, sf::Text &text_timer_falling_boxes, bool is_game_over, sf::Text &text_game_over, sf::Text &text_play_again, sf::Text& text_yes, sf::Text& text_no, sf::RenderWindow &window, sf::Font& font);

int CheckRestart(sf::RenderWindow &window, bool is_game_over);

void GameManagement(sf::RenderWindow &window);
