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

    static float CalculateDistance3D(float y1, float x1, float z1, float y2, float x2, float z2)
    {
        return std::sqrtf(std::powf(x2 - x1, 2) + std::powf(y2 - y1, 2) + std::powf(z2 - z1, 2));
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
        static std::string GetText(const std::string& fileName)
        {
            if (fileName.empty() == true)
            {
                return std::string();
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

        static void ReplaceAll(std::string& subject, const std::string& search, const std::string& replace)
        {
            std::string newString;
            newString.reserve(subject.length());

            std::string::size_type lastPosition = 0;
            std::string::size_type findPosition;

            while((findPosition = subject.find(search, lastPosition)) != std::string::npos)
            {
                newString.append(subject, lastPosition, findPosition - lastPosition);
                newString += replace;
                lastPosition = findPosition + search.length();
            }

            newString += subject.substr(lastPosition);

            subject.swap(newString);
        }

        static std::string GetFromBeginToEnd(const std::string& subject, const std::string& begin, const std::string& end)
        {
            std::size_t beginPosition;
            if ((beginPosition = subject.find(begin)) != std::string::npos)
            {
                std::size_t endPosition;
                if ((endPosition = subject.find(end, beginPosition)) != std::string::npos && endPosition != beginPosition)
                {
                    endPosition = endPosition + end.length();

                    return subject.substr(beginPosition, endPosition - beginPosition);
                }
            }

            return std::string();
        }

        static std::string GetBetweenBeginAndEnd(const std::string& subject, const std::string& begin, const std::string& end)
        {
            std::size_t beginPosition;
            if ((beginPosition = subject.find(begin)) != std::string::npos)
            {
                beginPosition = beginPosition + begin.length();

                std::size_t endPosition;
                if ((endPosition = subject.find(end, beginPosition)) != std::string::npos && endPosition != beginPosition)
                {
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

        static std::vector<std::string> GetAfterN(const std::string& subject, const std::string& search, std::size_t numGet)
        {
            std::vector<std::string> tokenList;
            tokenList.reserve(numGet);

            std::string token = subject;

            for (std::size_t i = 0; i < numGet; i++)
            {
                token = GetAfter(token, search);
                if (token.empty() == true)
                {
                    break;
                }

                tokenList.push_back(token);
            }

            return tokenList;
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
            std::vector<std::string> tokenList;

            std::istringstream iss(subject);
            for (std::string token; std::getline(iss, token, delimiter); tokenList.push_back(token));

            return tokenList;
        }

        static std::vector<std::string> SplitN(const std::string& subject, const char delimiter, std::size_t numSplit)
        {
            std::vector<std::string> tokenList;
            tokenList.reserve(numSplit);

            std::istringstream iss(subject);

            for (std::size_t i = 0; i < numSplit; i++)
            {
                std::string token;
                std::getline(iss, token, delimiter);

                if (token.empty() == true)
                {
                    break;
                }

                tokenList.push_back(token);
            }

            return tokenList;
        }

        static void TrimLeft(std::string& subject, const char delimiter)
        {
            subject.erase(0, subject.find_first_not_of(delimiter));
        }

        static void TrimRight(std::string& subject, const char delimiter)
        {
            subject.erase(subject.find_last_not_of(delimiter) + 1);
        }

        static void TrimSpacesOnLeftAndRight(std::string& subject)
        {
            TrimLeft(subject, ' ');
            TrimRight(subject, ' ');
        }

        static bool IsDigits(const std::string &subject)
        {
            return std::all_of(subject.begin(), subject.end(), ::isdigit);
        }

        static bool IsLowercase(const std::string &subject)
        {
            return std::all_of(subject.begin(), subject.end(), ::islower);
        }

        static bool IsUppercase(const std::string &subject)
        {
            return std::all_of(subject.begin(), subject.end(), ::isupper);
        }

        static void ToLowercase(std::string& subject)
        {
            std::transform(subject.begin(), subject.end(), subject.begin(), ::tolower);
        }

        static void ToUppercase(std::string& subject)
        {
            std::transform(subject.begin(), subject.end(), subject.begin(), ::toupper);
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

        static DWORD_PTR GetBaseAddressByProcessHandle(HANDLE processHandle)
        {
            DWORD_PTR baseAddress = NULL;

            if (processHandle == NULL)
            {
                return baseAddress;
            }

            DWORD bytesRequired = 0;

            if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
            {
                if (bytesRequired)
                {
                    LPBYTE moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

                    if (moduleArrayBytes)
                    {
                        DWORD moduleCount = bytesRequired / sizeof(HMODULE);
                        HMODULE* moduleArray = (HMODULE*)moduleArrayBytes;

                        if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
                        {
                            baseAddress = (DWORD_PTR)moduleArray[0];
                        }

                        LocalFree(moduleArrayBytes);
                    }
                }
            }

            return baseAddress;
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

        static void Beep()
        {
            MessageBeep(MB_OK);
        }

        static void BeepEx(UINT beepType)
        {
            MessageBeep(beepType);
        }

        static void CopyTextToClipboard(const char* text)
        {
            std::size_t length = std::strlen(text) + 1;

            HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, length);
            std::memcpy(GlobalLock(memory), text, length);
            GlobalUnlock(memory);

            OpenClipboard(0);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, memory);
            CloseClipboard();
        }
    }
}
