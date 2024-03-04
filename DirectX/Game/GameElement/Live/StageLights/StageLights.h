#pragma once
#include "DirectionalLight/DirectionalLight.h"
#include "PointLight/PointLight.h"
#include "SpotLight/SpotLight.h"
#include <memory>
#include <array>

class Camera;

class StageLights {
public: 

	StageLights();

	void Initialize();

	void Update(float time);

	void Draw(Camera* camera);

private:
	
	std::array<std::array<std::unique_ptr<SpotLight>, 2>, 5> spotLights_;
};
