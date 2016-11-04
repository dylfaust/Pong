#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>

using namespace sf;
class Paddle
{
public:
	RectangleShape paddle;
	Vector2f position = { 0,0 };
	sf::Vector2f paddleSize = { 25, 100 };
	Vector2f ballPos = { 0,0 };
	float speed = 1;
	float windWidth;
	float windHeight;
	RenderWindow& myWindow;
	float paddleSpeed = 0;
	bool isAI;
    bool isCenterPaddle;
    bool goingUp;
	bool multiBall = false;

	Paddle(RenderWindow& window, bool isAI, bool isCenterBlock, float windWidth, float windHeight, float paddleSpeed);

	void move(float dt, Vector2f ballPos, Vector2f ballPos2, bool multiBall);
	void draw();
	void reset();
};