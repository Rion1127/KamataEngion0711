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
		{10,10,0},
		{10,15,0},
		{20,15,0},
		{20,0,0},
		{30,0,0},
	};
	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
}

void RailCamera::Update()
{
	float speed = -0.05f;
	worldTransform.scale_ = { 1,1,1 };
	worldTransform.translation_ += { 0,0,-speed };
	worldTransform.rotation_ = { 0,0,0 };
	matrix.UpdateMatrix(worldTransform);

	viewProjection.eye = worldTransform.translation_;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
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
	debugText_->SetPos(50, 250);
	debugText_->Printf(
		"viewProjection.eye:(%f,%f,%f)", viewProjection.eye.x,
		viewProjection.eye.y,
		viewProjection.eye.z);
	debugText_->SetPos(50, 270);
	debugText_->Printf(
		"viewProjection.up:(%f,%f,%f)", viewProjection.up.x,
		viewProjection.up.y,
		viewProjection.up.z);
	debugText_->SetPos(50, 290);
	debugText_->Printf(
		"viewProjection.target:(%f,%f,%f)", viewProjection.target.x,
		viewProjection.target.y,
		viewProjection.target.z);
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
	for (size_t i = 0; i < segmentCount + 1; i++) {
		//PrimitiveDrawer::GetInstance()->DrawLine3d()
	}
}
