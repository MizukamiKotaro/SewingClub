#include "EnemyManager.h"
#include "DoesNotWorkEnemy.h"
#include "FollowEnemy.h"
#include "GameElement/Player/Player.h"

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(Player* pplayer)
{
	if (pplayer) {
		player_ptr = pplayer;
	}

	if (!stageEditor_) {
		IEnemy::StaticInitialize();
		DoesNotWorkEnemy::StaticInitialize();
		FollowEnemy::StaticInitialize();
		stageEditor_ = std::make_unique<StageEditor>("敵の設置");
		SetGlobalVariable();
	}
	FollowEnemy::SetPlayerPtr(player_ptr->GetPositionPtr());
	Clear();
	GenerateEnemies();
}

void EnemyManager::Update(const float& deltaTime, Camera* camera)
{
#ifdef _DEBUG
	// 前情報取得
	int preDoesNotWorkEnemyNum = doesNotWorkEnemyNum_;
	int preFollowEnemyNum = followEnemyNum_;
	
	ApplyGlobalVariable();
	// 数が違えば初期化
	bool initializeFrag = false; // 初期化用フラグ
	if (preDoesNotWorkEnemyNum != doesNotWorkEnemyNum_) {
		initializeFrag = true;
	}
	else if (preFollowEnemyNum != followEnemyNum_) {
		initializeFrag = true;
	}

	if (initializeFrag) {
		Initialize(nullptr);
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
	stageEditor_->AddItem("敵の数", followEnemyNum_, "追従する敵");
	ApplyGlobalVariable();
}

void EnemyManager::ApplyGlobalVariable()
{
	doesNotWorkEnemyNum_ = stageEditor_->GetIntValue("敵の数", "動かない敵");
	followEnemyNum_ = stageEditor_->GetIntValue("敵の数", "追従する敵");
}

void EnemyManager::GenerateEnemies()
{
	for (int i = 0; i < doesNotWorkEnemyNum_; i++) {
		enemies_.push_back(std::make_unique<DoesNotWorkEnemy>(i));
	}
	for (int i = 0; i < followEnemyNum_; i++) {
		enemies_.push_back(std::make_unique<FollowEnemy>(i));
	}
}
