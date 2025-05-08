#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include "TitleScene.h" 
#include "PlayScene.h"
#include "EndingScene.h"
#include <iostream>
#include <assert.h>

using namespace learning;

constexpr int MAX_SHAPES = 100;
ShapeBase* m_shapes[MAX_SHAPES] = { nullptr, };
int m_shapeCount = 0;

bool MyFirstWndGame::Initialize()
{
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();

    const wchar_t* className = L"MyFirstWndGame";
    const wchar_t* windowName = L"Feeding Frog";

    if (false == __super::Create(className, windowName, 1024, 720))
    {
        return false;
    }

    RECT rcClient = {};
    GetClientRect(m_hWnd, &rcClient);
    m_width = rcClient.right - rcClient.left;
    m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

#pragma region resource
    m_pPlayerBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/frog.png");
    m_pBackgroundBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/background.png");
    m_pEnemyBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/fly.png");
    m_pEnemyGoldBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/fly_gold.png");
    m_pGameStartBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/start_button.png");
    //m_pFrogHpBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/frogGauge.png");

    if (m_pPlayerBitmapInfo == nullptr  || m_pEnemyBitmapInfo == nullptr
        || m_pBackgroundBitmapInfo == nullptr || m_pEnemyGoldBitmapInfo == nullptr 
        || m_pGameStartBitmapInfo == nullptr /*|| m_pFrogHpBitmapInfo == nullptr*/)
    {
        std::cout << "Bitmap Load Failed!" << std::endl;
        return false;
    }

    m_pScenes[SceneType::SCENE_TITLE] = new TitleScene();
    m_pScenes[SceneType::SCENE_TITLE]->Initialize(this);

    m_pScenes[SceneType::SCENE_PLAY] = new PlayScene();
    m_pScenes[SceneType::SCENE_PLAY]->Initialize(this);

    m_pScenes[SceneType::SCENE_ENDING] = new EndingScene();
    m_pScenes[SceneType::SCENE_ENDING]->Initialize(this);
#pragma endregion

    return true;
    
}

void MyFirstWndGame::Run()
{ 
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                m_pScenes[m_eCurrentScene]->OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            /*else if (msg.message == WM_RBUTTONDOWN)
            {
                MyFirstWndGame::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }*/
            else if (msg.message == WM_MOUSEMOVE)
            {
                MyFirstWndGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void MyFirstWndGame::Finalize()
{
    delete m_pGameTimer;
    m_pGameTimer = nullptr;

    for (int i = 0; i < SceneType::SCENE_MAX; ++i)
    {
        if (m_pScenes[i])
        {
            m_pScenes[i]->Finalize();
            delete m_pScenes[i];
            m_pScenes[i] = nullptr;
        }
    }
    __super::Destroy();
}

void MyFirstWndGame::ChangeScene(SceneType eSceneType)
{
    if (m_eCurrentScene != eSceneType)
    {
        m_pScenes[m_eCurrentScene]->Leave();
        m_eCurrentScene = eSceneType;
        m_pScenes[m_eCurrentScene]->Enter();
    }
}

void MyFirstWndGame::SetLButtonDown(int x, int y)
{
    m_PlayerTargetPos.x = x;
    m_PlayerTargetPos.y = y;
}

void MyFirstWndGame::FixedUpdate()
{
    m_pScenes[m_eCurrentScene]->FixedUpdate();
}

void MyFirstWndGame::LogicUpdate()
{
    m_pScenes[m_eCurrentScene]->Update(m_fDeltaTime);
}

void MyFirstWndGame::Update()
{
    m_pGameTimer->Tick();

    LogicUpdate();

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
    m_fFrameCount += m_fDeltaTime;

    while (m_fFrameCount >= 1000.0f)
    {
        FixedUpdate();
        m_fFrameCount -= 1000.0f;
    }
    
}

void MyFirstWndGame::Render()
{
    //Clear the back buffer
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

    m_pScenes[m_eCurrentScene]->Render(m_hBackDC);

    //메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void MyFirstWndGame::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    //__super:: 키워드는 상위 클래스의 함수를 의미합니다.
    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrevBitmap);

    learning::SetScreenSize(width, height);
}

void MyFirstWndGame::OnClose()
{
    std::cout << __FUNCTION__ << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}

void MyFirstWndGame::OnMouseMove(int x, int y)
{
 /*   std::cout << __FUNCTION__ << std::endl;   
    std::cout << "x: " << x << ", y: " << y << std::endl;*/
    m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
      std::cout << __FUNCTION__ << std::endl;
 //std::cout << "x: " << x << ", y: " << y << std::endl;

    m_PlayerTargetPos.x = x;
    m_PlayerTargetPos.y = y;
}

void AddCircle(int centerX, int centerY, int radius, COLORREF color)
{
    if (m_shapeCount >= MAX_SHAPES) return;

    m_shapes[m_shapeCount] = new Circle(centerX, centerY, radius, color);
    ++m_shapeCount;
}

void AddRectangle(int left, int top, int right, int bottom, COLORREF color)
{
    if (m_shapeCount >= MAX_SHAPES) return;

    m_shapes[m_shapeCount] = new RectangleShape(left, top, right, bottom, color);//암묵적 업캐스팅
    //m_shapes[m_shapeCount] = (ShapeBase*)(left, top, right, bottom, color); //동일하다
    ++m_shapeCount;
}

bool RemoveCircle(int centerX, int centerY)
{
    for (int i = 0; i < m_shapeCount; i++)
    {
        if (m_shapes[i]->IsPointingCircle(centerX, centerY))
        {
            delete m_shapes[i];
            if (i < m_shapeCount)
            {
                for (int j = i; j < m_shapeCount - 1; j++)
                {
                    m_shapes[j] = m_shapes[j + 1];
                }
                m_shapes[m_shapeCount - 1] = nullptr;
                --m_shapeCount;
            }
            return true;
        }
    }
    return false;
}

bool RemoveRectangle(int X, int Y)
{
    for (int i = 0; i < m_shapeCount; i++)
    {
        if (m_shapes[i]->IsPointingRectangle(X, Y))
        {
            std::cout << m_shapes[i]->IsPointingRectangle(X, Y) << std::endl;
            delete m_shapes[i];
            if (i < m_shapeCount)
            {
                for (int j = i; j < m_shapeCount - 1; j++)
                {
                    m_shapes[j] = m_shapes[j + 1];
                }
                m_shapes[m_shapeCount - 1] = nullptr;
                --m_shapeCount;
            }
            return true;
        }
    }
    return false;
}

//파리 추가
void MyFirstWndGame::AddRandomEnemy(HWND m_hWnd)
{
    int x = rand() % 1081;
    int y = rand() % 721;

    srand((unsigned int)time(NULL));
    {
        AddCircle(x, y, 20, RGB(137, 223, 143));
        ::InvalidateRect(m_hWnd, NULL, TRUE); //화면 갱신
        std::cout << "ADD Circle" << std::endl;
    }
}

//void MyFirstWndGame::AddRandomEnemy(HWND  hwnd)
//{
//    int x = rand() % 1080;
//    int y = rand() % 720;
//
//    srand((unsigned int)time(NULL));
//    //랜덤한 모양 선택, 1초마다 그려지게
//    {
//        AddRectangle(x - 9, y - 9, x + 10, y + 10, RGB(0, 0, 255));
//        ::InvalidateRect(hwnd, NULL, TRUE); //화면 갱신
//        std::cout << "ADD Rectangle" << std::endl;
//    }
//}

//void MyFirstWndGame::OnRButtonDown(int x, int y)
//{
//    /*  std::cout << __FUNCTION__ << std::endl;
//   std::cout << "x: " << x << ", y: " << y << std::endl;*/
//    
//    //m_EnemySpawnPos.x = x;
//    //m_EnemySpawnPos.y = y;
//}

