#include "common.h"
#include "utility.h"

import eqapp_boxchatserver;

int main(int argc , char *argv[])
{
    g_BoxChatServer.Load();

    while (g_BoxChatServer.Loop() == true)
    {
        if (GetAsyncKeyState(VK_END))
        {
            break;
        }
    }

    g_BoxChatServer.Unload();

    return 0;
}
