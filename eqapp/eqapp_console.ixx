module;

#include "common.h"

export module eqapp_console;

export import eq;
export import eq_functions;

export
{

namespace eqapp
{

class Console
{

public:

    Console();
    ~Console();

    static Console& getInstance()
    {
        static Console instance;
        return instance;
    }

private:

    Console(const Console&) = delete;
    Console(Console&&) = delete;
    Console& operator=(const Console&) = delete;
    Console& operator=(Console&&) = delete;

public:

    void Load();
    void Unload();
    void Print();
    bool IsLoaded();

private:

    const std::string m_name = "Console";

    bool m_isLoaded = false;

    std::size_t m_bufferSize = 1024;

    std::stringstream m_stringStream;
    std::streambuf* m_streamBufferPointer = nullptr;

};

Console::Console()
{
    //
}

Console::~Console()
{
    //
}

void Console::Load()
{
    // line buffering
    setvbuf(stdout, 0, _IOLBF, m_bufferSize);

    // redirect cout to stringstream
    m_streamBufferPointer = std::cout.rdbuf(m_stringStream.rdbuf());

    m_isLoaded = true;
}

void Console::Unload()
{
    // restore cout
    std::cout.rdbuf(m_streamBufferPointer);

    m_isLoaded = false;
}

void Console::Print()
{
    for (std::string text; std::getline(m_stringStream, text, '\n'); )
    {
        if (text.size() == 0)
        {
            continue;
        }

        if (text[0] == '\n')
        {
            continue;
        }

        std::string printText = std::format("[{}] {}", m_name, text);

        EQ_PrintTextToChatByColor(printText.c_str(), eq::Constants::ChatTextColor::Yellow);
    }

    m_stringStream.str(std::string());
    m_stringStream.clear();
}

bool Console::IsLoaded()
{
    return m_isLoaded;
}

}


inline eqapp::Console& g_Console = eqapp::Console::getInstance();

}
