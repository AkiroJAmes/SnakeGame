#pragma once
#include "raymath.h"
#include "Snake.h"

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

	Snake m_snake[256] = { };
	Vector2 m_snakePosition[256] = { };
};

