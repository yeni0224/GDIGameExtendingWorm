#pragma once
#include "Scene.h"

class MyFirstWndGame;
class PlayScene;

class EndingScene : public Scene
{
public:
    EndingScene() = default;
    ~EndingScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;
    void OnLButtonDown(int x, int y) override;

protected:
    MyFirstWndGame* m_pGame = nullptr;
    
private:
    void PrintPlayerScore(HDC hDC);
    int score;
    wchar_t scoreText[64];

};

