#include <cstring>
#include <cstdint>
#include <vector>
#include <set>
#include <array>
#include <algorithm>
#include <bits/ranges_algo.h>

#include "parser.cpp"

constexpr long INPUT_BUFFER_SIZE = 32 * 1024;
constexpr int MAX_BLOCK_LENGTH = 10;

struct block {
    int file;
    int width;

    [[nodiscard]]
    bool is_used() const noexcept {
        return file >= 0;
    }

    [[nodiscard]]
    bool is_free() const noexcept {
        return file < 0;
    }

    [[nodiscard]]
    int64_t checksum(const int64_t idx) const noexcept {
        const int64_t file_id = file;
        const int64_t file_width = width;
        return file_id * (file_width * idx + (file_width * (file_width - 1) / 2));
    }
};

struct indexed_block : block {
    int index;
};

block findLastUsedBlock(const std::vector<block> &disk, int &idx, const int lowerLimit = -1) noexcept {
    while (idx > lowerLimit && disk[idx].is_free()) {
        idx--;
    }

    return disk[idx];
}

void runDay(char* const buffer, const int length) {
    std::vector<block> original_disk;

    int file_id = 0;
    for (int i = 0; i < length - 1; i++) {
        const int block_width = buffer[i] - '0';
        if (block_width > 0) {
            if (i % 2 == 0) {
                original_disk.emplace_back(file_id++, block_width);
            } else {
                original_disk.emplace_back(-1, block_width);
            }
        }
    }

    const int original_disk_size = static_cast<int>(original_disk.size());

    std::vector<block> fragmented_compact_disk;
    int current_block_idx = 0;
    int last_used_block_idx = original_disk_size - 1;

    block current_block = original_disk[current_block_idx];
    block last_used_block = findLastUsedBlock(original_disk, last_used_block_idx);
    while (current_block_idx < last_used_block_idx) {
        if (current_block.is_used()) {
            // block is already compact; add to the new disk
            fragmented_compact_disk.emplace_back(current_block);

            // get the next block to work on
            current_block = original_disk[++current_block_idx];
        } else {
            if (current_block.width == last_used_block.width) {
                // perfect fit; just move the block
                fragmented_compact_disk.emplace_back(last_used_block);

                // get a new block to work and move last used block
                current_block = original_disk[++current_block_idx];
                --last_used_block_idx;
                last_used_block = findLastUsedBlock(original_disk, last_used_block_idx, current_block_idx);
            } else if (current_block.width > last_used_block.width) {
                // Shrink current free block and move file to compacted disk
                current_block.width -= last_used_block.width;
                fragmented_compact_disk.emplace_back(last_used_block);

                // get next lastFreeBlock
                last_used_block_idx--;
                last_used_block = findLastUsedBlock(original_disk, last_used_block_idx, current_block_idx);
            } else {
                // set current block to represent the file and adjust the last blocks width accordingly
                current_block.file = last_used_block.file;
                last_used_block.width -= current_block.width;

                // move modified current block to compacted disk and get a new one
                fragmented_compact_disk.emplace_back(current_block);
                current_block = original_disk[++current_block_idx];
            }
        }
    }

    if (last_used_block.is_used()) {
        fragmented_compact_disk.emplace_back(last_used_block);
    }

    // build free list
    std::array<std::multiset<int>, MAX_BLOCK_LENGTH> free_lists;
    int index = 0;
    for (const auto& block : original_disk) {
        if (block.is_free()) {
            free_lists[block.width].emplace(index);
        }

        index += block.width;
    }
    const int original_disk_length = index;

    index = original_disk_length;
    std::vector<indexed_block> file_optimized_disk;
    for (int i = original_disk_size - 1; i >= 0; i--) {
        const auto& block = original_disk[i];
        index -= block.width;

        if (block.is_free())
            continue;

        assert (block.width < MAX_BLOCK_LENGTH);
        int slot_width_used = 0;
        int slot_index_used = original_disk_length;
        for (int width = block.width; width < MAX_BLOCK_LENGTH; width++) {
            const std::multiset<int> &free_list = free_lists[width];
            if (!free_list.empty()) {
                const int slot_index = *free_list.begin();
                if (slot_index < slot_index_used) {
                    slot_width_used = width;
                    slot_index_used = slot_index;
                }
            }
        }

        if (slot_width_used == 0 || slot_index_used > index) {
            // no space; just keep it at them same place
            file_optimized_disk.push_back({ block.file, block.width, index });
            free_lists[block.width].emplace(index);
        } else {
            std::multiset<int> &free_list = free_lists[slot_width_used];
            free_list.erase(free_list.begin());

            file_optimized_disk.push_back({ block.file, block.width, slot_index_used });

            if (slot_width_used > block.width) {
                const int remaining_slot_width = slot_width_used - block.width;
                free_lists[remaining_slot_width].emplace(slot_index_used + block.width);
            }
        }

    }

    // Compute results
    int64_t part1 = 0;
    int64_t part2 = 0;
    index = 0;
    for (const auto& block : fragmented_compact_disk) {
        part1 += block.checksum(index);
        index += block.width;
    }

    for (const auto& block : file_optimized_disk) {
        part2 += block.checksum(block.index);
    }

    printf("%lld\n", part1);
    printf("%lld\n", part2);
}

// BOILER PLATE CODE BELOW

int main()
{
    char buffer[INPUT_BUFFER_SIZE] {};
    const int length = static_cast<int>(fread(buffer, 1, INPUT_BUFFER_SIZE, stdin));
	runDay(buffer, length);
}
