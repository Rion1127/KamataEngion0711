#include "Matrix.h"
#include "TextureManager.h"
#include <random>
#include "Particle.h"


#pragma region
EngineParticle::~EngineParticle()
{
	delete Model_;
}

void EngineParticle::Ini( WorldTransform worldtransform)
{
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("fire.png");

	worldTransform_.Initialize();

	Model_ = Model::Create();

	Vector3 velocity = {0,0,-1};
	float kBulletSpeed = 0.3f;
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = transform(velocity, worldtransform.matWorld_);
	velocity.normalize();
	velocity *= kBulletSpeed;
	
	matrix.ScaleChange(worldTransform_, 0.15f, 0.15f, 0.15f, 1);
	matrix.RotaChange(worldTransform_,
		worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z);
	matrix.ChangeTranslation(worldTransform_,
		worldtransform.GetWorldPosition().x + velocity.x * 10,
		worldtransform.GetWorldPosition().y + velocity.y * 10 - 0.1f,
		worldtransform.GetWorldPosition().z + velocity.z - 2.0f);
	matrix.UpdateMatrix(worldTransform_);

	
	particleAliveTime = MaxParticleAlive;
	isDead = false;

	//乱数生成器
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float> x(-0.07f, 0.07f);
	std::uniform_real_distribution<float> y(-0.07f, 0.07f);
	std::uniform_real_distribution<float> z(-0.04f, -0.04f);
	
	velocity_ = { x(engine),y(engine),z(engine)};
	

}

void EngineParticle::Update()
{
	worldTransform_.AddPosition(velocity_);
	matrix.UpdateMatrix(worldTransform_);

	particleAliveTime--;
	if (particleAliveTime <= 0)
	{
		isDead = true;
	}
}

void EngineParticle::Draw(ViewProjection viewProjection)
{
	Model_->Draw(worldTransform_, viewProjection,textureHandle_);
}
#pragma endregion

BurstEffect::~BurstEffect()
{
}

void BurstEffect::Ini(Model* model ,WorldTransform worldtransform)
{
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white.png");

	worldTransform_.Initialize();

	Model_ = model;

	
	matrix.ScaleChange(worldTransform_, 0.15f, 0.15f, 0.15f, 1);
	matrix.RotaChange(worldTransform_,
		worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z);
	matrix.ChangeTranslation(worldTransform_,
		worldtransform.GetWorldPosition().x,
		worldtransform.GetWorldPosition().y,
		worldtransform.GetWorldPosition().z);
	matrix.UpdateMatrix(worldTransform_);


	particleAliveTime = MaxParticleAlive;
	isDead = false;

	//乱数生成器
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float> x(-0.1f, 0.1f);
	std::uniform_real_distribution<float> y(-0.1f, 0.1f);
	std::uniform_real_distribution<float> z(-0.1f, 0.1f);

	velocity_ = { x(engine),y(engine),z(engine) };
}

void BurstEffect::Update()
{
	worldTransform_.AddRotation(velocity_);
	worldTransform_.AddPosition(velocity_);
	matrix.UpdateMatrix(worldTransform_);

	particleAliveTime--;
	if (particleAliveTime <= 0)
	{
		isDead = true;
	}
}

void BurstEffect::Draw(ViewProjection viewProjection)
{
	Model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
