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
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);
	//更新
	void Update();
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	//描画
	void Draw(ViewProjection viewProjection_);

private:
	void Move();
	void Rotation();
	void Attack();
	//ワールド変換データ
	WorldTransform worldTransform_;
	Matrix matrix;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力
	Input* input_ = nullptr;
	
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
};

