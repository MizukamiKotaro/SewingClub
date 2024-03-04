#pragma once
#include "Model.h"
#include <memory>

class Camera;

class Dome {
public: 

	Dome();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

private:
	
	std::unique_ptr<Model> model_;
};
