#pragma once

#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelData;
class NodeData;
class Animation;

class ModelDataManager 
{
public:

	static ModelDataManager* GetInstance();

	void Finalize();

	//uint32_t LoadObj(const std::string& fileName);
	// 例)
	// Cubeフォルダ
	//   Cube.obj
	//   Cube.mtl
	//   hoge.png
	// の状態

	/// <summary>
	/// objファイルの読み込み
	/// </summary>
	/// <param name="fileName">.objが入っているフォルダの名前( "Cube" 等)</param>
	/// <returns>モデルデータのハンドル</returns>
	const ModelData* LoadObj(const std::string& fileName);

	/// <summary>
	/// glTFファイルの読み込み
	/// </summary>
	/// <param name="fileName">.glTFが入っているフォルダの名前( "Cube" 等)</param>
	/// <returns>モデルデータのハンドル</returns>
	const ModelData* LoadGLTF(const std::string& fileName);

	Animation LoadAnimation(const std::string& fileName);

private:
	ModelDataManager() = default;
	~ModelDataManager() = default;
	ModelDataManager(const ModelDataManager&) = delete;
	ModelDataManager& operator=(const ModelDataManager&) = delete;

	void LoadObjFile(const std::string& directoryPath, const std::string& fileName);

	NodeData ReadNode(aiNode* node);

	Animation LoadAnimation(const std::string& directoryPath, const std::string& fileName);

	void LoadGLTFFile(const std::string& directoryPath, const std::string& fileName);

private:
	std::vector<std::unique_ptr<ModelData>> modelDatas_;

	const std::string directoryPath_ = "Resources/Object";

};