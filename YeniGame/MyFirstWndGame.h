#pragma once
#include "NzWndBase.h" // ����� ���ؼ� �޾ƿ�
#include "Utillity.h"
#include "INC_Windows.h"
#include "Scene.h"

// [CHECK] #7  ���� ������ ����Ͽ� ��������� �������� ����.
class GameTimer;
//class GameObjectBase;
//class GameObject;

class Scnene;
namespace renderHelp
{
    class BitmapInfo;
}

class ShapeBase
{
public:
    virtual ~ShapeBase() {}
    virtual bool IsPointingCircle(int x, int y) { return false; }
    virtual bool IsPointingRectangle(int x, int y) { return false; }
};

class Circle : public ShapeBase
{
public:
    Circle(int centerX, int centerY, int radius, COLORREF color)
        : m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_color(color) {}
private:
    int m_centerX, m_centerY, m_radius;
    COLORREF m_color;
};

class MyFirstWndGame : public NzWndBase// NzWndBase�� ��ӹ޾� ������ Ŭ������ ������ ����.
{
public:
    MyFirstWndGame() = default; // ������
    ~MyFirstWndGame() override = default; // �Ҹ��� = �������̵� >> �θ� Ŭ�������� �Ҹ��ڴ� ������ �����Լ�
    bool Initialize(); // �ʱ�ȭ �Լ�
    void Run(); // �����Լ�(����)
    void Finalize(); // ���� �Լ�
    /*bool Create(const wchar_t* className, const wchar_t* windowName, int width, int height);
    void Destroy();
    void* GetHandle() const { return m_hWnd; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }*/ // �̰� ��ӹ޾Ƽ� ����

    void ChangeScene(SceneType eSceneType);

private:
    void Update();
    void Render();

    void OnResize(int width, int height) override;
    void OnClose() override;

    void OnMouseMove(int x, int y);
    void OnLButtonDown(int x, int y);
    void OnRButtonDown(int x, int y);

    void FixedUpdate();
    void LogicUpdate();

    //[20250422] PlayeScene �� �̵�
    //void CreatePlayer();
    //void CreateEnemy();

   // void UpdatePlayerInfo();

   // GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }
   // GameObject* GetObj(int index) const { return (GameObject*)m_GameObjectPtrTable[index]; }

private:
    HDC m_hFrontDC = nullptr;
    HDC m_hBackDC = nullptr;
    HBITMAP m_hBackBitmap = nullptr;
    HBITMAP m_hDefaultBitmap = nullptr;
      
    // [CHECK] #8. ���� Ÿ�̸Ӹ� ����Ͽ� �������� �����ϴ� ����.
    GameTimer* m_pGameTimer = nullptr;
    float m_fDeltaTime = 0.0f;
    float m_fFrameCount = 0.0f;

    // [CHECK] #8. ���� ������Ʈ�� �����ϴ� �����̳�.
   //[20240422] Scene ���� �����ϵ��� ����
   //int m_eCurrentScene = SCENE_PLAY; Enter �� �ȵǼ�.
    int m_eCurrentScene = SCENE_TITLE;
    Scene* m_pScenes[SceneType::SCENE_MAX] = { nullptr, nullptr, nullptr };
    //GameObjectBase** m_GameObjectPtrTable  = nullptr;

    struct MOUSE_POS
    {
        int x = 0;
        int y = 0;

        bool operator!=(const MOUSE_POS& other) const
        {
            return (x != other.x || y != other.y);
        }
    };

    MOUSE_POS m_MousePos = { 0, 0 };
    MOUSE_POS m_MousePosPrev = { 0, 0 };

    MOUSE_POS m_PlayerTargetPos = { 0, 0 };
    MOUSE_POS m_WormSegSpawnPos = { 0, 0 };
    MOUSE_POS m_ObstacleSegSpawnPos = { 0, 0 };
   
    bool* isOverlap = nullptr;

#pragma region resource
    using BitmapInfo = renderHelp::BitmapInfo;

    BitmapInfo* m_pPlayerBitmapInfo = nullptr;
    BitmapInfo* m_pWormSegBitmapInfo = nullptr;
    BitmapInfo* m_pBackgroundBitmapInfo = nullptr;
    BitmapInfo* m_pObstacleBitmapInfo = nullptr;

public:

    using Vector2f = learning::Vector2f;
    Vector2f PlayerTargetPosition() const { return Vector2f(m_PlayerTargetPos.x, m_PlayerTargetPos.y); }
    Vector2f WormSegSpawnPosition() const { return Vector2f(m_WormSegSpawnPos.x, m_WormSegSpawnPos.y); }
    Vector2f ObstacleSegSpawnPosition() const { return Vector2f(m_ObstacleSegSpawnPos.x, m_ObstacleSegSpawnPos.y); }
    void ResetWormSegSpawnPosition() { m_WormSegSpawnPos = { 0, 0 }; } 
    void ResetObstacleSegSpawnPosition() { m_ObstacleSegSpawnPos = { 0, 0 }; } 

    BitmapInfo* GetPlayerBitmapInfo() const { return m_pPlayerBitmapInfo; }
    BitmapInfo* GetWormSegBitmapInfo() const { return m_pWormSegBitmapInfo; }
    BitmapInfo* GetObstacleBitmapInfo() const { return m_pObstacleBitmapInfo; }

    BitmapInfo* GetBackgroundBitmapInfo() const { return m_pBackgroundBitmapInfo; }

#pragma endregion

};
