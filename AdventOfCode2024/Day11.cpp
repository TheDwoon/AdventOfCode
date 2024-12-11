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

void blink(std::list<uint64_t> &stones, const int blinks) {
#ifndef NDEBUG
    // printf("Blink: 0\n");
    // printStones(stones);
#endif

    for (int blink = 1; blink <= blinks; blink++) {
        for (auto it = stones.begin(); it != stones.end(); ++it) {
            uint64_t &stone = *it;
            if (stone == 0ULL) {
                stone = 1ULL;
            } else if (const int num_digits = countDigits(stone); stone >= 10 && num_digits % 2 == 0) {
                uint64_t factor = 1ULL;
                for (int i = 0; i < num_digits / 2; i++) {
                    factor *= 10ULL;
                }

                stones.insert(it, stone / factor);
                stone = stone % factor;
            } else {
                stone *= 2024ULL;
            }
        }


#ifndef NDEBUG
        // printf("%d, %llu\n", blink, stones.size());
        // // printf("Stones: %llu\n", stones.size());
        // if (stones.size() < 50) {
        //     printStones(stones);
        // }
#endif
    }
}

unsigned int cache_hit = 0;
unsigned int cache_miss = 0;

uint64_t blinkRecursive(const std::list<uint64_t> &stones, int total_blinks, const int blink_batch, std::unordered_map<uint64_t, std::list<uint64_t>> &blink_cache) {
    assert(total_blinks % blink_batch == 0);
    total_blinks -= blink_batch;

    uint64_t total = 0;
    for (auto stone : stones) {
        if (!blink_cache.contains(stone)) {
            std::list<uint64_t> stone_sample { stone };
            blink(stone_sample, blink_batch);
            blink_cache[stone] = stone_sample;
            cache_miss++;
        } else {
            cache_hit++;
        }

        const std::list<uint64_t> &stone_sample = blink_cache[stone];
        if (total_blinks == 0) {
            total += stone_sample.size();
        } else {
            total += blinkRecursive(stone_sample, total_blinks, blink_batch, blink_cache);
        }
    }

    return total;
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

    std::list<uint64_t> stones = original_stones;
    blink(stones, 25);
    part1 = stones.size();

    std::unordered_map<uint64_t, std::list<uint64_t>> blink_cache;
    part2 = blinkRecursive(original_stones, 75, 25, blink_cache);

    printf("Cache: %u | %u | %.2f%%\n", cache_hit, cache_miss, (double)cache_hit/(double)(cache_hit + cache_miss) * 100.0);

    // 199752 -> too low
    printf("%llu\n", part1);
    printf("%llu\n",part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
