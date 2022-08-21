#pragma once
#include "WorldTransform.h"
#include "Matrix.h"
#include <cassert>
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player
{
public:
	~Player();
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);
	//更新
	void Update();
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	WorldTransform GetWorldTransform() { return worldTransform_; }
	//描画
	void Draw(ViewProjection viewProjection_);
	void DrawReticle();

	void CollisionCooltime();
	void OnCollisioin();

	void SetParent(WorldTransform& worldTransform);
	void Get2DReticlePosition(ViewProjection viewProjection);

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	void Move();
	void Rotation();
	void Attack();
	//ワールド変換データ
	WorldTransform worldTransform_;
	Matrix matrix;
	//モデル
	Model* model_ = nullptr;
	Model* bulletModel = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力
	Input* input_ = nullptr;
	Controller pad;
	
	DebugText* debugText_ = nullptr;

	enum PartID {
		Root,	//大本
		Spine,	//脊髄
		Chest,	//胸
		Head,	//頭
		ArmL,	//左腕
		ArmR,	//右腕
		Hip,	//尻
		LegL,	//左足
		LegR,	//右足

		endNum,	//最後の番号
	};

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int cooltime;


	Vector3 position;
	float time = 0;
	float timeRate = 0;
	
	

	Vector3 start = { 0, 0, 0 };
	Vector3 p2 = { 20,10,10 };
	Vector3 p3 = { -10,-10,20 };
	Vector3 end = { 0,15,30 };
	std::vector<Vector3> points{ start,start,p2,p3,end,end };
	//p1からスタートする
	size_t startIndex = 1;

	const int maxCollisionCoolTime = 20;
	int collisionCoolTime = maxCollisionCoolTime;

	//レティクル
	Model* ReticleModel;
	WorldTransform reticlePosition;
	//2Dレティクル用スプライト
	std::unique_ptr <Sprite> sprite2Dreticle;
};

