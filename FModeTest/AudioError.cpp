#include <iostream>
#include <thread>
#include <chrono>
#include <fmod.hpp>
#include "AudioError.h"

int main()
{
    FMOD::System* system = nullptr;

    // 1) �ý��� ����
    FMOD_CHECK(FMOD::System_Create(&system));

    // 2) �ʱ�ȭ (���� ä�� 512��, �⺻ �÷���)
    FMOD_CHECK(system->init(512, FMOD_INIT_NORMAL, nullptr));

    // 3) ���� �ε� (�޸� ���)
    FMOD::Sound* sound1 = nullptr;//ä�� �� ���� 1��
    FMOD::Sound* sound2 = nullptr;

    //FMOD_LOOP_NORMAL, FMOD_DEFAULT
    FMOD_CHECK(system->createSound("../../Sound/mid-nights-sound-291477.mp3", FMOD_DEFAULT, nullptr, &sound1)); //�� ä�ο� ���� �Ҵ�
    FMOD_CHECK(system->createSound("../../Sound/ding-126626.mp3", FMOD_DEFAULT, nullptr, &sound2));


    // 4) ���� ���
    FMOD::Channel* channel1 = nullptr;
    FMOD::Channel* channel2 = nullptr;

    FMOD_CHECK(system->playSound(sound1, nullptr, false, &channel1));
    FMOD_CHECK(system->playSound(sound2, nullptr, false, &channel2));


    // 5) ��� ���� & update
    bool isPlaying = true;
    while (isPlaying)
    {
        FMOD_CHECK(channel1->isPlaying(&isPlaying));     // ��� ���� üũ

        if (isPlaying)
        {
            // Just for testing!!! ���ӿ� ������ ���� Fixed Time �� ó��

            FMOD_CHECK(system->update()); //�����Ӵ� 1�� ���, ���⼭ �Ҹ� ���� ���� �ƴϴ�. (������ �����忡�� ������Ʈ �ϰ� �ִ�.)

            std::this_thread::sleep_for(std::chrono::milliseconds(20000));
        }
    }

    // 6) ����
    FMOD_CHECK(sound1->release());                      // Sound ����
    FMOD_CHECK(sound2->release());
    FMOD_CHECK(system->close());                       // �ý��� ����
    FMOD_CHECK(system->release());                     // �ý��� ����

    return 0;
}
