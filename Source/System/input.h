#pragma once
#define MOUSE_ON_VALUE (0x80)					//!< オン状態の値

enum MouseButton
{
	Left,		//!< 左
	Right,		//!< 右
	Center,		//!< 真ん中
};

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool InitInput();
	static void UpdateMouse();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	static bool OnMousePush(MouseButton button_type);
	static bool OnMouseDown(MouseButton button_type);
	static bool OnMouseUp(MouseButton button_type);
	static D3DXVECTOR2 GetMousePos();

	static bool CreateInputInterface();
	static bool CreateMouseDevice();
	static BOOL StartMouseControl();

};
