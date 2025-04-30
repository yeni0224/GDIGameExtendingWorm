#include "TitleScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

using namespace learning;

void TitleScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

    pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());

    m_rect.left = 50;
    m_rect.top = 150;
    m_rect.right = 200;
    m_rect.bottom = 170;

    m_storyRect.left = 50;
    m_storyRect.top = 175;
    m_storyRect.right = 500;
    m_storyRect.bottom = 450;

    m_pBackground = pNewObject;
}

void TitleScene::Update(float deltaTime)
{
    static float time = 0.0f;
    time += deltaTime;
    //if(Game Start ��ư�� ������) Play Scene���� ��ȯ
    

    if (time > 3000.0f)
    {
        time = 0.0f;
        m_pGame->ChangeScene(SceneType::SCENE_PLAY);
    }
    else
    {
        wcscpy_s(m_szTitle, L"Game Story");
        wcscpy_s(m_szStory,
            L"������ �����̾��� �ǰ��ϰ� ������ִ�..\r\n"
            L"����� ������ �ٷ� �μ����� ������ �����̴�..\r\n"
            L"�����̾��� ���� ��ƿ �� �ְ� ��������..!");
    }
}

void TitleScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
   
    
    // ���� �� (�׵θ���)
    //HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    //HPEN hOldPen = (HPEN)SelectObject(hDC, hRedPen);
    //Rectangle(hDC, 50, 100, 450, 620);
    
    DrawText(hDC, m_szStory, -1, &m_storyRect, DT_TOP | DT_LEFT | DT_WORDBREAK);
    DrawText(hDC, m_szTitle, -1, &m_rect, DT_TOP | DT_LEFT | DT_VCENTER);

    // ���� �귯�ÿ� �� ����
    //SelectObject(hDC, hOldPen);
    //DeleteObject(hRedPen);

}


void TitleScene::Finalize()
{
    if (m_pBackground)
    {
        delete m_pBackground;
        m_pBackground = nullptr;
    }
}

void TitleScene::Enter()
{

}

void TitleScene::Leave()
{
}

