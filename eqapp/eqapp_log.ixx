module;

#include "common.h"

export module eqapp_log;

export
{

namespace eqapp
{

class Log
{

public:

    Log();
    ~Log();

    static Log& getInstance()
    {
        static Log instance;
        return instance;
    }

private:

    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;

public:

    struct Properties_t
    {
        bool IsEnabled = true;

        bool PrintToConsole = false;
        bool WriteToFile = true;
    };

    typedef struct _FormatString
    {
        fmt::string_view str;
        std::source_location loc;

        _FormatString
        (
            const char* str,
            const std::source_location& loc = std::source_location::current()
        ) : str(str), loc(loc) {}
    } FormatString, *FormatString_ptr;

    void vwrite(const FormatString& format, fmt::format_args args);

    template <typename... Args>
    void write(const FormatString& format, Args&&... args)
    {
        if (m_properties.IsEnabled == false)
        {
            return;
        }

        vwrite(format, fmt::make_format_args(args...));
    }

    Properties_t* getProperties();

    void open();
    void close();
    void deleteFileContents();
    bool createFolder();

    bool isEnabled();
    void setEnabled(bool b);

    std::string getText();

private:

    Properties_t m_properties;

    const std::string m_folderName = "eqapp";

    const std::string m_fileName = "eqapp/log.txt";

    std::ofstream m_file;

    std::string m_text;
    std::size_t m_textReserveSize = 4096 * 1000;

    std::string m_sourceText;
    std::size_t m_sourceTextReserveSize = 128;

    std::string m_logText;
    std::size_t m_logTextReserveSize = 256;

};

Log::Log()
{
    //
}

Log::~Log()
{
    //
}

void Log::vwrite(const FormatString& format, fmt::format_args args)
{
    const auto& loc = format.loc;

    std::string_view fileName = loc.file_name();

    // trim the path down to just the file name and extension
    if (fileName.contains("\\") == true)
    {
        fileName = fileName.substr(fileName.rfind("\\") + 1);
    }

    m_sourceText = fmt::format(FMT_COMPILE("[{}:{}:{}()] "), fileName, loc.line(), loc.function_name());

    std::stringstream ss;
    ss << m_sourceText;
    fmt::vprint(ss, format.str, args);

    m_logText = ss.str();

    // append to internal string
    m_text.append(m_logText);

    // print to console
    if (m_properties.PrintToConsole == true)
    {
        std::cout << m_logText;
    }

    // write to file
    if (m_properties.WriteToFile == true)
    {
        m_file << m_logText;
        m_file.flush();
    }
}

Log::Properties_t* Log::getProperties()
{
    return &m_properties;
}

void Log::open()
{
    createFolder();

    m_text.clear();
    m_text.reserve(m_textReserveSize);

    m_sourceText.clear();
    m_sourceText.reserve(m_sourceTextReserveSize);

    m_logText.clear();
    m_logText.reserve(m_logTextReserveSize);

    m_file.open(m_fileName, std::ofstream::out | std::ofstream::app);
}

void Log::close()
{
    m_text.clear();
    m_sourceText.clear();
    m_logText.clear();

    m_file.flush();
    m_file.close();
}

void Log::deleteFileContents()
{
    createFolder();

    m_file.open(m_fileName, std::ofstream::out | std::ofstream::trunc);
    m_file.close();
}

bool Log::createFolder()
{
    namespace fs = std::filesystem;

    if (fs::is_directory(m_folderName) == false || fs::exists(m_folderName) == false)
    {
        return fs::create_directory(m_folderName);
    }

    return false;
}

bool Log::isEnabled()
{
    return m_properties.IsEnabled;
}

void Log::setEnabled(bool b)
{
    m_properties.IsEnabled = b;
}

std::string Log::getText()
{
    return m_text;
}

}


inline eqapp::Log& g_Log = eqapp::Log::getInstance();

}
