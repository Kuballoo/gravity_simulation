#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/*
*	This class will act as game engine
*/
class Game {
public:
	Game();
	~Game();

	// Public methods
	void update();
	void render();

	// Accessors
	const bool isWindowOpen() const;
private:
	// Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	unsigned int frameRateLimit;

	// DeltaTime
	sf::Clock deltaClock;
	sf::Time dt;
	
	// Physics
	sf::Clock tictac;
	float gravityForce;

	// Objects
	// Ball
	sf::CircleShape ball;
	float ballRadius;
	float startH;
	float startVel;
	float throwAngle;
	int selValue;

	sf::Vector2f ballVel;
	sf::Vector2f ballPos;
	sf::Time ballTimer;
	sf::Time moveStart;
	bool ballShooted;

	// Trajectory
	sf::CircleShape trajectoryPoint;
	float pointRadius;
	std::vector<sf::CircleShape> trajectoryPoints;

	// Text
	sf::Font font;
	sf::Text txt;

	// Private methods
	void initVariables();
	void initWindow();
	void pollEvents();

	// Ball methods
	void initBall();
	void updateBall();
	void renderBall(sf::RenderTarget& target);
	void shootBall();
	void restarBall();
	void changeValues();

	// Trajectory methods
	void initPoint();
	void updatePoints();
	void renderPoints(sf::RenderTarget& target);
	void clearTrajectories();

	// Text methods
	void initText();
	void updateText();
	void renderText(sf::RenderTarget& target);
};

