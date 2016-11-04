/*

 Name : PONG
 Author: Dylan Faust
 Date : 11/3/16
 Purpose: To rebuild the classic game PONG in SFML and C++

 */

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include "ball.h"
#include "paddle.h"

using namespace sf;
using namespace std;

void update_state(float dt);
void render_frame();
RenderWindow window;
//CircleShape ball;
Vector2f vel(-300, 600);

bool winner = false;
bool multiBall = false;

Texture tex;

float windWidth = 800;
float windHeight = 600;

Ball myBall = Ball(window);
Ball myBall2 = Ball(window);
Paddle leftPaddle = Paddle(window, false, false, windWidth, windHeight, 500.0f);
Paddle rightPaddle = Paddle(window, true, false, windWidth, windHeight, 400.0f);
Paddle centerPaddle = Paddle(window, true, true, windWidth, windHeight, 300.0f);

RectangleShape bg;
RectangleShape centerBlock;

// Sounds
SoundBuffer buf;
Sound sound;

bool justLost = false;

sf::Text scoreL;
sf::Text scoreR;
sf::Text replayText;
Text winnerText;
Font font;

// Main function
int main()
{

	window.create(VideoMode(windWidth, windHeight), "PONG");
	Clock clock;
	// Background texture
	tex.loadFromFile("Court.png");

	// Initialize everything
	bg.setSize(Vector2f(windWidth, windHeight));
	bg.setPosition(0, 0);
	bg.setTexture(&tex);
	bg.setPosition(0, 0);

	centerBlock.setSize(Vector2f(50, 50));
	centerBlock.setFillColor(sf::Color(250, 250, 250));
	centerBlock.setPosition(windWidth/2-30, windHeight/2);

	font.loadFromFile("Gameplay.TTF");
	scoreL.setFont(font);
	scoreL.setCharacterSize(50);
	scoreL.setString("0");
	scoreL.setColor(Color::White);
	scoreL.setPosition(windWidth/2 - 80, 30);
	scoreR.setFont(font);
	scoreR.setCharacterSize(50);
	scoreR.setString("0");
	scoreR.setColor(Color::White);
	scoreR.setPosition(windWidth/2 + 50, 30);

	replayText.setFont(font);
	replayText.setCharacterSize(30);
	replayText.setString("Press Space to Begin Round\nOr Enter for Multiball");
	replayText.setColor(Color::White);
	replayText.setPosition(windWidth / 2 - 240, windHeight/2);

	winnerText.setFont(font);
	winnerText.setCharacterSize(40);
	winnerText.setString("PLAYER 2, You Win!");
	winnerText.setColor(Color::Green);
	winnerText.setPosition(windWidth / 2 - 210, windHeight / 2 - 70);

	myBall2.direction = -myBall.direction;

	// So the game starts paused
	myBall.lost = true;
	
	// Main game loop
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();
		update_state(dt);
		render_frame();
		window.display();
	}

	return 0;
}

// Updates the state of the game, moving all necessary objects, etc...
void update_state(float dt)
{
	// If player hits space, play normal mode
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (myBall.lost == true || myBall2.lost == true)) 
	{
		multiBall = false;
		sound.play();
		if (winner == true)
		{
			winner = false;
			myBall.scoreR = 0;
			myBall.scoreL = 0;
			myBall2.scoreR = 0;
			myBall2.scoreL = 0;
		}

		myBall.lost = false;
		myBall.reset();
		myBall2.lost = false;
		myBall2.reset();
		myBall2.direction = -myBall.direction;
		leftPaddle.reset();
		rightPaddle.reset();
	}

	// If player hits enter, multiball
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && (myBall.lost == true || myBall2.lost == true))
	{
		multiBall = true;
		if (winner == true)
		{
			winner = false;
			myBall.scoreR = 0;
			myBall.scoreL = 0;
			myBall2.scoreR = 0;
			myBall2.scoreL = 0;
		}

		myBall.lost = false;
		myBall.reset();
		myBall2.lost = false;
		myBall2.reset();
		myBall2.direction = -myBall.direction;
		leftPaddle.reset();
		rightPaddle.reset();
	}

	if (myBall.lost == false && myBall2.lost == false)
	{

		leftPaddle.move(dt, myBall.ball.getPosition(), myBall2.ball.getPosition(), multiBall);
		rightPaddle.move(dt, myBall.ball.getPosition(), myBall2.ball.getPosition(), multiBall);
        centerPaddle.move(dt, myBall.ball.getPosition(), myBall2.ball.getPosition(), multiBall);
		
		// If ball is in left side
		if (myBall.ball.getPosition().x < windWidth / 2)
		{
			myBall.move(dt, leftPaddle.paddle.getPosition(), leftPaddle.paddle.getSize(), centerPaddle.paddle.getPosition(), centerPaddle.paddle.getSize());
		}
		
		// If ball is in right side
		else
		{
			myBall.move(dt, rightPaddle.paddle.getPosition(), rightPaddle.paddle.getSize(), centerPaddle.paddle.getPosition(), centerPaddle.paddle.getSize());
		}
		
		// If ball 2 is in left side and multiball is active
		if (myBall2.ball.getPosition().x < windWidth / 2 && multiBall == true)
		{
			myBall2.move(dt, leftPaddle.paddle.getPosition(), leftPaddle.paddle.getSize(), centerPaddle.paddle.getPosition(), centerPaddle.paddle.getSize());
		}
		
		// If ball 2 is in right side and multiball is active
		else if (multiBall == true)
		{
			myBall2.move(dt, rightPaddle.paddle.getPosition(), rightPaddle.paddle.getSize(), centerPaddle.paddle.getPosition(), centerPaddle.paddle.getSize());
		}

		// Set scores based on ball class' score variables
		scoreL.setString(std::to_string(myBall.scoreL + myBall2.scoreL));
		scoreR.setString(std::to_string(myBall.scoreR + myBall2.scoreR));

		// If a player wins...
		if (myBall.scoreL + myBall2.scoreL >= 5)
		{
			winner = true;
			winnerText.setString("PLAYER 1, You Win!");
		}
		if (myBall.scoreR + myBall2.scoreR >= 5)
		{
			winner = true;
			winnerText.setString("PLAYER 2, You Win!");
		}
	}
	
}

// Renders the window, drawing everything within
void render_frame()
{
	window.clear(sf::Color(0, 0, 0, 255));
	window.draw(bg);
	
	// If neither player has lost, draw all of the game contents
	if (myBall.lost == false && myBall2.lost == false)
	{
		myBall.draw();
		if (multiBall == true)
		{
			myBall2.draw();
		}
		leftPaddle.draw();
		rightPaddle.draw();
        centerPaddle.draw();
	}

	// Otherwise, give them the replay promt
	else
	{
		window.draw(replayText);
	}

	if (winner == true)
	{
		window.draw(winnerText);
	}

	// Update the score
	window.draw(scoreL);
	window.draw(scoreR);
}