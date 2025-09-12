#include <cmath>
#include <cstdint>
#include <cstring>
#include <list>
#include <unordered_map>

#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;

#ifndef NDEBUG
void printStones(const std::list<uint64_t> &stones) {
    for (auto stone : stones) {
        printf("%llu ", stone);
    }
    printf("\n");
}
#endif

int countDigits(uint64_t x) {
    int digits = 0;
    for (; x > 0; x /= 10, ++digits);
    return digits;

    // return static_cast<int>(std::ceil(std::log10(x)));
}

typedef std::tuple<uint64_t, int> cache_key;
struct key_hash
{
    std::size_t operator()(const cache_key& k) const
    {
        return std::get<0>(k) ^ std::get<1>(k);
    }
};

uint64_t blink_cached(std::unordered_map<const cache_key, uint64_t, key_hash> &cache, const uint64_t stone, const int blinks, const int blink = 1) {
    const cache_key key = std::make_tuple(stone, blink);
    if (blink > blinks) {
        return 1ULL;
    }

    if (cache.contains(key)) {
        return cache[key];
    }

    if (stone == 0ULL) {
        const uint64_t score = blink_cached(cache, 1ULL, blinks, blink + 1);
        cache[key] = score;
        return score;
    } else if (const int num_digits = countDigits(stone); stone >= 10 && num_digits % 2 == 0) {
        uint64_t factor = 1ULL;
        for (int i = 0; i < num_digits / 2; i++) {
            factor *= 10ULL;
        }

        const uint64_t first_next_stone = stone / factor;
        const uint64_t second_next_stone = stone % factor;

        const uint64_t score = blink_cached(cache, first_next_stone, blinks, blink + 1)
            + blink_cached(cache, second_next_stone, blinks, blink + 1);
        cache[key] = score;
        return score;
    } else {
        const uint64_t next_stone = stone * 2024ULL;
        const uint64_t score = blink_cached(cache, next_stone, blinks, blink + 1);

        cache[key] = score;
        return score;
    }
}

void runDay(const char* const buffer, const int length) {
    uint64_t part1 = 0;
    uint64_t part2 = 0;

    std::list<uint64_t> original_stones;
    Parser p(buffer);
    while (p.isNumeric()) {
        int stone;
        p.readNumber(stone);
        original_stones.push_back(stone);
        p.consume(' ');
    }

    std::unordered_map<const cache_key, uint64_t, key_hash> cache;
    for (const auto stone : original_stones) {
        part1 += blink_cached(cache, stone, 75, 51);
        part2 += blink_cached(cache, stone, 75, 1);
    }

    printf("%llu\n", part1);
    printf("%llu\n", part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
