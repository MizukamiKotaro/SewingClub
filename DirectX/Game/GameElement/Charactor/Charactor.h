//#pragma once
//#include "Model.h"
//#include "ViewProjection.h"
//#include "WorldTransform.h"
//#include "Input.h"
//#include "BaseCharacter.h"
//#include <optional>
//
//class Player : public BaseCharacter {
//public:
//	/// <summary>
//	/// 初期化
//	/// </summary>
//	void Initialize(const std::vector<Model*>& models) override;
//
//	/// <summary>
//	/// 更新
//	/// </summary>
//	void Update() override;
//
//	// <summary>
//	/// 描画
//	/// </summary>
//	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
//	void Draw(const ViewProjection& viewProjection) override;
//
//	//const WorldTransform& GetWorldTransform() { return worldTransformBase_; }
//
//	void SetViewProjection(const ViewProjection* viewProjection) {
//		viewProjection_ = viewProjection;
//	}
//
//	void ApplyGlobalVariable();
//
//private:
//
//	void InitializeFloatingGimmick();
//
//	void UpdateFloatingGimmick();
//
//	void BehaviorRootInitialize();
//
//	void BehaviorRootUpdate();
//
//	void BehaviorAttackInitialize();
//
//	void BehaviorAttackUpdate();
//
//	void AttackBehaviorExtraInitialize();
//
//	void AttackBehaviorExtra2Initialize();
//
//	void AttackBehaviorAttackInitialize();
//
//	void AttackBehaviorRigorInitialize();
//
//	void AttackBehaviorReturnInitialize();
//
//	void AttackBehaviorExtraUpdate();
//
//	void AttackBehaviorExtra2Update();
//
//	void AttackBehaviorAttackUpdate();
//
//	void AttackBehaviorRigorUpdate();
//
//	void AttackBehaviorReturnUpdate();
//
//	enum class Behavior {
//		kRoot, // 通常状態
//		kAttack, // 攻撃中
//	};
//
//	enum class BehaviorAttack {
//		kExtra, // 予備動作
//		kExtra2,  // 予備動作2
//		kAttack, // 攻撃
//		kRigor, // 硬直
//		kReturn, // 姿勢を戻す
//	};
//
//	void EaseVectorClear();
//
//private:
//	// ワールド変換データ
//	//WorldTransform worldTransformBase_;
//	enum Joints {
//		kModelIndexBody,
//		kModelIndexHead,
//		kModelIndexL_arm,
//		kModelIndexR_arm,
//		kModelIndexWeapon,
//	};
//
//	WorldTransform worldTransformBody_;
//	WorldTransform worldTransformHead_;
//	WorldTransform worldTransformL_arm_;
//	WorldTransform worldTransformR_arm_;
//
//	WorldTransform worldTransformWeapon_;
//
//	// 振る舞い
//	Behavior behavior_ = Behavior::kRoot;
//	BehaviorAttack behaviorAttack_ = BehaviorAttack::kExtra;
//
//	// 次の振る舞いリクエスト
//	std::optional<Behavior> behaviorRequest_ = std::nullopt;
//
//	// 次の振る舞いリクエスト
//	std::optional<BehaviorAttack> behaviorAttackRequest_ = std::nullopt;
//
//	// イージング用
//	std::vector<Vector3> easeStartPos_;
//	std::vector<Vector3> easeEndPos_;
//	std::vector<Vector3> easeStartRot_;
//	std::vector<Vector3> easeEndRot_;
//	int count_ = 0;
//
//	enum EaseNum {
//		kBody,
//		kLArm,
//		kRArm,
//		kWeapon
//	};
//
//
//
//	// モデル
//	/*Model* modelBody_ = nullptr;
//	Model* modelHead_ = nullptr;
//	Model* modelL_arm_ = nullptr;
//	Model* modelR_arm_ = nullptr;
//	*/
//	// テクスチャハンドル
//	//uint32_t textureHandle_ = 0u;
//
//	Input* input_ = nullptr;
//
//	const ViewProjection* viewProjection_ = nullptr;
//
//	float floatingParameter_ = 0.0f;
//};
