#include <iostream>
#include <thread>
#include <chrono>
#include <fmod.hpp>
#include "AudioError.h"

int main()
{
    FMOD::System* system = nullptr;

    // 1) 시스템 생성
    FMOD_CHECK(FMOD::System_Create(&system));

    // 2) 초기화 (가상 채널 512개, 기본 플래그)
    FMOD_CHECK(system->init(512, FMOD_INIT_NORMAL, nullptr));

    // 3) 사운드 로드 (메모리 모드)
    FMOD::Sound* sound1 = nullptr;//채널 당 사운드 1개
    FMOD::Sound* sound2 = nullptr;

    //FMOD_LOOP_NORMAL, FMOD_DEFAULT
    FMOD_CHECK(system->createSound("../../Sound/mid-nights-sound-291477.mp3", FMOD_DEFAULT, nullptr, &sound1)); //각 채널에 사운드 할당
    FMOD_CHECK(system->createSound("../../Sound/ding-126626.mp3", FMOD_DEFAULT, nullptr, &sound2));


    // 4) 사운드 재생
    FMOD::Channel* channel1 = nullptr;
    FMOD::Channel* channel2 = nullptr;

    FMOD_CHECK(system->playSound(sound1, nullptr, false, &channel1));
    FMOD_CHECK(system->playSound(sound2, nullptr, false, &channel2));


    // 5) 재생 루프 & update
    bool isPlaying = true;
    while (isPlaying)
    {
        FMOD_CHECK(channel1->isPlaying(&isPlaying));     // 재생 여부 체크

        if (isPlaying)
        {
            // Just for testing!!! 게임에 적용할 때는 Fixed Time 에 처리

            FMOD_CHECK(system->update()); //프레임당 1번 재생, 여기서 소리 내는 것이 아니다. (별도의 스레드에서 업데이트 하고 있다.)

            std::this_thread::sleep_for(std::chrono::milliseconds(20000));
        }
    }

    // 6) 정리
    FMOD_CHECK(sound1->release());                      // Sound 해제
    FMOD_CHECK(sound2->release());
    FMOD_CHECK(system->close());                       // 시스템 종료
    FMOD_CHECK(system->release());                     // 시스템 해제

    return 0;
}
