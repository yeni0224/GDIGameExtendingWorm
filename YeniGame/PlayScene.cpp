#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include "Collider.h"
#include <iostream>
#include <intsafe.h>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

void PlayScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr);

    m_GameObjectPtrTable = new GameObjectBase*[MAX_GAME_OBJECT_COUNT];

    arrbool = new bool[MAX_GAME_OBJECT_COUNT];

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[i] = nullptr;
    }
    m_GameObjectPos = new Vector2f[MAX_GAME_OBJECT_COUNT];
    
    
    hBluePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    hRedPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

    pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());

    m_pBackground = pNewObject;
}

void PlayScene::FixedUpdate()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
    Vector2f wormPos = m_pGame->WormSegSpawnPosition();
    std::cout << "wormPos.x : " << wormPos.x << "wormPos.y : " << wormPos.y  << std::endl;
    
    CreateWormSegment();
    m_pGame->ResetWormSegSpawnPosition();
    
}

void PlayScene::Update(float deltaTime)
{
    UpdatePlayerInfo();
    UpdateWormSegInfo();    
    
    //((GameObject*)m_GameObjectPtrTable[0])->GetColliderCircle();
    
    m_centerX = gameObj->GetColliderCircle()->center.x;
    m_centerY = gameObj->GetColliderCircle()->center.y;
    m_radius = gameObj->GetColliderCircle()->radius;

    int x = LOWORD(lparam);
    int y = HIWORD(lparam);

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(deltaTime);
            m_GameObjectPos[i] = m_GameObjectPtrTable[i]->GetPosition();
            
        }
    }
    if (m_GameObjectPtrTable[2] != nullptr)
    {
        /*if (IsSamePos())
        {
            m_pGame->ChangeScene(SceneType::SCENE_ENDING);
        }*/
        if (IsCollideWorm(x, y))
        {

        }
    }
    
}

void PlayScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    m_pBackground->Render(hDC);

    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[0]->Render(hDC);
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(hDC, arrbool[i]);
        }
    }
}

GameObject* PlayScene::GetGameObj(int index)
{
    GameObject* gameObj = dynamic_cast<GameObject*> (m_GameObjectPtrTable[index]);
    return gameObj;
}

void PlayScene::Finalize()
{
   if (m_GameObjectPtrTable)
   {
       for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
       {
           if (m_GameObjectPtrTable[i])
           {
               delete m_GameObjectPtrTable[i];
               m_GameObjectPtrTable[i] = nullptr;
           }
       }
       delete[] arrbool;
       arrbool = nullptr;
       delete [] m_GameObjectPtrTable;
       m_GameObjectPtrTable = nullptr;
       delete[] m_GameObjectPos;
       m_GameObjectPos = nullptr;
   }
   delete m_pBackground;
   m_pBackground = nullptr;
}

void PlayScene::Enter()
{
    // [CHECK]. ù ��° ���� ������Ʈ�� �÷��̾� ĳ���ͷ� ����!
    CreatePlayer();
    gameObj = GetGameObj(0);

}

void PlayScene::Leave()
{
}

void PlayScene::CreatePlayer()
{
    assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");
    assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::PLAYER);

    pNewObject->SetName("Player");
    pNewObject->SetPosition(720.0f, 320.0f); // �ϴ�, ���Ƿ� ���� 
    pNewObject->SetSpeed(0.5f); // �ϴ�, ���Ƿ� ����  
    pNewObject->SetWidth(29); // �ϴ�, ���Ƿ� ����
    pNewObject->SetHeight(29); // �ϴ�, ���Ƿ� ����

    pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo());
    pNewObject->SetColliderCircle(5.0f); // �ϴ�, ���Ƿ� ����. ������Ʈ ������ �� �� �ϰ� ���� ����.

    m_GameObjectPtrTable[0] = pNewObject;
}

void PlayScene::CreateWormSegment() //������ �������� ����
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
    
    GameObject* pNewObject = new GameObject(ObjectType::SEGMENT);

    pNewObject->SetName("Segment");

    //Vector2f wormSegPos = m_pGame->WormSegSpawnPosition();

    pNewObject->SetPosition(100, 100);

    pNewObject->SetSpeed(0.3f); // �ϴ�, ���Ƿ� ����  
    pNewObject->SetWidth(20); // �ϴ�, ���Ƿ� ����
    pNewObject->SetHeight(20); // �ϴ�, ���Ƿ� ����
    
    //pNewObject->SetBitmapInfo(m_pGame->GetWormSegBitmapInfo()); //����

    pNewObject->SetColliderCircle(10.0f); // �ϴ�, ���Ƿ� ����. ������Ʈ ������ �� �� �ϰ� ���� ����.

    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0��°�� ������ �÷��̾� �Ӹ�
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            std::cout << i << std::endl;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT)
    {
        // ���� ������Ʈ ���̺��� ���� á���ϴ�.
        delete pNewObject;
        pNewObject = nullptr;
    }
}

void PlayScene::CreateObstacle()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::OBSTACLE);
    pNewObject->SetName("Obstacle");
    Vector2f ObstaclePos = m_pGame->WormSegSpawnPosition();
    pNewObject->SetPosition(ObstaclePos.x, ObstaclePos.y);
    pNewObject->SetBitmapInfo(m_pGame->GetObstacleBitmapInfo());
    pNewObject->SetColliderBox(15.0f, 10.0f);
}

void PlayScene::UpdatePlayerInfo()
{
    static GameObject* pPlayer = GetPlayer();

    assert(pPlayer != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    Vector2f targetPos = m_pGame->PlayerTargetPosition();
    Vector2f playerPos = pPlayer->GetPosition();

    Vector2f playerDir = targetPos - playerPos;
    float distance = playerDir.Length(); // �Ÿ� ���

    if (distance > 50.f) //���Ƿ� ������ �Ÿ�
    {
        playerDir.Normalize(); // ����ȭ
        pPlayer->SetDirection(playerDir); // �÷��̾� ���� ����
    }
    else
    {
        pPlayer->SetDirection(Vector2f(0, 0)); // �÷��̾� ����
    }
}

void PlayScene::UpdateWormSegInfo() //������ ����
{
    static GameObject* pPlayer = GetPlayer();
    assert(pPlayer != nullptr);

    int n = 0;

    Vector2f playerPos = GetPlayer()->GetPosition();
    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0��°�� ������ �÷��̾�!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pWormSeg = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
            
            Vector2f wormSegPos = pWormSeg->GetPosition();
            
            Vector2f wormSegDir = playerPos - wormSegPos;
            float distance = wormSegDir.Length(); // �Ÿ� ���

            if (distance < 50.f)
            {
                arrbool[i] = false;
                n++;
                //arrbool[0] = true;
            }

            else
            {
                arrbool[i] = true;
                //arrbool[0] = false;
            }

             if (distance > 25.f) //���Ƿ� ������ ������Ʈ�� player ���� �Ÿ�
            {
                wormSegDir.Normalize(); // ����ȭ
                pWormSeg->SetDirection(wormSegDir); // ���� ����
            }
            else
            {
                pWormSeg->SetDirection(Vector2f(0, 0)); //  ����
            }
        }
    }
    if (n > 0) arrbool[0] = false;
    else arrbool[0] = true;
    //arrbool[0] = (n > 0);
}
// ������ �� �Լ� �ȿ��� �÷��̾� ������ �޾ƿͼ� �ٸ� ��ü�� ������ �����ͼ� ���� ���ϰ�
// �÷��̾ ���� �������� ? distance > 25.f �Ÿ� �ɶ�����
// �װ������� �浹�� true false�� �ٲٰ� ����

// �׷� ���ؾ��ϳ�? ���� �÷��̾�� �ϴ°� �ƴ϶� ���ʿ� ��ü vs ��ü�� ó���ؾߴ�
//�浹ó��, �Ÿ��񱳿� ���� ��� �Ǻ� (�� ��ü���� for�� ��->Update���� ȣ��)

//�浹 ó��
bool PlayScene::IsCollideWorm(int x, int y)
{
    
    int px = m_centerX - x;
    int py = m_centerY - y;
    int pz = px * px + py * py;
    int dist = m_radius + m_radius;
    int sqrZ = dist * dist;

    if (pz <= sqrZ)
    {
        return true;
    }
    return false;
}

//�Ÿ��񱳿� ���� ��� ���
void PlayScene::CheckOverlap()
{

    return false;
}


void PlayScene::UpdateObstacleInfo()
{
    static GameObject* pPlayer = GetPlayer();
    assert(pPlayer != nullptr);
}

bool PlayScene::IsSamePos()
{
    float x = 0;
    float y = 0;
    for (int i = 2; i < MAX_GAME_OBJECT_COUNT; i++)
    {
        x = m_GameObjectPos[i].x - m_GameObjectPos[1].x;
        if (x < 0) x = -x;
        y = m_GameObjectPos[i].y - m_GameObjectPos[1].y;
        if (y < 0) y = -y;

        if (x < 0.1 && y < 0.1)
        {
            return true;
        }
    }
    return false;
}

