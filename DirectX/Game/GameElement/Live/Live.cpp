#include "Live.h"
#include "Camera.h"

Live::Live()
{
	dome_ = std::make_unique<Dome>();
	screen_ = std::make_unique<Screen>();
	stage_ = std::make_unique<Stage>();
	stageLights_ = std::make_unique<StageLights>();
}

void Live::Initialize(Camera* camera)
{
	dome_->Initialize();
	screen_->Initialize();
	stage_->Initialize();
	stageLights_->Initialize();
	WriteScreen(camera);
}

void Live::Update(float time)
{
	dome_->Update();
	screen_->Update();
	stage_->Update();
	stageLights_->Update(time);
}

void Live::Draw(Camera* camera)
{
	dome_->Draw(camera);
	stage_->Draw(camera);
	screen_->Draw(camera);

	stageLights_->Draw(camera);
}

void Live::WriteScreen(Camera* camera)
{
	screen_->PreDrawScene();

	dome_->Draw(camera);
	stage_->Draw(camera);

	screen_->PostDrawScene();
}
