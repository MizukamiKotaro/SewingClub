#pragma once
#include "Model.h"
#include <vector>

// textureのアニメーション
// 今回は板ポリでスプライト同様の描画を行うためModelを使用
class Animation2D {
public:
	Animation2D() = default;
	~Animation2D() = default;

	void Initialize(Model* model, const float& height, const float& width, const uint32_t& hDivNum, const uint32_t& wDivNum);

	void Update();

private:
	// アニメーションNo.の格納
	void SceneEntry();

private:
	Model* model_ = nullptr; // modelPtr

	struct TextureParameter	{
		Vector2 divisionNumber; // 分割数
		Vector2 textureSize; // textureの全体サイズ
		Vector2 sceneSize; // 1シーンのSpriteサイズ
	};
	TextureParameter texParam_;
	Vector3 uvScale_; // uv用のscale変数

	std::vector<Vector3> sceneNumberList_; // 左上を0としたアニメーション番号で、positionを格納する
	uint32_t nowScene_ = 0u; // 今のアニメーション番号を格納

};