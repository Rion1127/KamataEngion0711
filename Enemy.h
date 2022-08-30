#pragma once
#include "Audio.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"


//自機クラスの前方宣言
class Player;

class Enemy
{
public:
	
	Enemy();
	~Enemy();
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);
	//更新
	void Update();

	void Shot();
	bool IsDead() { return isDead; }
	bool IsAlive() { return  isAlive; }
	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	void SetPlayer(Player* player) { player_ = player; }
	//描画
	void Draw(const ViewProjection& viewProjection);

	void CollisionCooltime();
	void OnCollisioin(uint32_t hitSE);

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//入力
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	DebugText* debugText_ = nullptr;

	Model* bulletModel = nullptr;

	//HP
	bool isAlive = true;
	bool isDead = false;
	const int maxHp = 5;
	int hp = maxHp;
	bool isCollision = false;

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
	static const int kFireInterval = 360;
	int32_t shotCoolTime = 0;

	Player* player_ = nullptr;

	const int maxCollisionCoolTime = 15;
	int collisionCoolTime;
	int num;

	//エフェクト
	uint32_t effectTexture;
	WorldTransform effectWorldTransform_;
	int effectAliveTime;
	uint32_t hitSE;
};
