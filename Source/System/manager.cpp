
#include "../Main/main.h"
#include "../System/manager.h"
#include "../System/renderer.h"
#include "../System/input.h"
#include "../GameObject/Field/meshField.h"
#include "../GameObject/Camera/camera.h"
#include "../System/Model/model.h"
#include "../GameObject/Player/player.h"
#include "../Scene/gameScene.h"
#include "../System/time.h"
#include "../System/debug.h"

Scene* Manager::m_Scene;

void Manager::Init()
{

	Renderer::Init();
	Input::Init();

	SetScene<GameScene>();
}

void Manager::Uninit()
{

	m_Scene->Uninit();
	delete m_Scene;

	Input::Uninit();
	Renderer::Uninit();

}

void Manager::Update()
{
	Input::Update();
	m_Scene->Update();

}

void Manager::Draw()
{

	Renderer::Begin();

	m_Scene->Draw();

#ifdef _DEBUG
	Renderer::imguiDraw();
#endif // _DEBUG

	Renderer::End();

}
