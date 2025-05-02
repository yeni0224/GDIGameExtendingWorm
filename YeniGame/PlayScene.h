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
    GameObject* GetGameObj(int index);
    GameObject* gameObj;
    //GameObject* gameObj = dynamic_cast<GameObject*> (m_GameObjectPtrTable[0]);
private:
    void CreatePlayer();
    void CreateEnemy();

    void UpdatePlayerInfo();
    void UpdateEnemyInfo();
    void IsUpdateEnemyGoal();
    void CheckLastIndex();
    void UpdateObstacleInfo();
    bool IsSamePos();
    bool IsCollideEnemy(int x, int y);
    //bool CheckOverlap();
    //bool ComparePPrevDistance(int i);
    int m_centerX = 0;
    int m_centerY = 0;
    int m_radius = 0;

    float EnemyFrameCount = 0;
    float currTime = 0;
    int spawnCnt = 0;
    int FrogGauge = 0;
    int FrogMaxGauge = 200;
   
    bool* arrbool = nullptr;

    GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }

    MyFirstWndGame* m_pGame = nullptr;
    GameObjectBase* m_pBackground = nullptr;

    float randWidth = 0;
    float randHieght = 0;

    HPEN hRedPen = nullptr;
    HPEN hBluePen = nullptr;

    std::random_device rd;

    learning::Vector2f* m_GameObjectPos = nullptr;

    int lastindex = -1;
};

