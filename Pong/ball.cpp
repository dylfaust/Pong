// The ball class must be responsible for tracking and updating the ball's location,
// speed, and direction, as well as drawing the ball to the screen.

#include "ball.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructor
Ball::Ball(RenderWindow& window) : myWindow(window)
{
	bounce.loadFromFile("pistol.wav");
	sound.setBuffer(bounce);
	pi = 2.141593;
	radius = 10;
	ball.setRadius(radius);
	ball.setOrigin(16, 16);
	ball.setFillColor(Color::White);
	ball.setPosition(400, 300);
	position = ball.getPosition();
	direction = { 5,1 };
	speed = 70.0f;
	srand((unsigned)time(NULL));
	do
	{
		// Make sure the ball initial angle is not too much vertical
		angle = (rand() % 360) * 2 * pi / 360;
	} while (std::abs(std::cos(angle)) < 0.7f);
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

void Ball::move(float dt, Vector2f paddlePos, Vector2f paddleSize, Vector2f middlePos, Vector2f middleSize)
{
	position += direction * speed * dt;
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
		//direction.x = -direction.x;
	if (position.x < 0 && direction.x < 0)
	{
		lost = true;
		scoreR += 1;
		ball.setPosition(400, 300);
		position = ball.getPosition();
		ball.setPosition(0, 0);
	}
		//direction.x = -direction.x;

	//Check if its inside paddle before moving, only make it bounce right if its moving to left

	// Going left
	if (ball.getPosition().x - radius < paddlePos.x + paddleSize.x / 2 &&
		ball.getPosition().x - radius > paddlePos.x &&
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

	// Going right
	else if (ball.getPosition().x + radius > paddlePos.x - paddleSize.x / 2 &&
		ball.getPosition().x + radius < paddlePos.x &&
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
	//std::cout << direction.y << std::endl;
}


void Ball::draw()
{
	myWindow.draw(ball);
}

void Ball::hit()
{
	sound.play();
	speed += 1.5;
	std::cout << speed;
}

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
//	direction = { dirA, dirB };
}