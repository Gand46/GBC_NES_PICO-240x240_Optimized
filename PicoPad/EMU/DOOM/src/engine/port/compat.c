#include <stddef.h>
#include <string.h>

static inline char doom_port_tolower(char c)
{
        if (c >= 'A' && c <= 'Z')
        {
                c = (char)(c + ('a' - 'A'));
        }
        return c;
}

int strnicmp(const char *a, const char *b, size_t len)
{
        int diff = 0;
        for (size_t i = 0; (i < len) && a[i]; ++i)
        {
                diff = (int)doom_port_tolower(a[i]) - (int)doom_port_tolower(b[i]);
                if (diff)
                {
                        break;
                }
        }
        return diff;
}

int stricmp(const char *a, const char *b)
{
        return strnicmp(a, b, strlen(a));
}
