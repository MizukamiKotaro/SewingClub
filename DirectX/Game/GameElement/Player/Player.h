#pragma once
#include "GameElement/Charactor/Charactor.h"
#include "Vector2.h"
#include "Vector3.h"
#include <list>
#include "GravityAreaSearch.h"
#include <vector>
#include "StageEditor/StageEditor.h"
#include"Audio.h"
#include"GameElement/Effects/EffectExtraJump.h"
#include"GameElement/Effects/EffectOutWater.h"
#include"GameElement/Effects/EffectEnterWater.h"
#include"GameElement/Effects/EffectUIEnterWater.h"
#include"GameElement/Effects/particleAcceleration.h"
#include "GameElement/Animation/Animation2D.h"

class Input;
class WaterManager;

class Player : public Charactor
{
public:

	Player();
	// 初期化
	void Initialize() override;
	// 更新処理
	void Update(float deltaTime) override;
	// 描画、model描画
	void Draw(const Camera* camera) override;

	//エフェクト描画
	void EffectDraw();

	//UI描画
	void DrawUI();

	//シーン変わり目の処理
	void Finalize();
public:
	// ポジションの取得
	const Vector3& GetPosition() const;

	const Vector3* GetPositionPtr() const;

	const bool& GetIsHitEnemy() const { return isHitEnemy_; }
	//void SetIsInWater(bool is) { isInWater_ = is; }

	const bool& GetPreInWater() const { return preIsInWater_; }

	const Vector3& GetVelocity() const { return velocity_; }

	const Vector3& GetRotate() const { return model_->transform_.rotate_; }

	const float& GetSpeed() const { return speed_; }

private:
	// グローバル変数の初期化、std::vectorに変更したためここで定義している
	void InitializeGlobalVariable();
	// 衝突したときの処理
	void OnCollision(const Collider& collider) override;
	// コライダーのセット
	void SetCollider();
	// グローバル変数のセット
	void SetGlobalVariable() override;
	// グローバル変数の更新
	void ApplyGlobalVariable() override;

	// 水や惑星内での更新処理
	void Move(float deltaTime);
	// 水や惑星から飛び出たときの処理
	void PopUpFromWater();
	// 水や惑星に入ったときの処理
	void ComeToWater();
	// 水や惑星の外での処理
	void OutWater(float deltaTime);
	// 入力による加速の更新処理
	void UpdateInputAcceleration(float deltaTime);
	// リセット
	void Reset();
	// プレイヤーのアニメーションの初期化(気にしなくていい)
	void InitializeFloating();
	// プレイヤーのアニメーションの更新処理(気にしなくていい)
	void UpdateFloating();

private:
	Input* input_ = nullptr;
	WaterManager* waterManager_ = nullptr;

	std::unique_ptr<GravityAreaSearch> gravityAreaSearch_;
	std::unique_ptr<StageEditor> stageEditor_;

#pragma region JＳＯＮ関係

	enum FloatParamater {
		kAcceleration, // 加速度
		kAttenuation, // 減衰率
		kMaxSpeed, // 最大速度
		kMinSpeed, // 最低速度
		kMaxAddAcceleration, // 加算される加速度の最大値
		kInterpolationRateInWater, // 水中での補間の割合
		kInterpolationRate, // 補間の割合
		kFloatingTime, // 上下挙動の1往復の時間
		kOutWaterAcceleration, // 水から飛び出したときの加速度
		kOutWaterTime, // 水から飛び出したときに加速させる時間
		kMinPositionY, // プレイヤーの最低の高さ
		kKeepSpeedTime, // 加速を維持する時間
		kGravity, // 重力
		kBuoyancy, // 水の浮力
		kGravityDown, // 降下中の重力
		kGravityWater, // 水の塊の重力
		kWaterSize, // プレイヤーが生成する水のサイズ
		kWaterRecovery, // 水の回復時間
		kWaterSizeMove, // ジャンプで生成する水のサイズ
		kDelayTime, // 水の生成に遅延させる時間
		kJumpInputAcceleration, // ジャンプ中の入力の加速度
		kInputAcceleration, // ボタン入力による加速度
		kRecoveryInputTime, // ボタン加速のクールタイム
		kInputAccelerationTime, // ボタン入力による加速させる時間
		kFloatEnd,
	};
	std::vector<const char*> fNames;
	std::vector<float> fParas_;

	enum BoolParamater {
		kGravityArea, // 水ごとに重力がありか
		kIsBuoyancy, // 水の浮力があるか
		kGravityAreaSearch, // 一番近くの重力場に引き寄せられるか
		kAddWaterTriger, // ボタンを押したときに水を生成するか
		kAddWaterMove, // ジャンプしたときに水を生成するか
		kJumpInput, // ジャンプ中に入力を受け付けるか
		kAccelerationInput, // ボタン入力で加速できるか
		kAccelerationInJump, // ボタン入力でジャンプ中に加速できるか
		kRecoveryInJump, // ボタン入力で加速後ジャンプしたときに加速ボタンが回復するか
		kBoolEnd,
	};
	std::vector<const char*> bNames;
	std::vector<bool> bParas_;

	enum Tree1 {
		kTree1Status,
		kTree1Gravity,
		kTree1GenerationWater,
		kTree1InputAcceleration,
		kTree1End,
	};
	std::vector<const char*> tree1Name_;
	std::vector<std::pair<int, int>> fTree1;
	std::vector<std::pair<int, int>> bTree1;
#pragma endregion

	Vector3 velocity_; // 速度
	Vector2 vector_; // 移動方向ベクトル
	float speed_;
	float addAcceleration_;
	Vector2 gravityVelocity_;
	Vector2 gravityPos_;
	bool isGravity_;

	Vector2 waterGravityPos_;

	float memoOutWaterSpeed_;
	bool isHitEnemy_;

	float timeCount_;
	float coolTimeCount_;
	float accelerationTimeCount_;
	bool isUseInputAcceleration_;
	bool isInputAcceleration_;
	float waterRecoveryTimeCount_;

	bool preIsInWater_;
	bool isInWater_;

	float floatingParameter_;

	//水に入る音
	Audio seIn2Water_;
	//水から出る音
	Audio seOutWater_;
	//水の中の音
	Audio seStayWater_;

	//通常水面ジャンプ演出の数
	int normalJumpEffectNum_ = 5;

	//追加ジャンプエフェクト
	std::unique_ptr<EffectExtraJump>effeExtraJump_;

	// アニメーション
	std::unique_ptr<Animation2D> animation_;


	//水しぶきエフェクト
	std::unique_ptr<EffectOutWater>effectOutWater_;
	std::unique_ptr<EffectEnterWater>effeEnterWater_;
	std::unique_ptr<EffectUIEnterWater>effeUIEnterWater_;
	//加速時のパティ
	std::unique_ptr<ParticleAcceleration>particleScceleration_;
};