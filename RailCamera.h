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
	
	//�X�v���C���Ȑ�����X�i�ʉߓ_�j
	std::vector<Vector3> controlPoints_;
	//�����ŕ`�悷��悤�̒��_���X�g
	std::vector<Vector3> pointsDrawing;
	//�����̐�
	const size_t segmentCount = 100;

};

