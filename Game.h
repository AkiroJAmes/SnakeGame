#pragma once
#include "raymath.h"

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

	Vector2 m_offset;
};

