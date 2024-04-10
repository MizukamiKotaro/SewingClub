#pragma once
#include "Model.h"
#include <memory>
#include "Animation2D.h"

class TestModel {
public:
	TestModel();
	~TestModel() = default;

	void Update();
	void Draw(const Camera* camera);

private:
	std::unique_ptr<Model> model_;
	std::unique_ptr<Animation2D> animation_;

};