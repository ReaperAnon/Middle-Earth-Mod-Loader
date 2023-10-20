#include "framework.h"

class ModLoaderLog
{
    HANDLE                              logHandle;
    ImGuiTextFilter                     textFilter;
    std::string                         textBuffer;
    std::vector<size_t>                 entryBorders;
    float                               textScale;

public:
    static inline std::mutex logMutex;

    static ModLoaderLog* GetSingleton()
    {
        static ModLoaderLog singleton;
        return std::addressof(singleton);
    }

    ModLoaderLog();

    ~ModLoaderLog();

    void Clear();

    void Log(std::string_view source, std::string_view message);

    void Show(const char* title, bool* showMenu = NULL);
};