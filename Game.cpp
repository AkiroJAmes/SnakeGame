#include "Game.h"
#include "raylib.h"
#include <iostream>

Game::Game() {
	m_gridHeight = 800;
	m_gridWidth = 450;
	m_cellSize = 31;

	m_offset.x = m_gridWidth % m_cellSize;
	m_offset.y = m_gridHeight % m_cellSize;
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

void Game::Load() { }

void Game::Unload() { }

void Game::Update(float dt) { }

void Game::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (int i = 0; i < m_gridWidth / m_cellSize + 1; i++)
		DrawLineV(Vector2 { m_cellSize* i + m_offset.x / 2, m_offset.y / 2 }, Vector2 { m_cellSize* i + m_offset.x / 2, m_gridHeight - m_offset.y / 2 }, LIGHTGRAY);

	for (int i = 0; i < m_gridHeight / m_cellSize + 1; i++)
		DrawLineV(Vector2 { m_offset.x / 2, m_cellSize* i + m_offset.y / 2 }, Vector2 { m_gridWidth - m_offset.x / 2, m_cellSize* i + m_offset.y / 2 }, LIGHTGRAY);

	EndDrawing();
}
