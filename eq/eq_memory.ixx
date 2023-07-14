module;

#include "common.h"

export module eq_memory;

export
{

namespace eq
{

namespace Memory
{

template <class T>
T Read(uintptr_t address)
{
    T* buffer = (T*)address;
    return (*buffer);
}

template <class T>
void Write(uintptr_t address, T value)
{
    T* buffer = (T*)address;
    *buffer = value;
}

template <class T>
T ReadProtected(uintptr_t address)
{
    DWORD oldProtect;
    VirtualProtectEx(GetCurrentProcess(), (LPVOID)address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);

    T* buffer = (T*)address;

    VirtualProtectEx(GetCurrentProcess(), (LPVOID)address, sizeof(T), oldProtect, &oldProtect);

    return (*buffer);
}

template <class T>
void WriteProtected(uintptr_t address, T value)
{
    DWORD oldProtect;
    VirtualProtectEx(GetCurrentProcess(), (LPVOID)address, sizeof(value), PAGE_EXECUTE_READWRITE, &oldProtect);

    T* buffer = (T*)address;
    *buffer = value;

    VirtualProtectEx(GetCurrentProcess(), (LPVOID)address, sizeof(value), oldProtect, &oldProtect);
}

void WriteBytesProtected(uintptr_t address, char* bytes, size_t size)
{
    DWORD oldProtect;
    VirtualProtectEx(GetCurrentProcess(), (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);

    std::memcpy((void*)address, (void*)bytes, size);

    VirtualProtectEx(GetCurrentProcess(), (LPVOID)address, size, oldProtect, &oldProtect);
}

char* ReadString(uintptr_t address)
{
    char* buffer = (char*)((char*)address);

    return buffer;
}

void WriteString(uintptr_t address, const char* value)
{
    size_t length = std::strlen(value);

    size_t j = 0;

    for (size_t i = 0; i < length; i++)
    {
        *(unsigned char*)(address + j) = value[i];
        j++;
    }

    *(unsigned char*)(address + j) = '\0';
}

}

}

}
