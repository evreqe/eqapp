module;

#include "common.h"
#include "utility.h"

export module eqapp_soundmanager;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class SoundManager
{

public:

    SoundManager();
    ~SoundManager();

    static SoundManager& GetInstance()
    {
        static SoundManager instance;
        return instance;
    }

private:

    SoundManager(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    bool HandleSoundManagerPlaySound(int soundID);
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "Sound Manager";

    bool m_isEnabled = true;
    bool m_isLoaded = false;
    bool m_muteSoundInBackground = true;

};

SoundManager::SoundManager()
{
    //
}

SoundManager::~SoundManager()
{
    //
}

void SoundManager::Load()
{
    //

    m_isLoaded = true;
}

void SoundManager::Unload()
{
    //

    m_isLoaded = false;
}

bool SoundManager::IsEnabled()
{
    return m_isEnabled;
}

bool SoundManager::IsLoaded()
{
    return m_isLoaded;
}

void SoundManager::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);
}

void SoundManager::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void SoundManager::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

bool SoundManager::HandleSoundManagerPlaySound(int soundID)
{
    if (m_muteSoundInBackground == true)
    {
        if (EQ_IsWindowInBackground() == true)
        {
            return true;
        }
    }

    return false;
}

bool SoundManager::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//SoundManagerToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//SoundManagerEnable" || commandText == "//SoundManagerOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//SoundManagerDisable" || commandText == "//SoundManagerOff")
    {
        Disable();
        return true;
    }

    return false;
}

}

inline eqapp::SoundManager& g_SoundManager = eqapp::SoundManager::GetInstance();

}
