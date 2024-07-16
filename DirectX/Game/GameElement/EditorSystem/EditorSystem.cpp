#include "EditorSystem.h"
#include "Camera.h"
#include "Input.h"
#include <algorithm>
#include "GameElement/WaterManager/WaterManager.h"
#include "Engine/Base/WindowsInfo/WindowsInfo.h"
#ifdef _DEBUG
#include "ImGuiManager/Guizmo.h"
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

EditorSystem::EditorSystem(Camera* camera)
{
	type_ = Type::WATER;
	guizmoType_ = GuizmoType::TRANSLATE;
	state_ = State::NONE;
	waterManager_ = WaterManager::GetInstance();

	camera_ = camera;
	mousePos_ = {};
	input_ = Input::GetInstance();
	no_ = 0;
	isSelectQuad_ = false;
}

void EditorSystem::Update()
{
#ifdef _DEBUG
	mousePos_ = input_->GetMousePosition();
	mousePos_ = { mousePos_.x - 640.0f, 360.0f - mousePos_.y };
	Vector2 win = WindowsInfo::GetInstance()->GetWindowSize();
	float ratio = win.y / (std::tanf(0.225f) * (-camera_->transform_.translate_.z) * 2);

	Vector2 pos{};
	pos.x = mousePos_.x / ratio + camera_->transform_.translate_.x;
	pos.y = mousePos_.y / ratio + camera_->transform_.translate_.y;

	mousePos_ = pos;

	if (input_->PressedMouse(Input::MouseButton::CENTER)) {
		state_ = State::NONE;
	}
	else if (input_->PressedMouse(Input::MouseButton::RIGHT)) {
		state_ = State::GUIZMO;
	}
	float s = 0.0f;
	int no = 0;
	std::unordered_map<int, std::unique_ptr<WaterChunk>>& waterMap = waterManager_->GetWater();
	Guizmo::Type type = Guizmo::Type::TRANSLATE;
	Vector3 scale = {};
	Vector3 v3 = {};
	Matrix4x4 mat = {};

	switch (state_)
	{
	case EditorSystem::State::SLECT:
		if (input_->PressedMouse(Input::MouseButton::RIGHT)) {
			for (std::pair<const int, std::unique_ptr<WaterChunk>>& water : waterMap) {
				if (water.second->IsHitMouse(mousePos_)) {
					no_ = water.first;
					state_ = State::GUIZMO;
					break;
				}
			}
		}
		break;
	case EditorSystem::State::PUT:
		if (input_->PressedMouse(Input::MouseButton::LEFT)) {
			waterManager_->AddWater(mousePos_);
		}
		if (input_->PressedKey(DIK_LSHIFT)) {
			state_ = State::MAKE_QUADRANGLE;
			isSelectQuad_ = false;
		}
		break;
	case EditorSystem::State::MAKE_QUADRANGLE:
		no = 0;
		if (input_->PressedMouse(Input::MouseButton::LEFT)) {
			for (std::pair<const int, std::unique_ptr<WaterChunk>>& water : waterMap) {
				if (water.second->IsHitMouse(mousePos_)) {
					if (isSelectQuad_) {
						isSelectQuad_ = false;
						no = water.first;
					}
					else {
						isSelectQuad_ = true;
						no_ = water.first;
					}
					break;
				}
			}
			if (!isSelectQuad_) {
				if ((waterMap[no_]->GetIsTarget() && waterMap[no_]->GetEndNo() == no) || (waterMap[no]->GetIsTarget() && waterMap[no]->GetEndNo() == no_)) {
					if (waterMap[no_]->GetEndNo() == no) {
						waterMap[no_]->SetIsQuadrangle(false, no);
					}
					else {
						waterMap[no]->SetIsQuadrangle(true, no_);
					}
				}
				else {
					waterMap[no_]->SetIsQuadrangle(true, no);
				}
			}
		}
		if (input_->PressedKey(DIK_LSHIFT)) {
			state_ = State::NONE;
		}
		break;
	case EditorSystem::State::GUIZMO:
		if (input_->PressedMouse(Input::MouseButton::RIGHT)) {
			for (std::pair<const int, std::unique_ptr<WaterChunk>>& water : waterMap) {
				if (water.second->IsHitMouse(mousePos_)) {
					no_ = water.first;
					break;
				}
			}
		}
		s = waterMap[no_]->GetScale();
		scale = { s,s,1.0f };
		mat = Matrix4x4::MakeAffinMatrix(scale, Vector3{}, waterMap[no_]->GetPosition());
		switch (guizmoType_)
		{
		case EditorSystem::GuizmoType::TRANSLATE:
			type = Guizmo::Type::TRANSLATE;
			break;
		case EditorSystem::GuizmoType::SCALE:
			type = Guizmo::Type::SCALE;
			break;
		default:
			break;
		}

		v3 = Guizmo::ShowTransformGizmo(type, camera_->GetViewMatrix(), camera_->GetProjection(), mat);

		switch (guizmoType_)
		{
		case EditorSystem::GuizmoType::TRANSLATE:
			waterMap[no_]->SetPosition(v3);
			break;
		case EditorSystem::GuizmoType::SCALE:
			waterMap[no_]->SetScale(v3.x);
			break;
		default:
			break;
		}

		break;
	case EditorSystem::State::NONE:
		if (input_->PressedKey(DIK_LSHIFT)) {
			state_ = State::PUT;
		}
		break;
	default:
		break;
	}

	ImGui::Begin("エディターの状態");
	ImGui::Text("マウスの真ん中でエディターモードオフ");
	ImGui::Text("マウスの右クリックでギズモ");
	switch (state_)
	{
	case EditorSystem::State::SLECT:
		break;
	case EditorSystem::State::PUT:
		ImGui::Text("モード ： 設置");
		ImGui::Text("左クリックで配置");
		ImGui::Text("LShiftでモードを四角形生成に");
		break;
	case EditorSystem::State::GUIZMO:
		ImGui::Text("モード ： ギズモ");
		break;
	case EditorSystem::State::MAKE_QUADRANGLE:
		ImGui::Text("モード ： 四角形生成");
		ImGui::Text("LShiftでモードをオフに");
		break;
	case EditorSystem::State::NONE:
		ImGui::Text("LShiftでモードを設置に");
		break;
	default:
		break;
	}
	ImGui::End();
#endif // _DEBUG

}
