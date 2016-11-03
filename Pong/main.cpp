/*

 Name : PONG
 Author: Dylan Faust
 Date : 11/3/16
 Purpose: To rebuild the classic game PONG in SFML and C++
 
 
 GOALS: 
 
[X] A - When the program is run, a window is displayed. 
The window can be moved around without freezing. 
When the window is closed, the application exits.

[X] B - There is a "paddle" on one edge of the screen. 
The player can control the paddle using the keyboard and/or the mouse. 
The game prevents the paddle from leaving the playing area.

[X] C - There is another "paddle" on the other edge of the screen. This paddle is controlled by an "A.I." player. The A.I. must attempt to block the ball, but it must also not play perfectly.

[ ] D - There is also a "ball" that starts in the middle of the screen. It starts flying towards one of the screen edges occupied by a player.

[X] E - Both the "ball" and "paddles" are implemented using C++ classes. Both paddles must use the same class. The ball class must be responsible for tracking and updating the ball's location, speed, and direction, as well as drawing the ball to the screen. The paddle class must be responsible for tracking the paddle's location as well as drawing the paddle to the screen.

[X] F - When the ball hits one of the player's paddles, it bounces off towards the other player. The angle at which it bounces off must depend on where on the paddle the ball hit.

[X] G - Every time the ball hits one of the player's paddles, it increases in speed by a small amount.

[X] H -If the ball hits an edge of the screen that is not occupied by a player's paddle, then the ball bounces off that edge.

[X] I - If the ball goes past the player's paddle and off their edge of the screen, a point is awarded to the other player. The ball then resets back to the middle and starts flying again towards a player. The game displays the score for each player at all times.

[X] J - The game displays at least one image texture (such as a sprite for the paddle or ball, or an image for the background) that is loaded from a file.

[ ] K - The game plays at least one sound effect (such as when the ball hits a paddle or screen edge, or when a point is scored) that is loaded from a file.

[X] L - When one player scores 5 points, that player is declared the winner. The player can then press the spacebar to play a new game.

Extra

[/] O - There is an obstacle in the middle of the playing area that the ball will bounce off of when hit.

[ ] s - 
The game has an absurd, Peggle-esque attention to detail. Just about everything that can happen in the game has fun visual and sound effects to go along with it. To get the full 15 points for this bonus objective, you must have all of these things:
Visual and sound effects when the game starts
Visual and sound effects when the ball bounces off the edges of the screen
Visual and sound effects when the ball bounces off a player's paddle
Visual and sound effects when a player scores a point.
Visual and sound effects when a player has won.
Visual effects (particle trail?) as the ball moves.
Visual effects (particle trail?) as the paddles move.
Visual and sound effects for any other bonus objectives, such as hitting an obstacle, the black-hole/repulsor, or powerups.
An awesome musical soundtrack.

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
void update_state(float dt);
void render_frame();
RenderWindow window;
//CircleShape ball;
Vector2f vel(-300, 600);

bool winner = false;

Texture tex;

float windWidth = 800;
float windHeight = 600;

Ball myBall = Ball(window);
Paddle leftPaddle = Paddle(window, false, false, windWidth, windHeight, 500.0f);
Paddle rightPaddle = Paddle(window, true, false, windWidth, windHeight, 400.0f);
Paddle centerPaddle = Paddle(window, true, true, windWidth, windHeight, 300.0f);

RectangleShape bg;
RectangleShape centerBlock;

// Sounds
SoundBuffer buf;
Sound sound;

sf::Text scoreL;
sf::Text scoreR;
sf::Text replayText;
Text winnerText;
// select the font
Font font;

//The paddle class must be responsible for tracking the paddle's location as well as drawing the paddle to the screen.

int main()
{
	buf.loadFromFile("pistol.wav");
	sound.setBuffer(buf);

	window.create(VideoMode(windWidth, windHeight), "SFML Example");
	Clock clock;
	tex.loadFromFile("Court.png");

	bg.setSize(Vector2f(windWidth, windHeight));
	bg.setPosition(0, 0);
	//shape.setFillColor(Color::Red);
	bg.setTexture(&tex);
	bg.setPosition(0, 0);

	centerBlock.setSize(Vector2f(50, 50));
	centerBlock.setFillColor(sf::Color(250, 250, 250));
	centerBlock.setPosition(windWidth/2-30, windHeight/2);

	
	//shape.setFillColor(Color::Red);
	//bg.setTexture(&tex);

	font.loadFromFile("Gameplay.TTF");
	scoreL.setFont(font);
	scoreL.setCharacterSize(50);
	scoreL.setString("0");
	scoreL.setColor(Color::White);
	scoreL.setPosition(windWidth/2 - 50, 30);
	scoreR.setFont(font);
	scoreR.setCharacterSize(50);
	scoreR.setString("0");
	scoreR.setColor(Color::White);
	scoreR.setPosition(windWidth/2 + 50, 30);

	replayText.setFont(font);
	replayText.setCharacterSize(30);
	replayText.setString("Press Space to Restart");
	replayText.setColor(Color::White);
	replayText.setPosition(windWidth / 2 - 220, windHeight/2);

	winnerText.setFont(font);
	winnerText.setCharacterSize(40);
	winnerText.setString("PLAYER 2, You Win!");
	winnerText.setColor(Color::Green);
	winnerText.setPosition(windWidth / 2 - 210, windHeight / 2 - 70);
	
	// inside the main loop, between window.clear() and window.display()


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
void update_state(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && myBall.lost == true) 
	{
		sound.play();
		if (winner == true)
		{
			winner = false;
			myBall.scoreR = 0;
			myBall.scoreL = 0;

		}

		myBall.lost = false;
		myBall.reset();
		leftPaddle.reset();
		rightPaddle.reset();
	}

	if (myBall.lost == false)
	{

		leftPaddle.move(dt, myBall.ball.getPosition());
		rightPaddle.move(dt, myBall.ball.getPosition());
        centerPaddle.move(dt, myBall.ball.getPosition());
		// If ball is in left side
		if (myBall.ball.getPosition().x < windWidth / 2)
		{
			//std::cout << "right";
			myBall.move(dt, leftPaddle.paddle.getPosition(), leftPaddle.paddle.getSize(), centerPaddle.paddle.getPosition(), centerPaddle.paddle.getSize());
		}
		// If ball is in right side
		else
		{
			myBall.move(dt, rightPaddle.paddle.getPosition(), rightPaddle.paddle.getSize());
		}

		scoreL.setString(std::to_string(myBall.scoreL));
		scoreR.setString(std::to_string(myBall.scoreR));

		if (myBall.scoreL == 5)
		{
			winner = true;
			winnerText.setString("PLAYER 1, You Win!");
		}
		if (myBall.scoreR == 5)
		{
			winner = true;
			winnerText.setString("PLAYER 2, You Win!");
		}
	}
	
}
void render_frame()
{
	window.clear(sf::Color(0, 0, 0, 255));
	window.draw(bg);
	if (myBall.lost == false)
	{
		//window.draw(centerBlock);
		myBall.draw();
		leftPaddle.draw();
		rightPaddle.draw();
        centerPaddle.draw();
	}
	else
	{
		window.draw(replayText);
	}

	if (winner == true)
	{
		window.draw(winnerText);
	}

	window.draw(scoreL);
	window.draw(scoreR);
	//window.draw(text);
}