//==========================================================
//	[game.cpp]
//
//									作成日 2022/09/26
//									作成者 松平康功
//==========================================================
#include "../Main/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/input.h"
#include "../Scene/gameScene.h"
#include "../InterFace/gameObject.h"
#include "../InterFace/scene.h"
#include "../System/Model/model.h"
#include "../GameObject/Player/player.h"
#include "../GameObject/Enemy/enemy.h"
#include "../GameObject/Camera/camera.h"
#include "../GameObject/Field/meshField.h"
#include "../GameObject/SkyBox/skyBox.h"
#include "../GameObject/Water/water.h"


//==========================================================
// グローバル変数
//==========================================================


static bool	g_goal;
static bool g_start = false;



void GameScene::Init()
{
	ShowCursor(true);
	g_goal = false;

	// [0] システム
	// [1] 3Dオブジェ
	// [2] UI

	AddGameObject<Camera>(0);

	//==========================================================
	// SEの初期化・設定
	//==========================================================
	Scene* scene = Manager::GetScene();
	

	AddGameObject<Player>(1);
	//AddGameObject<Enemy>(1);
	AddGameObject <MeshField>(1);
	AddGameObject <Water>(1);
	AddGameObject<SkyBox>(1);
	

}

void GameScene::Uninit()
{
	//==========================================================
	// 継承元クラスのメソッド呼び出し
	//==========================================================
	Scene::Uninit();
}

void GameScene::Update()
{

	Scene::Update();
	
	

}
