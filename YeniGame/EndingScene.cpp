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
    SetRect(&rect,0, 360, 1024, 720); 
    DrawText(hDC, L"Game Over", -1, &rect, DT_CENTER | DT_VCENTER);

    //RECT rect2;
    //SetRect(&rect2, 0, 330, 1024, 720);
    //DrawText(hDC, scoreText, -1, &rect2, DT_CENTER | DT_VCENTER);

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
    swprintf_s(scoreText, L"Score : %d", score);

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
    //PlayScene* m_pScene = dynamic_cast<PlayScene*>(m_pGame->GetScene(SceneType::SCENE_PLAY));

    //score = m_pScene->GetScore();
}

void EndingScene::Leave()
{
}

