#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <fstream>

Game::Game() {
	m_gridHeight = 600;
	m_gridWidth = 450;
	m_cellSize = 31;

	m_moveTimer = 0.0f;
	m_moveTime = 0.2f;

	m_snake = { 0 };
	m_snakePosition = { 0 };
	m_offset = { 0 };
	m_food = { 0 };

	m_tailCounter = 0;
	m_score = 0;
	m_finalScore = 0;

	m_canMove = false;
	m_gameOver = false;
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

	m_tailCounter = 3;
	m_score = 0;
	m_finalScore = 0;

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

			for (int i = 1; i < m_tailCounter; i++)
				if (m_snake[0].m_position.x == m_snake[i].m_position.x && m_snake[0].m_position.y == m_snake[i].m_position.y) {
					m_gameOver = true;
					m_finalScore = m_score;
				}
		}

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

		if ((m_snake[0].m_position.x < (m_food.m_position.x + m_food.m_size.x) && (m_snake[0].m_position.x + m_snake[0].m_size.x) > m_food.m_position.x) &&
			(m_snake[0].m_position.y < (m_food.m_position.y + m_food.m_size.y) && (m_snake[0].m_position.y + m_snake[0].m_size.y) > m_food.m_position.y))
		{
			m_snake[m_tailCounter].m_position = m_snakePosition[m_tailCounter - 1];
			m_score++;
			m_tailCounter++;
			m_food.m_active = false;
		}
	}
	else {
		if(IsKeyPressed(KEY_ENTER)) {
			Load();
			m_gameOver = false;
		}
	}
}

void Game::LoadHighscores() {
	char fileName[] = "Highscore.amongus";
	std::fstream highscoreFile;
	int scorePos = 1;

	highscoreFile.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

	// Create file if it doesnt exist
	if(!highscoreFile) {
		std::cout << "Highscore file not found, creating a new file..." << std::endl;

		highscoreFile.open(fileName, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

		int defaultScore = 0;
		highscoreFile.write(reinterpret_cast<const char*>(&defaultScore), sizeof(int));
		highscoreFile.write(reinterpret_cast<const char*>(&defaultScore), sizeof(int));
		highscoreFile.write(reinterpret_cast<const char*>(&defaultScore), sizeof(int));
	}

	highscoreFile.seekg(0);

	while (true) {
		int score = 0;
		highscoreFile.read((char*)&score, sizeof(int));

		if (m_score > score) {
			std::cout << "Writing new score to file..." << std::endl;
			int pos = highscoreFile.tellg();
			highscoreFile.seekg(pos - sizeof(int));
			highscoreFile.write(reinterpret_cast<const char*>(&m_score), sizeof(int));

			m_score = score;
			highscoreFile.seekg(0);
			continue;
		}

		std::string text;
		text = std::to_string(scorePos) + ". " + (score <= 0 ? "---" : std::to_string(score));

		DrawText(text.c_str(), GetScreenWidth() / 2 - 2, GetScreenHeight() / 2 + 60 + (scorePos * 10), 10, GRAY);
		scorePos++;

		if(scorePos > 3)
			break;
	}

	highscoreFile.close();
}

void Game::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	// Score convertion
	std::string scoreText;
	scoreText = std::to_string(m_score);


	if (!m_gameOver) {
		for (int i = 0; i < m_gridWidth / m_cellSize + 1; i++)
			DrawLineV(Vector2{ m_cellSize * i + m_offset.x / 2, m_offset.y / 2 }, Vector2{ m_cellSize * i + m_offset.x / 2, m_gridHeight - m_offset.y / 2 }, LIGHTGRAY);

		for (int i = 0; i < m_gridHeight / m_cellSize + 1; i++)
			DrawLineV(Vector2{ m_offset.x / 2, m_cellSize * i + m_offset.y / 2 }, Vector2{ m_gridWidth - m_offset.x / 2, m_cellSize * i + m_offset.y / 2 }, LIGHTGRAY);

		DrawRectangleV(m_food.m_position, m_food.m_size, m_food.m_colour);

		for (int i = m_tailCounter - 1; i >= 0; i--) DrawRectangleV(m_snake[i].m_position, m_snake[i].m_size, m_snake[i].m_colour);

		//DrawText("Stage 1 -- Sussy Vents", m_gridWidth / 4, m_gridHeight + 50, 20, BLACK);

		DrawText(scoreText.c_str(), 10, m_gridHeight + 15, 10, BLACK);
	}
	else {
		scoreText = std::to_string(m_finalScore);

		DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 30) / 2, GetScreenHeight() / 5, 30, GRAY);
		DrawText(scoreText.c_str(), GetScreenWidth() / 2 - MeasureText(scoreText.c_str(), 40) / 2, GetScreenHeight() / 5 + 40, 40, GRAY);
		DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 + 50, 20, GRAY);

		LoadHighscores();

	}

	EndDrawing();
}
