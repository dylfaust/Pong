#pragma once

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

using namespace sf;
class Ball
{
public:
	CircleShape ball;
	float radius = 12;
	float pi = 2.141593;
	Vector2f position = { 0,0 };
	Vector2f direction = { 0,0 };
	Vector2f paddlePos = {0,0};
	RenderWindow& myWindow;
	float speed = 100;
	float startingSpeed = 0;
	int scoreL = 0;
	int scoreR = 0;
	bool lost = false;
	float angle = 0;
	float base = 0.0f;

	// Sounds
	SoundBuffer bounce;
	Sound sound;

	Ball(RenderWindow& window);

	void move(float dt, Vector2f paddlePos, Vector2f paddleSize);
	void draw();
	void reset();
	void hit();
};