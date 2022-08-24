#include "MathUtility.h"
#include "RailCamera.h"

RailCamera::~RailCamera()
{
	
}

void RailCamera::Ini(Vector3 pos, Vector3 rot)
{
	//ワールドトランスフォームの初期設定
	worldTransform.translation_ = pos;
	worldTransform.rotation_ = rot;
	
	worldTransform.Initialize();
	matrix.UnitMatrix(worldTransform.matWorld_);
	//ビュープロジェクションの初期化
	viewProjection.Initialize();

	debugText_ = DebugText::GetInstance();

	controlPoints_ = {
		{0,0,0},
		{10,10,10},
		{10,15,20},
		{20,15,30},
		{20,0,40},
		{30,0,50},
	};
	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
	
	for (int i = 0; i < railPoints.size() - 3; i++) {
		for (int j = 0; j < 100; j++) {
			Vector3 pos = SplinePosition(railPoints, startIndex, t);
			positions[i][j] = pos + Vector3(1,-1,5);
			t += 0.01f;
		}
		//次の制御点に移動する
		startIndex += 1;
		//リセット
		t = 0;
	}
	startIndex = 1;
}

void RailCamera::Update()
{
	//レールカメラの挙動
	Move();

	/*float speed = -0.05f;
	worldTransform.scale_ = { 1,1,1 };
	worldTransform.translation_ += { 0,0,-speed };*/
	
	matrix.UpdateMatrix(worldTransform);

	viewProjection.eye = worldTransform.translation_;
	//ワールド前方ベクトル
	Vector3 forward(0, -0.2f, 1);
	//レールカメラの回転を反映
	forward = transform(forward, worldTransform.matWorld_);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection.target = viewProjection.eye + forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映（レールカメラの上方ベクトル）
	viewProjection.up = transform(up, worldTransform.matWorld_);
	viewProjection.UpdateMatrix();
	viewProjection.TransferMatrix();

	//デバッグ表示
	/*debugText_->SetPos(50, 270);
	debugText_->Printf(
		"time:(%f)", time);
	debugText_->SetPos(50, 290);
	debugText_->Printf(
		"timeRate:(%f)", timeRate);
	debugText_->SetPos(50, 310);
	debugText_->Printf(
		"time / 120:(%f)", time / 60);*/
}

void RailCamera::Move()
{
	float maxTime = 5.0f;

	if (timeRate >= 1.0f) {
		//次の制御点がある場合
		if (startIndex < points.size() - 3) {
			startIndex++;
			time = 0.0f;
			timeRate = 0;
		}
		//最終地点だった場合1.0fにして動きを止める
		else {
			timeRate = 1.0f;
		}
	}
	
	time += 0.5f;
	//timeRate / FPS　で1秒のカウントをnowTimeに代入する 
	timeRate = time / 120;

	timeRate = min(timeRate / maxTime, 1.0f);
	//ベジエ曲線
	/*Vector3 a = lerp(start, p1, nowtime);
	Vector3 b = lerp(p1, end, nowtime);
	position = ease_in_out(a, b, nowtime);*/
	position = SplinePosition(points, startIndex, timeRate);
	//注視点移動（ベクトルの加算）
	worldTransform.translation_ = position;
}

void RailCamera::RailIni()
{
	
	//線分の数＋1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float f = 1.0f / segmentCount * i;
		//Vector3 pos = 
		//描画用頂点リストに追加
		//pointsDrawing.push_back(pos);
	}

}

void RailCamera::DrawRail()
{
	for (int i = 0; i < points.size() - 3; i++) {
		for (int j = 0; j < 99; j++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d(
				positions[i][j] ,
				positions[i][j + 1] ,
				Vector4(1,1,1,1));
		}
	}
	
}
