#pragma once
#include "Model.h"
#include <vector>

// textureのアニメーション
// 今回は板ポリでスプライト同様の描画を行うためModelを使用
class Animation2D {
public:
	Animation2D() = default;
	~Animation2D() = default;

	void Initialize(Model* model, const uint32_t& hDivNum, const uint32_t& wDivNum);

	void Update();

private:
	// アニメーションNo.の格納
	void SceneEntry();

private:
	Model* model_ = nullptr; // modelPtr

	struct TextureParameter	{
		Vector2 divisionNumber{}; // 分割数
	};
	TextureParameter texParam_;
	Vector3 uvScale_; // uv用のscale変数

	std::vector<Vector3> sceneNumberList_; // 左上を0としたアニメーション番号で、positionを格納する
	uint32_t nowScene_ = 0u; // 今のアニメーション番号を格納

	struct KeyParameter {
		float keyFrame = 0.0f; // 中間フレーム このフレーム経過したら次のsceneに切り替える
		uint32_t sceneNumber = 0u; // 今のscene。
	};
	std::vector<KeyParameter> keyParam_; // 中間フレームコンテナ

	float maxFrame_ = 0.0f;
	float nowFrame_ = 0.0f;


};