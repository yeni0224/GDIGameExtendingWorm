#pragma once
#include "Scene.h"
#include "Utillity.h"
#include <random>

class GameObject;
class GameObjectBase;
class MyFirstWndGame;

class PlayScene :public Scene
{
 public:
    PlayScene() = default;
    ~PlayScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override;
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;
    UIObject* pNewUI;
    UIObject* GetpNewUI();
    GameObject* GetGameObj(int index);
    GameObject* gameObj;
    //GameObject* gameObj = dynamic_cast<GameObject*> (m_GameObjectPtrTable[0]);
    int GetFrogGauge() { return FrogGauge; }
    int SetFrogGauge(int hp) { return FrogGauge = hp; }
    int GetScore() { return score; }

    void OnLButtonDown(int x, int y) override;
protected:
    MyFirstWndGame* m_pGame = nullptr;

private:

    void CreatePlayer();
    void CreateEnemy();
    void CreateHpBar();

    void UpdatePlayerInfo();
    void UpdateEnemyInfo();
    void IsUpdateEnemyGoal();
    void CheckLastIndex();
    bool IsSamePos();
    bool IsCollideEnemy(int x, int y);
    int m_centerX = 0;
    int m_centerY = 0;
    int m_radius = 0;

    float EnemyFrameCount = 0;
    float currTime = 0;
    int spawnCnt = 0;
    int FrogGauge = 0;
    int FrogMaxGauge = 100;
   
    bool* arrbool = nullptr;

    GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }
    UIObject* GetHpBar() const { return(UIObject*)m_UI; }
    void PrintFrogHPBar(HDC hDC);
    void PrintScore(HDC hDC);

    GameObjectBase* m_pBackground = nullptr;

    int score;
    float randWidth = 0;
    float randHieght = 0;

    HPEN hRedPen = nullptr;
    HPEN hBluePen = nullptr;

    std::random_device rd;

    learning::Vector2f* m_GameObjectPos = nullptr;

    int lastindex = -1;
};

