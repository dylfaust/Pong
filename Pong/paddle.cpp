// The ball class must be responsible for tracking and updating the ball's location,
// speed, and direction, as well as drawing the ball to the screen.

#include "paddle.h"

// Constructor
Paddle::Paddle(RenderWindow& window, bool isAI,  bool isCenterPaddle, float windWidth, float windHeight, float paddleSpeed) : myWindow(window)
{
	paddleSize = { 10,100 };
	paddle.setSize(paddleSize);// -sf::Vector2f(3, 3));
	paddle.setFillColor(sf::Color(250, 250, 250));
	this->isAI = isAI;
	this->paddleSpeed = paddleSpeed;
    this->isCenterPaddle = isCenterPaddle;

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
    
    if (isCenterPaddle)
    {
        paddle.setPosition(windWidth / 2 - 1, (windHeight / 2) + 150);
    }

	this->windWidth = windWidth;
	this->windHeight = windHeight;
}

// Move the paddle function
void Paddle::move(float dt, Vector2f ballPos, Vector2f ballPos2, bool multiBall)
{
	// If it's not the center paddle
    if (isCenterPaddle == false)
    {
        // If it's human controlled
        if (isAI == false)
        {
			// Move based on keyboard inputs
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
        }

		// If it's AI controlled, move based on ball position
        else
        {
			if (ballPos.x > windWidth / 2 || multiBall == false)
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

			else if (ballPos2.x > windWidth / 2)
			{
				if (ballPos2.y > paddle.getPosition().y && (paddle.getPosition().y + paddleSize.y / 2 < windHeight - 5.f))
				{
					paddle.setPosition(paddle.getPosition().x, paddle.getPosition().y + paddleSpeed * dt);
				}

				else if (ballPos2.y < paddle.getPosition().y &&
					(paddle.getPosition().y - paddleSize.y / 2 > 5.f))
				{
					paddle.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddleSpeed * dt);
				}
			}
        }
    }

	// If it is center paddle, just move up and down
    else
    {
        if (goingUp == false && (paddle.getPosition().y + paddleSize.y / 2 < windHeight - 5.f))
        {
            if (paddle.getPosition().y + paddleSize.y / 2 >= windHeight - 6.f)
                goingUp = true;
            
            paddle.setPosition(paddle.getPosition().x, paddle.getPosition().y + paddleSpeed * dt);
        }
        
        else if (goingUp == true &&
                 (paddle.getPosition().y - paddleSize.y / 2 > 5.f))
        {
            if (paddle.getPosition().y - paddleSize.y / 2 < 6.f)
                goingUp = false;
            
            paddle.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddleSpeed * dt);
        }
   
    }
    
}

// Draws the paddle
void Paddle::draw()
{
	myWindow.draw(paddle);
}

// Resets the paddle to starting position
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