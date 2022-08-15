#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MathUtility.h"
#include <cassert>
#define XM_PI 3.141592

bool BallCollision(WorldTransform a, WorldTransform b);

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete debugCamera_;
	delete player_;
	delete EnemyModel;
	delete enemy_;
	delete modelSkyDome;
	delete skyDome;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("enemy.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	EnemyModel = Model::Create();

	//カメラ視点座標を設定
	viewProjection_.eye = { 50,0,30 };
	////カメラの注視点座標を設定
	viewProjection_.target = { 0,0,30 };
	////カメラ上方向ベクトルを設定（右上45度指定）
	//viewProjection_.up = { cosf(XM_PI / 4.0f),sinf(XM_PI / 4.0f),0.0f };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//useViewProjevtion = railCamera_->GetViewProjection();
	useViewProjevtion = debugCamera_->GetViewProjection();
	//useViewProjevtion = viewProjection_;

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参考するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&useViewProjevtion);
	////ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&useViewProjevtion);
#pragma region ライン描画座標
	//ライン描画座標X
	for (int i = 0; i < 30; i++) {
		vector3X_[i] = { 0,0,0 + (float)(2.5f * i) };
		vector3X_2[i] = { 75.0f,0.0f,0.0f + (float)(2.5f * i) };
	}
	colorX = { 255,0,0,1 };
	//ライン描画座標Z
	for (int i = 0; i < 30; i++) {
		vector3Z_[i] = { 0 + (float)(2.5f * i) ,0,0 };
		vector3Z_2[i] = { 0.0f + (float)(2.5f * i) ,0.0f, 75.0f };
	}
	colorZ = { 0,0,255,1 };
#pragma endregion
	startPos = { 10,0,0 };
	endPos = { 25,10,0 };
	v0 = { 0,-20,0 };
	v1 = { 0,-10,0 };

	//自キャラ生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	enemy_ = new Enemy();
	enemy_->Initialize(EnemyModel, enemyTextureHandle_);
	enemy_->SetPlayer(player_);

	//false バイオ :: true カメラ視点
	isCamera = false;

	skyDome = new SkyDome();
	modelSkyDome = Model::CreateFromOBJ("skydome", true);
	skyDome->Ini(modelSkyDome);

	railCamera_ = new RailCamera();
	railCamera_->Ini(Vector3(0, 0, -50), Vector3(0, 0, 0));


	/*for (int i = 0; i < 100; i++) {
		Vector3 pos = GetPoint(startPos, endPos, v0, v1, t);
		worldTransforms_[i].Initialize();
		worldTransforms_[i].translation_ = pos;
		worldTransforms_[i].TransferMatrix();
		matrix.UpdateMatrix(worldTransforms_[i]);
		t += 0.01f;
	}*/


	for (int i = 0; i < points.size() - 3; i++) {
		for (int j = 0; j < 100; j++) {
			Vector3 pos = SplinePosition(points, startIndex, t);
			positions[i][j] = pos;
			t += 0.01f;
		}
		//次の制御点に移動する
		startIndex+=1;
		//リセット
		t = 0;
	}



}

void GameScene::Update()
{
	debugCamera_->Update();
	if (input_->TriggerKey(DIK_Q)) {
		if (isCamera == false)isCamera = true;
		else if (isCamera == true)isCamera = false;
	}

	railCamera_->Update();
	//自キャラ更新
	player_->Update();
	if (enemy_) {
		enemy_->Update();
	}

	CheckAllCollision(player_, enemy_);


	//デバッグ表示
	for (int i = 0; i < 5; i++) {
		debugText_->SetPos(50, 250 + i * 20);
		debugText_->Printf(
			"worldTransforms_:(%f,%f,%f)",
			worldTransforms_[i].translation_.x,
			worldTransforms_[i].translation_.y,
			worldTransforms_[i].translation_.z);
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(useViewProjevtion);
	enemy_->Draw(useViewProjevtion);

	skyDome->Draw(useViewProjevtion);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 99; j++) {
			//model_->Draw(worldTransforms_[i], useViewProjevtion);
			PrimitiveDrawer::GetInstance()->DrawLine3d(
				positions[i][j],
				positions[i][j + 1],
				colorX);
		}
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision(Player* player, Enemy* enemy)
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player->GetWorldPosition();
	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = enemy->GetWorldPosition();

		if (BallCollision(player->GetWorldTransform(), bullet.get()->GetWorldTransform())) {
			//自キャラの衝突時コールバックを呼び出す
			player->OnCollisioin();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollisioin();
		}

	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

#pragma endregion
}


bool BallCollision(WorldTransform a, WorldTransform b) {
	float x, y, z;
	float r;

	x = (float)pow(b.translation_.x - a.translation_.x, 2);
	y = (float)pow(b.translation_.y - a.translation_.y, 2);
	z = (float)pow(b.translation_.z - a.translation_.z, 2);

	float pos = x + y + z;

	r = (float)pow(a.scale_.x + b.scale_.x, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}