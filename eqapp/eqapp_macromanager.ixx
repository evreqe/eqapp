module;

#include "common.h"
#include "utility.h"

export module eqapp_macromanager;

export import eqapp_constants;
export import eqapp_log;
export import eqapp_timer;
export import eqapp_macro;
export import eqapp_interpretcommand;

export
{

namespace eqapp
{

class MacroManager
{

public:

    MacroManager();
    ~MacroManager();

    static MacroManager& getInstance()
    {
        static MacroManager instance;
        return instance;
    }

private:

    MacroManager(const MacroManager&) = delete;
    MacroManager(MacroManager&&) = delete;
    MacroManager& operator=(const MacroManager&) = delete;
    MacroManager& operator=(MacroManager&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    void PrintStatus();
    void LoadFiles();
    void InterpretMacro(const std::string& name);
    void SetMacroTimerInterval(const std::string& name, eqapp::Timer::TimeInterval timeInterval);
    std::string GetDisplayText();
    void Execute();
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "Macro Manager";

    const std::string m_folderName = "macros";
    const std::string m_fileExtension = ".txt";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

    std::vector<eqapp::Macro> m_macroList;

};

MacroManager::MacroManager()
{
    //
}

MacroManager::~MacroManager()
{
    //
}

void MacroManager::Load()
{
    LoadFiles();

    m_isLoaded = true;
}

void MacroManager::Unload()
{
    m_isLoaded = false;
}

bool MacroManager::IsEnabled()
{
    return m_isEnabled;
}

bool MacroManager::IsLoaded()
{
    return m_isLoaded;
}

void MacroManager::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);
}

void MacroManager::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void MacroManager::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

void MacroManager::PrintStatus()
{
    for (auto& macro : m_macroList)
    {
        eqapp::Timer::TimeInterval macroTimerInterval = macro.GetTimerInterval();

        if (macroTimerInterval == 0)
        {
            continue;
        }

        std::string macroName = macro.GetName();

        if (macroName.empty() == true)
        {
            continue;
        }

        std::print(std::cout, "{}: '{}' = {} second(s)\n", m_className, macroName, macroTimerInterval);
    }
}

void MacroManager::LoadFiles()
{
    m_macroList.clear();

    std::string folderPath = std::format("{}/{}/", eqapp::Constants::FolderName, m_folderName);

    if (std::filesystem::exists(folderPath) == false)
    {
        std::print(std::cout, "{}: folder path does not exist, attempting to create it...\n", m_className);
        std::filesystem::create_directory(folderPath);
    }

    for (auto& directoryEntry : std::filesystem::directory_iterator(folderPath))
    {
        std::string fileExtension = directoryEntry.path().extension().string();
        if (fileExtension != m_fileExtension)
        {
            std::print(std::cout, "{}: file extension of '{}' did not match '{}'\n", m_className, fileExtension, m_fileExtension);
            continue;
        }

        std::string filePath = directoryEntry.path().string();
        if (filePath.empty() == true)
        {
            std::print(std::cout, "{}: file path is empty\n", m_className);
            continue;
        }

        //std::print(std::cout, "{}: file path: '{}'\n", m_className, filePath);

        eqapp::Macro macro;
        bool result = macro.LoadFile(directoryEntry);
        if (result == true)
        {
            m_macroList.push_back(macro);
        }
        else
        {
            std::print(std::cout, "{}: failed to load macro '{}'\n", m_className, filePath);
        }
    }

    std::print(std::cout, "{}: loaded {} macros\n", m_className, m_macroList.size());
}

void MacroManager::InterpretMacro(const std::string& name)
{
    for (auto& macro : m_macroList)
    {
        if (macro.GetName() == name)
        {
            auto commandTextList = macro.GetCommandTextList();
            for (auto& commandText : *commandTextList)
            {
                g_InterpretCommand.AddCommandTextToList(commandText);
            }
            break;
        }
    }
}

void MacroManager::SetMacroTimerInterval(const std::string& name, eqapp::Timer::TimeInterval timeInterval)
{
    for (auto& macro : m_macroList)
    {
        if (macro.GetName() == name)
        {
            macro.SetTimerInterval(timeInterval);
            break;
        }
    }
}

std::string MacroManager::GetDisplayText()
{
    std::string displayText;

    auto displayTextBackInserter = std::back_inserter(displayText);

    if (IsEnabled() == true)
    {
        for (auto& macro : m_macroList)
        {
            eqapp::Timer::TimeInterval macroTimerInterval = macro.GetTimerInterval();

            if (macroTimerInterval == 0)
            {
                continue;
            }

            std::string macroName = macro.GetName();

            if (macroName.empty() == true)
            {
                continue;
            }

            std::format_to(displayTextBackInserter, "Macro: {} ({} second(s))\n", macroName, macroTimerInterval);
        }
    }
    else
    {
        std::format_to(displayTextBackInserter, "{} disabled\n", m_className);
    }

    return displayText;
}

void MacroManager::Execute()
{
    for (auto& macro : m_macroList)
    {
        eqapp::Timer::TimeInterval timerInterval = macro.GetTimerInterval();

        if (timerInterval == 0)
        {
            continue;
        }

        eqapp::Timer* timer = macro.GetTimer();

        if (timer == nullptr)
        {
            continue;
        }

        if (timer->HasTimeElapsedInSeconds(timerInterval) == true)
        {
            auto commandTextList = macro.GetCommandTextList();
            for (auto& commandText : *commandTextList)
            {
                g_InterpretCommand.AddCommandTextToList(commandText);
            }

            timer->Restart();
        }
    }
}

bool MacroManager::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//MacroManagerToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//MacroManagerEnable" || commandText == "//MacroManagerOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//MacroManagerDisable" || commandText == "//MacroManagerOff")
    {
        Disable();
        return true;
    }

    if (commandText == "//MacroManagerStatus" || commandText == "//Macros")
    {
        PrintStatus();
        return true;
    }

    if (commandText == "//MacroManagerLoadFiles" || commandText == "//LoadMacros")
    {
        LoadFiles();
        return true;
    }

    if (commandText.starts_with("//Macro ") == true)
    {
        if (auto result = scn::scan<std::string, std::string, eqapp::Timer::TimeInterval>(commandText, "{} {} {}"))
        {
            const auto& [arg0, arg1, arg2] = result->values();

            if (arg1.size() != 0)
            {
                SetMacroTimerInterval(arg1, arg2);
            }
        }
        else
        {
            if (auto result = scn::scan<std::string, std::string>(commandText, "{} {}"))
            {
                const auto& [arg0, arg1] = result->values();

                if (arg1.size() != 0)
                {
                    InterpretMacro(arg1);
                }
            }
            else
            {
                std::print(std::cout, "ERROR: {}", result.error().msg());
            }
        }

        return true;
    }

    return false;
}

}

inline eqapp::MacroManager& g_MacroManager = eqapp::MacroManager::getInstance();

}
