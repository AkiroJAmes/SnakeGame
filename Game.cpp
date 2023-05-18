#include "Game.h"
#include "raylib.h"
#include <iostream>

Game::Game() {
	m_gridHeight = 800;
	m_gridWidth = 450;
	m_cellSize = 31;

	m_moveTime = 0.2f;

	m_tailCounter = 5;

	m_canMove = false;
}

Game::~Game() { }

void Game::Run() {
	SetTargetFPS(60);
	InitWindow(m_gridWidth, m_gridHeight, "Retro Snake Game");
	Load();
	while (!WindowShouldClose())
	{
		Update(GetFrameTime());
		Draw();
	}
	Unload();
	CloseWindow();
}

void Game::Load() { 
	m_offset.x = m_gridWidth % m_cellSize;
	m_offset.y = m_gridHeight % m_cellSize;

	for (int i = 0; i < 256; i++) {
		m_snake[i].m_position = Vector2{ m_offset.x / 2, m_offset.y / 2 };
		m_snake[i].m_size = Vector2{ (float)m_cellSize, (float)m_cellSize };
		m_snake[i].m_speed = Vector2{ (float)31, 0 };

		if (i == 0) m_snake[i].m_colour = DARKGREEN;
		else if (i % 2 == 0) m_snake[i].m_colour = YELLOW;
		else m_snake[i].m_colour = GREEN;

		m_snakePosition[i] = Vector2{0, 0};
	}
}

void Game::Unload() { }

void Game::Update(float dt) { 
	if (IsKeyPressed(KEY_RIGHT) && (m_snake[0].m_speed.x == 0) && m_canMove)
	{
		m_snake[0].m_speed = Vector2{ (float)m_cellSize, 0 };
		m_canMove = false;
	}
	if (IsKeyPressed(KEY_LEFT) && (m_snake[0].m_speed.x == 0) && m_canMove)
	{
		m_snake[0].m_speed = Vector2{ -(float)m_cellSize, 0 };
		m_canMove = false;
	}
	if (IsKeyPressed(KEY_UP) && (m_snake[0].m_speed.y == 0) && m_canMove)
	{
		m_snake[0].m_speed = Vector2{ 0, -(float)m_cellSize };
		m_canMove = false;
	}
	if (IsKeyPressed(KEY_DOWN) && (m_snake[0].m_speed.y == 0) && m_canMove)
	{
		m_snake[0].m_speed = Vector2{ 0, (float)m_cellSize };
		m_canMove = false;
	}

	for (int i = 0; i < m_tailCounter; i++)
	{
		m_snakePosition[i] = m_snake[i].m_position;
	}

	m_moveTimer -= dt;

	if (m_moveTimer <= 0) {
		m_moveTimer = m_moveTime;
		for (int i = 0; i < m_tailCounter; i++)
		{
			if (i == 0)
			{
				m_snake[0].m_position.x += m_snake[0].m_speed.x;
				m_snake[0].m_position.y += m_snake[0].m_speed.y;
				m_canMove = true;
			}
			else m_snake[i].m_position = m_snakePosition[i - 1];
		}
	}
}

void Game::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (int i = 0; i < m_gridWidth / m_cellSize + 1; i++)
		DrawLineV(Vector2 { m_cellSize* i + m_offset.x / 2, m_offset.y / 2 }, Vector2 { m_cellSize* i + m_offset.x / 2, m_gridHeight - m_offset.y / 2 }, LIGHTGRAY);

	for (int i = 0; i < m_gridHeight / m_cellSize + 1; i++)
		DrawLineV(Vector2 { m_offset.x / 2, m_cellSize* i + m_offset.y / 2 }, Vector2 { m_gridWidth - m_offset.x / 2, m_cellSize* i + m_offset.y / 2 }, LIGHTGRAY);

	for (int i = 0; i < m_tailCounter; i++)
	{
		DrawRectangleV(m_snake[i].m_position, m_snake[i].m_size, m_snake[i].m_colour);
	}

	EndDrawing();
}
