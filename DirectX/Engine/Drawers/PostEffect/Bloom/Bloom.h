#pragma once

#include "PostEffect/PostEffect.h"
#include "HighLumi/HighLumi.h"
#include "GaussianBlur/GaussianBlur.h"
#include <memory>

class Bloom
{
public:

	Bloom();

	~Bloom();

	void Initialize();

	void Update();

	void Draw();

	void PreDrawScene();

	void PostDrawScene();

	HighLumi::HighLumiData* GetHighLumiData() const { return highLumi_->highLumiData_; }

private:

	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<HighLumi> highLumi_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;

};

