#pragma once
#include "Model.h"
#include <vector>
#include "GlobalVariables/GlobalVariableUser.h"

// textureのアニメーション
// 今回は板ポリでスプライト同様の描画を行うためModelを使用
class Animation2D {
	friend class AnimationManager;
public:
	Animation2D() = default;
	~Animation2D() = default;

	void Initialize(std::string fileName, const uint32_t& hDivNum = 1u, const uint32_t& wDivNum = 1u);

	/// <returns>アニメーション更新時はtrueを返す</returns>
	bool Update();

	// アニメーションの再生
	void Play(bool flag);

	// 指定したSceneのUV取得
	Transform GetSceneUV(const uint32_t& scene);
	// uv座標のゲッター
	Transform GetUVTrans() const { return transform_; }

	void SetRotate(const float& rotate) { uvRotate_ = rotate; }

private:
	// アニメーションNo.の格納
	void SceneEntry();
	// true:scene切り替え/false:scene継続
	bool AnimationCount();
	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();
	// transform更新
	void UpdateTrans(const uint32_t& listNum);

private:
	std::unique_ptr<GlobalVariableUser> global_;

	Transform transform_; // uv座標の返却用で簡易敵に用意
	float uvRotate_ = 0.0f;

	struct TextureParameter	{
		Vector2 divisionNumber{}; // 分割数
		Vector3 uvScale; // uv用のscale変数
	};
	TextureParameter texParam_;

	std::vector<Vector3> sceneNumberList_; // 左上を0としたアニメーション番号で、positionを格納する
	uint32_t nowScene_ = 0u; // 今のアニメーション番号を格納
	float nowFrame_ = 0.0f; // 今の経過フレーム、タイマー
	bool isLoop_ = true; // ループするか
	bool isPlay_ = false; // 再生するか

	struct KeyParameter {
		float keyFrame = 0.0f; // 中間フレーム このフレーム経過したら次のsceneに切り替える
		uint32_t sceneNumber = 0u; // 今のscene。
	};
	std::vector<KeyParameter> keyParam_; // 中間フレームコンテナ
	uint32_t maxKeyNumber_ = 1u; // keyの最大数

	const std::string chunkName = "Animation";

};