#pragma once
#include "Model.h"
#include "SpotLight/SpotLight.h"
#include "PointLight/PointLight.h"
#include <array>

class Camera;

class SpotLightBox {
public:
	SpotLightBox();
	void Initialize();
	void Update();
	void Draw(Camera* camera);

private:
	void UpdatePositions();

private:
	std::unique_ptr<Model> box_;
	std::unique_ptr<PointLight> point_;
	std::array<std::unique_ptr<SpotLight>, 2> spots_;
};