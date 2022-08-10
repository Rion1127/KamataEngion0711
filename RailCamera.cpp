#include "RailCamera.h"

void RailCamera::Ini(Vector3 pos, Vector3 rot)
{
	//ワールドトランスフォームの初期設定
	worldTransform.translation_ = pos;
	worldTransform.rotation_ = rot;
	worldTransform.CreateConstBuffer();
	//ビュープロジェクションの初期化
	viewProjection.Initialize();

	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update()
{
	float speed = -0.05f;

	worldTransform.translation_ += { 0,0,-speed };
	worldTransform.rotation_ += { 0,0,0 };
	//worldTransform.TransferMatrix();

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
	//viewProjection.up = transform(up, worldTransform.matWorld_);
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
