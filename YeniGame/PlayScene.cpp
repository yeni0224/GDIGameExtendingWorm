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
    
    //if (isGAmeover) retrun;  얘는 뭐냐면 게임끝나서 씬 전환을 했는데 게임이 죽으면 밑에있는 친구들어 없다고 꼬장부리는 것이니
    //여기서 아래 것들을 실행 못하게 막아야 한다.

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
    // [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
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
    pNewObject->SetPosition(720.0f, 320.0f); // 일단, 임의로 설정 
    pNewObject->SetSpeed(0.5f); // 일단, 임의로 설정  
    pNewObject->SetWidth(60); // 일단, 임의로 설정
    pNewObject->SetHeight(60); // 일단, 임의로 설정

    pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo());
    pNewObject->SetColliderCircle(20.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    m_GameObjectPtrTable[0] = pNewObject;
}

void PlayScene::CreateEnemy() //파리로 생성
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

    pNewObject->SetSpeed(0.3f); // 일단, 임의로 설정  
    pNewObject->SetWidth(30); // 일단, 임의로 설정
    pNewObject->SetHeight(30); // 일단, 임의로 설정
    pNewObject->SetColliderCircle(30.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어 머리
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
        // 게임 오브젝트 테이블이 가득 찼습니다.
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
    float distance = playerDir.Length(); // 거리 계산

    if (distance > 0.1f) //임의로 설정한 거리
    {
        playerDir.Normalize(); // 정규화
        pPlayer->SetDirection(playerDir); // 플레이어 방향 설정
    }
    else
    {
        pPlayer->SetDirection(Vector2f(0, 0)); // 플레이어 정지
    }
}

void PlayScene::UpdateEnemyInfo() //파리
{
    /*
    Vector2f playerPos = GetPlayer()->GetPosition(); << 플레이어 포지션 받아와서
     아래 오브젝트 테이블에서 객체 받아오고 >> 포지션 받아옴
     그 2포지션 비교ㅕ해서 오브젝트 방향벡터 구한 후에 이동시키는 코드
     랜덤 x,y값 필요
     아래 반복문으로 각 객체마다 돌리고 있으니까 for문안에서 x,y계속 생성하면서 해당 방향으로의 방향벡터 구하기
     이후에 그 방향대로 이동시키고 distance가 x,y에 인접하면 이동멈추게 만들기
    */

    int n = 0;

    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0번째는 언제나 플레이어!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pEnemy = static_cast<GameObject*>(m_GameObjectPtrTable[i]);

            unsigned long long seed = rd();

            //랜덤 포지션 만들기
            std::mt19937 gen(seed);
            std::uniform_int_distribution<int> distX(0, MAX_X_NUM - 1);
            std::uniform_int_distribution<int> distY(0, MAX_Y_NUM - 1);

            randWidth = distX(gen);
            randHieght = distY(gen);
            Vector2f randPos = { randWidth ,randHieght }; // 랜덤으로 생성한 위치
            pEnemy->SetTargetPos(randPos);
            Vector2f EnemyPos = pEnemy->GetPosition(); // 적의 위치
            std::cout <<i <<"번째 enemy 좌표 : " << EnemyPos.x << " " << EnemyPos.y << std::endl;
            Vector2f EnemyDir = randPos - EnemyPos;
            float distance = EnemyDir.Length(); // 거리 계산

            if (distance > 100.f) //임의로 설정한 거리
            {
                EnemyDir.Normalize(); // 정규화
                pEnemy->SetDirection(EnemyDir); // 플레이어 방향 설정
            }

            //---------------------------------------------------------------------------

            //else
            //{
            //
            //    pEnemy->SetDirection(Vector2f(0, 0)); // 플레이어 정지
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
            // if (distance > 0.1f) //임의로 설정한 오브젝트와 player 사이 거리
            // {
            //    EnemyDir.Normalize(); // 정규화
            //    pEnemy->SetDirection(EnemyDir); // 방향 설정
            // }
            //else
            //{
            //    pEnemy->SetDirection(randPos); //  정지
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

    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0번째는 언제나 플레이어!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pEnemy = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
            Vector2f targetPos = pEnemy->GetTargetPos();
            Vector2f nowPos = pEnemy->GetPosition();

            Vector2f EnemyDir = targetPos - nowPos;

            float distance = EnemyDir.Length();

            Vector2f EnemyPlayerDir = pPlayer->GetPosition() - nowPos;
            float distance_EP = EnemyPlayerDir.Length(); // 거리 계산


            //적이 목표지점에 도달하면 멈추기
            //만약 게임이 죽으면 여기서도 게임이 끝나면(씬이 전환되면), break를 걸어 while문을 빠져나가도록 해야함
            if (distance < 10.f) //임의로 설정한 거리
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
                //개구리 게이지 늘리기
                //if문으로 그냥파리는 작게 금파리는 크게
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
            return; // 첫 유효 인덱스를 찾았으면 종료
        }
    }
    // 모두 nullptr인 경우
    lastindex = -1;
}

// 지금은 이 함수 안에서 플레이어 포지션 받아와서 다른 객체들 포지션 가져와서 벡터 구하고
// 플레이어를 따라감 언제까지 ? distance > 25.f 거리 될때까지
// 그과정에서 충돌을 true false로 바꾸고 있음 (각 객체마다 for문 비교->Update에서 호출)

//충돌 처리
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

//첫번째, 두번째 원 거리비교 따른 결과 bool 출력(여러 개의 원이 겹쳤을 때)
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
//    // 현재 원과 전전 원과의 거리 비교
//    // 조건 비교 함수에서 i 값 받아와야함
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

