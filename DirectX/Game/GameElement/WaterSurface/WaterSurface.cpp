#include "WaterSurface.h"

WaterSurface::WaterSurface()
{
	noise_ = std::make_unique<Noise>();
	noise_->noiseData_->type = Noise::NoiseType::WATER_SURFACE;
	noise_->noiseData_->correctionUV = { -0.09f,-0.09f };
	noise_->noiseData_->density = 4.0f;
	add_ = 0.1f;
	global_ = std::make_unique<GlobalVariableUser>("Surface", "Surface");
	SetGlobalVariable();
}

void WaterSurface::Initialize()
{
}

void WaterSurface::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
	noise_->Update(deltaTime * add_);
}

void WaterSurface::Draw()
{
	noise_->Draw();
}

void WaterSurface::PreDraw()
{
	noise_->PreDrawScene();
}

void WaterSurface::PostDraw()
{
	noise_->PostDrawScene();
}

void WaterSurface::SetGlobalVariable()
{
	global_->AddItem("動きの速さ", add_);
	global_->AddItem("歪みの大きさ", noise_->noiseData_->density);
	ApplyGlobalVariable();
}

void WaterSurface::ApplyGlobalVariable()
{
	add_ = global_->GetFloatValue("動きの速さ");
	noise_->noiseData_->density = global_->GetFloatValue("歪みの大きさ");
}
