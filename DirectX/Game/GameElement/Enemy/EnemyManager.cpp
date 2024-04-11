#include "EnemyManager.h"
#include "DoesNotWorkEnemy.h"

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize()
{
	if (!stageEditor_) {
		IEnemy::StaticInitialize();
		DoesNotWorkEnemy::StaticInitialize();
		stageEditor_ = std::make_unique<StageEditor>("敵の設置");
		SetGlobalVariable();
	}
	Clear();
	GenerateEnemies();
}

void EnemyManager::Update(const float& deltaTime, Camera* camera)
{
#ifdef _DEBUG
	int preDoesNotWorkEnemyNum = doesNotWorkEnemyNum_;
	ApplyGlobalVariable();
	if (preDoesNotWorkEnemyNum != doesNotWorkEnemyNum_) {
		Initialize();
	}
#endif // _DEBUG

	for (std::list<std::unique_ptr<IEnemy>>::iterator it = enemies_.begin(); it != enemies_.end();) {
		(*it)->Update(deltaTime, camera);
		it++;
	}
}

void EnemyManager::Draw() const
{
	for (const std::unique_ptr<IEnemy>& enemy : enemies_) {
		enemy->Draw();
	}
}

void EnemyManager::SetGlobalVariable()
{
	stageEditor_->AddItem("敵の数", doesNotWorkEnemyNum_, "動かない敵");
	ApplyGlobalVariable();
}

void EnemyManager::ApplyGlobalVariable()
{
	doesNotWorkEnemyNum_ = stageEditor_->GetIntValue("敵の数", "動かない敵");
}

void EnemyManager::GenerateEnemies()
{
	for (int i = 0; i < doesNotWorkEnemyNum_; i++) {
		enemies_.push_back(std::make_unique<DoesNotWorkEnemy>(i));
	}
}
