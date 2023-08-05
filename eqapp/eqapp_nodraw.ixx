module;

#include "common.h"
#include "utility.h"

export module eqapp_nodraw;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class NoDraw
{

public:

    NoDraw();
    ~NoDraw();

    static NoDraw& GetInstance()
    {
        static NoDraw instance;
        return instance;
    }

private:

    NoDraw(const NoDraw&) = delete;
    NoDraw(NoDraw&&) = delete;
    NoDraw& operator=(const NoDraw&) = delete;
    NoDraw& operator=(NoDraw&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    bool HandleCRenderRenderScene();
    bool HandleCRenderUpdateDisplay();
    bool HandleCParticleSystemCreateSpellEmitter();
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "No Draw";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

    bool m_shouldRenderSceneOnce = false;
    uint32_t m_numRenderScene = 0;

    bool m_shouldUpdateDisplayOnce = false;
    uint32_t m_numUpdateDisplay = 0;

};

NoDraw::NoDraw()
{
    //
}

NoDraw::~NoDraw()
{
    //
}

void NoDraw::Load()
{
    //

    m_isLoaded = true;
}

void NoDraw::Unload()
{
    //

    m_isLoaded = false;
}

bool NoDraw::IsEnabled()
{
    return m_isEnabled;
}

bool NoDraw::IsLoaded()
{
    return m_isLoaded;
}

void NoDraw::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);
}

void NoDraw::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void NoDraw::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

bool NoDraw::HandleCRenderRenderScene()
{
    if (EQ_IsWindowInBackground() == true)
    {
        return true;
    }

    return false;
}

bool NoDraw::HandleCRenderUpdateDisplay()
{
    if (EQ_IsWindowInBackground() == true)
    {
        return true;
    }

    return false;
}

bool NoDraw::HandleCParticleSystemCreateSpellEmitter()
{
    if (EQ_IsWindowInBackground() == true)
    {
        return true;
    }

    return false;
}

bool NoDraw::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//NoDrawToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//NoDrawEnable" || commandText == "//NoDrawOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//NoDrawDisable" || commandText == "//NoDrawOff")
    {
        Disable();
        return true;
    }

    return false;
}

}

inline eqapp::NoDraw& g_NoDraw = eqapp::NoDraw::GetInstance();

}
