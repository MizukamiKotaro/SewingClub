#include "ModelDataManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include <cassert>
#include "Engine/Base/DebugLog/DebugLog.h"
#include <format>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "TextureManager/TextureManager.h"
#include "ModelData/ModelData.h"

ModelDataManager* ModelDataManager::GetInstance()
{
	static ModelDataManager instance;
	return &instance;
}

void ModelDataManager::Finalize()
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {
		modelDatas_[modelNum]->mesh.vertexResource_->Release();
	}
}

const ModelData* ModelDataManager::LoadObj(const std::string& fileName)
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {

		if (modelDatas_[modelNum]->fileName == fileName) {
			return modelDatas_[modelNum].get();
		}
	}

	LoadObjFile(directoryPath_, fileName);

	return modelDatas_.back().get();
}

uint32_t ModelDataManager::LoadGLTF(const std::string& fileName)
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {

		if (modelDatas_[modelNum]->fileName == fileName) {
			return modelNum;
		}
	}

	LoadGLTFFile(directoryPath_, fileName);

	return static_cast<uint32_t>(modelDatas_.size()) - 1;
}

void ModelDataManager::LoadObjFile(const std::string& directoryPath, const std::string& fileName)
{
	// 1. 中で必要となる変数の宣言
	modelDatas_.push_back(std::make_unique<ModelData>());; // 構築するModelData

	modelDatas_.back()->fileName = fileName;

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName + "/" + fileName + ".obj";
	const aiScene* scene_ = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene_->HasMeshes());

	// meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene_->mNumMeshes; meshIndex++) {
		aiMesh* mesh = scene_->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないmeshは非対応
		assert(mesh->HasTextureCoords(0)); // texCoordがないmeshは非対応

		// faceを解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート

			// vertexを解析
			for (uint32_t element = 0; element < face.mNumIndices; element++) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex;
				vertex.vertexPos = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				// aiProcess_MakeLefthandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.vertexPos.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				modelDatas_.back()->mesh.verteces.push_back(vertex);
			}
		}
	}

	bool isLoadTexture = false;
	// materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene_->mNumMaterials; materialIndex++) {
		aiMaterial* material = scene_->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			std::string texFilePath;
			if (textureFilePath.length > 0) {
				texFilePath = textureFilePath.C_Str();
				std::filesystem::path filePathName(texFilePath);
				texFilePath = filePathName.filename().string();
			}
			else {
				texFilePath = "white.png";
			}
			isLoadTexture = true;
			modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
		}
	}

	if (!isLoadTexture) {
		std::string texFilePath = "white.png";
		modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
	}

	modelDatas_.back()->mesh.vertexResource_ = DirectXBase::CreateBufferResource(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());

	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	modelDatas_.back()->mesh.vertexBufferView_.BufferLocation = modelDatas_.back()->mesh.vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	modelDatas_.back()->mesh.vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
	//頂点当たりのサイズ
	modelDatas_.back()->mesh.vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	modelDatas_.back()->mesh.vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.vertexData_));
	std::memcpy(modelDatas_.back()->mesh.vertexData_, modelDatas_.back()->mesh.verteces.data(), sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
}

NodeData ModelDataManager::ReadNode(aiNode* node)
{
	NodeData result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); // 列ベクトルを行ベクトルに転置
	
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.localMatrix.m[row][column] = aiLocalMatrix[row][column];
		}
	}

	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

void ModelDataManager::LoadGLTFFile(const std::string& directoryPath, const std::string& fileName)
{// 1. 中で必要となる変数の宣言
	modelDatas_.push_back(std::make_unique<ModelData>());; // 構築するModelData

	modelDatas_.back()->fileName = fileName;

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName + "/" + fileName + ".gltf";
	const aiScene* scene_ = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene_->HasMeshes());

	// meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene_->mNumMeshes; meshIndex++) {
		aiMesh* mesh = scene_->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないmeshは非対応
		assert(mesh->HasTextureCoords(0)); // texCoordがないmeshは非対応

		// faceを解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート
	
			// vertexを解析
			for (uint32_t element = 0; element < face.mNumIndices; element++) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex;
				vertex.vertexPos = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				// aiProcess_MakeLefthandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.vertexPos.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				modelDatas_.back()->mesh.verteces.push_back(vertex);
			}
		}

		if (mesh->HasBones()) {
			// boneの解析
			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
				aiBone* bone = mesh->mBones[boneIndex];
				aiString boneName = bone->mName;
				aiMatrix4x4 offsetMatrix = bone->mOffsetMatrix;
				offsetMatrix.Transpose();

				modelDatas_.back()->boneData.name = boneName.C_Str();
				for (int row = 0; row < 4; row++) {
					for (int column = 0; column < 4; column++) {
						modelDatas_.back()->boneData.offsetMatrix.m[row][column] = offsetMatrix[row][column];
					}
				}

				// VertexWeightの解析
				for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
					VertexWeight vertexWeight;

					vertexWeight.vertexID = bone->mWeights[weightIndex].mVertexId;
					vertexWeight.weight = bone->mWeights[weightIndex].mWeight;

					modelDatas_.back()->boneData.vertexWeights.push_back(vertexWeight);

				}
			}
		}
	}

	// materialを解析する
	// ここ間違い
	/*for (uint32_t materialIndex = 0; materialIndex < scene_->mNumMaterials; materialIndex++) {
		aiMaterial* material = scene_->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

			std::string texFilePath = textureFilePath.C_Str();
			std::filesystem::path filePathName(texFilePath);
			texFilePath = filePathName.filename().string();

			texFilePath = directoryPath + "/" + fileName + "/" + texFilePath.c_str();

			modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
		}
	}*/
	modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture("white.png");

	// rootNodeの解析
	modelDatas_.back()->rootNode = ReadNode(scene_->mRootNode);

	// animationの解析
	if (scene_->HasAnimations()) {
		for (uint32_t animationIndex = 0; animationIndex < scene_->mNumAnimations; animationIndex++) {
			aiAnimation* animation = scene_->mAnimations[animationIndex];
			modelDatas_.back()->animationData.duration = static_cast<float>(animation->mDuration);
			modelDatas_.back()->animationData.ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);

			for (uint32_t channelIndex = 0; channelIndex < animation->mNumChannels; channelIndex++) {
				aiNodeAnim* nodeAnim = animation->mChannels[channelIndex];
				
				NodeAnime channel;
				channel.name = nodeAnim->mNodeName.C_Str();

				for (uint32_t positionIndex = 0; positionIndex < nodeAnim->mNumPositionKeys; positionIndex++) {
					aiVectorKey key = nodeAnim->mPositionKeys[positionIndex];
					NodeAnime::Positions position;
					position.time = static_cast<float>(key.mTime);
					position.position = { key.mValue.x,key.mValue.y ,key.mValue.z };

					channel.positions.push_back(position);
				}
				for (uint32_t rotateIndex = 0; rotateIndex < nodeAnim->mNumRotationKeys; rotateIndex++) {
					aiQuatKey key = nodeAnim->mRotationKeys[rotateIndex];
					NodeAnime::Rotates rotate;
					rotate.time = static_cast<float>(key.mTime);
					rotate.rotate = { key.mValue.x,key.mValue.y ,key.mValue.z, key.mValue.w };

					channel.rotates.push_back(rotate);
				}
				for (uint32_t scaleIndex = 0; scaleIndex < nodeAnim->mNumScalingKeys; scaleIndex++) {
					aiVectorKey key = nodeAnim->mScalingKeys[scaleIndex];
					NodeAnime::Scales scale;
					scale.time = static_cast<float>(key.mTime);
					scale.scale = { key.mValue.x,key.mValue.y ,key.mValue.z };

					channel.scales.push_back(scale);
				}

				modelDatas_.back()->animationData.channels.push_back(channel);
			}
		}
	}

	modelDatas_.back()->mesh.vertexResource_ = DirectXBase::CreateBufferResource(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());

	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	modelDatas_.back()->mesh.vertexBufferView_.BufferLocation = modelDatas_.back()->mesh.vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	modelDatas_.back()->mesh.vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
	//頂点当たりのサイズ
	modelDatas_.back()->mesh.vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	modelDatas_.back()->mesh.vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.vertexData_));
	std::memcpy(modelDatas_.back()->mesh.vertexData_, modelDatas_.back()->mesh.verteces.data(), sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
}
