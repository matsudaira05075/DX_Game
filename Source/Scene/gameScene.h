//==========================================================
//	[game.h]
//
//									�쐬�� 2022/09/26
//									�쐬�� �����N��
//==========================================================
#pragma once
#include "../InterFace/scene.h"

class GameScene : public Scene
{
private:
	class Audio* m_checkSE;
public:
	void	Init();
	void	Uninit();
	void	Update();
};