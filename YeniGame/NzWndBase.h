#pragma once
#include "INC_Windows.h"

// �Լ� ����
LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// [CHECK] #2. ������ Ŭ���� ���� �� ����� Ŭ������ ������ ����.
// * ����� �޾� Ȯ���� �� �ֵ��� ����.


class NzWndBase
{
public:
    NzWndBase() = default;
    virtual ~NzWndBase() = default;

    bool Create(const wchar_t* className, const wchar_t* windowName, int width, int height);
    void Destroy();

    void* GetHandle() const { return m_hWnd; }

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

protected:

    // [CHECK] #3 friend �� �����Ͽ� �ش� �Լ����� ������ �� �ֵ��� ��. (���ϱ��?)
    friend LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    // [CHECK] #4 �޽��� ���ν����� �����Լ��� �����Ͽ� ��ӹ��� Ŭ�������� �������� �� �ֵ��� ��.
    virtual void OnResize(int width, int height);
    virtual void OnClose() {}

    HWND m_hWnd = HWND();
    int m_width = 0;
    int m_height = 0;


    // [CHECK] #2. ������ ���� �ڵ��ϱ��?
    NzWndBase(const NzWndBase&) = delete;
    NzWndBase& operator=(const NzWndBase&) = delete;
    NzWndBase(NzWndBase&&) = delete;
    NzWndBase& operator=(NzWndBase&&) = delete;

};
