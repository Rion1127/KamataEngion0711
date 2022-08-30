#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Matrix.h"
#include "DebugText.h"
#include <vector>
#include "PrimitiveDrawer.h"
class RailCamera
{
public:
	~RailCamera();
	void Ini(Vector3 pos,Vector3 rot);
	void Update();

	void Move();

	void RailIni();
	void DrawRail();
	

	WorldTransform& GetWorldTransform() { return worldTransform; }
	ViewProjection GetViewProjection() { return viewProjection; }

private:
	Matrix matrix;
	WorldTransform worldTransform;
	ViewProjection viewProjection;

	DebugText* debugText_ = nullptr;
	
	//スプライン曲線制御店（通過点）
	std::vector<Vector3> controlPoints_;
	//線分で描画するようの頂点リスト
	std::vector<Vector3> pointsDrawing;
	//線分の数
	const size_t segmentCount = 100;

	Vector3 position;
	float time = 0;
	float timeRate = 0;

	Vector3 start = { 0, 10, -25 };
	Vector3 p2 = { 0,10,600 };
	Vector3 p3 = { 0,10,1000 };
	Vector3 end = { 0,10,1400 };
	std::vector<Vector3> points{ start,start,p2,p3,end,end };
	//p1からスタートする
	size_t startIndex = 1;

	std::vector<Vector3> railPoints = points;
	Vector3 positions[6][100];
	float t = 0;


	float testtimer = 0;
};

