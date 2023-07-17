#pragma once

#include "common.h"

namespace util
{
    static void ToggleBool(bool& b)
    {
        b = !b;
    }

    static float CalculateDistance(float x1, float y1, float x2, float y2)
    {
        return std::sqrtf(std::powf(x1 - x2, 2) + std::powf(y1 - y2, 2));
    }

    namespace Random
    {
        static std::random_device randomDevice;
        static std::default_random_engine randomEngine(randomDevice());

        static uint32_t GetU32(uint32_t min, uint32_t max)
        {
            std::uniform_int_distribution<uint32_t> uniformDistribution(min, max);

            return uniformDistribution(randomEngine);
        }

        static uint64_t GetU64(uint64_t min, uint64_t max)
        {
            std::uniform_int_distribution<uint64_t> uniformDistribution(min, max);

            return uniformDistribution(randomEngine);
        }

        static float GetFloat(float min, float max)
        {
            std::uniform_real_distribution<float> uniformDistribution(min, max);

            return uniformDistribution(randomEngine);
        }
    }

    namespace File
    {
        static std::string getText(const char* fileName)
        {
            if (fileName == nullptr || std::strlen(fileName) == 0)
            {
                return {};
            }

            std::ifstream file(fileName);
            auto fileSize = std::filesystem::file_size(fileName);
            std::string text(fileSize, '\0');
            file.read(text.data(), fileSize);
            file.close();
            return text;
        }
    }

    namespace String
    {
        static void RemoveNullTerminator(std::string& text)
        {
            text.erase(std::remove(text.begin(), text.end(), '\0'), text.end());
        }

        static void RemoveWhiteSpace(std::string& text)
        {
            text.erase
            (
                std::remove_if
                (
                    text.begin(), text.end(),
                    [](char c)
                    {
                        return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f');
                    }
                ),
                text.end()
            );
        }

        static std::string GetBetweenBeginAndEnd(const std::string& subject, const std::string& begin, const std::string& end)
        {
            std::size_t beginPosition;
            if ((beginPosition = subject.find(begin)) != std::string::npos)
            {
                std::size_t endPosition;
                if ((endPosition = subject.find(end, beginPosition)) != std::string::npos && endPosition != beginPosition)
                {
                    endPosition = endPosition + 1;

                    return subject.substr(beginPosition, endPosition - beginPosition);
                }
            }

            return std::string();
        }

        static std::string GetBefore(const std::string& subject, const std::string& search)
        {
            std::string result = std::string();

            std::string::size_type position = subject.find(search);
            if (position != std::string::npos)
            {
                result = subject.substr(0, position);
            }

            return result;
        }

        static std::string GetAfter(const std::string& subject, const std::string& search)
        {
            std::string result = std::string();

            std::string::size_type position = subject.find(search);
            if (position != std::string::npos)
            {
                result = subject.substr(position + search.length(), subject.length());
            }

            return result;
        }

        static std::string Join(const std::vector<std::string>& stringList, const std::string& separator)
        {
            if (stringList.empty() == false)
            {
                std::stringstream ss;

                auto it = stringList.cbegin();
                while (true)
                {
                    ss << *it++;

                    if (it != stringList.cend())
                    {
                        ss << separator;
                    }
                    else
                    {
                        return ss.str();
                    }
                }
            }

            return std::string();
        }

        static std::vector<std::string> Split(const std::string& subject, const char delimiter)
        {
            std::vector<std::string> tokens;

            std::istringstream iss(subject);
            for (std::string token; std::getline(iss, token, delimiter); tokens.push_back(token));

            return tokens;
        }
    }

    namespace WinAPI
    {
        static void EnableDebugPrivileges()
        {
            HANDLE token;

            if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
            {
                TOKEN_PRIVILEGES tokenPrivileges;
                TOKEN_PRIVILEGES tokenPrivilegesPrevious;

                DWORD tokenPrivilegesSize = sizeof(TOKEN_PRIVILEGES);

                LUID locallyUniqueIdentifier;

                if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &locallyUniqueIdentifier))
                {
                    tokenPrivileges.PrivilegeCount           = 1;
                    tokenPrivileges.Privileges[0].Luid       = locallyUniqueIdentifier;
                    tokenPrivileges.Privileges[0].Attributes = 0;

                    AdjustTokenPrivileges
                    (
                        token,
                        FALSE,
                        &tokenPrivileges,
                        sizeof(TOKEN_PRIVILEGES),
                        &tokenPrivilegesPrevious,
                        &tokenPrivilegesSize
                    );

                    tokenPrivilegesPrevious.PrivilegeCount            = 1;
                    tokenPrivilegesPrevious.Privileges[0].Luid        = locallyUniqueIdentifier;
                    tokenPrivilegesPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    
                    AdjustTokenPrivileges
                    (
                        token,
                        FALSE,
                        &tokenPrivilegesPrevious,
                        tokenPrivilegesSize,
                        NULL,
                        NULL
                    );
                }
            }

            CloseHandle(token);
        }

        static bool IsProcessRunning(const wchar_t* fileName)
        {
            bool result = false;

            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
            if (snapshot == INVALID_HANDLE_VALUE)
            {
                return result;
            }

            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(snapshot, &pe32))
            {
                do
                {
                    if (lstrcmpi(pe32.szExeFile, fileName) == 0)
                    {
                        result = true;
                        break;
                    }

                } while (Process32Next(snapshot, &pe32));
            }

            CloseHandle(snapshot);

            return result;
        }
    }
}
