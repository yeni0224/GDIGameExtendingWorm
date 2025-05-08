#include "TitleScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

using namespace learning;

void TitleScene::Initialize(NzWndBase* pWnd)
{
	m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
	assert(m_pGame != nullptr && "Game object is not initialized!");

	Background* pNewObject = new Background(ObjectType::BACKGROUND);
	pNewObject->SetPosition(0.0f, 0.0f);
	int width = m_pGame->GetWidth();
	int height = m_pGame->GetHeight();

	pNewObject->SetWidth(width);
	pNewObject->SetHeight(height);

	pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());

	wcscpy_s(m_szTitle, L"Game Story");
	wcscpy_s(m_szStory,
		L"������ �������� �ǰ��ϰ� ������ִ�..\r\n"
		L"���� ���밡 ���ݸ� �ʾ����� �ٷ� ��������..\r\n"
		L"�������� ���� ��ƿ �� �ְ� ��������..!");

	m_rect.left = 400;
	m_rect.top = 200;
	m_rect.right = 600;
	m_rect.bottom = 220;

	m_storyRect.left = 340;
	m_storyRect.top = 230;
	m_storyRect.right = 660;
	m_storyRect.bottom = 450;

	m_pBackground = pNewObject;

	m_btnStartRect.left = 682;
	m_btnStartRect.top = 840;
	m_btnStartRect.right = 938;
	m_btnStartRect.bottom = 978;
	CreatebtnStart();
}

void TitleScene::Update(float deltaTime)
{
	static float time = 0.0f;
	time += deltaTime;

	//���콺 ��ġ�� start button �ȿ� ������
	RECT rect;
	rect = { 682, 840, 938, 978 };

	if (isClicked)
	{
		m_pGame->ChangeScene(SceneType::SCENE_PLAY);
	}
}

void TitleScene::OnLButtonDown(int x, int y)
{
	isClicked = false;
	if (x >= 380 && x <= 640)
	{
		if (y >= 411 && y <= 549)
		{
			isClicked = true;
		}
	}
}


void TitleScene::Render(HDC hDC)
{
	assert(m_pGame != nullptr && "Game object is not initialized!");
	DrawText(hDC, m_szStory, -1, &m_storyRect, DT_TOP | DT_CENTER | DT_WORDBREAK);
	DrawText(hDC, m_szTitle, -1, &m_rect, DT_TOP | DT_CENTER | DT_VCENTER);
	
	m_pButton->Render(hDC);
}


void TitleScene::Finalize()
{
	if (m_pBackground)
	{
		delete m_pBackground;
		m_pBackground = nullptr;
	}

	if (m_pButton)
	{
		delete m_pButton;
		m_pButton = nullptr;
	}
}

void TitleScene::Enter()
{
	CreatebtnStart();
}

void TitleScene::Leave()
{
	if (m_pButton)
	{
		delete m_pButton;
		m_pButton = nullptr;
	}
}

void TitleScene::CreatebtnStart()
{
	assert(m_pGame != nullptr && "Game object is not initialized!");

	GameObject* pNewObject = new GameObject(ObjectType::GAMESTART);

	int width = m_pGame->GetWidth();
	int height = m_pGame->GetHeight();

	pNewObject->SetName("Button");
	pNewObject->SetPosition(width / 2, height / 3 * 2); // �ϴ�, ���Ƿ� ����   
	pNewObject->SetWidth(256); // �ϴ�, ���Ƿ� ����
	pNewObject->SetHeight(138); // �ϴ�, ���Ƿ� ����
	pNewObject->SetBitmapInfo(m_pGame->GetGameStartBitmapInfo());
	//pNewObject->SetColliderCircle(40.0f); // �ϴ�, ���Ƿ� ����. ������Ʈ ������ �� �� �ϰ� ���� ����.

	m_pButton = pNewObject;
}


