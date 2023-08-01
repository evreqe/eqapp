module;

#include "common.h"
#include "utility.h"

export module eqapp_macro;

export import eq;
export import eq_functions;

export import eqapp_constants;
export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class Macro
{

public:

    Macro();
    ~Macro();

private:

    //

public:

    bool IsEnabled();
    void Toggle();
    void Enable();
    void Disable();
    void Reset();
    bool LoadFile(const std::filesystem::directory_entry& directoryEntry);
    void InterpretCommandTextList();
    std::string GetName();
    std::string GetFileName();
    std::string GetFileText();
    std::vector<std::string>* GetCommandTextList();
    eqapp::Timer* GetTimer();
    eqapp::Timer::TimeInterval GetTimerInterval();
    void SetTimerInterval(eqapp::Timer::TimeInterval timeInterval);

private:

    const std::string m_className = "Macro";

    const std::string m_fileExtension = ".txt";

    bool m_isEnabled = false;

    std::string m_name;
    std::string m_fileName;
    std::string m_fileText;
    std::vector<std::string> m_commandTextList;

    eqapp::Timer m_timer;
    eqapp::Timer::TimeInterval m_timerInterval = 0;

};

Macro::Macro()
{
    //
}

Macro::~Macro()
{
    //
}

bool Macro::IsEnabled()
{
    return m_isEnabled;
}

void Macro::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: '{}' = {}\n", m_className, m_name, m_isEnabled);
}

void Macro::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void Macro::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

void Macro::Reset()
{
    m_isEnabled = false;

    m_name = "";
    m_fileName = "";
    m_fileText = "";

    m_timerInterval = 0;

    m_commandTextList.clear();
}

bool Macro::LoadFile(const std::filesystem::directory_entry& directoryEntry)
{
    std::string fileExtension = directoryEntry.path().extension().string();
    if (fileExtension != m_fileExtension)
    {
        std::print(std::cout, "{}: file extension does not match\n", m_className);
        return false;
    }

    std::string fileName = directoryEntry.path().filename().string();
    if (fileName.empty() == true)
    {
        std::print(std::cout, "{}: file name is empty\n", m_className);
        return false;
    }

    std::string fileStem = directoryEntry.path().stem().string();
    if (fileStem.empty() == true)
    {
        std::print(std::cout, "{}: file stem is empty\n", m_className);
        return false;
    }

    std::string filePath = directoryEntry.path().string();
    if (filePath.empty() == true)
    {
        std::print(std::cout, "{}: file path is empty\n", m_className);
        return false;
    }

    std::string fileText = util::File::GetText(filePath);
    if (fileText.empty() == true)
    {
        std::print(std::cout, "{}: file text is empty\n", m_className);
        return false;
    }

    m_name = fileStem;
    m_fileName = fileName;
    m_fileText = fileText;

    //std::print(std::cout, "{}: m_name: '{}'\n", m_className, m_name);
    //std::print(std::cout, "{}: m_fileName: '{}'\n", m_className, m_fileName);
    //std::print(std::cout, "{}: m_fileText: '{}'\n", m_className, m_fileText);

    m_commandTextList.clear();

    std::istringstream iss(fileText);

    std::string line;
    while (std::getline(iss, line))
    {
        if (line.empty() == true)
        {
            continue;
        }

        if (line.starts_with("#") == true)
        {
            continue;
        }

        //std::print(std::cout, "{}: line: '{}'\n", m_className, line);

        m_commandTextList.push_back(line);
    }

    return true;
}

void Macro::InterpretCommandTextList()
{
    for (auto& commandText : m_commandTextList)
    {
        EQ_InterpretCommand(commandText);
    }
}

std::string Macro::GetName()
{
    return m_name;
}

std::string Macro::GetFileName()
{
    return m_fileName;
}

std::string Macro::GetFileText()
{
    return m_fileText;
}

std::vector<std::string>* Macro::GetCommandTextList()
{
    return &m_commandTextList;
}

eqapp::Timer* Macro::GetTimer()
{
    return &m_timer;
}

eqapp::Timer::TimeInterval Macro::GetTimerInterval()
{
    return m_timerInterval;
}

void Macro::SetTimerInterval(eqapp::Timer::TimeInterval timeInterval)
{
    m_timerInterval = timeInterval;
}

}

}

