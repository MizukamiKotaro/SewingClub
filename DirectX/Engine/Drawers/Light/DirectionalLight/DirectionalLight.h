#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "ILight/ILight.h"

class DirectionalLight : public ILight
{
public:
	struct DirectionalLightData
	{
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
	};

	DirectionalLight();
	~DirectionalLight() override;

	void Update() override;

public:
	DirectionalLightData* light_ = nullptr;
};