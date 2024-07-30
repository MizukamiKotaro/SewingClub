#include "SceneTransitionEffect.h"
#include "Texture.h"
#include"DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include"TextureManager/TextureManager.h"
#include"ImGuiManager/ImGuiManager.h"

SceneTransitionEffect::SceneTransitionEffect(const std::string& sceneName)
{
	//エフェクト初期化
	dissolve_ = std::make_unique<Dissolve>();

	//マスク画像読み込みと設定
	const Texture* tex = TextureManager::GetInstance()->LoadTexture("noise0.png");
	dissolve_->SetGPUDescriptorHandle(tex->handles_->gpuHandle);
	dissolveBackTex_ = std::make_unique<Sprite>("white.png");
	dissolveBackTex_->size_ = { 1280,720 };
	dissolveBackTex_->Update();
	sceneName;
	//デバッグ用設定
	gvu_ = new GlobalVariableUser("Scene", "SceneTransition",sceneName);
	gvu_->AddItem(keys[kLuminance], dissolve_->dissolveData_->baseLuminance);
	gvu_->AddItem(keys[kChangeSpd], changeSecond_);
	gvu_->AddItemColor(keys[kDissolveColor], dissolveColor_);
	gvu_->AddItem(keys[kDifference], dissolve_->dissolveData_->difference);
	gvu_->AddItemColor(keys[kEdgeColor], edgeColor);
	
}

void SceneTransitionEffect::SetGlobalV()
{
#ifdef _DEBUG
	dissolve_->dissolveData_->baseLuminance = gvu_->GetFloatValue(keys[kLuminance]);
#endif // _DEBUG

	changeSecond_ = gvu_->GetFloatValue(keys[kChangeSpd]);
	dissolveColor_ = gvu_->GetColor(keys[kDissolveColor]);
	dissolve_->dissolveData_->difference = gvu_->GetFloatValue(keys[kDifference]);
	edgeColor = gvu_->GetColor(keys[kEdgeColor]);
	dissolve_->dissolveData_->edgeColor = { edgeColor.x,edgeColor.y,edgeColor.z };

	dissolveBackTex_->SetColor(dissolveColor_);
	
}

SceneTransitionEffect::~SceneTransitionEffect()
{

}

void SceneTransitionEffect::Initialize(float value)
{
	SetGlobalV();

	Vector4 color = { dissolveColor_.x,dissolveColor_.y,dissolveColor_.z,1 };
	Vector4 eColor = { dissolve_->dissolveData_->edgeColor.x,dissolve_->dissolveData_->edgeColor.y,dissolve_->dissolveData_->edgeColor.z,1 };

	dissolveBackTex_->SetColor(color);
	dissolveColor_ = { color.x,color.y,color.z };
	gvu_->SetVariable(keys[kDissolveColor], dissolveColor_);

	dissolve_->dissolveData_->edgeColor = { eColor.x,eColor.y,eColor.z };
	gvu_->SetVariable(keys[kEdgeColor], dissolve_->dissolveData_->edgeColor);

	dissolve_->dissolveData_->baseLuminance = value;
	preSceneChangeActive_ = false;
	postSceneChangeActive_ = false;
}

bool SceneTransitionEffect::PreSceneTransition(float delta)
{
	//だんだんマスク量減らす
	if (!preSceneChangeActive_) {
		dissolve_->dissolveData_->baseLuminance += changeSecond_ * delta;
		if (dissolve_->dissolveData_->baseLuminance >= 1.0f) {
			dissolve_->dissolveData_->baseLuminance = 1.0f;
			preSceneChangeActive_ = true;
		}
	}

	return preSceneChangeActive_;
}

bool SceneTransitionEffect::PostSceneTransition(float delta)
{
	
	if (!postSceneChangeActive_) {
		dissolve_->dissolveData_->baseLuminance -= changeSecond_ * delta;
		if (dissolve_->dissolveData_->baseLuminance <= 0) {
			dissolve_->dissolveData_->baseLuminance = 0;
			postSceneChangeActive_ = true;
		}
	}
	return postSceneChangeActive_;
}

void SceneTransitionEffect::Debug()
{
	SetGlobalV();

#ifdef _DEBUG
	
#endif // _DEBUG
}

void SceneTransitionEffect::Draw()
{
	dissolve_->Draw();
}

void SceneTransitionEffect::DrawPE()
{
	dissolve_->PreDrawScene();
	dissolveBackTex_->Draw();
	dissolve_->PostDrawScene();
}


