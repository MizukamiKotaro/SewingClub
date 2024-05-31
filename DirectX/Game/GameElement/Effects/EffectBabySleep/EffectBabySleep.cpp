#include "EffectBabySleep.h"
#include"calc.h"
#include<numbers>
EffectBabySleep::EffectBabySleep()
{

	gvu_ = new GlobalVariableUser("Effects", "BabySleep");

	gvu_->AddItem(keys[SPWait], maxSpCount_);
	gvu_->AddItem(keys[Pos], spawnP_);
	gvu_->AddItem(keys[Velo], vector_);
	gvu_->AddItem(keys[Spd], spd_);
	gvu_->AddItem(keys[Size], stedSize_);
	gvu_->AddItem(keys[Alpha], stedAlpha_);

	gvu_->AddItem(keys[MaxC], maxCount_);
	gvu_->AddItem(keys[maxCC], changeModeC_);
	gvu_->AddItem(keys[SwingS], swingS_);
	gvu_->AddItem(keys[SwingN], swingN_);

}


void EffectBabySleep::GlobalV()
{
	maxSpCount_ = gvu_->GetFloatValue(keys[SPWait]);
	spawnP_ = gvu_->GetVector2Value(keys[Pos]);
	vector_ = gvu_->GetVector2Value(keys[Velo]);
	spd_ = gvu_->GetFloatValue(keys[Spd]);
	stedSize_ = gvu_->GetVector2Value(keys[Size]);
	stedAlpha_ = gvu_->GetVector2Value(keys[Alpha]);

	maxCount_ = gvu_->GetFloatValue(keys[MaxC]);
	changeModeC_ = gvu_->GetFloatValue(keys[maxCC]);
	swingS_ = gvu_->GetFloatValue(keys[SwingS]);
	swingN_ = gvu_->GetFloatValue(keys[SwingN]);
}

EffectBabySleep::~EffectBabySleep()
{
}

void EffectBabySleep::Initialize()
{
	datas_.clear();

	GlobalV();
}



void EffectBabySleep::Update(float delta)
{

#ifdef _DEBUG
	GlobalV();
#endif // _DEBUG


	delta;
#pragma region 出現処理
	if ((spCount_ += 1.0f) > maxSpCount_) {
		spCount_ = 0;

		Data newdata;
		newdata.sp = std::make_unique<Sprite>("Z.png");
		newdata.pos = spawnP_;
		newdata.velo = vector_ * spd_;
		float size = stedSize_.x;
		newdata.sp->size_ = { size,size };
		newdata.alpha = stedAlpha_.x;
		datas_.push_back(std::move(newdata));
	}
#pragma endregion

#pragma region 更新と廃棄処理
	for (auto& data : datas_) {

		if ((data.count += 1.0f) >= maxCount_) {
			data.isDead = true;
		}

		
		float t = data.count / maxCount_;

		float size = Calc::Lerp(stedSize_.x, stedSize_.y, t);
		data.sp->size_ = { size,size };

		if (data.mode == None) {
			if (data.count >= changeModeC_) {
				data.mode = FadeOut;
			}
		}
		else {
			float nt = (data.count-changeModeC_) / (maxCount_-changeModeC_);
			data.alpha = Calc::Lerp(stedAlpha_.x,stedAlpha_.y, nt);
			data.sp->SetColor({ 1,1,1,data.alpha });
		}

		//ある値を超えると0に戻す
		data.sinT += swingS_ / 60.0f;
		data.sinT = std::fmod(data.sinT, 2.0f * (float)std::numbers::pi);

		data.animeP.y -= std::sin(data.sinT) * swingN_;

		//サイン波のanimationの値を合わせたのを追加
		data.pos += data.velo;
		data.sp->pos_ = data.pos + data.animeP;
		data.sp->Update();

	}

	datas_.remove_if([](auto& data) {
		if (data.isDead == true) {
			return true;
		}
		else {
			return false;
		}
	});
#pragma endregion

}

void EffectBabySleep::Draw()
{
	for (auto& data : datas_) {
		data.sp->Draw();
	}
}

void EffectBabySleep::SetSpawnPos(const Vector2& pos)
{
	spawnP_ = pos;
}


