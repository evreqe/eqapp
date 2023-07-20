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
    void PrintStatus();
    bool Connect(const std::string& clientName);
    bool ConnectAsPlayerSpawnName();
    void ConnectFailed(PADDRINFOA addressInfo);
    void Disconnect();
    void DisconnectEx();
    bool SendText(const std::string& text);
    void PrintClientName();
    std::string GetClientName();
    bool SetClientName(const std::string& clientName);
    void PrintClientGlobalChannelName();
    std::string GetClientGlobalChannelName();
    bool SetClientGlobalChannelName(const std::string& clientGlobalChannelName);
    void PrintClientChannelName();
    std::string GetClientChannelName();
    bool SetClientChannelName(const std::string& clientChannelName);
    void PrintIPAddress();
    std::string GetIPAddress();
    bool SetIPAddress(const std::string& ipAddress);
    void PrintPortNumber();
    std::string GetPortNumber();
    bool SetPortNumber(const std::string& portNumber);
    void Execute();
    void InterpretCommands();
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_name = "Box Chat";

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

    const std::string m_keepAliveText = "$KeepAlive\n";
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
    std::print(std::cout, "{}: {}", m_name, m_isEnabled);
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

void BoxChatClient::PrintStatus()
{
    if (IsConnected() == false)
    {
        std::print(std::cout, "{} Status: You are disconnected.\n", m_name);
    }
    else
    {
        std::print(std::cout, "{} Status: You are connected.\n", m_name);

        PrintIPAddress();
        PrintPortNumber();
        PrintClientName();
        PrintClientGlobalChannelName();
        PrintClientChannelName();
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
        ConnectFailed(m_addressInfoResult);
        return false;
    }

    m_socket = socket(m_addressInfoResult->ai_family, m_addressInfoResult->ai_socktype, m_addressInfoResult->ai_protocol);
    if (m_socket == INVALID_SOCKET)
    {
        ConnectFailed(m_addressInfoResult);
        return false;
    }

    if (connect(m_socket, m_addressInfoResult->ai_addr, (int)m_addressInfoResult->ai_addrlen) != SOCKET_ERROR)
    {
        if (m_socket != INVALID_SOCKET)
        {
            std::string clientNameText = std::format("$ClientName {}\n", clientName);

            if (send(m_socket, clientNameText.c_str(), (int)clientNameText.size(), 0) == SOCKET_ERROR)
            {
                ConnectFailed(m_addressInfoResult);
                return false;
            }

            std::print(std::cout, "{} connected.\n", m_name);

            m_clientName = clientName;

            freeaddrinfo(m_addressInfoResult);
            m_isConnected = true;
            return true;
        }
    }

    ConnectFailed(m_addressInfoResult);
    return false;
}

bool BoxChatClient::ConnectAsPlayerSpawnName()
{
    std::string playerSpawnName = EQ_GetPlayerSpawnName();
    if (playerSpawnName.size() != 0)
    {
        return Connect(playerSpawnName);
    }

    return false;
}

void BoxChatClient::ConnectFailed(PADDRINFOA addressInfo)
{
    freeaddrinfo(m_addressInfoResult);
    DisconnectEx();

    std::print("{} failed to connect.\n", m_name);
}

void BoxChatClient::Disconnect()
{
    DisconnectEx();

    std::print(std::cout, "{} disconnected.\n", m_name);
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
        std::print(std::cout, "{}: text is empty\n", m_name);
        return false;
    }

    if (text.back() != '\n')
    {
        std::print(std::cout, "{}: text does not end with a newline character\n", m_name);
        return false;
    }

    if (m_socket == INVALID_SOCKET)
    {
        std::print(std::cout, "{}: socket is invalid\n", m_name);

        Disconnect();
        return false;
    }

    int sendResult = send(m_socket, text.c_str(), (int)text.size(), 0);
    if (sendResult == SOCKET_ERROR)
    {
        std::print(std::cout, "{}: send() failed with SOCKET_ERROR\n", m_name);

        Disconnect();
        return false;
    }

    return true;
}

void BoxChatClient::PrintClientName()
{
    std::print(std::cout, "{} Client Name: {}\n", m_name, m_clientName);
}

std::string BoxChatClient::GetClientName()
{
    return m_clientName;
}

bool BoxChatClient::SetClientName(const std::string& clientName)
{
    if (clientName.size() == 0)
    {
        std::print(std::cout, "{}: clientName is empty\n", m_name);
        return false;
    }

    std::string clientNameText = std::format("$ClientName {}\n", clientName);

    if (SendText(clientNameText) == true)
    {
        m_clientName = clientName;
        return true;
    }

    return false;
}

void BoxChatClient::PrintClientGlobalChannelName()
{
    std::print(std::cout, "{} Client Global Channel Name: {}\n", m_name, m_clientGlobalChannelName);
}

std::string BoxChatClient::GetClientGlobalChannelName()
{
    return m_clientGlobalChannelName;
}

bool BoxChatClient::SetClientGlobalChannelName(const std::string& clientGlobalChannelName)
{
    if (clientGlobalChannelName.size() == 0)
    {
        std::print(std::cout, "{}: clientGlobalChannelName is empty\n", m_name);
        return false;
    }

    std::string clientGlobalChannelNameText = std::format("$ClientGlobalChannelName {}\n", clientGlobalChannelName);

    if (SendText(clientGlobalChannelNameText) == true)
    {
        m_clientGlobalChannelName = clientGlobalChannelName;
        return true;
    }

    return false;
}

void BoxChatClient::PrintClientChannelName()
{
    std::print(std::cout, "{} Client Channel Name: {}\n", m_name, m_clientChannelName);
}

std::string BoxChatClient::GetClientChannelName()
{
    return m_clientChannelName;
}

bool BoxChatClient::SetClientChannelName(const std::string& clientChannelName)
{
    if (clientChannelName.size() == 0)
    {
        std::print(std::cout, "{}: clientChannelName is empty\n", m_name);
        return false;
    }

    std::string clientChannelNameText = std::format("$ClientChannelName {}\n", clientChannelName);

    if (SendText(clientChannelNameText) == true)
    {
        m_clientChannelName = clientChannelName;
        return true;
    }

    return false;
}

void BoxChatClient::PrintIPAddress()
{
    std::print(std::cout, "{} IP Address: {}\n", m_name, m_ipAddress);
}

std::string BoxChatClient::GetIPAddress()
{
    return m_ipAddress;
}

bool BoxChatClient::SetIPAddress(const std::string& ipAddress)
{
    if (ipAddress.size() == 0)
    {
        std::print(std::cout, "{}: ipAddress is empty\n", m_name);
        return false;
    }

    m_ipAddress = ipAddress;

    return true;
}

void BoxChatClient::PrintPortNumber()
{
    std::print(std::cout, "{} Port Number: {}\n", m_name, m_portNumber);
}

std::string BoxChatClient::GetPortNumber()
{
    return m_portNumber;
}

bool BoxChatClient::SetPortNumber(const std::string& portNumber)
{
    if (portNumber.size() == 0)
    {
        std::print(std::cout, "{}: portNumber is empty\n", m_name);
        return false;
    }

    m_portNumber = portNumber;

    return true;
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
        SendText(m_keepAliveText);

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

        ////std::print(std::cout, "recvText: {}\n", recvText);

        std::vector<std::string> recvTextList = util::String::Split(recvText, '\n');

        if (recvTextList.size() == 0)
        {
            ////std::print(std::cout, "recvTextList.size() == 0\n");
            return;
        }

        for (auto& recvTextItem : recvTextList)
        {
            std::vector<std::string> wordList = util::String::Split(recvTextItem, ' ');

            if (wordList.size() == 0)
            {
                ////std::print(std::cout, "wordList.size() == 0\n");
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
                        ////std::print(std::cout, "wordListEx.size() == 0\n");
                        continue;
                    }

                    std::string commandText = util::String::Join(wordListEx, " ");
                    if (commandText.size() == 0)
                    {
                        ////std::print(std::cout, "commandText.size() == 0\n");
                        break;
                    }

                    ////std::print(std::cout, "commandText: {}\n", commandText);

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
        m_interpretCommandList.pop_front();
        return;
    }

    if (commandText.starts_with("/") == false)
    {
        m_interpretCommandList.pop_front();
        return;
    }

    bool bUseTimer = false;

    bool bHasTimeElapsed = false;

    bool bUsePause = false;

    eqapp::Timer::TimeInterval timeInterval = 1;

    if (commandText.starts_with("//Pause ") == true)
    {
        bUsePause = true;

        std::string arg0;
        eqapp::Timer::TimeInterval arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            timeInterval = arg1;

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

    if (bUsePause == true)
    {
        return;
    }

    EQ_InterpretCommand(commandText.c_str());
}

bool BoxChatClient::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//BoxChatStatus" || commandText == "//BCS")
    {
        PrintStatus();
        return true;
    }

    if (commandText.starts_with("//BoxChatSendText ") == true || commandText.starts_with("//BCST ") == true)
    {
        std::string arg0;
        auto result = scn::scan(commandText, "{}", arg0);
        if (result)
        {
            std::string remainder = result.range_as_string();
            if (remainder.size() != 0)
            {
                SendText(remainder);
            }
        }

        return true;
    }

    if (commandText == "//BoxChatConnect" || commandText == "//BCC")
    {
        ConnectAsPlayerSpawnName();
        return true;
    }

    if (commandText.starts_with("//BoxChatConnect ") == true || commandText.starts_with("//BCC ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            if (arg1.size() != 0)
            {
                Connect(arg1);
            }
        }

        return true;
    }

    if (commandText == "//BoxChatDisconnect" || commandText == "//BCD")
    {
        Disconnect();
        return true;
    }

    if (commandText == "//BoxChatIP" || commandText == "//BCIP")
    {
        PrintIPAddress();
        return true;
    }

    if (commandText.starts_with("//BoxChatIP ") == true || commandText.starts_with("//BCIP ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            if (arg1.size() != 0)
            {
                SetIPAddress(arg1);
                PrintIPAddress();
            }
        }

        return true;
    }

    if (commandText == "//BoxChatPortNumber" || commandText == "//BCPN")
    {
        PrintPortNumber();
        return true;
    }

    if (commandText.starts_with("//BoxChatPortNumber ") == true || commandText.starts_with("//BCPN ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            if (arg1.size() != 0)
            {
                SetPortNumber(arg1);
                PrintPortNumber();
            }
        }

        return true;
    }

    if (commandText == "//BoxChatClientName" || commandText == "//BCCN")
    {
        PrintClientName();
        return true;
    }

    if (commandText.starts_with("//BoxChatClientName ") == true || commandText.starts_with("//BCCN ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            if (arg1.size() != 0)
            {
                SetClientName(arg1);
                PrintClientName();
            }
        }

        return true;
    }

    if (commandText == "//BoxChatClientGlobalChannelName" || commandText == "//BCCGCN")
    {
        PrintClientGlobalChannelName();
        return true;
    }

    if (commandText.starts_with("//BoxChatClientGlobalChannelName ") == true || commandText.starts_with("//BCCGCN ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            if (arg1.size() != 0)
            {
                SetClientGlobalChannelName(arg1);
                PrintClientGlobalChannelName();
            }
        }

        return true;
    }

    if (commandText == "//BoxChatClientChannelName" || commandText == "//BCCCN")
    {
        PrintClientChannelName();
        return true;
    }

    if (commandText.starts_with("//BoxChatClientChannelName ") == true || commandText.starts_with("//BCCCN ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            if (arg1.size() != 0)
            {
                SetClientChannelName(arg1);
                PrintClientChannelName();
            }
        }

        return true;
    }

    if (commandText.starts_with("//BoxChatTell ") == true || commandText.starts_with("//BCT ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            std::string remainder = result.range_as_string();
            util::String::TrimSpacesOnLeftAndRight(remainder);
            if (remainder.size() != 0)
            {
                std::string sendText = std::format("$BoxChatTell {} {}\n", arg1, remainder);

                SendText(sendText);
            }
        }

        return true;
    }

    if (commandText.starts_with("//BoxChatToChannel ") == true || commandText.starts_with("//BCTC ") == true)
    {
        std::string arg0;
        std::string arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            std::string remainder = result.range_as_string();
            util::String::TrimSpacesOnLeftAndRight(remainder);
            if (remainder.size() != 0)
            {
                std::string sendText = std::format("$BoxChatToChannel {} {}\n", arg1, remainder);

                SendText(sendText);
            }
        }

        return true;
    }

    if (commandText.starts_with("//BoxChatToOthers ") == true || commandText.starts_with("//BCTO ") == true)
    {
        std::string arg0;
        auto result = scn::scan(commandText, "{}", arg0);
        if (result)
        {
            std::string remainder = result.range_as_string();
            util::String::TrimSpacesOnLeftAndRight(remainder);
            if (remainder.size() != 0)
            {
                std::string sendText = std::format("$BoxChatToOthers {}\n", remainder);

                SendText(sendText);
            }
        }

        return true;
    }

    if (commandText.starts_with("//BoxChatToAll ") == true || commandText.starts_with("//BCTA ") == true)
    {
        std::string arg0;
        auto result = scn::scan(commandText, "{}", arg0);
        if (result)
        {
            std::string remainder = result.range_as_string();
            util::String::TrimSpacesOnLeftAndRight(remainder);
            if (remainder.size() != 0)
            {
                std::string sendText = std::format("$BoxChatToAll {}\n", remainder);

                SendText(sendText);
            }
        }

        return true;
    }

    return false;
}

}

inline eqapp::BoxChatClient& g_BoxChatClient = eqapp::BoxChatClient::GetInstance();

}
