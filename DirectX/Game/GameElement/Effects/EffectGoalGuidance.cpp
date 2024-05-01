#include "EffectGoalGuidance.h"
#include"WindowsInfo/WindowsInfo.h"
#include"Math/calc.h"
#include<numbers>

UIGoalGuidance::UIGoalGuidance()
{
	model_ = std::make_unique<Sprite>("pause_arrow.png");

	gVUser_= new GlobalVariableUser("Effects", "UIGoalGuidance");


	gVUser_->AddItem(keys[SpriteSize], scale_);
	gVUser_->AddItem(keys[SphereAreaSize], sphereAreaSize_);
	gVUser_->AddItem(keys[BoxAreaSize], area_);
	gVUser_->AddItem(keys[AreaType], areaType_);
	gVUser_->AddItem(keys[DirectionGoal], goalSize_);


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
	
	Update();
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

	//向きベクトル
	Vector3 direction = *goalPos_-*playerPos_;

	//スプライト配置領域
	Vector3 spritePos = *goalPos_-(direction.Normalize() * goalSize_);
	

	Vector2 windowSize = WindowsInfo::GetInstance()->GetWindowSize();
	Matrix4x4 cameraVPV = camera_->GetViewProjection() * MakeViewPortMatrix(0, 0, windowSize.x, windowSize.y, 0, 1);
	
	

	//画面外なら寄せる
	if (areaType_ == AreaType::Squea) {
		spritePos = TransformPosition(spritePos, cameraVPV);

		if (spritePos.x + scale_.x > area_.x) {
			spritePos.x = area_.x - scale_.x;
		}
		else if (spritePos.x - scale_.x < 0) {
			spritePos.x = scale_.x;
		}

		if (spritePos.y + scale_.y > area_.y) {
			spritePos.y =- scale_.y + area_.y;
		}
		else if (spritePos.y - scale_.y < 0) {
			spritePos.y = scale_.y;
		}
	}
	else if (areaType_ == AreaType::Sphere) {
		
		//エリアのサイズに落とし込み
		spritePos = (direction.Normalize() * sphereAreaSize_) + *playerPos_ ;
		spritePos = TransformPosition(spritePos, cameraVPV);


	}

	model_->size_ = scale_;
	
	model_->rotate_ = GetYRotate(Vector2{ direction.x,direction.y })-((1.0f/2.0f)*(float)std::numbers::pi);

	model_->pos_ = { spritePos.x,spritePos.y };

	model_->Update();
}

void UIGoalGuidance::Draw(const Camera* camera)
{

	if (isDraw_) {
		model_->Draw(*camera);
	}

}

void UIGoalGuidance::Debug()
{

	scale_=gVUser_->GetVector2Value(keys[SpriteSize]);
	sphereAreaSize_ = gVUser_->GetFloatValue(keys[SphereAreaSize]);
	area_ = gVUser_->GetVector2Value(keys[BoxAreaSize]);
	areaType_ = gVUser_->GetIntValue(keys[AreaType]);
	goalSize_ = gVUser_->GetFloatValue(keys[DirectionGoal]);
}
