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
	post_ = std::make_unique<PostEffect>();
	waterArea_ = std::make_unique<HighLumi>();
	waterArea_->highLumiData_->max = 1.1f;
	waterArea_->highLumiData_->isToWhite = 1;

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
	post_->Draw();
	outline_->Draw();
}

void WaterEffect::WaterAreaDraw()
{
	waterArea_->Draw();
}

void WaterEffect::PreDrawBackGround()
{
	post_->PreDrawScene();
}

void WaterEffect::PostDrawBackGround()
{
	post_->PostDrawScene();
}

void WaterEffect::PreDrawWaterArea()
{
	highLumi_->PreDrawScene();
}

void WaterEffect::PostDrawWaterArea()
{
	highLumi_->PostDrawScene();

	waterArea_->PreDrawScene();
	highLumi_->Draw();
	waterArea_->PostDrawScene();

	noise_->PreDrawScene();
	post_->Draw();
	noise_->PostDrawScene();

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
	global_->AddItem("映り込む背景の修正", noise_->noiseData_->correctionUV);

	if (stageEditor_) {
		stageEditor_->AddItem("水の色", Vector3{ 0.3f,1.0f,0.8f });
		stageEditor_->AddItem("うねうねの色", Vector3{ 0.8f,0.8f,0.8f });
		stageEditor_->AddItem("プレイヤーに重ねる色", Vector3{ 0.8f,0.8f,0.8f });
		stageEditor_->AddItem("プレイヤーに重ねる色の透明度", float(0.5f));
	}
	else {
		global_->AddItem("水の色", Vector3{ 0.3f,1.0f,0.8f });
		global_->AddItem("うねうねの色", Vector3{ 0.8f,0.8f,0.8f });
		global_->AddItem("プレイヤーに重ねる色", Vector3{ 0.8f,0.8f,0.8f });
		global_->AddItem("プレイヤーに重ねる色の透明度", float(0.5f));
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
		waterColor = stageEditor_->GetVector3Value("プレイヤーに重ねる色");
		waterArea_->color_ = { waterColor.x,waterColor.y,waterColor.z,stageEditor_->GetFloatValue("プレイヤーに重ねる色の透明度") };
	}
	else {
		Vector3 waterColor = global_->GetVector3Value("水の色");
		noise_->noiseData_->waterColor = { waterColor.x,waterColor.y,waterColor.z,1.0f };
		waterColor = global_->GetVector3Value("うねうねの色");
		noise_->noiseData_->lightningColor = { waterColor.x,waterColor.y,waterColor.z,1.0f };
		waterColor = global_->GetVector3Value("プレイヤーに重ねる色");
		waterArea_->color_ = { waterColor.x,waterColor.y,waterColor.z,global_->GetFloatValue("プレイヤーに重ねる色の透明度") };
	}

	outline_->color_ = noise_->noiseData_->lightningColor;

	noise_->noiseData_->correctionUV = global_->GetVector2Value("映り込む背景の修正");
}
