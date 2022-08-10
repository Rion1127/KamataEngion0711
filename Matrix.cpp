#include "Matrix.h"
#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"


Matrix::Matrix()
{
	matResult.Trans = MathUtility::Matrix4Identity();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matResult.Scale.m[i][j] = 0;
			matResult.Rot.m[i][j] = 0;
		}
	}
	
}

//単位行列
void Matrix::UnitMatrix(Matrix4& m) {
	Matrix4 unitMatrix;

	for (int i = 0; i < 4; i++) {
		unitMatrix.m[i][i] = 1.0f;
		for (int j = 0; j < 4; j++) {
			if (i != j) {
				unitMatrix.m[i][j] = 0.0f;
			}
		}
	}

	m = unitMatrix;
}

//初期設定
//スケーリングチェンジ
void Matrix::ScaleChange(WorldTransform& worldTransform, float m1, float m2, float m3, float m4) {
	//X,Y,Z方向のスケーリングを設定
	worldTransform.scale_ = { m1,m2,m3 };
	//スケーリング行列を宣言

	matResult.Scale.m[0][0] = worldTransform.scale_.x;
	matResult.Scale.m[1][1] = worldTransform.scale_.y;
	matResult.Scale.m[2][2] = worldTransform.scale_.z;
	matResult.Scale.m[3][3] = m4;
}
//回転
void Matrix::RotaChange(WorldTransform& worldTransform, float m1, float m2, float m3) {
	//X、Y、Z軸周りの回転角を設定
	worldTransform.rotation_ = { m1,m2,m3 };
	//X軸回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//各軸の回転行列を合成
	matResult.Rot = matRotZ;
	matResult.Rot *= matRotX;
	matResult.Rot *= matRotY;
}
//平行移動
void Matrix::ChangeTranslation(WorldTransform& worldTransform, float tx, float ty, float tz)
{
	//Ｘ，Ｙ，Ｚ軸周りの平行移動を設定
	worldTransform.translation_ = { tx,ty,tz };

	matResult.Trans.m[3][0] = worldTransform.translation_.x;
	matResult.Trans.m[3][1] = worldTransform.translation_.y;
	matResult.Trans.m[3][2] = worldTransform.translation_.z;
}

//連続処理
void Matrix::ScaleChange(WorldTransform& worldTransform)
{
	matResult.Scale.m[0][0] = worldTransform.scale_.x;
	matResult.Scale.m[1][1] = worldTransform.scale_.y;
	matResult.Scale.m[2][2] = worldTransform.scale_.z;
	matResult.Scale.m[3][3] = 1;
}
void Matrix::RotaChange(WorldTransform& worldTransform)
{
	//X軸回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[3][3] = 1;

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	//各軸の回転行列を合成
	matResult.Rot = matRotZ;
	matResult.Rot *= matRotX;
	matResult.Rot *= matRotY;
}
void Matrix::ChangeTranslation(WorldTransform& worldTransform)
{
	matResult.Trans.m[3][0] = worldTransform.translation_.x;
	matResult.Trans.m[3][1] = worldTransform.translation_.y;
	matResult.Trans.m[3][2] = worldTransform.translation_.z;
}

//組み合わせ
void Matrix::UpdateMatrix(WorldTransform& worldTransform)
{
	//行列の合成

	ScaleChange(worldTransform);
	RotaChange(worldTransform);
	ChangeTranslation(worldTransform);


	UnitMatrix(worldTransform.matWorld_);
	worldTransform.matWorld_ *= matResult.Scale;
	worldTransform.matWorld_ *= matResult.Rot;
	worldTransform.matWorld_ *= matResult.Trans;

	if (worldTransform.parent_) {
		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	}

	//行列の転送
	worldTransform.TransferMatrix();
}




