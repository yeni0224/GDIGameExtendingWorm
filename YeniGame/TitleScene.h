#pragma once

#include "Scene.h"

class GameObjectBase;
class MyFirstWndGame;

class TitleScene : public Scene
{
public:
    TitleScene() = default;
    ~TitleScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;


private:

    GameObjectBase* m_pBackground = nullptr;
    //GameObjectBase* m_pStartButton = nullptr;

    MyFirstWndGame* m_pGame = nullptr;

    WCHAR m_szTitle[50] = L"Game Story";
    WCHAR m_szStory[256] = L"Game Story";
    const wchar_t* story = L"";

    RECT m_rect = { 0, 0, 0, 0 };
    RECT m_storyRect = { 0, 0, 0, 0 };
    RECT m_btnStartRect = { 0, 0, 0, 0 };


    GameObjectBase* m_pButton = nullptr;
    void CreatebtnStart();
};

