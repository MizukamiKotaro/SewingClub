#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"

TitleScene::TitleScene()
{
	FirstInit();

	dome_ = std::make_unique<Dome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>();

	title_ = std::make_unique<Sprite>("title.png");
	title_->pos_ = { 630.0f,360.0f };
	title_->size_ *= 2;
	title_->Update();

	space_ = std::make_unique<Sprite>("space.png");
	space_->pos_ = { 640.0f,560.0f };
	space_->Update();

	rainbow_ = std::make_unique<Sprite>("rainbow.png");
	highLumi_ = std::make_unique<HighLumi>();
	post_ = std::make_unique<PostEffect>();
	texcoodY_ = 0.0f;

	scanNoise_ = std::make_unique<ScanNoise>();
	negaposi_ = std::make_unique<NegaPosiInverse>();
	mosaic_ = std::make_unique<Mosaic>();
	rgbShift_ = std::make_unique<RGBShift>();

	spotLightBox_ = std::make_unique<SpotLightBox>();
	puniru_ = std::make_unique<Sprite>("puniru.png");
	puniru_->size_ *= 0.5f;
	puniru_->Update();

	se_.LoadWave("SE/select.wav","決定音");
}

void TitleScene::Initialize()
{

}

void TitleScene::Update()
{
	texcoodY_ += 10.0f;
	if (texcoodY_ >= 720.0f) {
		texcoodY_ -= 720.0f;
	}
	rainbow_->SetTextureTopLeft({ 0.0f,texcoodY_ });

	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		ChangeScene(SELECT);
		se_.Play();
	}

#ifdef _DEBUG
	ImGui::Begin("スキャンノイズ");
	ImGui::SliderFloat("スクリーン座標の最小のy座標", &scanNoise_->scanNoiseData_->minY, 0.0f, 1.0f);
	ImGui::SliderFloat("ノイズを行う縦幅", &scanNoise_->scanNoiseData_->width, 0.0f, 1.0f);
	ImGui::SliderFloat("ノイズの強さ", &scanNoise_->scanNoiseData_->power, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Begin("モザイク");
	ImGui::SliderFloat("モザイクのサイズ", &mosaic_->mosaicData_->density, 0.1f, 100.0f);
	ImGui::SliderInt("正方形にするか", &mosaic_->mosaicData_->isSquare, 0, 1);
	ImGui::End();

	ImGui::Begin("RGBShift");
	ImGui::SliderFloat("シフトする大きさ", &rgbShift_->rgbShiftData_->shift, -1.0f, 1.0f);
	ImGui::End();
#endif // _DEBUG

	spotLightBox_->Update();
}

void TitleScene::Draw()
{
	WrightPostEffect();

	Kyoko::Engine::PreDraw();

	/*dome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());*/
	//title_->Draw();

	//scanNoise_->Draw();
	rgbShift_->Draw();
	puniru_->Draw();

	spotLightBox_->Draw(camera_.get());

	BlackDraw();

	Kyoko::Engine::PostDraw();
}

void TitleScene::WrightPostEffect()
{
	highLumi_->PreDrawScene();
	title_->Draw();
	highLumi_->PostDrawScene();

	post_->PreDrawScene();
	rainbow_->Draw();
	highLumi_->Draw(BlendMode::kBlendModeMultiply);
	post_->PostDrawScene();

	screen_->PreDrawScene();

	title_->Draw();
	post_->Draw();

	screen_->PostDrawScene();


	/*scanNoise_->PreDrawScene();
	scanNoise_->PostDrawScene();*/

	mosaic_->PreDrawScene();
	dome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());
	mosaic_->PostDrawScene();

	rgbShift_->PreDrawScene();
	mosaic_->Draw();
	space_->Draw();

	rgbShift_->PostDrawScene();
}

