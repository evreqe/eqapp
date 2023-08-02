module;

#include "common.h"
#include "utility.h"

export module eqapp_chatevents;

export import eq;
export import eq_functions;

export import eqapp_constants;
export import eqapp_log;
export import eqapp_timer;
export import eqapp_interpretcommand;

export
{

namespace eqapp
{

class ChatEvents
{

public:

    ChatEvents();
    ~ChatEvents();

    static ChatEvents& GetInstance()
    {
        static ChatEvents instance;
        return instance;
    }

private:

    ChatEvents(const ChatEvents&) = delete;
    ChatEvents(ChatEvents&&) = delete;
    ChatEvents& operator=(const ChatEvents&) = delete;
    ChatEvents& operator=(ChatEvents&&) = delete;

public:

    struct ChatEvent
    {
        std::string FileName;
        std::string ChatText;
        std::string CommandText;
    };

    using ChatEventList = std::vector<ChatEvent>;

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    void PrintStatus();
    bool LoadFile(const std::filesystem::directory_entry& directoryEntry);
    bool LoadFiles();
    bool HandleChatManagerPrintText(const std::string& chatText);
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "Chat Events";

    const std::string m_fileName = "chatevents.txt";

    const std::string m_folderName = "chatevents";
    const std::string m_fileExtension = ".txt";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

    ChatEventList m_chatEventList;

};

ChatEvents::ChatEvents()
{
    //
}

ChatEvents::~ChatEvents()
{
    //
}

void ChatEvents::Load()
{
    LoadFiles();

    m_isLoaded = true;
}

void ChatEvents::Unload()
{
    m_isLoaded = false;
}

bool ChatEvents::IsEnabled()
{
    return m_isEnabled;
}

bool ChatEvents::IsLoaded()
{
    return m_isLoaded;
}

void ChatEvents::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);
}

void ChatEvents::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void ChatEvents::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

void ChatEvents::PrintStatus()
{
    std::print(std::cout, "{}:\n", m_className);

    std::size_t index = 0;

    for (auto& chatEvent : m_chatEventList)
    {
        std::print(std::cout, "#{}: File Name: {}\n", index, chatEvent.FileName);
        std::print(std::cout, "#{}: Chat Text: {}\n", index, chatEvent.ChatText);
        std::print(std::cout, "#{}: Command Text: {}\n", index, chatEvent.CommandText);

        index++;
    }
}

bool ChatEvents::LoadFile(const std::filesystem::directory_entry& directoryEntry)
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

    ChatEventList chatEventList;

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

        if (line.contains("^") == false)
        {
            continue;
        }

        //std::print(std::cout, "{}: line: '{}'\n", m_className, line);

        std::vector<std::string> tokens = util::String::Split(line, '^');
        if (tokens.size() != 2)
        {
            continue;
        }

        ChatEvent chatEvent;
        chatEvent.FileName = fileName;
        chatEvent.ChatText = tokens.at(0);
        chatEvent.CommandText = tokens.at(1);

        chatEventList.push_back(chatEvent);
    }

    if (chatEventList.empty() == false)
    {
        std::copy(chatEventList.begin(), chatEventList.end(), std::back_inserter(m_chatEventList));
    }
    else
    {
        std::print(std::cout, "{}: failed to load chat events from file '{}'\n", m_className, filePath);
        return false;
    }

    return true;
}

bool ChatEvents::LoadFiles()
{
    m_chatEventList.clear();

    bool result = false;

    std::string filePath = std::format("{}/{}", eqapp::Constants::FolderName, m_fileName);

    std::filesystem::directory_entry fileEntry(filePath);

    if (std::filesystem::exists(filePath) == true)
    {
        result = LoadFile(fileEntry);
    }

    std::string playerName = EQ_GetPlayerSpawnName();
    if (playerName.empty() == true)
    {
        return false;
    }

    std::string folderPath = std::format("{}/{}/", eqapp::Constants::FolderName, m_folderName);

    if (std::filesystem::exists(folderPath) == false)
    {
        std::print(std::cout, "{}: folder path does not exist, attempting to create it...\n", m_className);
        std::filesystem::create_directory(folderPath);
    }

    std::string filePathForPlayer = std::format("{}{}.txt", folderPath, playerName);

    std::filesystem::directory_entry fileEntryForPlayer(filePathForPlayer);

    if (std::filesystem::exists(filePathForPlayer) == true)
    {
        result = LoadFile(fileEntryForPlayer);
    }

    std::print(std::cout, "{}: loaded {} chat events\n", m_className, m_chatEventList.size());

    return result;
}

bool ChatEvents::HandleChatManagerPrintText(const std::string& chatText)
{
    if (chatText.contains("#") == true)
    {
        return false;
    }

    bool result = false;

    for (auto& chatEvent : m_chatEventList)
    {
        if (chatText.contains(chatEvent.ChatText) == true)
        {
            g_InterpretCommand.AddCommandTextToList(chatEvent.CommandText);

            result = true;
        }
    }

    return result;
}

bool ChatEvents::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//ChatEventsToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//ChatEventsEnable" || commandText == "//ChatEventsOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//ChatEventsDisable" || commandText == "//ChatEventsOff")
    {
        Disable();
        return true;
    }

    if (commandText == "//ChatEventsStatus")
    {
        PrintStatus();
        return true;
    }

    if (commandText == "//ChatEventsLoadFiles" || commandText == "//LoadChatEvents")
    {
        LoadFiles();
        return true;
    }

    return false;
}

}

inline eqapp::ChatEvents& g_ChatEvents = eqapp::ChatEvents::GetInstance();

}
