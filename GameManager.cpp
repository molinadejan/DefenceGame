#include "GameManager.h"

void GameManager::GetScore(int _score)
{
	score += _score;
}

TCHAR BUFFER[1024];

void GameManager::PrintID(HDC hdc) const
{
	_stprintf_s(BUFFER, _T("ID : %s"), id);

	RECT t = { 0, 0, 100, 0 };
	DrawText(hdc, BUFFER, _tcslen(BUFFER), &t, DT_TOP);
}

void GameManager::PrintScore(HDC hdc) const
{
	_stprintf_s(BUFFER, _T("Score : %d"), score);

	RECT t = { 0, 0, 100, 24 };
	DrawText(hdc, BUFFER, _tcslen(BUFFER), &t, DT_TOP);
}

void GameManager::PrintTest(HDC hdc) const
{
	if (state == STATE::GAME_START)
	{
		RECT t = { 450, 0, 600, 24 };
		DrawText(hdc, _T("GAME_START"), _tcslen(_T("GAME_START")), &t, DT_TOP);
	}
	else if (state == STATE::GAME_PLAY)
	{
		RECT t = { 450, 0, 600, 24 };
		DrawText(hdc, _T("GAME_PLAY"), _tcslen(_T("GAME_PLAY")), &t, DT_TOP);
	}
	else
	{
		RECT t = { 450, 0, 600, 24 };
		DrawText(hdc, _T("GAME_OVER"), _tcslen(_T("GAME_OVER")), &t, DT_TOP);
	}
}

GameManager::GameManager()
	: player(Player(PLAYER_START_POINT, PLAYER_RADIUS)), score(0), state(STATE::GAME_START)
{
	srand((unsigned int)time(NULL));
	
	id[3] = id[2] = id[1] = id[0] = 0;

	for (int i = 0; i < 10; ++i)
	{
		DefenceWall wall(WALL_START_POINT + Point { (double)WALL_WIDTH * i, 0 }, (double)WALL_WIDTH, (double)WALL_HEIGHT);
		walls.push_back(wall);
	}
}

void GameManager::Fire(RECT& rect)
{
	Bullet bullet(player.GetShotPoint(), player.GetDir(), BULLET_SPEED, BULLET_RADIUS);
	bullets.push_back(bullet);
}

void GameManager::CreateEnemy(RECT rect)
{
	Point newPoint = {rand() % rect.right, 0};
	Enemy enemy(newPoint, rand() % 3 + 1, ENEMY_RADIUS);
	enemies.push_back(enemy);
}

void GameManager::DrawGameStart(HDC hdc)
{
	RECT t1 = { 400, 488, 600, 512 };
	DrawText(hdc, ID_INPUT_TEXT, _tcslen(ID_INPUT_TEXT), &t1, DT_CENTER);

	DrawRectangle(hdc, {500, 524}, 120, 24);

	RECT t2 = { 400, 512, 600, 536 };
	DrawText(hdc, id, _tcslen(id), &t2, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

void GameManager::DrawGamePlay(HDC hdc)
{
	player.DrawPlayer(hdc);

	for (const DefenceWall& wall : walls)
		wall.Draw(hdc);

	for (const Bullet& bullet : bullets)
		bullet.Draw(hdc);

	for (const Enemy& enemy : enemies)
		enemy.Draw(hdc);

	PrintID(hdc);
	PrintScore(hdc);
}

void GameManager::DrawGameOver(HDC hdc)
{
	_stprintf_s(BUFFER, _T("현재 기록\n%s : %d"), id, score);
	RECT t = { 400, 300, 600, 348 };
	DrawText(hdc, BUFFER, _tcslen(BUFFER), &t, DT_TOP | DT_CENTER | DT_VCENTER);

	for (int i = 0; i < datas.size() && i < 3; ++i)
	{
		TCHAR tmp[4];
		_tcscpy_s(tmp, CA2T(datas[i].id.c_str()));
		_stprintf_s(BUFFER, _T("%d : %s %d"), i + 1, tmp, datas[i].score);
		RECT t = { 400, 348 + i * 24, 600, 348 + (i + 1) * 24 };
		DrawText(hdc, BUFFER, _tcslen(BUFFER), &t, DT_TOP | DT_CENTER | DT_VCENTER);
	}
}

bool GameManager::Update(RECT rect)
{
	// Enemy가 맨 아래로 도달했는지 체크
	for (int i = 0; i < enemies.size(); ++i)
		if (enemies[i].GetPos().y > 1000)
			return false;

	// 남은 wall이 있는지 체크
	if (walls.empty())
		return false;

	// Bullet과 Enemy 충돌 체크
	for (int i = 0; i < enemies.size();)
	{
		bool isCol = false;

		for (int j = 0; j < bullets.size();)
		{
			if (GetDistance(enemies[i].GetPos(), bullets[j].GetPos()) < enemies[i].GetRadius() + bullets[j].GetRadius())
			{
				enemies.erase(enemies.begin() + i);
				bullets.erase(bullets.begin() + j);

				GetScore(ENEMY_SCORE);

				isCol = true;

				break;
			}
			else ++j;
		}

		if (!isCol) ++i;
	}

	// Bullet과 윈도우 충돌 체크
	for (int i = 0; i < bullets.size(); ++i)
	{
		Point pos = bullets[i].GetPos();

		if (pos.x < BULLET_RADIUS || pos.x > rect.right - BULLET_RADIUS || pos.y < BULLET_RADIUS)
		{
			bullets.erase(bullets.begin() + i);
			--i;
		}
	}

	// Enemy와 Wall 충돌 체크
	for (int i = 0; i < enemies.size();)
	{
		bool isCol = false;

		Point ePos = enemies[i].GetPos();

		for (int j = 0; j < walls.size();)
		{
			Point wPos = walls[j].GetPos();

			if (abs(ePos.y - wPos.y) < enemies[i].GetRadius() + walls[j].GetHeight() / 2 &&
				abs(ePos.x - wPos.x) < enemies[i].GetRadius() + walls[j].GetWidth() / 2)
			{
				enemies.erase(enemies.begin() + i);
				walls.erase(walls.begin() + j);

				isCol = true;

				break;
			}
			else ++j;
		}

		if (!isCol) ++i;
	}


	for (Bullet& bullet : bullets)
		bullet.Update();

	for (Enemy& enemy : enemies)
		enemy.Update();

	return true;
}

void GameManager::OpenRecordFile()
{
	std::fstream openFile;

	openFile.open(FILEPATH, std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	if (openFile.is_open())
	{
		while (!openFile.eof())
		{
			std::string _id;
			int _score;

			openFile >> _id >> _score;
			ScoreData loadData = {_id, _score};
			datas.push_back(loadData);
		}

		std::sort(datas.begin(), datas.end(), &cmp);

		openFile.close();
	}

	openFile.open(FILEPATH, std::ios_base::out | std::ios_base::app | std::ios_base::binary);

	if (openFile.is_open())
	{
		openFile << id << " " << score << "\n";
		openFile.close();
	}
}
