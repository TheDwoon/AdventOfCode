#include <cstring>
#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

void runDay(char* const buffer, const int length) {
    int part1 = 0;
    int part2 = 0;

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
