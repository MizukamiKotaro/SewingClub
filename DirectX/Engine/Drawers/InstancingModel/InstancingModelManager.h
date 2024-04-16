#pragma once
#include <list>
#include <unordered_map>
#include <memory>
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "Drawers/IDrawer/IDrawer.h"

#include "ModelData/ModelData.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "ILight/ILight.h"

#include "InstancingModelData.h"
#include "InstancingModelList/InstancingModelList.h"

class Camera;

class InstancingModelManager : public IDrawer
{
public:

	static InstancingModelManager* GetInstance();

	void FirstInitialize();

	void Draw(const Camera& camera);

	const InstancingMeshTexData* GetDrawData(const InstancingMeshTexData& data);
	// 板ポリの場合
	const InstancingMeshTexData* GetDrawData(const std::string& texturePath, const BlendMode& blendMode = BlendMode::kBlendModeNormal);

	InstancingModelData* const AddBox(const InstancingMeshTexData* modelData, InstancingModelData&& model);

	void Clear();

	void SetLight(const InstancingMeshTexData* modelData, const ILight* light);

private:
	InstancingModelManager() = default;
	~InstancingModelManager() = default;
	InstancingModelManager(const InstancingModelManager&) = delete;
	const InstancingModelManager& operator=(const InstancingModelManager&) = delete;

private:
	std::unordered_map<const InstancingMeshTexData*, std::unique_ptr<InstancingModelList>> instancingModelMap_;
	std::list<std::unique_ptr<InstancingMeshTexData>> drawDatas_;
	const ModelData* plane_;
};