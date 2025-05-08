#include "INC_Windows.h"
#include "Collider.h"
#include "RenderHelp.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

GameObject::~GameObject()
{
    if (m_pColliderCircle)
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }
}

void GameObject::Update(float deltaTime)
{
    UpdateFrame(deltaTime);
    Move(deltaTime);

    // Collider ������Ʈ
    if (m_pColliderCircle)
    {
        m_pColliderCircle->center = m_pos;
    }
    if (m_pColliderBox)
    {
        m_pColliderBox->center = m_pos;
    }
}

//fly �׸���
void GameObject::Render(HDC hdc, bool arrbool)
{
    //DrawCollider(hdc, arrbool);
    DrawBitmap(hdc);
}

//������ �׸���
void GameObject::Render(HDC hdc)
{
    //DrawCollider(hdc);
    DrawBitmap(hdc);
}

void GameObject::SetColliderCircle(float radius)
{
    if (m_pColliderCircle)
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    m_pColliderCircle = new ColliderCircle;

    assert(m_pColliderCircle != nullptr && "Failed to create ColliderCircle!");

    m_pColliderCircle->radius = radius;
    m_pColliderCircle->center = m_pos;
}


void GameObject::SetColliderBox(float width, float height)
{
    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }

    m_pColliderBox = new ColliderBox;

    assert(m_pColliderBox != nullptr && "Failed to create ColliderBox!");

    m_pColliderBox->center = m_pos;
    m_pColliderBox->halfSize.x = width / 2.0f;
    m_pColliderBox->halfSize.y = height / 2.0f;
}

void GameObject::DrawCollider(HDC hdc)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(116, 189, 121));
    
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    //HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(RGB(116,189,121)));

    if (m_pColliderCircle)
    {
        Ellipse(hdc, m_pColliderCircle->center.x - m_pColliderCircle->radius,
            m_pColliderCircle->center.y - m_pColliderCircle->radius,
            m_pColliderCircle->center.x + m_pColliderCircle->radius,
            m_pColliderCircle->center.y + m_pColliderCircle->radius);
    }

    if (m_pColliderBox)
    {
        Rectangle(hdc, m_pColliderBox->center.x - m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y - m_pColliderBox->halfSize.y,
            m_pColliderBox->center.x + m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y + m_pColliderBox->halfSize.y);
    }

    // ���� ��ü ���� �� �� ����
    SelectObject(hdc, hOldPen);
    //SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

void GameObject::DrawCollider(HDC hdc, bool arrbool)
{
    HPEN hPen = nullptr;
    if (arrbool)
    {
        hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    }
    else if (!arrbool)
    {
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    }

    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    if (m_pColliderCircle)
    {
        Ellipse(hdc, m_pColliderCircle->center.x - m_pColliderCircle->radius,
            m_pColliderCircle->center.y - m_pColliderCircle->radius,
            m_pColliderCircle->center.x + m_pColliderCircle->radius,
            m_pColliderCircle->center.y + m_pColliderCircle->radius);
    }

    if (m_pColliderBox)
    {
        Rectangle(hdc, m_pColliderBox->center.x - m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y - m_pColliderBox->halfSize.y,
            m_pColliderBox->center.x + m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y + m_pColliderBox->halfSize.y);
    }

    // ���� ��ü ���� �� �� ����
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void GameObject::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");

    m_pBitmapInfo = bitmapInfo;


    /* ��������Ʈ ������ �ϴ��� �ϵ��ڵ��ؿ�. 
     �ϴ�, ������ ũ��� �ð��� ���ٰ� �����մϴ�.
    m_frameWidth = m_pBitmapInfo->GetWidth() / 5;
    m_frameHeight = m_pBitmapInfo->GetHeight() / 3;
    m_frameIndex = 0;

    for (int i = 0; i < 5; ++i)
    {
        m_frameXY[i].x = i * m_frameWidth;
        m_frameXY[i].y = 0;
    }

    for (int i = 0; i < 5; ++i)
    {
        m_frameXY[i + 5].x = i * m_frameWidth;
        m_frameXY[i + 5].y = m_frameHeight;
    }

    for (int i = 0; i < 4; ++i)
    {
        m_frameXY[i + 10].x = i * m_frameWidth;
        m_frameXY[i + 10].y = m_frameHeight * 2;
    }*/
    m_frameWidth = m_pBitmapInfo->GetWidth();
    m_frameHeight = m_pBitmapInfo->GetHeight();
}

void GameObject::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

    HDC hBitmapDC = CreateCompatibleDC(hdc);

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION ���� (���� ä�� ó��)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // ���� ���� ä�� �״�� ���
    blend.AlphaFormat = AC_SRC_ALPHA;

    const int x = m_pos.x - m_width / 2;
    const int y = m_pos.y - m_height / 2;

    const int srcX = m_frameXY.x;
    const int srcY = m_frameXY.y;

    AlphaBlend(hdc, x, y, m_width, m_height,
        hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend);

    // ��Ʈ�� �ڵ� ����
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
}

void GameObject::Move(float deltaTime)
{
    GameObjectBase::Move(deltaTime);
}

void GameObject::UpdateFrame(float deltaTime)
{
    /*m_frameTime += deltaTime;
    if (m_frameTime >= m_frameDuration)
    {
        m_frameTime = 0.0f;
        m_frameIndex = (m_frameIndex + 1) % (m_frameCount);
    }*/
}

void GameObjectBase::SetName(const char* name)
{
    if (name == nullptr) return;

    strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
    m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}

Background::~Background()
{
}

void Background::Update(float deltaTime)
{
    // ����� ������Ʈ�� �ʿ����
    // Ȥ�� ����� ������ ��쿡�� �̰��� �����ϸ� ��
}

void Background::Render(HDC hdc)
{
    DrawBitmap(hdc);
}

void Background::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");

    m_pBitmapInfo = bitmapInfo;
}

void Background::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;
    HDC hBitmapDC = CreateCompatibleDC(hdc);

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION ���� (���� ä�� ó��)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // ���� ���� ä�� �״�� ���
    blend.AlphaFormat = AC_SRC_ALPHA;

    int screenWidth = 0;
    int screenHeight = 0;
    learning::GetScreenSize(screenWidth, screenHeight);

    AlphaBlend(hdc, 0, 0, screenWidth, screenHeight,
        hBitmapDC, 0, 0, m_width, m_height, blend);
    // ��Ʈ�� �ڵ� ����
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
}

UIObject::~UIObject()
{
}

void UIObject::Update(float deltaTime)
{
    // �÷��̾��� hp �� Ȯ��


    // �÷��̾��� hp�� 0�̶�� �� ��ȯ
}

void UIObject::Render(HDC hdc)
{
    DrawBitmap(hdc);
}

void UIObject::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;
    HDC hBitmapDC = CreateCompatibleDC(hdc);

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION ���� (���� ä�� ó��)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // ���� ���� ä�� �״�� ���
    blend.AlphaFormat = AC_SRC_ALPHA;

    int screenWidth = 0;
    int screenHeight = 0;
    learning::GetScreenSize(screenWidth, screenHeight);
    AlphaBlend(hdc, 0, 0, screenWidth, screenHeight,
        hBitmapDC, 0, 0, m_width, m_height, blend);
    // ��Ʈ�� �ڵ� ����
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
}
