#pragma once
#pragma once
#include "WorldTransform.h"


//自機クラスの前方宣言
class Player;

class Enemy2
{
public:

	Enemy2();
	~Enemy2();
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);
	//更新
	void Update();


	bool IsDead() { return isDead; }
	bool IsAlive() { return  isAlive; }
	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	void SetPlayer(Player* player) { player_ = player; }
	//描画
	void Draw(const ViewProjection& viewProjection);

	void CollisionCooltime();
	void OnCollisioin(uint32_t hitSE);

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//入力
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	DebugText* debugText_ = nullptr;

	//HP
	bool isAlive = true;
	bool isDead = false;
	const int maxHp = 3;
	int hp = maxHp;
	bool isCollision = false;

	//行動フェーズ
	enum class Phase {
		OverTake,	//接近する
		Assault,		//離脱する
	};

	//フェーズ
	Phase phase_ = Phase::OverTake;

	Vector3 speed = { 0,0,-0.1f };

	void phase_OverTake();
	void phase_AssaultIni();
	void phase_Assault();
	Vector3 AssaultVec;

	//メンバ関数ポインタのテーブル
	static void (Enemy2::* spFuncTable[])();

	Player* player_ = nullptr;

	const int maxCollisionCoolTime = 20;
	int collisionCoolTime;
	int num;

	//エフェクト
	uint32_t effectTexture;
	WorldTransform effectWorldTransform_;
	int effectAliveTime;
	
};


