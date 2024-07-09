#pragma once

#include <memory>
#include "GlobalVariables/GlobalVariableUser.h"

enum class HitStopType {
	kBabyJump,
	kBabyCatch,
	kBabyComb,
	kHitStopEnd,
};

class HitStop
{
public:
	static void Initialize();
	static void Update(float& deltaTime);
	static void SetHitStop(const HitStopType& type);

private:
	static float time_;
	static float endTime_;
	static std::unique_ptr<GlobalVariableUser> global_;
	static std::vector<const char*> fPara_;
};