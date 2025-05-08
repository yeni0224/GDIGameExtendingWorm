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
		L"개발자 개굴씨는 피곤하고 병들어있다..\r\n"
		L"영양 섭취가 조금만 늦어져도 바로 쓰러진다..\r\n"
		L"개굴씨가 오래 버틸 수 있게 도와주자..!");

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

	//마우스 위치가 start button 안에 있으면
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
	pNewObject->SetPosition(width / 2, height / 3 * 2); // 일단, 임의로 설정   
	pNewObject->SetWidth(256); // 일단, 임의로 설정
	pNewObject->SetHeight(138); // 일단, 임의로 설정
	pNewObject->SetBitmapInfo(m_pGame->GetGameStartBitmapInfo());
	//pNewObject->SetColliderCircle(40.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

	m_pButton = pNewObject;
}


