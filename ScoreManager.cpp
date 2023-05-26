#include "ScoreManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

ScoreManager::ScoreManager() {
	m_scores = new int[MAX_SCORE_SIZE];
}

ScoreManager::~ScoreManager() { }

void ScoreManager::LoadScores() {
	char fileName[] = "Highscore.amongus";
	std::fstream highscoreFile;
	int scorePos = 1;

	highscoreFile.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

	// Create file if it doesnt exist
	if (!highscoreFile) {
		std::cout << "Highscore file not found, creating a new file..." << std::endl;

		highscoreFile.open(fileName, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

		// Add default scores to newly created file
		int defaultScore[] = { 300, 150, 0 };
		highscoreFile.write(reinterpret_cast<const char*>(&defaultScore[0]), sizeof(int));
		highscoreFile.write(reinterpret_cast<const char*>(&defaultScore[1]), sizeof(int));
		highscoreFile.write(reinterpret_cast<const char*>(&defaultScore[2]), sizeof(int));
	}
	else {
		std::cout << "Found highscore file, opening..." << std::endl;
	}

	highscoreFile.seekg(0);

	// Read and store scores
	for (int i = 0; i < MAX_SCORE_SIZE; i++) {
		highscoreFile.read((char*)&m_scores[i], sizeof(int));
	}

	highscoreFile.close();
}

void ScoreManager::UpdateScores(int score) {
	// Only write to file if the scores are actually changing
	bool writeToFile = false;

	for (int i = 0; i < MAX_SCORE_SIZE; i++) {
		// Reorder scores
		if (score > m_scores[i]) {
			int tempScore = m_scores[i];
			m_scores[i] = score;
			score = tempScore;

			writeToFile = true;
		}
	}

	if(writeToFile)
		SaveScores();
}

int * ScoreManager::GetScores() {
	return m_scores;
}

int ScoreManager::ScoreSize() {
	return MAX_SCORE_SIZE;
}

void ScoreManager::SaveScores() {
	char fileName[] = "Highscore.amongus";
	std::fstream highscoreFile;

	highscoreFile.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

	// Regenerate and restore scores if the file is somehow missing upon saving a score
	if (!highscoreFile) {
		std::cout << "Highscore file not found when attempting to write to file..." << std::endl;

		int tempScores[MAX_SCORE_SIZE];

		// Save a copy of the array before file regen
		for (int i = 0; i < MAX_SCORE_SIZE; i++)
			tempScores[i] = m_scores[i];

		// Reload the scores
		LoadScores();

		// Load original scores back
		for (int i = 0; i < MAX_SCORE_SIZE; i++)
			m_scores[i] = tempScores[i];
	}
	
	std::cout << "Highscore file found, updating scores..." << std::endl;

	// Write scores to file
	for (int i = 0; i < MAX_SCORE_SIZE; i++) {
		highscoreFile.write(reinterpret_cast<const char*>(&m_scores[i]), sizeof(int));
	}

	highscoreFile.close();
}