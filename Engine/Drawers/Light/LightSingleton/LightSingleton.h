#pragma once
#include <memory>

class ILight;

// モデルなどを生成したときに最初に入れる用
class LightSingleton
{
public:
	static LightSingleton* GetInstance();

	void Initialize();

	const ILight* GetDirectionaLight() const { return directionalLight_.get(); }
	const ILight* GetPointLight() const { return pointLight_.get(); }
	const ILight* GetSpotLight() const { return spotLight_.get(); }

private:
	LightSingleton() = default;
	~LightSingleton() = default;
	LightSingleton(const LightSingleton&) = delete;
	LightSingleton& operator=(const LightSingleton&) = delete;

private:
	std::unique_ptr<ILight> directionalLight_;
	std::unique_ptr<ILight> pointLight_;
	std::unique_ptr<ILight> spotLight_;
};
