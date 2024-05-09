#include "DrawQuadrangle.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include <algorithm>
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "WindowsInfo/WindowsInfo.h"
#include "Texture.h"
#include "GraphicsPipelineSystem/PipelineTypeConfig.h"
#include "GraphicsPipelineSystem/GraphicsPiplineManager/GraphicsPiplineManager.h"

DrawQuadrangle::DrawQuadrangle()
{
	CreateVertexRes();
	CreateMaterialRes();
	CreateTranformRes();
	TransferUV();
}

DrawQuadrangle::~DrawQuadrangle()
{
	vertexResource_->Release();
	transformResource_->Release();
	materialResource_->Release();
}

void DrawQuadrangle::Draw(const Camera& camera, const Vector4& color, const Texture& texture, BlendMode blendMode)
{
	psoManager_->PreDraw(PipelineType::SPRITE);

	materialData_->color = color;
	transformData_->WVP = Matrix4x4::MakeIdentity4x4() * camera.GetOrthographicMat();

	psoManager_->SetBlendMode(PipelineType::SPRITE, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(2, texture.handles_->gpuHandle);
	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(6, 1, 0, 0);
}

void DrawQuadrangle::SetVertex(const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom)
{
	vertexData_[0].vertexPos = { leftBottom.x,leftBottom.y,0.0f,1.0f }; // 左下
	vertexData_[1].vertexPos = { leftTop.x,leftTop.y,0.0f,1.0f }; // 左上
	vertexData_[2].vertexPos = { rightBottom.x,rightBottom.y,0.0f,1.0f }; // 右下

	vertexData_[3].vertexPos = { leftTop.x,leftTop.y,0.0f,1.0f }; // 左上
	vertexData_[4].vertexPos = { rightTop.x,rightTop.y,0.0f,1.0f }; // 右上
	vertexData_[5].vertexPos = { rightBottom.x,rightBottom.y,0.0f,1.0f }; // 右下
}

void DrawQuadrangle::TransferUV()
{
	vertexData_[0].texcoord = { 0.0f,1.0f }; // 左下
	vertexData_[1].texcoord = {}; // 左上
	vertexData_[2].texcoord = { 1.0f,1.0f }; // 右下

	vertexData_[3].texcoord = {}; // 左上
	vertexData_[4].texcoord = { 1.0f,0.0f }; // 右上
	vertexData_[5].texcoord = { 1.0f,1.0f }; // 右下
}

void DrawQuadrangle::CreateVertexRes()
{
	//Sprite用の頂点リソースを作る
	vertexResource_ = DirectXBase::CreateBufferResource(sizeof(VertexData) * 6);
	//頂点バッファーを作成する
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void DrawQuadrangle::CreateMaterialRes()
{
	//マテリアル用のリソースを作る。今回はcolor1つ分を用意する
	materialResource_ = DirectXBase::CreateBufferResource(sizeof(Material));
	//マテリアルデータを書き込む
	//書き込むためのアドレスを取得\l
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//今回は赤を書き込んでいる
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	//*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , false };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();
}

void DrawQuadrangle::CreateTranformRes()
{
	//Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformResource_ = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	//書き込むためのアドレスを取得
	transformResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));
}

