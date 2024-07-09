#include "UIEnemyGuidance.h"
#include"WindowsInfo/WindowsInfo.h"
#include"calc.h"

UIEnemyGuidance::~UIEnemyGuidance()
{
	for (auto& data : datas_) {
		delete data->sp;
	}

	datas_.clear();
}

void UIEnemyGuidance::Initialize(const Camera* camera)
{
	for (auto& data : datas_) {
		delete data->sp;
	}
	datas_.clear();

	camera_ = camera;


}

//ビューポート変換
Matrix4x4 MakeViewPortM(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
		width / 2.0f,0,0,0,
		0,-(height / 2.0f),0,0,
		0,0,maxDepth - minDepth,0,
		left + (width / 2.0f),top + (height / 2.0f),minDepth,1
	};
}

//座標変換
Vector3 TransformP(const Vector3& v, const Matrix4x4& m) {
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

//領域チェック
bool CheckInside(const Vector3& p) {
	if (p.x < 1280 && p.x>0 && p.y < 720 && p.y>0) {
		return true;
	}
	else {
		return false;
	}
}

Vector2 SetArea(const Vector3& pos, const Vector2& center, const Vector2& area) {
	Vector2 re = {pos.x,pos.y};
	if (re.x > center.x + area.x) {
		re.x = center.x + area.x;
	}
	else if (re.x < center.x - area.x) {
		re.x = center.x - area.x;
	}

	if (re.y > center.y + area.y) {
		re.y = center.y + area.y;
	}
	else if (re.y < center.y - area.y) {
		re.y = center.y - area.y;
	}

	return re;
}

void UIEnemyGuidance::Update()
{

	Vector2 windowSize = WindowsInfo::GetInstance()->GetWindowSize();
	Matrix4x4 cameraVPV = camera_->GetViewProjection() * MakeViewPortM(0, 0, windowSize.x, windowSize.y, 0, 1);


	for (auto& data : datas_) {
		//スクリーン座標系に修正
		Vector3 p = TransformP(*data->pos, cameraVPV);

		//領域内に収める
		Vector2 ans = SetArea(p, {640,360}, dangerline_);

		if (CheckInside(p)) {
			if (data->count_++ >= maxCount_) {
				data->count_ = maxCount_;
			}
		}
		else {
			if (data->count_-- <= 0) {
				data->count_ = 0;
			}
		}

		float t = data->count_ / maxCount_;
		//透明度
		float alpha = Calc::Lerp(1, 0, t);
		data->sp->SetColor({ 1,1,1,alpha });

		data->sp->pos_ = ans;
		data->sp->size_ = scale_;
		data->sp->Update();
	}


}

void UIEnemyGuidance::Draw()
{
	for (auto& data : datas_) {
		data->sp->Draw();
	}
}

void UIEnemyGuidance::SetEnemyPos(const Vector3& pos)
{

	Data* newd = new Data();
	newd->pos = &pos;
	newd->sp = new Sprite(path_);

	datas_.push_back(std::move(newd));
}
