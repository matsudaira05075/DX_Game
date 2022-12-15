//==========================================================
//	[Sceneクラス<InterFace>]
//
//									作成日 2022/07/04
//									作成者 松平康功
//==========================================================
#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "../InterFace/gameObject.h"

class Scene
{
protected:
	// STLのリスト構造
	std::list<GameObject*> m_gameObject[3]; 

public:
	Scene(){}
	virtual ~Scene(){}

	virtual void Init() = 0; // 純粋仮想関数

	// テンプレート関数
	template <typename T> 
	T* GetGameObject(int Layer)
	{
		for (GameObject* object : m_gameObject[Layer])
		{
			// 型を調べる(RTTI動的型情報)
			if (typeid(*object) == typeid(T))
			{
				return (T*)object;
			}
		}
		return NULL;
	}


	template <typename T>
	void GetGameObjects(int Layer, std::vector<T*>& outObjects) {
		for (GameObject* object : m_gameObject[Layer]) {
			if (typeid(*object) == typeid(T)) {
				// 引数で渡されたvectorに返したいオブジェクトを詰める
				outObjects.emplace_back((T*)object);
			}
		}
	}

	template <typename T> 
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_gameObject[Layer].emplace_back(gameObject);
		gameObject->Init();

		return gameObject;
	}


	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			// 範囲forループ
			for (GameObject* object : m_gameObject[i]) 
			{
				object->Uninit();
				delete object;
			}
			m_gameObject[i].clear(); // リストのクリア
		}
	}

	virtual void Update()
	{

		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_gameObject[i])
			{
				object->Update(); // ポリモフィズム（多態性）
			}

			m_gameObject[i].remove_if([](GameObject* object)
			{return object->Destroy(); });
			// ラムダ式
		}
	}

	virtual void Draw()
	{
#ifdef _DEBUG
		ImGui::Begin("Operation Description");
		ImGui::Text("Cntl : MouseControl");
#endif
		for (int i = 0; i < 3; i++)
		{
			// 描画の順番大事 カメラ ＜ 3D ＜ 2D (奥から描画)
			for (GameObject* object : m_gameObject[i])
			{
				object->Draw();
			}
		}

#ifdef _DEBUG
		ImGui::End();
#endif
	}


};