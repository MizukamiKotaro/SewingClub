#include "DebugCamera.h"
#include "Camera.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Input.h"

DebugCamera::DebugCamera(Camera* camera, Input* input)
{
	camera_ = camera;
	input_ = input;
	beforeDebugTransform_ = {};
	isDebug_ = false;
	is2D_ = true;
	isPreDebug_ = isDebug_;

	isTranslate_ = false;
	isRotate_ = false;
	isRotateTranslate_ = false;

	subRotate_ = {};
	subTranslate_ = {};
	mousePos_ = {};
}

void DebugCamera::Update()
{
#ifdef _DEBUG
	isPreDebug_ = isDebug_;
	if (!ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
	}
	else {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("デバッグカメラ")) {
				ImGui::Checkbox("デバッグモードか", &isDebug_);
				ImGui::Text("LShift : デバッグモード切替");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	if (input_->PressedKey(DIK_LSHIFT)) {
		if (isDebug_) {
			isDebug_ = false;
			camera_->transform_ = beforeDebugTransform_;
		}
		else {
			isDebug_ = true;
			beforeDebugTransform_ = camera_->transform_;
			isTranslate_ = false;
			isRotate_ = false;
			isRotateTranslate_ = false;
		}
	}
	else if (!isPreDebug_ && isDebug_) {
		if (isDebug_) {
			beforeDebugTransform_ = camera_->transform_;
			isTranslate_ = false;
			isRotate_ = false;
			isRotateTranslate_ = false;
		}
		else {
			camera_->transform_ = beforeDebugTransform_;
		}
	}
#endif // _DEBUG
}

void DebugCamera::DebugUpdate()
{
#ifdef _DEBUG
	if (isDebug_) {
		if (!ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_MenuBar)) {
			ImGui::End();
		}
		else {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("デバッグカメラ")) {
					ImGui::Checkbox("2Dカメラのデバッグか(マウスでの調整)", &is2D_);
					ImGui::DragFloat3("ポジション", &camera_->transform_.translate_.x, 0.01f);
					ImGui::DragFloat3("角度", &camera_->transform_.rotate_.x, 0.01f);
					ImGui::Text("マウス真ん中長押し : 上下左右移動");
					if (!is2D_) {
						ImGui::Text("マウスホイール : 前後移動");
						ImGui::Text("マウス左長押し : 回転移動");
						ImGui::Text("マウス右長押し : 回転");
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}

		if (is2D_) {
			if (input_->PressedMouse(Input::MouseButton::CENTER) && !isRotate_ && !isRotateTranslate_) {
				mousePos_ = input_->GetMousePosition();
				isTranslate_ = true;
				subTranslate_ = camera_->transform_.translate_;
			}
			else if (input_->PressingMouse(Input::MouseButton::CENTER) && isTranslate_) {
				Vector2 vector = input_->GetMousePosition();

				Vector3 move = { -(vector.x - mousePos_.x) / 80,(vector.y - mousePos_.y) / 45,0.0f };
				move = Matrix4x4::Multiply(move, Matrix4x4::MakeRotateXYZMatrix(camera_->transform_.rotate_));

				camera_->transform_.translate_ = subTranslate_ + move;
			}
			else {
				isTranslate_ = false;
			}

			if (input_->GetWheel() && !isRotate_ && !isRotateTranslate_ && !isTranslate_) {
				float moveLength = float(input_->GetWheel()) / 120;
				Vector3 move = { 0.0f,0.0f,1.0f };
				move = Matrix4x4::Multiply(move, Matrix4x4::MakeRotateXYZMatrix(camera_->transform_.rotate_));
				move *= moveLength;
				camera_->transform_.translate_ += move;
			}
		}
		else {
			if (input_->PressedMouse(Input::MouseButton::LEFT) && !isTranslate_ && !isRotate_) {
				mousePos_ = input_->GetMousePosition();
				isRotateTranslate_ = true;
				subRotate_ = camera_->transform_.rotate_;
				subTranslate_ = camera_->transform_.translate_;
			}
			else if (input_->PressingMouse(Input::MouseButton::LEFT) && isRotateTranslate_) {

				Vector2 length = input_->GetMousePosition() - mousePos_;

				float pi = 3.141592f;

				camera_->transform_.rotate_.y = subRotate_.y - length.x / 640 * pi;
				camera_->transform_.rotate_.x = subRotate_.x + length.y / 360 * pi;

				camera_->transform_.translate_ = Matrix4x4::Multiply(subTranslate_, Matrix4x4::MakeRotateXYZMatrix({ length.y / 360 * pi, -length.x / 640 * pi,0.0f }));

			}
			else {
				isRotateTranslate_ = false;
			}

			if (input_->PressedMouse(Input::MouseButton::RIGHT) && !isTranslate_ && !isRotateTranslate_) {
				mousePos_ = input_->GetMousePosition();
				isRotate_ = true;
				subRotate_ = camera_->transform_.rotate_;
			}
			else if (input_->PressingMouse(Input::MouseButton::RIGHT) && isRotate_) {
				Vector2 length = input_->GetMousePosition() - mousePos_;
				float pi = 3.141592f;

				camera_->transform_.rotate_.y = subRotate_.y + length.x / 640 * pi;
				camera_->transform_.rotate_.x = subRotate_.x + length.y / 360 * pi;
			}
			else {
				isRotate_ = false;
			}

			if (input_->PressedMouse(Input::MouseButton::CENTER) && !isRotate_ && !isRotateTranslate_) {
				mousePos_ = input_->GetMousePosition();
				isTranslate_ = true;
				subTranslate_ = camera_->transform_.translate_;
			}
			else if (input_->PressingMouse(Input::MouseButton::CENTER) && isTranslate_) {
				Vector2 vector = input_->GetMousePosition();

				Vector3 move = { -(vector.x - mousePos_.x) / 80,(vector.y - mousePos_.y) / 45,0.0f };
				move = Matrix4x4::Multiply(move, Matrix4x4::MakeRotateXYZMatrix(camera_->transform_.rotate_));

				camera_->transform_.translate_ = subTranslate_ + move;
			}
			else {
				isTranslate_ = false;
			}

			if (input_->GetWheel() && !isRotate_ && !isRotateTranslate_ && !isTranslate_) {
				float moveLength = float(input_->GetWheel()) / 120;
				Vector3 move = { 0.0f,0.0f,1.0f };
				move = Matrix4x4::Multiply(move, Matrix4x4::MakeRotateXYZMatrix(camera_->transform_.rotate_));
				move *= moveLength;
				camera_->transform_.translate_ += move;
			}
		}
		camera_->Update();
	}
#endif // _DEBUG
}
