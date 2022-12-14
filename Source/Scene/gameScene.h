//==========================================================
//	[game.h]
//
//									ì¬“ú 2022/09/26
//									ì¬Ò ¼•½NŒ÷
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