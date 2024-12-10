#ifndef HEADER_HASH_MAP
#define HEADER_HASH_MAP
#include "buffer.cpp"

namespace std {
    template<class T>
      struct hash;
}

namespace aoc {
    template<typename TKey, typename TValue, unsigned int BUCKETS = 32, unsigned int BufferSize = 16>
    class hash_map {
        struct node {
          TKey key;
          TValue value;
        };

        struct iterator {
            ring_buffer<node, BufferSize> * const buckets_pointer;
            unsigned int current_bucket = 0;
            unsigned int current_offset = 0;

            node& operator*() {
                const auto& bucket = buckets_pointer[current_bucket];
                return bucket[current_offset];
            }

            node* operator->() {
                auto& bucket = buckets_pointer[current_bucket];
                return &bucket[current_offset];
            }

            iterator& operator++() {
                const auto& bucket = buckets_pointer[current_bucket];

                current_offset += 1;
                if (current_offset >= bucket.available()) {
                    current_offset = 0;
                    current_bucket += 1;

                    while (current_bucket < BUCKETS && buckets_pointer[current_bucket].is_empty())
                        current_bucket += 1;
                }

                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const iterator& lhs, const iterator& rhs) {
                return lhs.current_bucket == rhs.current_bucket && lhs.current_offset == rhs.current_offset;
            }

            friend bool operator!=(const iterator& lhs, const iterator& rhs) {
                return lhs.current_bucket != rhs.current_bucket || lhs.current_offset != rhs.current_offset;
            }
        };

        std::hash<TKey> hash_function = std::hash<TKey>();
        ring_buffer<node, BufferSize> buckets[BUCKETS];
    public:
        TValue& operator[](const TKey &key) {
            ring_buffer<node, BufferSize>& bucket = buckets[hash_function(key) % BUCKETS];
            for (int i = 0; i < bucket.available(); i++) {
                if (bucket[i].key == key) {
                    return bucket[i].value;
                }
            }

            bucket.emplace_back({});
            return bucket[bucket.available() - 1].value;
        }

        constexpr iterator begin() {
            int first_non_empty_bucket = 0;
            while (first_non_empty_bucket < BUCKETS && buckets[first_non_empty_bucket].is_empty())
                first_non_empty_bucket += 1;

            return {buckets, first_non_empty_bucket, 0};
        }

        constexpr iterator end() {
            return {buckets, BUCKETS, 0};
        }
    };
}
#endif