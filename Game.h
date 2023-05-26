#pragma once
#include "raymath.h"
#include "Snake.h"
#include "Food.h"
#include "ScoreManager.h"
#include <string>

class Game
{
public:
	Game();
	~Game();

	void Run();

protected:
	void Load();
	void Unload();

	void LoadHighscores();
	float CalculateScoreMulti();

	void Update(float dt);
	void Draw();

protected:
	int m_gridHeight;
	int m_gridWidth;
	int m_cellSize;

	int m_tailCounter;
	int m_score;
	int m_scoreTarget;
	int m_moveCounter;

	Vector2 m_offset;

	float m_moveTimer;
	float m_moveTime;
	float m_scoreTimer;

	bool m_canMove;
	bool m_gameOver;

	Snake* m_snake;
	Vector2* m_snakePosition;
	Food m_food;

	ScoreManager m_ScoreManager;
};

