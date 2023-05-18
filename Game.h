#pragma once
#include "raymath.h"
#include "Snake.h"
#include "Food.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

protected:
	void Load();
	void Unload();

	void Update(float dt);
	void Draw();

protected:
	int m_gridHeight;
	int m_gridWidth;
	int m_cellSize;

	int m_tailCounter;

	Vector2 m_offset;

	float m_moveTimer;
	float m_moveTime;

	bool m_canMove;
	bool m_gameOver;

	Snake* m_snake;
	Vector2* m_snakePosition;
	Food m_food;
};

