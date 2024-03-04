#pragma once
#include <d3d12.h>
#include <string>
#include <unordered_map>
#include <memory>
#include "Externals/DirectXTex/DirectXTex.h"

class Texture;
class DescriptorHeap;

class TextureManager {
public:
	static TextureManager* GetInstance();

	void Initialize();

	void Finalize();

public:
	const Texture* LoadTexture(const std::string& filePath);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	DirectX::ScratchImage Load(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata);

	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private:
	const std::string directoryPath_ = "Resources/Texture/";

	ID3D12Device* device_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	DescriptorHeap* srvHeap_ = nullptr;

	std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap_;
};