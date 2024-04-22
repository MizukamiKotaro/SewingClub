#include "WaterEffect.h"
#include "Vector3.h"
#include "SceneSystem/IScene/IScene.h"


WaterEffect::WaterEffect(const Vector3& cameraPos)
{
	noise_ = std::make_unique<Noise>();
	noise_->SetCameraPos(cameraPos);
	highLumi_ = std::make_unique<HighLumi>();
	highLumi_->highLumiData_->max = 1.1f;
	highLumi_->highLumiData_->isToWhite = 1;
	outline_ = std::make_unique<WaterOutline>();
	uneune_ = 20;

	global_ = std::make_unique<GlobalVariableUser>("Water", "WaterEffect");
	if (IScene::sceneNo_ == SCENE::STAGE) {
		stageEditor_ = std::make_unique<StageEditor>("水のエフェクトの設定");
	}
	SetGlobalVariable();
}

void WaterEffect::Initialize()
{
	noise_->Initialize();
	stageEditor_->Initialize();
	noise_->Update(0.01f);
	
	SetGlobalVariable();
}

void WaterEffect::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();

#endif // _DEBUG


	noise_->Update(deltaTime / uneune_);
}

void WaterEffect::Draw()
{
	outline_->Draw();
}

void WaterEffect::PreDrawBackGround()
{
	noise_->PreDrawScene();
}

void WaterEffect::PostDrawBackGround()
{
	noise_->PostDrawScene();
}

void WaterEffect::PreDrawWaterArea()
{
	highLumi_->PreDrawScene();
}

void WaterEffect::PostDrawWaterArea()
{
	highLumi_->PostDrawScene();

	outline_->PreDrawScene();
	noise_->Draw();
	highLumi_->Draw(BlendMode::kBlendModeMultiply);
	outline_->PostDrawScene();
}

void WaterEffect::SetGlobalVariable()
{
	global_->AddItem("密度", noise_->noiseData_->density);
	global_->AddItem("カメラの影響の受けにくさ", noise_->noiseData_->moveScale);
	global_->AddItem("うねうねの動きにくさ", uneune_);

	if (stageEditor_) {
		stageEditor_->AddItem("水の色", Vector3{ 0.3f,1.0f,0.8f });
		stageEditor_->AddItem("うねうねの色", Vector3{ 0.8f,0.8f,0.8f });
	}
	else {
		global_->AddItem("水の色", Vector3{ 0.3f,1.0f,0.8f });
		global_->AddItem("うねうねの色", Vector3{ 0.8f,0.8f,0.8f });
	}

	ApplyGlobalVariable();
}

void WaterEffect::ApplyGlobalVariable()
{
	noise_->noiseData_->density = global_->GetFloatValue("密度");
	noise_->noiseData_->moveScale = global_->GetFloatValue("カメラの影響の受けにくさ");
	uneune_ = global_->GetIntValue("うねうねの動きにくさ");
	if (uneune_ <= 0) {
		uneune_ = 1;
	}

	if (stageEditor_) {
		Vector3 waterColor = stageEditor_->GetVector3Value("水の色");
		noise_->noiseData_->waterColor = { waterColor.x,waterColor.y,waterColor.z,1.0f };
		waterColor = stageEditor_->GetVector3Value("うねうねの色");
		noise_->noiseData_->lightningColor = { waterColor.x,waterColor.y,waterColor.z,1.0f };
	}
	else {
		Vector3 waterColor = global_->GetVector3Value("水の色");
		noise_->noiseData_->waterColor = { waterColor.x,waterColor.y,waterColor.z,1.0f };
		waterColor = global_->GetVector3Value("うねうねの色");
		noise_->noiseData_->lightningColor = { waterColor.x,waterColor.y,waterColor.z,1.0f };
	}

	outline_->color_ = noise_->noiseData_->lightningColor;
}
