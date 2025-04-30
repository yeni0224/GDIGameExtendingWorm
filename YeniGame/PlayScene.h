#pragma once
#include "Scene.h"
#include "Utillity.h"

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
    void CreateWormSegment();
    void CreateObstacle();

    void UpdatePlayerInfo();
    void UpdateWormSegInfo();
    void UpdateObstacleInfo();
    bool IsSamePos();
    bool IsCollideWorm(int x, int y);
    void CheckOverlap();
    int m_centerX = 0;
    int m_centerY = 0;
    int m_radius = 0;
    LPARAM lparam;

    bool* arrbool = nullptr;

    GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }

    MyFirstWndGame* m_pGame = nullptr;
    GameObjectBase* m_pBackground = nullptr;

    HPEN hRedPen = nullptr;
    HPEN hBluePen = nullptr;

    learning::Vector2f* m_GameObjectPos = nullptr;

};

