#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Model.h"
class EngineParticle
{
public:
	~EngineParticle();
	void Ini(WorldTransform worldtransform);

	void Update();

	void Draw(ViewProjection viewProjection);

	bool GetDead() { return isDead; }
private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* Model_ = nullptr;
	Vector3 velocity_;
	uint32_t textureHandle_;
	const int MaxParticleAlive = 15;
	int particleAliveTime = MaxParticleAlive;
	bool isDead = false;
};

class BurstEffect {
public:
	~BurstEffect();
	void Ini(Model* model, WorldTransform worldtransform);

	void Update();

	void Draw(ViewProjection viewProjection);

	bool GetDead() { return isDead; }

private:
	WorldTransform worldTransform_;
	Matrix matrix;
	Model* Model_ = nullptr;
	Vector3 velocity_;
	uint32_t textureHandle_;
	const int MaxParticleAlive = 80;
	int particleAliveTime = MaxParticleAlive;
	bool isDead = false;

};