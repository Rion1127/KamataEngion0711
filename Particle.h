#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Model.h"
class Particle
{
public:
	~Particle();
	void Ini(WorldTransform worldtransform);

	void Update();

	void Draw(ViewProjection viewProjection);

	bool GetDead() { return isDead; }
private:
	Controller pad;
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* Model_ = nullptr;
	Vector3 velocity_;
	uint32_t textureHandle_;
	const int MaxParticleAlive = 15;
	int particleAliveTime = MaxParticleAlive;
	bool isDead = false;
};

