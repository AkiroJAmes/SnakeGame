#pragma once
class ScoreManager
{
public:
	ScoreManager();
	~ScoreManager();

	/// <summary>
	/// Read scores once and store locally
	/// </summary>
	void LoadScores();
	/// <summary>
	/// Update local scores, if there is a new score write back to file
	/// </summary>
	/// <param name="score"></param>
	void UpdateScores(int score);
	/// <summary>
	/// Return the current scores
	/// </summary>
	/// <returns></returns>
	int* GetScores();
	/// <summary>
	/// Get the current max score size
	/// </summary>
	/// <returns></returns>
	int ScoreSize();


protected:
	/// <summary>
	/// Save scores to local file
	/// </summary>
	void SaveScores();

protected:
	static const int MAX_SCORE_SIZE = 3;
	int* m_scores;
};

