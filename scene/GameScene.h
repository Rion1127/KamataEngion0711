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
#include <sstream>

#include "Matrix.h"
#include "XMFLOAT3.h"

#include "Player.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "SkyDome.h"
#include "RailCamera.h"
#include "Object.h"


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

	void CheckAllCollision(Player* player,Enemy* enemy);
	void CheckAllCollision(Player* player, std::unique_ptr<Enemy2>& enemy);

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//BGM
	uint32_t gumishipBGM;
	//SE
	uint32_t hitSE;


	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//テクスチャハンドル
	uint32_t enemyTextureHandle_ = 0;
	//3Dモデル
	Model* model_ = nullptr;
	Model* EnemyModel = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	Player* player_ = nullptr;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//敵2
	std::list<std::unique_ptr<Enemy2>> enemys2_;
	uint32_t enemy2TextureHandle_;

	//待機中フラグ
	bool isWait = false;
	//待機タイマー
	int waitTimer = 0;
	void LoadEnemyPopData();
	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();
	

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCamera = false;
	//カメラ上方向の角度
	float viewAngle = 0.0f;
	RailCamera* railCamera_ = nullptr;

	ViewProjection useViewProjevtion;


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

	Vector3 startPos;
	Vector3 endPos;
	Vector3 v0;
	Vector3 v1;
	float t;


	Vector3 start = { 0, 0, -50 };
	Vector3 p2 = { 20,-30,40 };
	Vector3 p3 = { -10,20,70 };
	Vector3 end = { 0,0,100 };
	std::vector<Vector3> points{ start,start,p2,p3,end,end };
	size_t startIndex = 1;
	//size_t potinsSize = points.size();
	Vector3 positions[6][100];


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
	/// 
	SkyDome* skyDome = nullptr;

	Model* gumiship = nullptr;
	Model* modelSkyDome = nullptr;

	Model* blueOrangeObj;
	Model* boxObject;
	//青とオレンジのオブジェクト
	std::list<std::unique_ptr<BlueOrangeObject>> obj;
	//箱のオブジェクト
	std::list<std::unique_ptr<BoxObject>> boxObj;
	uint32_t boxObjTextureHandle;

	//オブジェ発生コマンド
	std::stringstream objCommands;
	void LoadObjData();
	void IniObjData();
};
