#include "Day06.h"
#include <sstream>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif // WIN32

std::vector<group_vote> Day06::parseInput(std::string& input)
{
    std::vector<group_vote> parsed;
    std::stringstream stream(input);
    
    group_vote group;
    for (std::string line; std::getline(stream, line);)
    {
        if (line.empty())
        {
            parsed.push_back(group);
            group = group_vote();
        }
        else
        {
            std::array<uint8_t, 32> vote = std::array<uint8_t, 32>();
            for (size_t i = 0; i < line.size() / sizeof(char); i++)
            {
                vote[line[i] - 'a'] = 1;
            }

            group.voters += 1;
            group.votes.push_back(vote);
        }
    }

    parsed.push_back(group);
    return parsed;
}

std::string Day06::runPart1(day_t& input)
{
    std::stringstream output;
    
    const __m256i zeros = _mm256_set1_epi8(0);
    const __m256i ones = _mm256_set1_epi8(1);

    __m256i sum1 = _mm256_set1_epi16(0);
    __m256i sum2 = _mm256_set1_epi16(0);
    for (group_vote& group : input) 
    {
        __m256i acc = _mm256_set1_epi8(0);
        for (auto& vote : group.votes) {
            __m256i v = _mm256_loadu_si256((__m256i*) vote.data());
            acc = _mm256_add_epi8(v, acc);
        }

        acc = _mm256_cmpgt_epi8(acc, zeros);
        acc = _mm256_and_si256(acc, ones);
        sum1 = _mm256_add_epi16(_mm256_cvtepi8_epi16(((__m128i*) &acc)[0]), sum1);
        sum2 = _mm256_add_epi16(_mm256_cvtepi8_epi16(((__m128i*) &acc)[1]), sum2);
    }

    int result = 0;
    for (size_t i = 0; i < 16; ++i)
        result += reinterpret_cast<uint16_t*>(&sum1)[i];
    for (size_t i = 0; i < 16; ++i)
        result += reinterpret_cast<uint16_t*>(&sum2)[i];

    output << result;
    return output.str();
}

std::string Day06::runPart2(day_t& input)
{
    std::stringstream output;

    const __m256i zeros = _mm256_set1_epi8(0);
    const __m256i ones = _mm256_set1_epi8(1);

    __m256i sum1 = _mm256_set1_epi16(0);
    __m256i sum2 = _mm256_set1_epi16(0);
    for (group_vote& group : input)
    {
        __m256i acc = _mm256_set1_epi8(1);
        for (auto& vote : group.votes) {
            __m256i v = _mm256_loadu_si256((__m256i*) vote.data());
            acc = _mm256_and_si256(acc, v);
        }

        sum1 = _mm256_add_epi16(_mm256_cvtepi8_epi16(((__m128i*) & acc)[0]), sum1);
        sum2 = _mm256_add_epi16(_mm256_cvtepi8_epi16(((__m128i*) & acc)[1]), sum2);
    }

    int result = 0;
    for (size_t i = 0; i < 16; ++i)
        result += reinterpret_cast<uint16_t*>(&sum1)[i];
    for (size_t i = 0; i < 16; ++i)
        result += reinterpret_cast<uint16_t*>(&sum2)[i];

    output << result;
    return output.str();
}
