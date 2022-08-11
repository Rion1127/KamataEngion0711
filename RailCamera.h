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

	void RailIni();
	void DrawRail();

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

};

