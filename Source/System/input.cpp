
#include "../Main/main.h"
#include "../System/input.h"
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];

static LPDIRECTINPUT8 g_InputInterface;			//!< DIRECTINPUT8のポインタ
static LPDIRECTINPUTDEVICE8 g_MouseDevice;		//!< DIRECTINPUTDEVICE8のポインタ
static DIMOUSESTATE g_CurrentMouseState;		//!< マウスの現在の入力情報
static DIMOUSESTATE g_PrevMouseState;			//!< マウスの現在の入力情報
static D3DXVECTOR2 g_MousePos;

POINT p;

void Input::Init()
{
	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);

	InitInput();
}

void Input::Uninit()
{
	// デバイスの解放
	if (g_MouseDevice != nullptr)
	{
		// 制御を停止
		g_MouseDevice->Unacquire();
		g_MouseDevice->Release();
		g_MouseDevice = nullptr;
	}

	// インターフェースの解放
	if (g_InputInterface != nullptr)
	{
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

void Input::Update()
{
	memcpy(m_OldKeyState, m_KeyState, 256);

	GetKeyboardState(m_KeyState);

	UpdateMouse();
}

bool Input::InitInput()
{
	// インターフェース作成
	if (CreateInputInterface() == false)
	{
		return false;
	}

	// デバイス作成
	if (CreateMouseDevice() == false)
	{
		Uninit();
		return false;
	}

	// 入力情報の初期化
	ZeroMemory(&g_CurrentMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&g_PrevMouseState, sizeof(DIMOUSESTATE));

	return true;
}

void Input::UpdateMouse()
{
	if (g_MouseDevice == nullptr)
	{
		return;
	}

	// 更新前に最新マウス情報を保存する
	g_PrevMouseState = g_CurrentMouseState;

	// マウスの状態を取得します
	HRESULT	hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	if (FAILED(hr))
	{
		g_MouseDevice->Acquire();
		hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	}
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}


D3DXVECTOR2 Input::GetMousePos()
{
	// マウス座標を取得する
	GetCursorPos(&p);
	ScreenToClient(FindWindowA("AppClass", nullptr), &p);
	return D3DXVECTOR2(p.x,p.y);
}

bool Input::OnMousePush(MouseButton button_type)
{
	if (g_PrevMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE &&
		g_CurrentMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE)
	{
		return true;
	}

	return false;
}

bool Input::OnMouseDown(MouseButton button_type)
{
	if (!(g_PrevMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE) &&
		g_CurrentMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE)
	{
		return true;
	}

	return false;
}



bool Input::OnMouseUp(MouseButton button_type)
{
	if (g_PrevMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE &&
		!(g_CurrentMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE))
	{
		return true;
	}

	return false;
}

bool Input::CreateInputInterface()
{
	// インターフェース作成
	HRESULT ret = DirectInput8Create(
		GetModuleHandle(nullptr),	// インスタンスハンドル
		DIRECTINPUT_VERSION,		// DirectInputのバージョン
		IID_IDirectInput8,			// 使用する機能
		(void**)&g_InputInterface,	// 作成されたインターフェース代入用
		NULL						// NULL固定
	);

	if (FAILED(ret))
	{
		return false;
	}

	return true;
}

bool Input::CreateMouseDevice()
{
	g_MouseDevice = nullptr;

	// デバイス生成
	HRESULT hr = g_InputInterface->CreateDevice(
		GUID_SysMouse,
		&g_MouseDevice,
		NULL);

	if (FAILED(hr))
	{
		return false;
	}

	// 入力フォーマットの指定
	hr = g_MouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	int count = 0;
	// 制御開始
	while (StartMouseControl() == false)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	return true;
}

BOOL Input::StartMouseControl()
{
	// デバイスが生成されてない
	if (g_MouseDevice == nullptr)
	{
		return false;
	}
	// 制御開始
	if (FAILED(g_MouseDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	g_MouseDevice->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		/*
			PollはAcquireの前に行うとされていたが、
			Acquireの前で実行すると失敗したので
			後で実行するようにした
		*/
		if (FAILED(g_MouseDevice->Poll()))
		{
			return false;
		}
	}

	return true;
}

