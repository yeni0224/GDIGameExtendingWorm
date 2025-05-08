#include "EndingScene.h"
#include "MyFirstWndGame.h"
#include "PlayScene.h"


void EndingScene::Initialize(NzWndBase* pWnd)
{
}

void EndingScene::Update(float deltaTime)
{
}

void EndingScene::Render(HDC hDC)
{
    RECT rect;
    SetRect(&rect,400, 500, 400, 600); 
    DrawText(hDC, L"Game Over", -1, &rect, DT_CENTER | DT_VCENTER);
}

void EndingScene::OnLButtonDown(int x, int y)
{
    m_pGame->SetLButtonDown(x, y);
}

void EndingScene::PrintPlayerScore(HDC hDC)
{
    SetTextColor(hDC, RGB(0, 100, 0)); // ���� ��: ���

    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

    // ����� ���ڿ� �����
    wchar_t scoreText[64];
    //swprintf_s(scoreText, L"Score : %d", PlayScene::GetScore());

    // ��� ��ġ (������ ���� ����)
    TextOut(hDC, 400, 20, scoreText, wcslen(scoreText));

    // ���� ��Ʈ ����
    SelectObject(hDC, hOldFont);
}

void EndingScene::Finalize()
{
}

void EndingScene::Enter()
{
}

void EndingScene::Leave()
{
}

