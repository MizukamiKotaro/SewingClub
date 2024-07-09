#pragma once
#include <memory>
#include "GlobalVariables/GlobalVariableUser.h"
#include "Sprite.h"
#include "Vector3.h"
#include "Vignette/Vignette.h"
#include "Audio.h"

class FragmentVignette
{
public:

	FragmentVignette();

	void Initialize();

	void Update(const float& hp);

	void Draw();

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:

	std::unique_ptr<GlobalVariableUser> globalVariable_;
	std::unique_ptr<Vignette> vignette_;
	std::unique_ptr<Sprite> sprite_;
	Vector3 finalColor_;
	float minScale_;
	float maxScale_;
	bool isDraw_;
	// 割れたときの音
	Audio se_;
	float preHP_;
	bool isSe_[4];
};