#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Matrix.h"
#include "XMFLOAT3.h"

#include "Player.h"
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//テクスチャハンドル
	uint32_t enemyTextureHandle_ = 0;
	//3Dモデル
	Model* model_ = nullptr;
	Model* EnemyModel = nullptr;
	//ワールドトランスフォーム
	/*WorldTransform worldTransforms_[100];*/
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCamera = false;
	//カメラ上方向の角度
	float viewAngle = 0.0f;


	//ライン描画X座標
	Vector3 vector3X_[30];
	Vector3 vector3X_2[30];
	//ライン描画色
	Vector4 colorX;
	//ライン描画Z座標
	Vector3 vector3Z_[30];
	Vector3 vector3Z_2[30];
	//ライン描画色
	Vector4 colorZ;

	//ワールド変換行列
	Matrix matrix;

	//ランダム
	Vector3 random;

	//false バイオ :: true カメラ視点
	bool isCamera;
	XMFLOAT3 resultEye;
	XMFLOAT3 resultTarget;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
