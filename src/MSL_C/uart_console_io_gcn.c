#include "uart_console_io_gcn.h"
#include <revolution/euart.h>
#include <revolution/os.h>

DSIOResult __write_console(__file_handle handle, unsigned char* buffer, size_t* count, __idle_proc idle_fn)
{
    static int initialized;

    if ((OSGetConsoleType() & 0x20000000) == 0)
    {
        int uart_init = 0;
        if (initialized == FALSE)
        {
            uart_init = InitializeUART(0xE100);

            if (uart_init == 0)
            {
                initialized = 1;
            }
        }

        if (uart_init != 0)
        {
            return 1;
        }

        if (WriteUARTN((const char*)buffer, *count) != 0)
        {
            *count = 0;
            return 1;
        }
    }

    __TRK_write_console(handle, buffer, count, idle_fn);
    return 0;
}

// __close_console: from zeldaret/tp (CC0-1.0); present in this game's DOL (0x8033B08C).
DSIOResult __close_console(__file_handle handle)
{
    return 0;
}

// Unknown function at 0x8033B094 (li r3, 0; blr), called by fn_80333B34. Its real name is
// not known yet, so it keeps its address-based name.
int fn_8033B094(void)
{
    return 0;
}
