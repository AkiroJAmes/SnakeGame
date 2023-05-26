#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

Game::Game() {
	m_gridHeight = 600;
	m_gridWidth = 450;
	m_cellSize = 31;

	m_moveTimer = 0.0f;
	m_moveTime = 0.1f;

	m_snake = { 0 };
	m_snakePosition = { 0 };
	m_offset = { 0 };
	m_food = { 0 };

	m_tailCounter = 0;
	m_score = 0;
	m_moveCounter = 0;

	m_canMove = false;
	m_gameOver = false;
}

Game::~Game() { }

void Game::Run() {
	SetTargetFPS(60);
	InitWindow(m_gridWidth, m_gridHeight + 200, "Snake Game");

	// Load scores from file
	m_ScoreManager.LoadScores();

	Load();
	while (!WindowShouldClose())
	{
		Update(GetFrameTime());
		Draw();
	}
	Unload();
	CloseWindow();
}
/// <summary>
/// Reset variables for new game
/// </summary>
void Game::Load() {
	const int SNAKE_MAX_SIZE = (m_gridHeight / m_cellSize) * (m_gridWidth / m_cellSize);
	m_snake = new Snake[SNAKE_MAX_SIZE];
	m_snakePosition = new Vector2[SNAKE_MAX_SIZE];

	m_tailCounter = 3;
	m_score = 0;
	m_scoreTarget = 0;
	m_moveCounter = 0;

	m_offset.x = float(m_gridWidth % m_cellSize);
	m_offset.y = float(m_gridHeight % m_cellSize);

	for (int i = 0; i < SNAKE_MAX_SIZE; i++) {
		m_snake[i].m_position = Vector2{ m_offset.x / 2, m_offset.y / 2 };
		m_snake[i].m_size = Vector2{ (float)m_cellSize, (float)m_cellSize };
		m_snake[i].m_speed = Vector2{ (float)31, 0 };

		if (i == 0) m_snake[i].m_colour = DARKGREEN;
		else if (i % 2 == 0) m_snake[i].m_colour = Color{ 0, 170, 46, 255 };
		else m_snake[i].m_colour = GREEN;

		m_snakePosition[i] = Vector2{ 0, 0 };
	}

	m_food.m_size = Vector2{ (float)m_cellSize, (float)m_cellSize };
	m_food.m_colour = RED;
	m_food.m_active = false;
}

void Game::Unload() { }

void Game::Update(float deltaTime) {
	if (!m_gameOver) {
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
		m_scoreTimer += deltaTime;

		for (int i = 0; i < m_tailCounter; i++) m_snakePosition[i] = m_snake[i].m_position;

		if (m_scoreTarget > m_score && m_scoreTimer >= 0.03f) {
			m_scoreTimer = 0;
			m_score++;
		}

		if (m_moveTimer <= 0) {
			m_moveTimer = m_moveTime;
			m_moveCounter++;

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

			// Collision check with self
			for (int i = 1; i < m_tailCounter; i++)
				if (m_snake[0].m_position.x == m_snake[i].m_position.x && m_snake[0].m_position.y == m_snake[i].m_position.y) {
					m_gameOver = true;
					m_score = m_scoreTarget;
					m_ScoreManager.UpdateScores(m_score);
				}
		}

		// Spawn new food
		if (!m_food.m_active) {
			m_food.m_active = true;
			m_food.m_position = Vector2{ GetRandomValue(0, (m_gridWidth / m_cellSize) - 1) * m_cellSize + m_offset.x / 2, GetRandomValue(0, (m_gridHeight / m_cellSize) - 1) * m_cellSize + m_offset.y / 2 };

			for (int i = 0; i < m_tailCounter; i++)
			{
				while ((m_food.m_position.x == m_snake[i].m_position.x) && (m_food.m_position.y == m_snake[i].m_position.y))
				{
					m_food.m_position = Vector2{ GetRandomValue(0, (m_gridWidth / m_cellSize) - 1) * m_cellSize + m_offset.x / 2, GetRandomValue(0, (m_gridHeight / m_cellSize) - 1) * m_cellSize + m_offset.y / 2 };
					i = 0;
				}
			}
		}

		// Snek ate the food, make him grow!
		if ((m_snake[0].m_position.x < (m_food.m_position.x + m_food.m_size.x) && (m_snake[0].m_position.x + m_snake[0].m_size.x) > m_food.m_position.x) &&
			(m_snake[0].m_position.y < (m_food.m_position.y + m_food.m_size.y) && (m_snake[0].m_position.y + m_snake[0].m_size.y) > m_food.m_position.y))
		{
			m_snake[m_tailCounter].m_position = m_snakePosition[m_tailCounter - 1];

			float calculatedScore = (1 * CalculateScoreMulti());

			m_scoreTarget += calculatedScore;


			m_tailCounter++;
			m_food.m_active = false;
			m_moveCounter = 0;
		}
	}
	else {
		if (IsKeyPressed(KEY_ENTER)) {
			Load();
			m_gameOver = false;
		}
	}
}

/// <summary>
/// Calculate score multiplier based on moves to food
/// </summary>
/// <returns></returns>
float Game::CalculateScoreMulti() {
	int x = m_moveCounter;
	float a = -0.4f;
	float b = 100.0f;
	float c = 18.0f;
	float d = -8.1f;

	float multi = round((pow((x - d), a) * b - c) * 100.0f) / 100.0f;

	return multi > 1 ? multi : 1.00f;
}

void Game::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	// Score convertion
	std::string scoreText, moveText, multiText, sizeText;
	scoreText = std::to_string(m_score);
	moveText = "Moves taken: " + std::to_string(m_moveCounter);

	std::stringstream stream;
	stream.precision(2);
	stream << std::fixed;
	stream << CalculateScoreMulti();

	multiText = "Score multi: " + stream.str() + "x";
	sizeText = "Food consumed: " + std::to_string(m_tailCounter - 3);


	if (!m_gameOver) {
		for (int i = 0; i < m_gridWidth / m_cellSize + 1; i++)
			DrawLineV(Vector2{ m_cellSize * i + m_offset.x / 2, m_offset.y / 2 }, Vector2{ m_cellSize * i + m_offset.x / 2, m_gridHeight - m_offset.y / 2 }, LIGHTGRAY);

		for (int i = 0; i < m_gridHeight / m_cellSize + 1; i++)
			DrawLineV(Vector2{ m_offset.x / 2, m_cellSize * i + m_offset.y / 2 }, Vector2{ m_gridWidth - m_offset.x / 2, m_cellSize * i + m_offset.y / 2 }, LIGHTGRAY);

		DrawRectangleV(m_food.m_position, m_food.m_size, m_food.m_colour);

		for (int i = m_tailCounter - 1; i >= 0; i--) DrawRectangleV(m_snake[i].m_position, m_snake[i].m_size, m_snake[i].m_colour);

		//DrawText("Stage 1 -- Sussy Vents", m_gridWidth / 4, m_gridHeight + 50, 20, BLACK);

		DrawText(scoreText.c_str(), GetScreenWidth() / 2 - MeasureText(scoreText.c_str(), 40) / 2, m_gridHeight + 15, 40, BLACK);
		DrawText(sizeText.c_str(), GetScreenWidth() / 2 - 90, m_gridHeight + 80, 20, GRAY);
		DrawText(moveText.c_str(), GetScreenWidth() / 2 - 90, m_gridHeight + 110, 20, GRAY);
		DrawText(multiText.c_str(), GetScreenWidth() / 2 - 90, m_gridHeight + 140, 20, GRAY);
	}
	else {
		DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 30) / 2, GetScreenHeight() / 5, 30, GRAY);
		DrawText(scoreText.c_str(), GetScreenWidth() / 2 - MeasureText(scoreText.c_str(), 40) / 2, GetScreenHeight() / 5 + 40, 40, GRAY);
		DrawText(sizeText.c_str(), GetScreenWidth() / 2 - MeasureText(sizeText.c_str(), 30) / 2, GetScreenHeight() / 5 + 90, 30, GRAY);

		DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() - 50, 20, GRAY);

		int* scores = m_ScoreManager.GetScores();

		for (int i = 1; i < m_ScoreManager.ScoreSize() + 1; i++) {
			// Text formatting
			std::string text;
			text = std::to_string(i) + ". " + (scores[i - 1] <= 0 ? "---" : std::to_string(scores[i - 1]));

			Color textColour = GRAY;

			DrawText(text.c_str(), GetScreenWidth() / 2 - 40, GetScreenHeight() / 2 + 60 + (i * 28), 30, textColour);
		}

	}

	EndDrawing();
}