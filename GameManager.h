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

	// 게임 진행 상태
	STATE state;

	// 아이디
	TCHAR id[4];

	// 생성자
	GameManager();

	// 플레이어 객체를 반환합니다.
	Player& GetPlayer() { return player; }

	// 총알을 발사합니다.
	void Fire(RECT &rect);

	// 새로운 적을 생성합니다.
	void CreateEnemy(RECT rect);

	// 게임 초기 화면을 그립니다.
	void DrawGameStart(HDC hdc);

	// 게임 플레이 시 오브젝트들을 그립니다.
	void DrawGamePlay(HDC hdc);

	// 게임 오버 시 화면을 그립니다.
	void DrawGameOver(HDC hdc);

	// 오브젝트들을 업데이트 합니다.
	bool Update(RECT rect);

	// 저장파일을 엽니다.
	void OpenRecordFile();
};

#endif // !__GAMEMANAGER_H__

