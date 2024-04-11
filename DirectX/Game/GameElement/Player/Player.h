#pragma once
#include "GameElement/Charactor/Charactor.h"
#include "Vector2.h"
#include "Vector3.h"
#include "GameElement/Yarn/Yarn.h"
#include <list>
#include "GameElement/Client/Client.h"
#include "GravityAreaSearch.h"
#include <vector>
#include "StageEditor/StageEditor.h"

class Input;
class WaterManager;
class ClientManager;

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
	// 左上の客の描画、Sprite描画
	void DrawClient();

public:
	// 惑星と衝突したときの処理、惑星にPlayerのポインタを持たせて呼び出している
	void OnCollisionPlanet(const PlanetType type, std::list<std::unique_ptr<Client>>& clients);
	// ポジションの取得
	const Vector3& GetPosition() const;

	const Vector3* GetPositionPtr() const;

	const bool& GetIsHitEnemy() const { return isHitEnemy_; }
	//void SetIsInWater(bool is) { isInWater_ = is; }

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
	// プレイヤーの軌跡に水を発生させる処理(気にしなくていい)
	void UpdateDelayProcess(float deltaTime);
	// 入力による加速の更新処理
	void UpdateInputAcceleration(float deltaTime);
	// リセット
	void Reset();
	// プレイヤーのアニメーションの初期化(気にしなくていい)
	void InitializeFloating();
	// プレイヤーのアニメーションの更新処理(気にしなくていい)
	void UpdateFloating();
	// 客を飛ばす処理
	void FireClient(float deltaTime);
	// 客を飛ばす内部的な処理
	void FireClientProcess(float deltaTime);
	// 自動で水や惑星に向かう挙動
	void AutoMove(float deltaTime);

private:
	Input* input_ = nullptr;
	WaterManager* waterManager_ = nullptr;
	ClientManager* clientManager_ = nullptr;

	std::unique_ptr<GravityAreaSearch> gravityAreaSearch_;
	std::unique_ptr<StageEditor> stageEditor_;

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
		kClientFirstSpeed, // 客を飛ばしたときの客の初速
		kClientMinSpeed, // 客を飛ばすために必要な速度
		kClientFireAngle, // 客を飛ばす角度
		kClientAbsoluteSpeed, // 客を飛ばすタイミングの速さの絶対値
		kAutoAcceleration, // 自動の時の加速度
		kAutoMaxSpeed, // 自動の時の最大速度
		kAutoLerp, // 自動の時の補間
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
		kInputFireClient, // 入力で客を飛ばすか
		kBoolEnd,
	};
	std::vector<const char*> bNames;
	std::vector<bool> bParas_;

	enum Tree1 {
		kTree1Status,
		kTree1Gravity,
		kTree1GenerationWater,
		kTree1InputAcceleration,
		kTree1Client,
		kTree1AutoMove,
		kTree1End,
	};
	std::vector<const char*> tree1Name_;
	std::vector<std::pair<int, int>> fTree1;
	std::vector<std::pair<int, int>> bTree1;

	Vector3 velocity_; // 速度
	Vector2 vector_; // 移動方向ベクトル
	float speed_;
	float addAcceleration_;
	Vector2 gravityVelocity_;
	Vector2 gravityPos_;
	bool isGravity_;
	Vector2 dotTargetPos_;
	bool isDotTarget_;
	float addAutoAcceleration_;

	Vector2 waterGravityPos_;

	int kFireClientNum_;
	int kMaxPutClient_;
	int kMaxPutWaterNum_;
	int putWaterNum_;

	float memoOutWaterSpeed_;
	bool isFireClients_;
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

	std::unique_ptr<Yarn> yarn_;
	std::list<std::unique_ptr<Client>> clients_;

	struct DelayProcess
	{
		Vector2 position_;
		float count_;
	};

	std::list<DelayProcess> delayProcess_;
	bool isMemoryPos_;

};