module;

#include "common.h"
#include "utility.h"

export module eqapp_boxchatclient;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class BoxChatClient
{

public:

    BoxChatClient();
    ~BoxChatClient();

    static BoxChatClient& GetInstance()
    {
        static BoxChatClient instance;
        return instance;
    }

private:

    BoxChatClient(const BoxChatClient&) = delete;
    BoxChatClient(BoxChatClient&&) = delete;
    BoxChatClient& operator=(const BoxChatClient&) = delete;
    BoxChatClient& operator=(BoxChatClient&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    bool IsConnected();
    bool IsServerRunning();
    void Toggle();
    void Enable();
    void Disable();
    bool Connect(const std::string& clientName);
    bool ConnectAsPlayerSpawnName();
    void Disconnect();
    void DisconnectEx();
    bool SendText(const std::string& text);
    bool SetGlobalChannel(const std::string& globalChannelName);
    bool SetChannel(const std::string& channelName);
    void Execute();
    void InterpretCommands();
    bool HandleInterpetCommand(const char* text);

private:

    const std::string m_name = "EQ Box Chat Client";

    const char* m_processFileName = "eqapp_boxchatserver.exe";

    const wchar_t* m_processFileNameWide = L"eqapp_boxchatserver.exe";

    bool m_isEnabled = true;
    bool m_isLoaded = false;
    bool m_isConnected = false;

    std::string m_clientName = "UNKNOWN";
    std::string m_clientGlobalChannelName = "Global1";
    std::string m_clientChannelName = "Default";

    std::list<std::string> m_interpretCommandList;
    eqapp::Timer m_interpretCommandTimer;

    eqapp::Timer m_keepAliveTimer;
    eqapp::Timer::TimeInterval m_keepAliveTimerInterval = 3;

    std::string m_ipAddress = "127.0.0.1"; // localhost
    std::string m_portNumber = "29015";

    WSADATA m_wsaData;

    SOCKET m_socket = INVALID_SOCKET;
    fd_set m_readFdSet;

    struct addrinfo m_addressInfoHints;
    struct addrinfo* m_addressInfoResult = nullptr;

    const uint32_t m_recvBufferSize = 4096;
    char m_recvBuffer[4096] = {0};

};

BoxChatClient::BoxChatClient()
{
    //
}

BoxChatClient::~BoxChatClient()
{
    //
}

void BoxChatClient::Load()
{
    m_interpretCommandList.clear();

    m_isLoaded = true;
}

void BoxChatClient::Unload()
{
    Disconnect();

    m_isLoaded = false;
}

bool BoxChatClient::IsEnabled()
{
    return m_isEnabled;
}

bool BoxChatClient::IsLoaded()
{
    return m_isLoaded;
}

bool BoxChatClient::IsConnected()
{
    return m_isConnected;
}

bool BoxChatClient::IsServerRunning()
{
    return util::WinAPI::IsProcessRunning(m_processFileNameWide);
}

void BoxChatClient::Toggle()
{
    util::ToggleBool(m_isEnabled);
    EQ_PrintTextToChat(std::format("Box Chat: {}", m_isEnabled).c_str());
}

void BoxChatClient::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void BoxChatClient::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

bool BoxChatClient::Connect(const std::string& clientName)
{
    m_interpretCommandList.clear();

    // functions WSAStartup() and WSACleanup() are not needed
    // because the game already calls them for us
    // if we call the functions ourselves, the game will crash

    DisconnectEx();

    ZeroMemory(&m_addressInfoHints, sizeof(m_addressInfoHints));
    m_addressInfoHints.ai_family   = AF_INET;
    m_addressInfoHints.ai_socktype = SOCK_STREAM;
    m_addressInfoHints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(m_ipAddress.c_str(), m_portNumber.c_str(), &m_addressInfoHints, &m_addressInfoResult) != 0)
    {
        freeaddrinfo(m_addressInfoResult);
        DisconnectEx();
        return false;
    }

    m_socket = socket(m_addressInfoResult->ai_family, m_addressInfoResult->ai_socktype, m_addressInfoResult->ai_protocol);
    if (m_socket == INVALID_SOCKET)
    {
        freeaddrinfo(m_addressInfoResult);
        DisconnectEx();
        return false;
    }

    if (connect(m_socket, m_addressInfoResult->ai_addr, (int)m_addressInfoResult->ai_addrlen) != SOCKET_ERROR)
    {
        if (m_socket != INVALID_SOCKET)
        {
            std::string clientNameText = std::format("$ClientName {}\n", clientName);

            if (send(m_socket, clientNameText.c_str(), (int)clientNameText.size(), 0) == SOCKET_ERROR)
            {
                freeaddrinfo(m_addressInfoResult);
                DisconnectEx();
                return false;
            }

            EQ_PrintTextToChat("Box Chat connected.");

            m_clientName = clientName;

            freeaddrinfo(m_addressInfoResult);
            m_isConnected = true;
            return true;
        }
    }

    freeaddrinfo(m_addressInfoResult);
    DisconnectEx();
    return false;
}

bool BoxChatClient::ConnectAsPlayerSpawnName()
{
    std::string playerSpawnName = EQ_GetPlayerSpawnName();
    if (playerSpawnName.size() != 0)
    {
        bool result = Connect(playerSpawnName);

        return result;
    }

    return false;
}

void BoxChatClient::Disconnect()
{
    DisconnectEx();

    EQ_PrintTextToChat("Box Chat disconnected.");
}

void BoxChatClient::DisconnectEx()
{
    shutdown(m_socket, SD_BOTH);
    closesocket(m_socket);
    m_socket = INVALID_SOCKET;

    m_clientName = "UNKNOWN";
    m_clientGlobalChannelName = "Global1";
    m_clientChannelName = "Default";

    m_isConnected = false;
}

bool BoxChatClient::SendText(const std::string& text)
{
    if (text.size() == 0)
    {
        EQ_PrintTextToChat("Box Chat: text is empty");
        return false;
    }

    if (text.back() != '\n')
    {
        EQ_PrintTextToChat("Box Chat: text does not end with a newline character");
        return false;
    }

    if (m_socket == INVALID_SOCKET)
    {
        EQ_PrintTextToChat("Box Chat: socket is invalid");

        Disconnect();
        return false;
    }

    int sendResult = send(m_socket, text.c_str(), (int)text.size(), 0);
    if (sendResult == SOCKET_ERROR)
    {
        EQ_PrintTextToChat("Box Chat: send() failed with SOCKET_ERROR");

        Disconnect();
        return false;
    }

    return true;
}

bool BoxChatClient::SetGlobalChannel(const std::string& globalChannelName)
{
    if (globalChannelName.size() == 0)
    {
        EQ_PrintTextToChat("Box Chat: globalChannelName is empty");
        return false;
    }

    std::string globalChannelNameText = std::format("$ClientGlobalChannel {}\n", globalChannelName);

    if (SendText(globalChannelNameText) == true)
    {
        m_clientGlobalChannelName = globalChannelName;
        return true;
    }

    return false;
}

bool BoxChatClient::SetChannel(const std::string& channelName)
{
    if (channelName.size() == 0)
    {
        EQ_PrintTextToChat("Box Chat: channelName is empty");
        return false;
    }

    std::string channelNameText = std::format("$ClientChannel {}\n", channelName);

    if (SendText(channelNameText) == true)
    {
        m_clientChannelName = channelName;
        return true;
    }

    return false;
}

void BoxChatClient::Execute()
{
    if (m_isConnected == false)
    {
        return;
    }

    if (m_socket == INVALID_SOCKET)
    {
        Disconnect();
        return;
    }

    FD_ZERO(&m_readFdSet);
    FD_SET(m_socket, &m_readFdSet);

    timeval timeVal;
    timeVal.tv_sec  = 0;
    timeVal.tv_usec = 0;

    int selectResult = select(0, &m_readFdSet, NULL, NULL, &timeVal);
    if (selectResult == SOCKET_ERROR)
    {
        return;
    }

    if (m_keepAliveTimer.HasTimeElapsedInSeconds(m_keepAliveTimerInterval) == true)
    {
        SendText("$KeepAlive\n");

        m_keepAliveTimer.Restart();
    }

    if (FD_ISSET(m_socket, &m_readFdSet))
    {
        std::memset(m_recvBuffer, 0, sizeof(m_recvBuffer));

        int recvResult = recv(m_socket, m_recvBuffer, sizeof(m_recvBuffer), 0);
        if (recvResult <= 0)
        {
            return;
        }

        std::string recvText = m_recvBuffer;
        if (recvText.size() == 0)
        {
            return;
        }

        ////std::print("recvText: {}\n", recvText);

        std::vector<std::string> recvTextList = util::String::Split(recvText, '\n');

        if (recvTextList.size() == 0)
        {
            ////std::print("recvTextList.size() == 0\n");
            return;
        }

        for (auto& recvTextItem : recvTextList)
        {
            std::vector<std::string> wordList = util::String::Split(recvTextItem, ' ');

            if (wordList.size() == 0)
            {
                ////std::print("wordList.size() == 0\n");
                continue;
            }

            if (wordList.size() > 1)
            {
                if (wordList.at(0) == "$InterpretCommand")
                {
                    std::vector<std::string>::const_iterator wordListFirst = wordList.begin() + 1;
                    std::vector<std::string>::const_iterator wordListLast = wordList.end();

                    std::vector<std::string> wordListEx(wordListFirst, wordListLast);

                    if (wordListEx.size() == 0)
                    {
                        ////std::print("wordListEx.size() == 0\n");
                        continue;
                    }

                    std::string commandText = util::String::Join(wordListEx, " ");
                    if (commandText.size() == 0)
                    {
                        ////std::print("commandText.size() == 0\n");
                        break;
                    }

                    ////std::print("commandText: {}\n", commandText);

                    m_interpretCommandList.push_back(commandText);
                }
            }
        }
    }

    InterpretCommands();
}

void BoxChatClient::InterpretCommands()
{
    if (m_interpretCommandList.size() == 0)
    {
        return;
    }

    std::string commandText = m_interpretCommandList.front();
    if (commandText.size() == 0)
    {
        return;
    }

    if (commandText.starts_with("/") == false)
    {
        return;
    }

    bool bUseTimer = false;

    bool bHasTimeElapsed = false;

    eqapp::Timer::TimeInterval timeInterval = 1;

    if (commandText.starts_with("//Pause ") == true)
    {
        std::string timeInteralAsString = util::String::GetAfter(commandText, " ");
        if (timeInteralAsString.size() != 0)
        {
            timeInterval = std::stoll(timeInteralAsString);

            bUseTimer = true;
        }
    }

    if (m_interpretCommandTimer.HasTimeElapsedInSeconds(timeInterval) == true)
    {
        bHasTimeElapsed = true;

        m_interpretCommandTimer.Restart();
    }

    if (bUseTimer == true && bHasTimeElapsed == false)
    {
        return;
    }

    m_interpretCommandList.pop_front();

    EQ_InterpretCommand(commandText.c_str());
}

bool BoxChatClient::HandleInterpetCommand(const char* text)
{
    std::string commandText = text;

    if (commandText == "//BoxChatConnect")
    {
        ConnectAsPlayerSpawnName();
        return true;
    }

    if (commandText == "//BoxChatDisconnect")
    {
        Disconnect();
        return true;
    }

    return false;
}

}

inline eqapp::BoxChatClient& g_BoxChatClient = eqapp::BoxChatClient::GetInstance();

}
