#pragma once

#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "framework.h"

#include "Player.h"
#include "DefenceWall.h"
#include "Bullet.h"
#include "Enemy.h"

enum STATE { GAME_START, GAME_PLAY, GAME_OVER };

class GameManager
{
private:

	struct ScoreData
	{
		std::string id;
		int score;
	};

	static bool cmp(const ScoreData& d1, const ScoreData& d2)
	{
		return d1.score > d2.score;
	}

	const Point PLAYER_START_POINT = {500, 875};
	const Point WALL_START_POINT = { 50, 900 };
	//const Point ENEMY_SPAWN_POINT = { 25, 0 };

	const double ENEMY_RADIUS = 20;

	const int WALL_WIDTH = 100;
	const int WALL_HEIGHT = 25;

	const double PLAYER_RADIUS = 50;
	const int START_WALL_CNT = 10;

	const int BULLET_SPEED = 5;
	const int BULLET_RADIUS = 5;

	const int ENEMY_SCORE = 10;

	const TCHAR *ID_INPUT_TEXT = _T("INPUT YOUR ID (3 char)");
	const std::string FILEPATH = "record.txt";

	Player player;
	int score;

	std::vector<DefenceWall> walls;
	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;
	std::vector<ScoreData> datas;

	void GetScore(int _score);

	void PrintID(HDC hdc) const;
	void PrintScore(HDC hdc) const;

public:

	void PrintTest(HDC hdc) const;

	// ���� ���� ����
	STATE state;

	// ���̵�
	TCHAR id[4];

	// ������
	GameManager();

	// �÷��̾� ��ü�� ��ȯ�մϴ�.
	Player& GetPlayer() { return player; }

	// �Ѿ��� �߻��մϴ�.
	void Fire(RECT &rect);

	// ���ο� ���� �����մϴ�.
	void CreateEnemy(RECT rect);

	// ���� �ʱ� ȭ���� �׸��ϴ�.
	void DrawGameStart(HDC hdc);

	// ���� �÷��� �� ������Ʈ���� �׸��ϴ�.
	void DrawGamePlay(HDC hdc);

	// ���� ���� �� ȭ���� �׸��ϴ�.
	void DrawGameOver(HDC hdc);

	// ������Ʈ���� ������Ʈ �մϴ�.
	bool Update(RECT rect);

	// ���������� ���ϴ�.
	void OpenRecordFile();
};

#endif // !__GAMEMANAGER_H__

