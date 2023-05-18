#include "Game.h"
#include "raylib.h"
#include <iostream>

Game::Game() {
	m_gridHeight = 600;
	m_gridWidth = 450;
	m_cellSize = 31;

	m_moveTimer = 0.0f;
	m_moveTime = 0.2f;

	m_snake = { 0 };
	m_snakePosition = { 0 };
	m_offset = { 0 };

	m_tailCounter = 5;
	m_canMove = false;
}

Game::~Game() { }

void Game::Run() {
	SetTargetFPS(60);
	InitWindow(m_gridWidth, m_gridHeight + 200, "Snake Game");
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
	const int SNAKE_MAX_SIZE = (m_gridHeight / m_cellSize) * (m_gridWidth / m_cellSize);
	m_snake = new Snake[SNAKE_MAX_SIZE];
	m_snakePosition = new Vector2[SNAKE_MAX_SIZE];

	m_offset.x = float(m_gridWidth % m_cellSize);
	m_offset.y = float(m_gridHeight % m_cellSize);

	for (int i = 0; i < SNAKE_MAX_SIZE; i++) {
		m_snake[i].m_position = Vector2{ m_offset.x / 2, m_offset.y / 2 };
		m_snake[i].m_size = Vector2{ (float)m_cellSize, (float)m_cellSize };
		m_snake[i].m_speed = Vector2{ (float)31, 0 };

		if (i == 0) m_snake[i].m_colour = DARKGREEN;
		else if (i % 2 == 0) m_snake[i].m_colour = Color { 0, 170, 46, 255 };
		else m_snake[i].m_colour = GREEN;

		m_snakePosition[i] = Vector2{0, 0};
	}
}

void Game::Unload() { }

void Game::Update(float deltaTime) { 
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

	// Timer for movement to limit snake speed
	m_moveTimer -= deltaTime;

	for (int i = 0; i < m_tailCounter; i++) m_snakePosition[i] = m_snake[i].m_position;

	if (m_moveTimer <= 0) {
		m_moveTimer = m_moveTime;

		for (int i = 0; i < m_tailCounter; i++) {
			if (i == 0) {
				m_snake[0].m_position.x += m_snake[0].m_speed.x;
				m_snake[0].m_position.y += m_snake[0].m_speed.y;
				m_canMove = true;
			}
			else m_snake[i].m_position = m_snakePosition[i - 1];
			
			// Transition snake to other side of grid when reaching the edge
			if (m_snake[i].m_position.x < 0) m_snake[i].m_position.x = m_gridWidth - (m_offset.x / 2) - m_cellSize;
			if (m_snake[i].m_position.x > m_gridWidth - m_offset.x / 2 - m_cellSize) m_snake[i].m_position.x = m_offset.x / 2;
			if (m_snake[i].m_position.y < 0) m_snake[i].m_position.y = m_gridHeight - (m_offset.y / 2) - m_cellSize;
			if (m_snake[i].m_position.y > m_gridHeight - m_offset.y / 2 - m_cellSize) m_snake[i].m_position.y = m_offset.y / 2;
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

	for (int i = 0; i < m_tailCounter; i++) DrawRectangleV(m_snake[i].m_position, m_snake[i].m_size, m_snake[i].m_colour);

	DrawText("Stage 1 -- Sussy Vents", m_gridWidth / 4, m_gridHeight + 50, 20, BLACK);

	EndDrawing();
}
