module;

#include "common.h"
#include "utility.h"

export module eqapp_boxchatserver;

export
{

namespace eqapp
{

class BoxChatServer
{

public:

    BoxChatServer();
    ~BoxChatServer();

    static BoxChatServer& GetInstance()
    {
        static BoxChatServer instance;
        return instance;
    }

private:

    BoxChatServer(const BoxChatServer&) = delete;
    BoxChatServer(BoxChatServer&&) = delete;
    BoxChatServer& operator=(const BoxChatServer&) = delete;
    BoxChatServer& operator=(BoxChatServer&&) = delete;

public:

    bool Load();
    bool Unload();
    bool Loop();

private:

    const std::string m_name = "EQ Box Chat Server";

    const char* m_keepAliveText = "$KeepAlive\n";

    uint16_t m_portNumber = 29015;

    struct sockaddr_in m_server;
    struct sockaddr_in m_serverAddress;

    unsigned long long m_serverAddressLength = sizeof(struct sockaddr_in);

    WSADATA m_wsaData;

    const size_t m_numClients = 256;
    std::array<SOCKET, 256> m_clientSocketList;
    std::array<std::string, 256> m_clientNameList;
    std::array<std::string, 256> m_clientGlobalChannelNameList;
    std::array<std::string, 256> m_clientChannelNameList;

    SOCKET m_listenSocket = INVALID_SOCKET;
    SOCKET m_acceptSocket = INVALID_SOCKET;
    SOCKET m_recvSocket = INVALID_SOCKET;
    SOCKET m_sendSocket = INVALID_SOCKET;

    fd_set m_ReadFdSet;

    const uint32_t m_recvBufferSize = 4096;
    char m_recvBuffer[4096] = {0};

};

BoxChatServer::BoxChatServer()
{
    //
}

BoxChatServer::~BoxChatServer()
{
    //
}

bool BoxChatServer::Load()
{
    std::print("{}\n", m_name);

    std::print("Build: {} {}\n", __DATE__, __TIME__);

    int resultOfWSAStartup = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
    if (resultOfWSAStartup != 0)
    {
        std::print("ERROR: WSAStartup() failed with error code: {}\n", resultOfWSAStartup);
        return false;
    }

    m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenSocket == INVALID_SOCKET)
    {
        std::print("ERROR: listen socket() failed with error code: {}\n", WSAGetLastError());
        closesocket(m_listenSocket);
        return false;
    }

    m_server.sin_family      = AF_INET;
    m_server.sin_addr.s_addr = INADDR_ANY;
    m_server.sin_port        = htons(m_portNumber);

    if (bind(m_listenSocket, (struct sockaddr*)&m_server, sizeof(m_server)) == SOCKET_ERROR)
    {
        std::print("ERROR: bind() failed with error code: {}\n", WSAGetLastError());
        closesocket(m_listenSocket);
        return false;
    }

    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::print("ERROR: listen() failed with error code: {}\n", WSAGetLastError());
        closesocket(m_listenSocket);
        return false;
    }

    for (size_t i = 0 ; i < m_numClients; i++)
    {
        m_clientSocketList[i] = INVALID_SOCKET;
        m_clientNameList[i] = "UNKNOWN";
        m_clientGlobalChannelNameList[i] = "Global1";
        m_clientChannelNameList[i] = "Default";
    }

    std::print("Loaded\n");

    return true;
}

bool BoxChatServer::Unload()
{
    closesocket(m_listenSocket);
    closesocket(m_acceptSocket);
    closesocket(m_recvSocket);
    closesocket(m_sendSocket);

    for (size_t i = 0 ; i < m_numClients; i++)
    {
        closesocket(m_clientSocketList[i]);
    }
 
    int resultOfWSACleanUp = WSACleanup();
    if (resultOfWSACleanUp != 0)
    {
        std::print("ERROR: WSACleanup() failed with error code: {}\n", WSAGetLastError());
        return false;
    }

    std::print("Unloaded\n");

    return true;
}

bool BoxChatServer::Loop()
{
    FD_ZERO(&m_ReadFdSet);
    FD_SET(m_listenSocket, &m_ReadFdSet);

    for (size_t i = 0 ; i < m_numClients; i++)
    {
        m_recvSocket = m_clientSocketList[i];

        if (m_recvSocket != INVALID_SOCKET)
        {
            FD_SET(m_recvSocket , &m_ReadFdSet);
        }
    }

    int selectResult = select(0, &m_ReadFdSet, NULL, NULL, NULL);
    if (selectResult == SOCKET_ERROR)
    {
        std::print("ERROR: select() failed with error code: {}\n", WSAGetLastError());
        return false;
    }

    if (FD_ISSET(m_listenSocket, &m_ReadFdSet))
    {
        m_acceptSocket = accept(m_listenSocket, (struct sockaddr*)&m_serverAddress, (int*)&m_serverAddressLength);
        if (m_acceptSocket == INVALID_SOCKET)
        {
            std::print("ERROR: accept() failed with error code: {}\n", WSAGetLastError());
            return false;
        }

        char ipAddressAsString[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(m_serverAddress.sin_addr), ipAddressAsString, INET_ADDRSTRLEN);

        std::print("Client connected. Socket: '{}' IP Address: '{}' Port Number: '{}'\n", m_acceptSocket, ipAddressAsString, ntohs(m_serverAddress.sin_port));

        for (size_t i = 0; i < m_numClients; i++)
        {
            if (m_clientSocketList[i] == INVALID_SOCKET)
            {
                m_clientSocketList[i] = m_acceptSocket;
                std::print("Client assigned to socket index: {}\n", i);
                break;
            }
        }
    }

    for (size_t i = 0; i < m_numClients; i++)
    {
        m_recvSocket = m_clientSocketList[i];

        if (FD_ISSET(m_recvSocket, &m_ReadFdSet))
        {
            getpeername(m_recvSocket, (struct sockaddr*)&m_serverAddress, (int*)&m_serverAddressLength);

            if (send(m_recvSocket, m_keepAliveText, (int)std::strlen(m_keepAliveText), 0) == SOCKET_ERROR)
            {
                std::print("Client disconnected because keep alive failed. Socket: '{}' Name: '{}'\n", m_clientSocketList[i], m_clientNameList[i]);

                closesocket(m_recvSocket);

                m_clientSocketList[i] = INVALID_SOCKET;
                m_clientNameList[i] = "UNKNOWN";
                m_clientGlobalChannelNameList[i] = "Global1";
                m_clientChannelNameList[i] = "Default";
            }

            std::memset(m_recvBuffer, 0, m_recvBufferSize);

            int recvResult = recv(m_recvSocket, m_recvBuffer, m_recvBufferSize, 0);

            int errorCode = WSAGetLastError();
            if (errorCode == WSAECONNRESET)
            {
                std::print
                (
                    "Client disconnected unexpectedly. Socket: '{}' Name: '{}' Global Channel: '{}' Channel: '{}'\n",
                    m_clientSocketList[i],
                    m_clientNameList[i],
                    m_clientGlobalChannelNameList[i],
                    m_clientChannelNameList[i]
                );

                closesocket(m_recvSocket);

                m_clientSocketList[i] = INVALID_SOCKET;
                m_clientNameList[i] = "UNKNOWN";
                m_clientGlobalChannelNameList[i] = "Global1";
                m_clientChannelNameList[i] = "Default";
            }

            if (recvResult == SOCKET_ERROR)
            {
                std::print
                (
                    "Client disconnected. Socket: '{}' Name: '{}'\n",
                    m_clientSocketList[i],
                    m_clientNameList[i]
                );

                closesocket(m_recvSocket);

                m_clientSocketList[i] = INVALID_SOCKET;
                m_clientNameList[i] = "UNKNOWN";
                m_clientGlobalChannelNameList[i] = "Global1";
                m_clientChannelNameList[i] = "Default";
            }
            else
            {
                ////std::print("recvResult: {}\n", recvResult);

                m_recvBuffer[m_recvBufferSize - 1] = '\0'; // append null terminator

                std::string recvText = m_recvBuffer;
                if (recvText.empty() == true)
                {
                    continue;
                }

                ////std::print("recvText: {}\n", recvText);

                std::vector<std::string> recvTextList = util::String::Split(recvText, '\n');

                for (auto& recvTextToken : recvTextList)
                {
                    if (recvTextToken != "$KeepAlive")
                    {
                        std::print("#{} [{}] ({}): {}\n", m_clientNameList[i], m_clientGlobalChannelNameList[i], m_clientChannelNameList[i], recvTextToken);
                    }

                    std::vector<std::string> wordList = util::String::Split(recvTextToken, ' ');

                    if (wordList.size() > 1)
                    {
                        if (wordList.at(0) == "$ClientName")
                        {
                            std::string clientName = wordList.at(1);

                            if (clientName.size() != 0)
                            {
                                for (size_t k = 0; k < m_numClients; k++)
                                {
                                    if (m_clientSocketList[k] == m_recvSocket)
                                    {
                                        m_clientNameList[k] = clientName;

                                        std::print("Client socket index '{}' assigned client name '{}'\n", m_clientSocketList[k], clientName);

                                        break;
                                    }
                                }
                            }
                        }

                        if (wordList.at(0) == "$ClientGlobalChannelName")
                        {
                            std::string clientGlobalChannelName = wordList.at(1);

                            if (clientGlobalChannelName.size() != 0)
                            {
                                for (size_t k = 0; k < m_numClients; k++)
                                {
                                    if (m_clientSocketList[k] == m_recvSocket)
                                    {
                                        m_clientGlobalChannelNameList[k] = clientGlobalChannelName;

                                        std::print("Client socket index '{}' assigned to client global channel name '{}'\n", m_clientSocketList[k], clientGlobalChannelName);

                                        break;
                                    }
                                }
                            }
                        }

                        if (wordList.at(0) == "$ClientChannelName")
                        {
                            std::string clientChannelName = wordList.at(1);

                            if (clientChannelName.size() != 0)
                            {
                                for (size_t k = 0; k < m_numClients; k++)
                                {
                                    if (m_clientSocketList[k] == m_recvSocket)
                                    {
                                        m_clientChannelNameList[k] = clientChannelName;

                                        std::print("Client socket index '{}' assigned to client channel name '{}'\n", m_clientSocketList[k], clientChannelName);

                                        break;
                                    }
                                }
                            }
                        }

                        bool bBCT  = false;
                        bool bBCTC = false;
                        bool bBCTO  = false;
                        bool bBCTA = false;

                        bool bBroadcast = false;

                        if (wordList.at(0) == "$BoxChatTell" || wordList.at(0) == "$BCT")
                        {
                            bBCT  = true;
                            bBCTC = false;
                            bBCTO  = false;
                            bBCTA = false;

                            bBroadcast = true;
                        }

                        if (wordList.at(0) == "$BoxChatToChannel" || wordList.at(0) == "$BCTC")
                        {
                            bBCT  = false;
                            bBCTC = true;
                            bBCTO  = false;
                            bBCTA = false;

                            bBroadcast = true;
                        }
 
                        if (wordList.at(0) == "$BoxChatToOthers" || wordList.at(0) == "$BCTO")
                        {
                            bBCT  = false;
                            bBCTC = false;
                            bBCTO  = true;
                            bBCTA = false;

                            bBroadcast = true;
                        }

                        if (wordList.at(0) == "$BoxChatToAll" || wordList.at(0) == "$BCTA")
                        {
                            bBCT  = false;
                            bBCTC = false;
                            bBCTO  = false;
                            bBCTA = true;

                            bBroadcast = true;
                        }

                        if (bBroadcast == true)
                        {
                            int sendIndex = -1;
                            int recvIndex = -1;

                            for (size_t l = 0; l < m_numClients; l++)
                            {
                                if (m_clientSocketList[l] == INVALID_SOCKET)
                                {
                                    continue;
                                }

                                if (m_clientSocketList[l] == m_sendSocket)
                                {
                                    sendIndex = (int)l;
                                }

                                if (m_clientSocketList[l] == m_recvSocket)
                                {
                                    recvIndex = (int)l;
                                }
                            }

                            // skip clients that do not have the same global channel as the sender
                            if (sendIndex != -1 && recvIndex != -1)
                            {
                                if (m_clientGlobalChannelNameList[sendIndex] != m_clientGlobalChannelNameList[recvIndex])
                                {
                                    continue;
                                }
                            }

                            for (size_t j = 0; j < m_numClients; j++)
                            {
                                if (m_clientSocketList[j] == 0)
                                {
                                    continue;
                                }

                                // skip clients with empty names or UNKNOWN
                                if (bBCT == true || bBCTC == true)
                                {
                                    if (m_clientNameList[j].size() == 0)
                                    {
                                        continue;
                                    }

                                    if (m_clientNameList[j] == "UNKNOWN")
                                    {
                                        continue;
                                    }
                                }

                                m_sendSocket = m_clientSocketList[j];

                                // do not send broadcast to the sender
                                if (m_sendSocket == m_recvSocket)
                                {
                                    if (bBCT == true || bBCTC == true || bBCTO == true)
                                    {
                                        continue;
                                    }
                                }

                                auto wordListFirst = wordList.begin();
                                auto wordListLast  = wordList.end();

                                if (bBCT == true)
                                {
                                    std::string sendToClientName = wordList.at(1);
                                    if (sendToClientName.size() == 0)
                                    {
                                        continue;
                                    }

                                    // skip clients that do not match the BCT tell name
                                    if (m_clientNameList[j] != sendToClientName)
                                    {
                                        continue;
                                    }

                                    if (wordList.size() > 2)
                                    {
                                        wordListFirst = wordList.begin() + 2;
                                        wordListLast  = wordList.end();
                                    }
                                }
                                else if (bBCTC == true)
                                {
                                    std::string sendToChannelName = wordList.at(1);
                                    if (sendToChannelName.size() == 0)
                                    {
                                        continue;
                                    }

                                    // skip clients that do not match the BCTC tell name
                                    if (m_clientChannelNameList[j] != sendToChannelName)
                                    {
                                        continue;
                                    }

                                    if (wordList.size() > 2)
                                    {
                                        wordListFirst = wordList.begin() + 2;
                                        wordListLast  = wordList.end();
                                    }
                                }
                                else
                                {
                                    if (wordList.size() > 1)
                                    {
                                        wordListFirst = wordList.begin() + 1;
                                        wordListLast  = wordList.end();
                                    }
                                }

                                std::vector<std::string> wordListEx(wordListFirst, wordListLast);
                                if (wordListEx.empty() == true)
                                {
                                    continue;
                                }

                                std::string wordListExAsString = util::String::Join(wordListEx, " ");
                                if (wordListExAsString.size() == 0)
                                {
                                    continue;
                                }

                                std::vector<std::string> commandTokens = util::String::Split(wordListExAsString, ';');
                                if (commandTokens.empty() == true)
                                {
                                    continue;
                                }

                                for (auto& commandToken : commandTokens)
                                {
                                    if (commandToken.size() == 0)
                                    {
                                        continue;
                                    }

                                    std::string commandText = std::format("$InterpretCommand {}\n", commandToken);
                                    if (commandText.size() == 0)
                                    {
                                        continue;
                                    }

                                    ////std::print("commandText: {}\m", commandText);

                                    send(m_sendSocket, commandText.c_str(), (int)commandText.size(), 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

}

inline eqapp::BoxChatServer& g_BoxChatServer = eqapp::BoxChatServer::GetInstance();

}
