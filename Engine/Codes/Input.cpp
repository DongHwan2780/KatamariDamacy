#include "..\Headers\Input.h"

IMPLEMENT_SINGLETON(CInput)

CInput::CInput()
{
}

HRESULT CInput::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	// 키보드 초기화
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))		// 어떤 장치(키보드,  마우스등)을 세팅할건지
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/)))		// 장치 협력수준 설정
		return E_FAIL;

	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;

	// 마우스 초기화
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))	
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInput::SetUp_InputDeviceState()
{
	if (m_pKeyBoard == nullptr || m_pMouse == nullptr)
		return E_FAIL;

	if (FAILED(m_pKeyBoard->GetDeviceState(256, m_KeyBoardState)))
		return E_FAIL;

	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
		return E_FAIL;

	return S_OK;
}

void CInput::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInputSDK);
}
