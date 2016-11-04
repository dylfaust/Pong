// The ball class must be responsible for tracking and updating the ball's location,
// speed, and direction, as well as drawing the ball to the screen.

#include "ball.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h> 

// Constructor
Ball::Ball(RenderWindow& window) : myWindow(window)
{
	bounce.loadFromFile("resources/Click.wav");
	sound.setBuffer(bounce);
	pi = 2.141593;
	radius = 10;
	ball.setRadius(radius);
	ball.setOrigin(16, 16);
	ball.setFillColor(Color::White);
	ball.setPosition(400, 300);
	position = ball.getPosition();
	direction = { 5,1 };
	speed = 500.0f;
	scoreL = 0;
	scoreR = 0;
	srand((unsigned)time(NULL));
	base = 0.13f;
	startingSpeed = speed;

	// Set starting direction
	float dirY = rand() % 5;
	int negX = rand() % 2;
	int negY = rand() % 2;

	direction.x = 5;
	direction.y = dirY;

	if (negX == 0)
	{
		direction.x = -direction.x;
	}
	if (negY == 0)
	{
		direction.y = -direction.y;
	}
}

// Moves the ball
void Ball::move(float dt, Vector2f paddlePos, Vector2f paddleSize, Vector2f middlePos, Vector2f middleSize)
{
	float l = sqrt(pow(direction.x, 2.0f) + pow(direction.x, 2.0f)); // 5.831
	Vector2f b;
	b.x = direction.x / l; // 0.514
	b.y = direction.y / l; // 0.857
	position += b * speed * dt;
	ball.setPosition(position);
	this->paddlePos = paddlePos;

	if (position.y > myWindow.getSize().y && direction.y > 0)
		direction.y = -direction.y;
	if (position.y < (0 + ball.getRadius()) && direction.y < 0)
		direction.y = -direction.y;
	if (position.x > myWindow.getSize().x && direction.x > 0)
	{
		lost = true;
		scoreL += 1;
		ball.setPosition(400, 300);
		position = ball.getPosition();
	}

	if (position.x < 0 && direction.x < 0)
	{
		lost = true;
		scoreR += 1;
		ball.setPosition(400, 300);
		position = ball.getPosition();
		ball.setPosition(0, 0);
	}

	//Check if its inside paddle before moving, only make it bounce right if its moving to left
	// Going left collision detection
	if (ball.getPosition().x - radius < paddlePos.x + paddleSize.x / 2 &&
		ball.getPosition().x - radius > paddlePos.x - paddleSize.x / 2 &&
		ball.getPosition().y + radius >= paddlePos.y - paddleSize.y / 2 &&
		ball.getPosition().y - radius <= paddlePos.y + paddleSize.y / 2 && direction.x < 0)
	{
		direction.x = -direction.x;
		hit();

		// If it's on the underside of the paddle
		if (ball.getPosition().y > paddlePos.y )
		{
			float dist = ball.getPosition().y - paddlePos.y;
			direction.y = base * dist;
		}

		else
		{
			float dist =  paddlePos.y - ball.getPosition().y;
			direction.y = -(base * dist);
		}
	}

	// Going right collision detection
	else if (ball.getPosition().x + radius > paddlePos.x - paddleSize.x / 2 + 7 &&
		ball.getPosition().x + radius < paddlePos.x + paddleSize.x/2 &&
		ball.getPosition().y + radius >= paddlePos.y - paddleSize.y / 2 &&
		ball.getPosition().y - radius <= paddlePos.y + paddleSize.y / 2 && direction.x > 0)
	{
		direction.x = -direction.x;
		hit();
		if (ball.getPosition().y > paddlePos.y)
		{
			float dist = ball.getPosition().y - paddlePos.y;
			direction.y = base * dist;
		}
		else
		{
			float dist = paddlePos.y - ball.getPosition().y;
			direction.y = -(base * dist);
		}
	}

	// Middle paddle from the left collision detection
	else if (ball.getPosition().x - radius < middlePos.x + middleSize.x / 2 &&
		ball.getPosition().x - radius > middlePos.x - middleSize.x / 2 &&
		ball.getPosition().y + radius >= middlePos.y - middleSize.y / 2 &&
		ball.getPosition().y - radius <= middlePos.y + middleSize.y / 2 && direction.x < 0)
	{
		direction.x = -direction.x;
		hit();
		if (ball.getPosition().y > middlePos.y)
		{
			float dist = ball.getPosition().y - middlePos.y;
			direction.y = base * dist;
		}
		else
		{
			float dist = middlePos.y - ball.getPosition().y;
			direction.y = -(base * dist);
		}
	}

	// Middle paddle from the right collision detection
	else if (ball.getPosition().x + radius > middlePos.x - middleSize.x / 2 + 7 &&
		ball.getPosition().x + radius < middlePos.x + middleSize.x / 2 &&
		ball.getPosition().y + radius >= middlePos.y - middleSize.y / 2 &&
		ball.getPosition().y - radius <= middlePos.y + middleSize.y / 2 && direction.x > 0)
	{
		direction.x = -direction.x;
		hit();

		// If it's on the underside of the paddle
		if (ball.getPosition().y > middlePos.y)
		{
			float dist = ball.getPosition().y - middlePos.y;
			direction.y = base * dist;
		}

		else
		{
			float dist = middlePos.y - ball.getPosition().y;
			direction.y = -(base * dist);
		}
	}
}

// Draw ball
void Ball::draw()
{
	myWindow.draw(ball);
}

// If ball hits a paddle, do this
void Ball::hit()
{
	myWindow.clear(sf::Color(255, 255, 255, 255));
	sound.play();
	speed += 10;
}

// Resets ball to starting position
void Ball::reset()
{
	ball.setPosition(400, 300);
	position = ball.getPosition();
	float dirY = rand() % 5;
	int negX = rand() % 2;
	int negY = rand() % 2;

	direction.x = 5;
	direction.y = dirY;

	if (negX == 0)
	{
		direction.x = -direction.x;
	}
	if (negY == 0)
	{
		direction.y = -direction.y;
	}

	speed = startingSpeed;
}