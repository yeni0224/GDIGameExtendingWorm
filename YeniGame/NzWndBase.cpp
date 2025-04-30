#include "NzWndBase.h"
#include <iostream>

LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//[CHECK] #3-Ask NzWndBase�� protected ��������� ���� ������ ������?
	switch (msg)
	{
	case WM_SIZE:
	{
		NzWndBase* pNzWnd = (NzWndBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pNzWnd)pNzWnd->OnResize(LOWORD(lparam), HIWORD(lparam));

		break;
	}

	case WM_CLOSE:
	{
		NzWndBase* pNzWnd = (NzWndBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pNzWnd) pNzWnd->OnClose();
		PostQuitMessage(0);
		break;
	}


	default:
		return::DefWindowProc(hwnd, msg, wparam, lparam);
	}//switch

	return NULL;
}


bool NzWndBase::Create(const wchar_t* className, const wchar_t* windowName, int width, int height)
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = className;
	wc.lpfnWndProc = NzWndProc; // //[CHECK] . ������ ���ν���(�Լ�)�� ������ ���


	ATOM classId = 0;
	if (!GetClassInfoEx(HINSTANCE(), className, &wc))
	{
		classId = RegisterClassEx(&wc);

		if (0 == classId) return false;
	}

	m_width = width;
	m_height = height;

	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	//[CHECK] AdjustWindowRect()�� �ǹ̴�?
	m_hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

	if (NULL == m_hWnd) return false;

	::SetWindowText((HWND)m_hWnd, windowName);

	//[CHECK] #5. SetWindowLongPtr()�� �ǹ̴�?
	SetWindowLongPtr((HWND)m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow((HWND)m_hWnd, SW_SHOW);
	UpdateWindow((HWND)m_hWnd);

	return true;
}

void NzWndBase::Destroy()
{
	if (NULL != m_hWnd)
	{
		DestroyWindow((HWND)m_hWnd);
		m_hWnd = NULL;
	}
}

void NzWndBase::OnResize(int width, int height)
{
	m_width = width;
	m_height = height;
}