#pragma once
#include "Sprite.h"
#include <memory>

class PopupUI {
public:
	PopupUI();
	~PopupUI() = default;
	void Initialize();
	void Update(const float& delta);
	void Draw();
	uint32_t GetPhase() const { return static_cast<uint32_t>(phase); }

private:
	void PopUp(); // 拡大
	void Wait(); // 待機
	void FadeOut(); // 消える

private:

	enum class Phase {
		PopUp,
		Wait,
		FadeOut
	};
	Phase phase = Phase::PopUp;

	std::unique_ptr<Sprite> sprite_;
	Vector2 scale_ = Vector2(0.0f, 0.0f);
	const Vector2 kMaxSize_ = Vector2(720.0f, 240.0f);

	struct Frame {
		float nowFrame = 0.0f;
		const float kMaxPopFrame = 1.0f;
		const float kMaxWaitFrame = 0.0f;
		const float kMaxFadeFrame = 1.2f;
		void Initialize() {
			nowFrame = 0.0f;
		}
	};
	Frame counter_;
	
	float alpha_ = 1.0f;
	bool isFinished_ = false; // 演出終了フラグ

};
