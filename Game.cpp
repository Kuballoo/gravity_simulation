#include "Game.h"

Game::Game() {
	initVariables();
	initWindow();
	initBall();
	initPoint();
	initText();
}

Game::~Game() {
	delete this->window;
}

/*
*	PUBLIC SECTION
*/

void Game::update() {
	/*
	*	Method tasks:
	*	- update everything every frame
	*	- checking occuring events
	*	- handle delta clock
	*/
	pollEvents();
	if(this->ballShooted) {
		updatePoints();
	}
	updateBall();
	updateText();
	this->dt = this->deltaClock.restart();
}

void Game::render() {
	/*
	*	Method tasks:
	*	- clearing window and displaying them
	*	- render objects
	*/
	this->window->clear(sf::Color::Black);
	renderBall(*this->window);
	renderPoints(*this->window);
	renderText(*this->window);
	this->window->display();
}

// Accesors
const bool Game::isWindowOpen() const {
	return this->window->isOpen();
}

/*
*	PRIVATE SECTION
*/

void Game::initVariables() {
	this->window = nullptr;
	this->videoMode = {1600, 900};
	this->frameRateLimit = 60;

	this->tictac.restart();
	this->gravityForce = 10.f;
	this->ballShooted = false;
	this->selValue = 0;
}

void Game::initWindow() {
	this->window = new sf::RenderWindow(this->videoMode, "Gravity simulation!", sf::Style::Close);
	this->window->setFramerateLimit(this->frameRateLimit);
}

void Game::pollEvents() {
	while(this->window->pollEvent(this->ev)) {
		switch(this->ev.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if(this->ev.key.code == sf::Keyboard::Space)
				shootBall();
			else if(this->ev.key.code == sf::Keyboard::R)
				restarBall();
			else if(this->ev.key.code == sf::Keyboard::C)
				clearTrajectories();
			break;
		case sf::Event::MouseWheelScrolled:
			changeValues();
			break;
		case sf::Event::MouseButtonPressed:
			this->selValue++;
			if(this->selValue > 2) this->selValue = 0;
		default:
			break;
		}
	}
}

void Game::initBall() {
	this->ball.setFillColor(sf::Color::Green);
	this->ballRadius = 15.f;
	this->ball.setRadius(this->ballRadius);
	sf::Vector2f origin{
		this->ballRadius,
		this->ballRadius
	};
	this->ball.setOrigin(origin);
	this->startH = 200.f;
	this->startH = this->videoMode.height - this->startH - this->ballRadius;
	this->ballPos = {this->ballRadius, this->startH};
	this->ball.setPosition(this->ballPos);

	this->throwAngle = 45.f;
	this->startVel = 50.f;
	this->ballTimer = sf::Time::Zero;
}

void Game::updateBall() {
	if(ballShooted) {
		if(this->ballPos.y + this->ballRadius >= videoMode.height) {
			this->ballPos.y = this->videoMode.height - this->ballRadius;
		} else {
			this->ballTimer = (this->tictac.getElapsedTime() - this->moveStart) * 4.f;
			this->ballPos = {this->ballRadius + this->ballVel.x * this->ballTimer.asSeconds(), (this->startH + this->ballVel.y * this->ballTimer.asSeconds()) + this->gravityForce * static_cast<float>(pow(this->ballTimer.asSeconds(), 2)) / 2.f};
		}
	} else {
		this->ballPos = {this->ballRadius, this->startH};
	}
	
	this->ball.setPosition(this->ballPos);
}

void Game::renderBall(sf::RenderTarget& target) {
	target.draw(this->ball);
}

void Game::shootBall() {
	if(this->ballShooted == false) {
		this->moveStart = this->tictac.getElapsedTime();
		this->ballShooted = true;
		this->ballVel = {this->startVel * cos(static_cast<float>( - (this->throwAngle * M_PI / 180))), this->startVel * sin(static_cast<float>(-(this->throwAngle * M_PI / 180)))};
		this->ballTimer = sf::Time::Zero;
	}
}

void Game::restarBall() {
	this->ballShooted = false;
	this->ballPos = {this->ballRadius, this->startH};
	this->ball.setPosition(this->ballPos);
}

void Game::changeValues() {
	if(!ballShooted) {
		switch(this->selValue) {
		case 0:
			this->throwAngle += ev.mouseWheelScroll.delta;
			if(this->throwAngle >= 90.f) this->throwAngle = 90.f;
			else if(this->throwAngle <= -90.f) this->throwAngle = -90.f;
			break;
		case 1:
			this->startH -= ev.mouseWheelScroll.delta;
			if(this->startH <= this->ballRadius) this->startH = this->ballRadius;
			else if(this->startH >= this->videoMode.height - this->ballRadius) this->startH = this->videoMode.height - this->ballRadius-1;
			break;
		case 2:
			this->startVel += ev.mouseWheelScroll.delta;
			if(this->startVel <= 0) this->startVel = 0.f;
			break;
		default:
			break;
		}
	}
}

void Game::initPoint() {
	this->trajectoryPoint.setFillColor(sf::Color(252, 118, 0));
	this->pointRadius = 2.f;
	this->trajectoryPoint.setRadius(this->pointRadius);
	sf::Vector2f origin{
		this->pointRadius,
		this->pointRadius
	};
	this->trajectoryPoint.setOrigin(origin);
}

void Game::updatePoints() {
	if(!(this->ballPos.y + this->ballRadius >= videoMode.height)) {
		this->trajectoryPoint.setPosition(this->ballPos);
		this->trajectoryPoints.push_back(trajectoryPoint);
	}
}

void Game::renderPoints(sf::RenderTarget& target) {
	for(auto& point : trajectoryPoints)
		target.draw(point);
}

void Game::clearTrajectories() {
	if(!ballShooted) {
		this->trajectoryPoints.clear();
	}
}

void Game::initText() {
	if(!this->font.loadFromFile("Roboto-Regular.ttf"))
		std::cout << "Font error!" << std::endl;

	this->txt.setFont(this->font);
	this->txt.setString("NONE");
	this->txt.setCharacterSize(12);
	this->txt.setFillColor(sf::Color::White);


}

void Game::updateText() {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2);
	stream << "Angel: " << this->throwAngle << std::endl;
	stream << "Height: " << this->videoMode.height - this->startH - this->ballRadius << std::endl;
	stream << "Start velocity: " << this->startVel << std::endl;
	this->txt.setString(stream.str());
}

void Game::renderText(sf::RenderTarget& target) {
	target.draw(this->txt);
}
