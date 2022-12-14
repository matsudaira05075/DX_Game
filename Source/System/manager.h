//==========================================================
//	[manager.h]
//
//									ì¬“ú 2022/07/04
//									ì¬Ò ¼•½NŒ÷
//==========================================================
#pragma once

#include "../InterFace/scene.h"


class Manager
{
private:
	static class	Scene* m_Scene;

public:
	static void		Init();
	static void		Uninit();
	static void		Update();
	static void		Draw();

	static class Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void  SetScene()
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}
		m_Scene = new T();
		m_Scene->Init();
	}

};