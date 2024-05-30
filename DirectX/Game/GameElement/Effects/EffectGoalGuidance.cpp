#include "EffectGoalGuidance.h"
#include"WindowsInfo/WindowsInfo.h"
#include"Math/calc.h"
#include<numbers>
#include <algorithm>
UIGoalGuidance::UIGoalGuidance()
{
	model_ = std::make_unique<Sprite>("HUD_arrow.png");

	gVUser_ = new GlobalVariableUser("Effects", "UIGoalGuidance");


	gVUser_->AddItem(keys[SpriteSize], scale_);
	gVUser_->AddItem(keys[SphereAreaSize], sphereAreaSize_);
	gVUser_->AddItem(keys[BoxAreaSize], area_);
	gVUser_->AddItem(keys[AreaType], areaType_);
	gVUser_->AddItem(keys[DirectionGoal], goalSize_);
	gVUser_->AddItem(keys[FadeOutUI], maxFadeoutGoalCount_);

	gVUser_->AddItem(keys[QuotaUIDirection], quotaDirection_);
	gVUser_->AddItem(keys[QuotaUISize], quotaUISize_);
	gVUser_->AddItem(keys[QuotaUIType], quotaAreaType_);
	gVUser_->AddItem(keys[QuotaColor], quotaColor_);
	gVUser_->AddItem(keys[GoalColor], goalColor_);

}

UIGoalGuidance::~UIGoalGuidance()
{
}

void UIGoalGuidance::Initialize(const Vector3* playerPos, const Vector3* goalPos, const Camera* camera)
{
	camera_ = camera;

	playerPos_ = playerPos;

	goalPos_ = goalPos;

	model_->Initialize();

	quota_.clear();
	//ゴールとアイテムのUIの初期更新
	isQuota_ = true;
	Update();
	isQuota_ = false;
	Update();

	isQuota_ = false;
}

//ビューポート変換
Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
		width / 2.0f,0,0,0,
		0,-(height / 2.0f),0,0,
		0,0,maxDepth - minDepth,0,
		left + (width / 2.0f),top + (height / 2.0f),minDepth,1
	};
}

//座標変換
Vector3 TransformPosition(const Vector3& v, const Matrix4x4& m) {
	Vector3 result = {
		.x{v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0]},
		.y{v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1]},
		.z{v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2] },
	};
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];

	if (w == 0) {
		return Vector3();
	}

	return {
		result.x / w,
		result.y / w,
		result.z / w,
	};

}

float GetYRotate(const Vector2& v) {
	Vector2 offset = { 0,1 };


	float dot = Calc::Dot(offset, v);

	float leng = offset.Length() * Calc::MakeLength(v);

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}



	return angle;

}

void UIGoalGuidance::Update()
{
	Debug();

	if (!isQuota_) {

		//データ削除
		quota_.remove_if([](auto& data) {
			if (*data.isDead_ == true) {
				return true;
			}
			else {
				return false;
			}
			});

		Vector2 windowSize = WindowsInfo::GetInstance()->GetWindowSize();
		Matrix4x4 cameraVPV = camera_->GetViewProjection() * MakeViewPortMatrix(0, 0, windowSize.x, windowSize.y, 0, 1);


		//残ったものの更新処理
		for (auto& data : quota_) {
			//プレイヤーから目的地への向きベクトル
			Vector3 direction = *data.position_ - *playerPos_;
			//向きベクトルの線分内に画像を配置する（目的地にかぶらない分引く）
			Vector3 spritePos = *data.position_ - (direction.Normalize() * (quotaDirection_));
			//Vector3 spritePos = data.position_;

			Vector3 PP = TransformPosition(*data.position_, cameraVPV);

			//領域内チェック
			bool isScreen = false;
			//画面外なら寄せる
			if (quotaAreaType_ == AreaType::Squea) {
				spritePos = TransformPosition(spritePos, cameraVPV);

				///X領域処理
				//最大領域外チェック
				if (spritePos.x + quotaUISize_.x >(640.0f+ area_.x/2.0f)) {
					spritePos.x = (640.0f + area_.x / 2.0f) - quotaUISize_.x;
				}//最小領域外チェック
				else if (spritePos.x - quotaUISize_.x < (640.0f - area_.x / 2.0f)) {
					spritePos.x = (640.0f - area_.x / 2.0f)+ quotaUISize_.x;
				}

				if (spritePos.y + quotaUISize_.y > (360.0f + area_.y / 2.0f)) {
					spritePos.y = (360.0f + area_.y / 2.0f) - quotaUISize_.y;
				}
				else if (spritePos.y - quotaUISize_.y < (360.0f - area_.y / 2.0f)) {
					spritePos.y = (360.0f - area_.y / 2.0f)+ quotaUISize_.y;
				}

			}
			else if (quotaAreaType_ == AreaType::Sphere) {

				if (direction.Length() >= sphereAreaSize_) {
					//エリアのサイズに落とし込み
					spritePos = (direction.Normalize() * sphereAreaSize_) + *playerPos_;
					spritePos = TransformPosition(spritePos, cameraVPV);


				}
				else {
				}
			}

			if (PP.x > 0 && PP.x < 1280 && PP.y > 0 && PP.y < 720) {
				isScreen = true;
			}

			//ゴールが画面内での透明処理
			if (isScreen) {
				data.quotaFadeoutGoalCount_++;
				if (data.quotaFadeoutGoalCount_ > maxquotaFadeoutGoalCount_) {
					data.quotaFadeoutGoalCount_ = maxquotaFadeoutGoalCount_;
				}
			}
			else {
				data.quotaFadeoutGoalCount_--;
				if (data.quotaFadeoutGoalCount_ < 0) {
					data.quotaFadeoutGoalCount_ = 0;
				}

			}
			float t = (float)data.quotaFadeoutGoalCount_ / (float)maxquotaFadeoutGoalCount_;

			float alpha = Calc::Lerp(1, 0, t);

			data.sprite_->SetColor(Vector4{ quotaColor_.x,quotaColor_.y,quotaColor_.z,alpha });

			data.sprite_->size_ = quotaUISize_;

			data.sprite_->rotate_ = GetYRotate(Vector2{ direction.x,direction.y }) - ((1.0f / 2.0f) * (float)std::numbers::pi);

			data.sprite_->pos_ = { spritePos.x,spritePos.y };

			data.sprite_->Update();
		}

		if (quota_.size() == 0) {
			isQuota_ = true;
		}
	}
	if (isQuota_) {
#pragma region ゴール案内のUI処理
		//向きベクトル
		Vector3 direction = *goalPos_ - *playerPos_;

		//スプライト配置領域
		Vector3 spritePos = *goalPos_ - (direction.Normalize() * goalSize_);


		Vector2 windowSize = WindowsInfo::GetInstance()->GetWindowSize();
		Matrix4x4 cameraVPV = camera_->GetViewProjection() * MakeViewPortMatrix(0, 0, windowSize.x, windowSize.y, 0, 1);

		//領域内チェック
		bool isScreen = false;

		//画面外なら寄せる
		if (areaType_ == AreaType::Squea) {
			spritePos = TransformPosition(spritePos, cameraVPV);

			if (spritePos.x + scale_.x > (640.0f + area_.x / 2.0f)) {
				spritePos.x = (640.0f + area_.x / 2.0f) - scale_.x;
			}
			else if (spritePos.x - scale_.x < (640.0f - area_.x / 2.0f)) {
				spritePos.x = (640.0f - area_.x / 2.0f)+ scale_.x;
			}

			if (spritePos.y + scale_.y > (360.0f + area_.x / 2.0f)) {
				spritePos.y = -scale_.y + (360.0f + area_.x / 2.0f);
			}
			else if (spritePos.y - scale_.y < (360.0f - area_.x / 2.0f)) {
				spritePos.y = (360.0f - area_.x / 2.0f)+ scale_.y;
			}

		}
		else if (areaType_ == AreaType::Sphere) {

			//エリアのサイズに落とし込み
			spritePos = (direction.Normalize() * sphereAreaSize_) + *playerPos_;
			spritePos = TransformPosition(spritePos, cameraVPV);
		}

		if (goalPos_->x > 0 && goalPos_->x < 1280 && goalPos_->y>0 && goalPos_->y < 720) {
			isScreen = true;
		}

		//ゴールが画面内での透明処理
		if (isScreen) {
			fadeoutGoalCount_++;
			if (fadeoutGoalCount_ > maxFadeoutGoalCount_) {
				fadeoutGoalCount_ = maxFadeoutGoalCount_;
			}
		}
		else {
			fadeoutGoalCount_--;
			if (fadeoutGoalCount_ < 0) {
				fadeoutGoalCount_ = 0;
			}

		}

		float t = (float)fadeoutGoalCount_ / (float)maxFadeoutGoalCount_;

		float alpha = Calc::Lerp(1, 0, t);

		model_->SetColor(Vector4{ goalColor_.x,goalColor_.y,goalColor_.z ,alpha });

		model_->size_ = scale_;

		model_->rotate_ = GetYRotate(Vector2{ direction.x,direction.y }) - ((1.0f / 2.0f) * (float)std::numbers::pi);

		model_->pos_ = { spritePos.x,spritePos.y };

		model_->Update();
#pragma endregion
	}
}


void UIGoalGuidance::Draw(const Camera* camera)
{
	//ノルマ未達成ならノルマまでのガイド描画
	if (!isQuota_) {
		for (auto& data : quota_) {
			data.sprite_->Draw();
		}
	}
	else {
		if (isDraw_) {
			model_->Draw(*camera);
		}
	}
}

void UIGoalGuidance::SetQuota(const Vector3* position, const float size, const bool* isdead)
{

	QuotaData newdata;
	newdata.sprite_ = std::make_unique<Sprite>("HUD_arrow.png");
	newdata.sprite_->Initialize();
	newdata.position_ = position;
	newdata.size_ = size;
	newdata.isDead_ = isdead;

	quota_.emplace_back(std::move(newdata));

}

void UIGoalGuidance::Debug()
{

	scale_ = gVUser_->GetVector2Value(keys[SpriteSize]);
	sphereAreaSize_ = gVUser_->GetFloatValue(keys[SphereAreaSize]);
	area_ = gVUser_->GetVector2Value(keys[BoxAreaSize]);
	areaType_ = gVUser_->GetIntValue(keys[AreaType]);
	goalSize_ = gVUser_->GetFloatValue(keys[DirectionGoal]);
	maxFadeoutGoalCount_ = gVUser_->GetIntValue(keys[FadeOutUI]);

	quotaDirection_ = gVUser_->GetFloatValue(keys[QuotaUIDirection]);
	quotaUISize_ = gVUser_->GetVector2Value(keys[QuotaUISize]);
	quotaAreaType_ = gVUser_->GetIntValue(keys[QuotaUIType]);

	quotaColor_ = gVUser_->GetVector3Value(keys[QuotaColor]);
	goalColor_ = gVUser_->GetVector3Value(keys[GoalColor]);
}
