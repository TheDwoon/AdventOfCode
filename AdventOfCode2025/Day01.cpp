#include <cstring>
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

    int dial = 50;

    Parser p(buffer);
    while (!p.eof())
    {
        int delta;
        switch (p.read())
        {
        case 'L':
            p.readNumber(delta);
            p.readNewLine();

            delta *= -1;
            break;
        case 'R':
            p.readNumber(delta);
            p.readNewLine();
            break;
        default:
            throw std::exception();
        }

        if (delta > 0)
        {
            part2 += (delta + dial) / 100;
        }
        else if (delta < 0)
        {
            part2 += dial == 0 ? delta / -100  : (delta - (100 - dial)) / -100;
        }

        dial += delta;
        while (dial < 0)
            dial += 100;
        while (dial > 99)
            dial -= 100;

        part1 += dial == 0;
    }

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
