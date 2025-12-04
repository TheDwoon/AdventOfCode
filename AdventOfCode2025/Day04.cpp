#include <cstring>
#include "parser.cpp"
#include "util.cpp"
#include "vec2.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

int countAdjacentBoxes(const aoc::map<char> &m, const vec2i& pos)
{
    int count = 0;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if ((dx != 0 || dy != 0) && m.contains(pos.x + dx, pos.y + dy) && m(pos.x + dx, pos.y + dy) == '@')
            {
                count++;
            }
        }
    }

    return count;
}

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    Parser p(buffer);
    int offset;
    p.findNext("\n", offset);

    aoc::map m(buffer, offset, length / (offset + 1), offset + 1);

    int i = 0;
    std::vector<vec2i> accessibleBoxes;
    vec2i pos(0, 0);
    do
    {
        accessibleBoxes.clear();
        for (pos.y = 0; pos.y < m.height; pos.y++)
        {
            for (pos.x = 0; pos.x < m.width; pos.x++)
            {
                if (m(pos) == '@' && countAdjacentBoxes(m, pos) < 4)
                {
                    accessibleBoxes.push_back(pos);
                }
            }
        }

        if (i++ == 0)
            part1 += static_cast<int>(accessibleBoxes.size());

        part2 += static_cast<int>(accessibleBoxes.size());
        for (const auto& box : accessibleBoxes)
        {
            m.at(box) = '.';
        }
    } while (!accessibleBoxes.empty());

    printf("%d\n",part1);
    printf("%d\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
