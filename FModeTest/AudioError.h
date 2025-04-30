#pragma once

#include <iostream>
#include <cstdlib>
#include <fmod_errors.h>

// 1) ����� üũ�� �α븸 �� ��
inline void FMOD_LogIfError(FMOD_RESULT result,
    const char* expr,
    const char* file,
    int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << "[" << file << ":" << line << "] "
            << expr << " failed �� "
            << FMOD_ErrorString(result)
            << " (" << result << ")\n";
    }
}

// 2) ���� �� false ���ϱ��� ��� üũ�� ��
inline bool FMOD_CheckOrReturn(FMOD_RESULT result,
    const char* expr,
    const char* file,
    int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << "[" << file << ":" << line << "] "
            << expr << " failed �� "
            << FMOD_ErrorString(result)
            << " (" << result << ")\n";
        return false;
    }
    return true;
}

// 3) ���� �� ��� ������ ��
inline void FMOD_AssertExit(FMOD_RESULT result,
    const char* expr,
    const char* file,
    int line)
{
    if (result != FMOD_OK)
    {
        std::cerr << "[" << file << ":" << line << "] "
            << expr << " failed �� "
            << FMOD_ErrorString(result)
            << " (" << result << ")\n";
        std::abort();
    }
}

#define FMOD_LOG(expr)     FMOD_LogIfError((expr), #expr, __FILE__, __LINE__)
#define FMOD_CHECK(expr)   if (!FMOD_CheckOrReturn((expr), #expr, __FILE__, __LINE__)) return false;
#define FMOD_ASSERT(expr)  FMOD_AssertExit((expr), #expr, __FILE__, __LINE__)
