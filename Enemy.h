#pragma once
#include "WorldTransform.h"
#include "EnemyBullet.h"

//自機クラスの前方宣言
class Player;

class Enemy
{
public:
	
	Enemy();
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新
	void Update();

	void Shot();
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }
	//描画
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//入力
	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	//行動フェーズ
	enum class Phase {
		Approach,	//接近する
		Leave,		//離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;
	
	Vector3 speed = { 0,0,-0.1f };

	void phase_Approach();
	void phese_ApproachIni();
	void phase_Leave();

	//メンバ関数ポインタのテーブル
	static void (Enemy::* spFuncTable[])();

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//ShotTimer
	static const int kFireInterval = 60;
	int32_t shotCoolTime = 0;

	Player* player_ = nullptr;

};
