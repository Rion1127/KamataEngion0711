#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MathUtility.h"
#include <cassert>
#include <fstream>
#define XM_PI 3.141592

bool BallCollision(const WorldTransform& a, const WorldTransform& b);
bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize);
bool RayCollision(WorldTransform ray, WorldTransform obj);

GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete debugCamera_;
	delete player_;
	delete EnemyModel;
	delete modelSkyDome;
	delete skyDome;
	delete blueOrangeObj;
	delete boxObject;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//BGM
	gumishipBGM = audio_->LoadWave("gumishipBGM.wav");
	//SE
	hitSE = audio_->LoadWave("hitSE.wav");

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("enemy.jpg");
	enemy2TextureHandle_ = TextureManager::Load("enemy2.jpg");
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

	railCamera_ = new RailCamera();
	railCamera_->Ini(Vector3(0, 10, -25), Vector3(0, 0, 0));

	useViewProjevtion = railCamera_->GetViewProjection();
	//useViewProjevtion = debugCamera_->GetViewProjection();
	//useViewProjevtion = viewProjection_;

	//軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参考するビュープロジェクションを指定する（アドレス渡し）
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
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
	gumiship = Model::CreateFromOBJ("gumiship", true);
	player_->Initialize(gumiship, textureHandle_);
	player_->SetParent(railCamera_->GetWorldTransform());


	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Initialize(EnemyModel, enemyTextureHandle_);
		enemy->SetPlayer(player_);
	}

	//false バイオ :: true カメラ視点
	isCamera = false;

	skyDome = new SkyDome();
	modelSkyDome = Model::CreateFromOBJ("skydome", true);
	skyDome->Ini(modelSkyDome);





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
		startIndex += 1;
		//リセット
		t = 0;
	}

	blueOrangeObj = Model::CreateFromOBJ("testObj", true);
	boxObject = Model::Create();
	boxObjTextureHandle = TextureManager::Load("boxObj\\boxObj1.png");
	LoadObjData();
	IniObjData();
}

void GameScene::Update()
{
	//BGM
	if (audio_->IsPlaying(gumishipBGM) == false) {
		audio_->PlayWave(gumishipBGM, true, 0.07f);
	}
	//天球
	skyDome->Update();

	debugCamera_->Update();
	if (input_->TriggerKey(DIK_Q)) {
		if (isCamera == false)isCamera = true;
		else if (isCamera == true)isCamera = false;
	}

	railCamera_->Update();
	//自キャラ更新
	player_->Update();
	player_->Get2DReticlePosition(useViewProjevtion);

	//敵更新
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		CheckAllCollision(player_, enemy.get());
		enemy->Update();
	}
	//デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemys_) {
		return enemys_->IsDead();
		});
	//敵２更新
	for (std::unique_ptr<Enemy2>& enemy : enemys2_) {
		CheckAllCollision(player_, enemy);
		enemy->Update();
	}
	//デスフラグの立った敵を削除
	enemys2_.remove_if([](std::unique_ptr<Enemy2>& enemys_) {
		return enemys_->IsDead();
		});

	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	//オブジェ更新
	//オブジェ更新
	//for (int i = 0; i < objWorldTransforms_.size(); i++)
	//{
	//	//条件に合ったら削除
	//	if (railCamera_->GetWorldTransform().translation_.z >
	//		std::next(objWorldTransforms_.begin(), i)->get()->translation_.z)
	//	{
	//		objWorldTransforms_.remove_if([](std::unique_ptr<WorldTransform>& worldTransform) {
	//			return true;
	//			});
	//	}
	//}
	//for (int i = 0; i < obj2WorldTransforms_.size(); i++)
	//{
	//	//条件に合ったら削除
	//	if (railCamera_->GetWorldTransform().translation_.z >
	//		std::next(obj2WorldTransforms_.begin(), i)->get()->translation_.z)
	//	{
	//		obj2WorldTransforms_.remove_if([](std::unique_ptr<WorldTransform>& worldTransform) {
	//			return true;
	//			});
	//	}
	//}


	//デバッグ表示
	/*for (int i = 0; i < 5; i++) {
		debugText_->SetPos(50, 250 + i * 20);
		debugText_->Printf(
			"worldTransforms_:(%f,%f,%f)",
			worldTransforms_[i].translation_.x,
			worldTransforms_[i].translation_.y,
			worldTransforms_[i].translation_.z);
	}*/
	/*debugText_->SetPos(50, 290);
	debugText_->Printf(
		"Collision:(%d)",
		debugCamera_->GetViewProjection().eye.x,
		debugCamera_->GetViewProjection().eye.y,
		debugCamera_->GetViewProjection().eye.z);*/
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

	//弾描画
	for (std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(useViewProjevtion);
	}
	for (std::unique_ptr<Enemy2>& enemy : enemys2_) {
		enemy->Draw(useViewProjevtion);
	}


	skyDome->Draw(viewProjection_);
	//水色とオレンジのオブジェクト
	for (std::unique_ptr<BlueOrangeObject>& Obj : obj) {
		if (player_->GetWorldPosition().z < Obj->worldTransform_.translation_.z + 10) {
			Obj->Draw(useViewProjevtion);
		}
	}
	//箱のオブジェ
	for (std::unique_ptr<BoxObject>& Obj : boxObj) {
		if (player_->GetWorldPosition().z < Obj->worldTransform_.translation_.z + 10) {
			Obj->Draw(useViewProjevtion, boxObjTextureHandle);
		}
	}

	//レールカメラの通る線を引く
	//railCamera_->DrawRail();

	//for (int i = 0; i < 3; i++) {
	//	for (int j = 0; j < 99; j++) {
	//		//model_->Draw(worldTransforms_[i], useViewProjevtion);
	//		PrimitiveDrawer::GetInstance()->DrawLine3d(
	//			positions[i][j],
	//			positions[i][j + 1],
	//			colorX);
	//	}
	//}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();

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
		posB = bullet->GetWorldTransform().GetWorldPosition();

		if (BallCollision(player->GetWorldPosition(), 1.0f, posB, 1.0f)) {
			//自キャラの衝突時コールバックを呼び出す
			player->OnCollisioin();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollisioin();
		}

	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	enemy->CollisionCooltime();
	debugText_->SetPos(50, 100);
	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		if (enemy->IsAlive() == true) {
			if (RayCollision(bullet->GetWorldTransform(), enemy->GetWorldTransform())) {
				//敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollisioin(hitSE);
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollisioin();
			}
		}
	}
#pragma endregion

}

void GameScene::CheckAllCollision(Player* player, std::unique_ptr<Enemy2>& enemy)
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

#pragma region 自弾と敵キャラの当たり判定
	enemy->CollisionCooltime();
	debugText_->SetPos(50, 100);
	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		if (enemy->IsAlive() == true) {
			if (RayCollision(bullet->GetWorldTransform(), enemy->GetWorldTransform())) {
				//敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollisioin(hitSE);
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollisioin();
			}
		}
	}
#pragma endregion

#pragma region 敵キャラと自キャラの当たり判定
	
	if (BallCollision(player->GetWorldPosition(), 1.0f, enemy->GetWorldPosition(), 1.0f)) {
		player->OnCollisioin();
	}

#pragma endregion

}

void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources\\enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	//ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			//待機完了
			isWait = false;
		}
		return;
	}


	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//、区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//敵の種類
			std::getline(line_stream, word, ',');
			int enemyType = (int)std::atof(word.c_str());

			Vector3 pos = {
				x + railCamera_->GetWorldTransform().translation_.x,
				y ,
				z + railCamera_->GetWorldTransform().translation_.z
			};

			if (enemyType == 0) {
				// 敵を生成し、初期化
				std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
				newenemy->Initialize(model_, enemyTextureHandle_, pos);
				newenemy->SetPlayer(player_);
				// 敵を登録する
				enemys_.push_back(std::move(newenemy));
			}
			else if (enemyType == 1) {
				// 敵を生成し、初期化
				//1の敵の場合ｚ座標を指定する
				pos.z = railCamera_->GetWorldTransform().translation_.z - 20;
				std::unique_ptr<Enemy2> newenemy = std::make_unique<Enemy2>();
				newenemy->Initialize(model_, enemy2TextureHandle_, pos);
				newenemy->SetPlayer(player_);
				// 敵を登録する
				enemys2_.push_back(std::move(newenemy));
			}
		}
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');
			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait = true;
			waitTimer = waitTime;
			//コマンドループを抜ける
			break;
		}


	}
}

void GameScene::LoadObjData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources\\objData.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	objCommands << file.rdbuf();
	//ファイルを閉じる
	file.close();
}

void GameScene::IniObjData()
{
	//座標
	Vector3 pos{};
	Vector3 rot{};
	Vector3 scale{};
	int type = 0;

	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (std::getline(objCommands, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//、区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		//座標コマンド
		if (word.find("POS") == 0) {
			//x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//座標を代入
			pos = { x,y,z };

		}
		//、区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		//回転コマンド
		if (word.find("ROT") == 0) {
			//x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//回転を代入
			rot =
			{
				ConvertAngleToRadian(x),
				ConvertAngleToRadian(y),
				ConvertAngleToRadian(z)
			};
		}
		//、区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		//スケールコマンド
		if (word.find("SCL") == 0) {
			//x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//スケールを代入
			scale = { x,y,z };
		}
		std::getline(line_stream, word, ',');
		if (word.find("TYPE") == 0) {
			//オブジェクトのタイプを判別
			std::getline(line_stream, word, ',');
			type = (int)std::atof(word.c_str());
		}
		
		WorldTransform worldtransform;
		worldtransform.translation_ = pos;
		worldtransform.rotation_ = rot;
		worldtransform.scale_ = scale;

		if (type == 0) {
			// オブジェを生成し、初期化
			std::unique_ptr<BlueOrangeObject> newObj = std::make_unique<BlueOrangeObject>();
			newObj->Ini(blueOrangeObj, worldtransform);
			obj.push_back(std::move(newObj));
		}
		else if (type == 1) {
			// オブジェを生成し、初期化
			std::unique_ptr<BoxObject> newObj = std::make_unique<BoxObject>();
			newObj->Ini(boxObject, worldtransform);
			boxObj.push_back(std::move(newObj));
		}
	}
	/*for (std::unique_ptr<WorldTransform>& worldTransform : objWorldTransforms_) {
		worldTransform.get()->Initialize();
		assert(worldTransform.get());
		matrix.UpdateMatrix(*worldTransform.get());
	}
	for (std::unique_ptr<WorldTransform>& worldTransform : obj2WorldTransforms_) {
		worldTransform.get()->Initialize();
		assert(worldTransform.get());
		matrix.UpdateMatrix(*worldTransform.get());
	}*/
}


bool BallCollision(const WorldTransform& a, const WorldTransform& b) {
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

bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize) {
	float x, y, z;
	float r;

	x = (float)pow(b.x - a.x, 2);
	y = (float)pow(b.y - a.y, 2);
	z = (float)pow(b.z - a.z, 2);

	float pos = x + y + z;

	r = (float)pow(aSize + bSize, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}

bool RayCollision(WorldTransform ray, WorldTransform obj)
{
	//レイの当たり判定

	//ワールド座標を代入
	Vector3 objPos;
	Vector3 rayPos;
	objPos = obj.GetWorldPosition();
	rayPos = ray.GetWorldPosition();
#pragma region レイの当たり判定
	//レイの始点と終点を代入
	Vector3 rayStart;
	Vector3 rayEnd;
	rayStart = {
		ray.GetWorldPosition().x,
		ray.GetWorldPosition().y,
		ray.GetWorldPosition().z - ray.scale_.z - 1.0f,
	};
	rayEnd = {
		ray.GetWorldPosition().x,
		ray.GetWorldPosition().y,
		ray.GetWorldPosition().z + ray.scale_.z + 1.0f,
	};
	//始点と終点からレイのベクトル(a→)を求める
	Vector3 rayVec;
	rayVec = rayEnd - rayStart;
	float raySize;
	raySize = rayVec.length();
	//正規化(a→N)
	rayVec.normalize();
	//レイとオブジェクトのベクトル(b→)を求める
	// レイの始点とオブジェクトへのベクトル(b→)を求める
	Vector3 ABVec;
	ABVec = {
		objPos.x - rayStart.x,
		objPos.y - rayStart.y,
		objPos.z - rayStart.z
	};

	//b→・a→N をray2ObjLengthに格納
	float ray2ObjectLength = ABVec.dot(rayVec);

	//Qを求める a→N * b→・a→N + P
	Vector3 Q;
	//Q = rayVec * ABVec.dot(rayVec) + rayPos;
	Q = rayVec * ray2ObjectLength + rayStart;
	//オブジェクトからレイの垂線(obj-Q)を求める
	Vector3 line;
	line = objPos - Q;
	//垂線の長さを求める
	line.length();

	//垂線の長さが半径+半径より短ければ当たってる
	bool isHit = false;
	if (line.length() <= obj.scale_.x + 0.2f) {
		if (raySize >= ray2ObjectLength) {
			isHit = true;
		}
	}

	if (ray2ObjectLength <= 0) {
		isHit = false;
	}

	return isHit;

#pragma endregion
}