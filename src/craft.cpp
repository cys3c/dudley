/**
 * @file craft.cpp
 * @author Brandon Miller
 * @brief craft packets from parsed dudley files
 */

#include "../include/craft.hpp"

namespace Crafter
{
    /// hexdump a line
    static void dump_line(const uint8_t *line, size_t n, size_t offset)
    {
        size_t i = 0;
        size_t plen = n % 16;

        printf("%.4zx  ", offset);
        for (i = 0; i < n; i++)
        {
            if (i > 0 && i % 4 == 0)
                printf("  ");

            printf(" %02x ", line[i]);
        }

        if (plen != 0)
        {
            while (i < 16)
            {
                if (i % 4 == 0)
                    printf("  ");

                printf("    ");
                i++;
            }
        }

        printf("  ");

        for (i = 0; i < n; i++)
        {
            if (*(line + i) < 32 ||
                *(line + i) > 126)
                printf (".");
            else
                printf("%c", line[i]);
        }

        printf("\n");
    }

    /// populate packet buffer with string; lstrip and rstrip the quotes
    std::size_t d_string(std::vector<std::string> params, uint8_t *buffer, std::size_t max)
    {
        std::size_t n;
        if (buffer == NULL || params[1].size() >= max)
            return 0;

        std::memcpy(buffer, params[1].c_str() + 1, params[1].size() - 2);
        return params[1].size() - 2;
    }

    /// populate packet buffer with repeated string and return the size of the data written
    std::size_t d_string_repeat(std::vector<std::string> params, uint8_t *buffer, std::size_t max)
    {
        std::size_t n = 0;
        int i, repcnt;

        if (buffer == NULL || params[1].size() >= max)
            return 0;

        repcnt = std::stoi(params[2]);
        for (i = 0; i < repcnt; i++)
        {
            std::memcpy(buffer + n, params[1].c_str() + 1, params[1].size() - 2);
            n += params[1].size() - 2;
        }

        return n;
    }

    /// zeroize the packet buffer
    void d_clear(uint8_t *buffer, std::size_t n)
    {
        if (buffer != NULL)
            std::memset(buffer, '\0', n);
    }

    /// print packet buffer contents for debug pruposes
    void d_hexdump(uint8_t *buffer, size_t n)
    {
        size_t idx = 0, lnb = 0;
        size_t total, offset;

        std::cout << "---------- Packet Buffer ----------" << std::endl;
        for (total = 0; total <= n; total++)
        {
            if ((lnb > 0 && lnb % 16 == 0) ||
                (idx == n))
            {
                offset = idx - lnb;
                dump_line(buffer + offset, lnb, offset);
                lnb = 0;
            }

            lnb++;
            idx++;
        }
    }
}
