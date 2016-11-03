// The ball class must be responsible for tracking and updating the ball's location,
// speed, and direction, as well as drawing the ball to the screen.

#include "paddle.h"

// Constructor
Paddle::Paddle(RenderWindow& window, bool isAI, float windWidth, float windHeight, float paddleSpeed) : myWindow(window)
{
	paddleSize = { 20,100 };
	paddle.setSize(paddleSize - sf::Vector2f(3, 3));
	paddle.setFillColor(sf::Color(250, 250, 250));
	this->isAI = isAI;
	this->paddleSpeed = paddleSpeed;

	paddle.setOrigin(paddleSize / 2.f);

	if (isAI == false)
	{
		paddle.setPosition(10 + paddleSize.x / 2, windHeight / 2);
	}
	else
	{
		paddle.setPosition(windWidth - 10 - paddleSize.x / 2, windHeight / 2);
		paddle.setFillColor(sf::Color(250, 250, 250));
	}

	this->windWidth = windWidth;
	this->windHeight = windHeight;
	//paddle.setPosition(10 + paddleSize.x / 2, windHeight / 2);
}

void Paddle::move(float dt, Vector2f ballPos)
{
	if (isAI == false)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
			(paddle.getPosition().y - paddleSize.y / 2 > 5.f))
		{
			paddle.move(0.f, -paddleSpeed * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
			(paddle.getPosition().y + paddleSize.y / 2 < windHeight - 5.f))
		{
			paddle.move(0.f, paddleSpeed * dt);
		}

		this->ballPos = ballPos;
		//position += direction * speed;
		//ball.setPosition(position);

		/*if (position.y > myWindow.getSize().y && direction.y > 0)
			direction.y = -direction.y;
		if (position.y < 0 && direction.y < 0)
			direction.y = -direction.y;
		if (position.x > myWindow.getSize().x && direction.x > 0)
			direction.x = -direction.x;
		if (position.x < 0 && direction.x < 0)
			direction.x = -direction.x;*/
	}

	else
	{
		if (ballPos.y > paddle.getPosition().y && (paddle.getPosition().y + paddleSize.y / 2 < windHeight - 5.f))
		{
			paddle.setPosition(paddle.getPosition().x, paddle.getPosition().y + paddleSpeed * dt);
		}

		else if (ballPos.y < paddle.getPosition().y &&
			(paddle.getPosition().y - paddleSize.y / 2 > 5.f))
		{
			paddle.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddleSpeed * dt);
		}
	}
}

void Paddle::draw()
{
	myWindow.draw(paddle);
}

void Paddle::reset()
{
	if (isAI == false)
	{
		paddle.setPosition(10 + paddleSize.x / 2, windHeight / 2);
	}
	else
	{
		paddle.setPosition(windWidth - 10 - paddleSize.x / 2, windHeight / 2);
	}
}