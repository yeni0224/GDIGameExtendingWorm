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
    SetTextColor(hDC, RGB(0, 100, 0)); // 글자 색: 녹색

    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

    // 출력할 문자열 만들기
    wchar_t scoreText[64];
    //swprintf_s(scoreText, L"Score : %d", PlayScene::GetScore());

    // 출력 위치 (적당히 수정 가능)
    TextOut(hDC, 400, 20, scoreText, wcslen(scoreText));

    // 원래 폰트 복원
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

