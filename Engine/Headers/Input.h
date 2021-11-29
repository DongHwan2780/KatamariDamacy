#pragma once

#ifndef __INPUT_H__
#define __INPUT_H__

#include "Base.h"

BEGIN(Engine)

class CInput final : public CBase
{
	DECLARE_SINGLETON(CInput)

public:
	enum MOUSEMOVESTATE { MOUSE_MOVEX, MOUSE_MOVEY, MOUSE_MOVEZ, MOUSE_MOVEEND };
	enum MOUSEBUTTONSTATE { MOUSE_LBUTTON, MOUSE_RBUTTON, MOUSE_WHEEL, MOUSE_END };
private:
	CInput();
	virtual ~CInput() = default;

public:
	_byte Get_DIKState(_ubyte byKeyID) const { return m_KeyBoardState[byKeyID]; }
	_long Get_MouseMoveState(MOUSEMOVESTATE eMoveState) const { return ((_long*)&m_MouseState)[eMoveState]; }
	_byte Get_MouseButtonState(MOUSEBUTTONSTATE eButtonState) const { return m_MouseState.rgbButtons[eButtonState]; }

public:
	HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	HRESULT SetUp_InputDeviceState();		// 디바이스의 상태를 얻어옴

public:
	virtual void Free() override;

private:
	IDirectInput8*			m_pInputSDK = nullptr;
	IDirectInputDevice8*	m_pKeyBoard = nullptr;
	IDirectInputDevice8*	m_pMouse = nullptr;

private:
	_byte				m_KeyBoardState[256];
	DIMOUSESTATE		m_MouseState;
};
END
#endif // !__INPUT_H__
