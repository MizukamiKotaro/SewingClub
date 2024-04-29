#pragma once
#include"Model.h"
#include<iostream>

class EffectGoalGuidance {

public:

	EffectGoalGuidance();
	~EffectGoalGuidance();

	void Initialize(const Vector3&playerPos,const Vector3&goalPos,const float&goalSize);

	void Update();

	void Draw(const Camera* camera);

private:

	std::unique_ptr<Model>model_;

	float scale_=1.0f;

	const Vector3* playerPos_;

	const Vector3* goalPos_;

	const float* goalSize_;

	bool isDraw_;

	bool isSphereArea_;

	float sphereAreaSize_;

	//配置制限
	Vector2 area_;


};
