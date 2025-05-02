#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include "Collider.h"
#include <iostream>
#include <intsafe.h>
#include <random>


using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 50;
constexpr int MAX_X_NUM = 1025;
constexpr int MAX_Y_NUM = 721;

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

    FrogGauge = 100;
}

void PlayScene::FixedUpdate()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
    Vector2f flyPos = m_pGame->EnemySpawnPosition();
    //std::cout << "flyPos.x : " << flyPos.x << "flyPos.y : " << flyPos.y  << std::endl;
    
    CreateEnemy();
    m_pGame->ResetEnemySpawnPosition();
    FrogGauge -= 5;
    std::cout << FrogGauge << std::endl;
}

void PlayScene::Update(float deltaTime)
{
    
    CheckLastIndex();
    UpdatePlayerInfo();
    IsUpdateEnemyGoal();
    
    //if (isGAmeover) retrun;  ��� ���ĸ� ���ӳ����� �� ��ȯ�� �ߴµ� ������ ������ �ؿ��ִ� ģ����� ���ٰ� ����θ��� ���̴�
    //���⼭ �Ʒ� �͵��� ���� ���ϰ� ���ƾ� �Ѵ�.

    currTime += deltaTime;
    EnemyFrameCount += deltaTime;
    while (EnemyFrameCount >= 2000.0f)
    {
        EnemyFrameCount = 0;
        UpdateEnemyInfo();
    }
    
    m_centerX = gameObj->GetColliderCircle()->center.x;
    m_centerY = gameObj->GetColliderCircle()->center.y;
    m_radius = gameObj->GetColliderCircle()->radius;

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        //if (m_GameObjectPtrTable[2] != nullptr)
        //{
        //    if (IsSamePos())return;
        //    {
        //        m_pGame->ChangeScene(SceneType::SCENE_ENDING);
        //    }
        //}
        if ((i == 1 || i == 2) && m_GameObjectPtrTable[i])
        {
            //!CheckOverlap();
            IsSamePos();
        }
        else if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(deltaTime);
            m_GameObjectPos[i] = m_GameObjectPtrTable[i]->GetPosition();
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
    pNewObject->SetWidth(60); // �ϴ�, ���Ƿ� ����
    pNewObject->SetHeight(60); // �ϴ�, ���Ƿ� ����

    pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo());
    pNewObject->SetColliderCircle(20.0f); // �ϴ�, ���Ƿ� ����. ������Ʈ ������ �� �� �ϰ� ���� ����.

    m_GameObjectPtrTable[0] = pNewObject;
}

void PlayScene::CreateEnemy() //�ĸ��� ����
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
    GameObject* pNewObject = nullptr;
    if (currTime < 1500.f)
    {
        pNewObject = new GameObject(ObjectType::ENEMY);
        pNewObject->SetName("Enemy");
        pNewObject->SetBitmapInfo(m_pGame->GetEnemyBitmapInfo());
    }
    else
    {
        if (spawnCnt % 3 == 0)
        {
            pNewObject = new GameObject(ObjectType::ENEMY_GOLD);
            pNewObject->SetName("Enemy_Gold");
            pNewObject->SetBitmapInfo(m_pGame->GetEnemyGoldBitmapInfo());

        }
        else
        {
            pNewObject = new GameObject(ObjectType::ENEMY);
            pNewObject->SetName("Enemy");
            pNewObject->SetBitmapInfo(m_pGame->GetEnemyBitmapInfo());
        }

        spawnCnt++;

        std::cout << spawnCnt << std::endl;
    }

    //Vector2f EnemyPos = m_pGame->EnemySpawnPosition();
    //unsigned int randHieght = rand() % (/*m_pGame->GetHeight()*/ 720 + 1);
    //unsigned int randWidth = rand() % (/*m_pGame->GetWidth()*/ 1024 + 1);

    unsigned long long seed = rd();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> distX(0, MAX_X_NUM - 1);
    std::uniform_int_distribution<int> distY(0, MAX_Y_NUM - 1);

    randWidth = distX(gen);
    randHieght = distY(gen);

    pNewObject->SetPosition(randWidth, randHieght);

    pNewObject->SetSpeed(0.3f); // �ϴ�, ���Ƿ� ����  
    pNewObject->SetWidth(30); // �ϴ�, ���Ƿ� ����
    pNewObject->SetHeight(30); // �ϴ�, ���Ƿ� ����
    pNewObject->SetColliderCircle(30.0f); // �ϴ�, ���Ƿ� ����. ������Ʈ ������ �� �� �ϰ� ���� ����.

    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0��°�� ������ �÷��̾� �Ӹ�
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            //std::cout << i << std::endl;
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

void PlayScene::UpdatePlayerInfo()
{
    static GameObject* pPlayer = GetPlayer();

    assert(pPlayer != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    Vector2f targetPos = m_pGame->PlayerTargetPosition();
    Vector2f playerPos = pPlayer->GetPosition();

    Vector2f playerDir = targetPos - playerPos;
    float distance = playerDir.Length(); // �Ÿ� ���

    if (distance > 0.1f) //���Ƿ� ������ �Ÿ�
    {
        playerDir.Normalize(); // ����ȭ
        pPlayer->SetDirection(playerDir); // �÷��̾� ���� ����
    }
    else
    {
        pPlayer->SetDirection(Vector2f(0, 0)); // �÷��̾� ����
    }
}

void PlayScene::UpdateEnemyInfo() //�ĸ�
{
    /*
    Vector2f playerPos = GetPlayer()->GetPosition(); << �÷��̾� ������ �޾ƿͼ�
     �Ʒ� ������Ʈ ���̺��� ��ü �޾ƿ��� >> ������ �޾ƿ�
     �� 2������ �񱳤��ؼ� ������Ʈ ���⺤�� ���� �Ŀ� �̵���Ű�� �ڵ�
     ���� x,y�� �ʿ�
     �Ʒ� �ݺ������� �� ��ü���� ������ �����ϱ� for���ȿ��� x,y��� �����ϸ鼭 �ش� ���������� ���⺤�� ���ϱ�
     ���Ŀ� �� ������ �̵���Ű�� distance�� x,y�� �����ϸ� �̵����߰� �����
    */

    int n = 0;

    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0��°�� ������ �÷��̾�!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pEnemy = static_cast<GameObject*>(m_GameObjectPtrTable[i]);

            unsigned long long seed = rd();

            //���� ������ �����
            std::mt19937 gen(seed);
            std::uniform_int_distribution<int> distX(0, MAX_X_NUM - 1);
            std::uniform_int_distribution<int> distY(0, MAX_Y_NUM - 1);

            randWidth = distX(gen);
            randHieght = distY(gen);
            Vector2f randPos = { randWidth ,randHieght }; // �������� ������ ��ġ
            pEnemy->SetTargetPos(randPos);
            Vector2f EnemyPos = pEnemy->GetPosition(); // ���� ��ġ
            std::cout <<i <<"��° enemy ��ǥ : " << EnemyPos.x << " " << EnemyPos.y << std::endl;
            Vector2f EnemyDir = randPos - EnemyPos;
            float distance = EnemyDir.Length(); // �Ÿ� ���

            if (distance > 100.f) //���Ƿ� ������ �Ÿ�
            {
                EnemyDir.Normalize(); // ����ȭ
                pEnemy->SetDirection(EnemyDir); // �÷��̾� ���� ����
            }

            //---------------------------------------------------------------------------

            //else
            //{
            //
            //    pEnemy->SetDirection(Vector2f(0, 0)); // �÷��̾� ����
            //}
            //
            //if (distance < 0.1f)
            //{
            //    arrbool[i] = false;
            //    n++;
            //    m_GameObjectPtrTable[i]->SetSpeed(0.1f);
            //    
            //    //arrbool[0] = true;
            //}
            //
            //else
            //{
            //    arrbool[i] = true;
            //    //arrbool[0] = false;
            //}
            //
            // if (distance > 0.1f) //���Ƿ� ������ ������Ʈ�� player ���� �Ÿ�
            // {
            //    EnemyDir.Normalize(); // ����ȭ
            //    pEnemy->SetDirection(EnemyDir); // ���� ����
            // }
            //else
            //{
            //    pEnemy->SetDirection(randPos); //  ����
            //}
        }
    }
    if (n > 0) arrbool[0] = false;
    else arrbool[0] = true;
    //arrbool[0] = (n > 0);
}
void PlayScene::IsUpdateEnemyGoal()
{
    int n = 0;
    GameObject* pPlayer = static_cast<GameObject*>(m_GameObjectPtrTable[0]);

    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0��°�� ������ �÷��̾�!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pEnemy = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
            Vector2f targetPos = pEnemy->GetTargetPos();
            Vector2f nowPos = pEnemy->GetPosition();

            Vector2f EnemyDir = targetPos - nowPos;

            float distance = EnemyDir.Length();

            Vector2f EnemyPlayerDir = pPlayer->GetPosition() - nowPos;
            float distance_EP = EnemyPlayerDir.Length(); // �Ÿ� ���


            //���� ��ǥ������ �����ϸ� ���߱�
            //���� ������ ������ ���⼭�� ������ ������(���� ��ȯ�Ǹ�), break�� �ɾ� while���� ������������ �ؾ���
            if (distance < 10.f) //���Ƿ� ������ �Ÿ�
            {
                pEnemy->SetSpeed(0);
            }
            else
            {
                pEnemy->SetSpeed(0.3f);
            }

            if (distance_EP < 35.f)
            {
                std::cout << m_GameObjectPtrTable[i]->GetName() << std::endl;
                if (strcmp(m_GameObjectPtrTable[i]->GetName(), "Enemy") == 0)
                {
                    FrogGauge += 3;
                }
                else if (strcmp(m_GameObjectPtrTable[i]->GetName(), "Enemy_Gold") == 0)
                {
                    FrogGauge += 6;
                }
                //������ ������ �ø���
                //if������ �׳��ĸ��� �۰� ���ĸ��� ũ��
                //m_GameObjectPtrTable[i]->GetName() == "Enemy" || "Enemy_Gold"

                delete m_GameObjectPtrTable[i];
                m_GameObjectPtrTable[i] = m_GameObjectPtrTable[lastindex];
                m_GameObjectPtrTable[lastindex] = nullptr;
            }
        }
    }
}

void PlayScene::CheckLastIndex()
{
    for (int i = MAX_GAME_OBJECT_COUNT - 1; i >= 0; --i)
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            lastindex = i;
            return; // ù ��ȿ �ε����� ã������ ����
        }
    }
    // ��� nullptr�� ���
    lastindex = -1;
}

// ������ �� �Լ� �ȿ��� �÷��̾� ������ �޾ƿͼ� �ٸ� ��ü�� ������ �����ͼ� ���� ���ϰ�
// �÷��̾ ���� �������� ? distance > 25.f �Ÿ� �ɶ�����
// �װ������� �浹�� true false�� �ٲٰ� ���� (�� ��ü���� for�� ��->Update���� ȣ��)

//�浹 ó��
bool PlayScene::IsCollideEnemy(int x, int y)
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

//ù��°, �ι�° �� �Ÿ��� ���� ��� bool ���(���� ���� ���� ������ ��)
//bool PlayScene::CheckOverlap()
//{
//    int firstSegPosX = m_GameObjectPtrTable[0]->GetPosition().x;
//    int firstSegPosY = m_GameObjectPtrTable[0]->GetPosition().y;
//    Vector2f fisrtIndex = (firstSegPosX, firstSegPosY);
//
//    int secondSegPosX = m_GameObjectPtrTable[1]->GetPosition().x;
//    int secondSegPosY = m_GameObjectPtrTable[1]->GetPosition().y;
//    Vector2f secondIndex = (secondSegPosX, secondSegPosY);
//
//    int distance = fisrtIndex.Distance(secondIndex);
//    if (std::abs(distance - (m_radius * 2)) < 0.01f) return true;
//    else if (distance < (m_radius * 2)) return false;
//    else return false;
//}

//bool PlayScene::ComparePPrevDistance(int i)
//{
//    // ���� ���� ���� ������ �Ÿ� ��
//    // ���� �� �Լ����� i �� �޾ƿ;���
//    int pprevEnemyPosX = m_GameObjectPtrTable[i]->GetPosition().x;
//    int pprevEnemyPosY = m_GameObjectPtrTable[i]->GetPosition().y;
//    Vector2f fisrtIndex = (pprevEnemyPosX, pprevEnemyPosY);
//
//    int nnextEnemyPosX = m_GameObjectPtrTable[i + 2]->GetPosition().x;
//    int nnextEnemyPosY = m_GameObjectPtrTable[i + 2]->GetPosition().y;
//    Vector2f secondIndex = (nnextEnemyPosX, nnextEnemyPosY);
//
//    int distance = fisrtIndex.Distance(secondIndex);
//    if (std::abs(distance - (m_radius * 4)) < 0.01f) return true;
//    else if (distance < (m_radius * 4)) return false;
//    else return false;
//
//    return false;
//}

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

