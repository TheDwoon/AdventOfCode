#ifndef HEADER_BUFFER
#define HEADER_BUFFER
#include <algorithm>
#include <cassert>

namespace aoc {

    template<typename T, unsigned int BUFFER_SIZE = 16>
    class ring_buffer {
        T buffer[BUFFER_SIZE];

        unsigned int readPointer { 0 };
        unsigned int writePointer { 0 };

    public:
        [[nodiscard]]
        static inline int capacity() noexcept {
            return BUFFER_SIZE;
        }

        [[nodiscard]]
        bool is_empty() const noexcept {
            return readPointer == writePointer;
        }

        [[nodiscard]]
        unsigned int available() const noexcept {
            return writePointer - readPointer;
        }

        T& operator[](const int index) noexcept {
            assert ((index + readPointer) < writePointer);
            return buffer[(index + readPointer) % BUFFER_SIZE];
        }

        T&& pop_front() noexcept {
            return std::move(buffer[readPointer++ % BUFFER_SIZE]);
        }

        void push_back(T element) noexcept {
            buffer[writePointer++ % BUFFER_SIZE] = std::move(element);
        }

        void emplace_back(T&& element) noexcept {
            buffer[writePointer++ % BUFFER_SIZE] = element;
        }
    };
}

#endif