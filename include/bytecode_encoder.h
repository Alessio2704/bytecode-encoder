#pragma once
#include <cassert>
#include <initializer_list>
#include <numeric>
#include <vector>

namespace be {
    template<uint64_t width = 64,
        class = std::enable_if_t<(width > 0 && width <= 64)> >
    class BytecodeEncoder {
    private:
        std::vector<uint64_t> m_sizes = {};

    public:
        BytecodeEncoder() = default;

        BytecodeEncoder(std::initializer_list<uint64_t> init_list) {
            const uint64_t total_size = std::accumulate(init_list.begin(), init_list.end(), static_cast<uint64_t>(0));
            assert(total_size <= width && "Total size of fields exceeds the specified width");

            std::ranges::copy(init_list, std::back_inserter(m_sizes));
        };

        static uint64_t get_mask(const uint64_t free_width, const uint64_t size) {
            if (size == 0) {
                return 0;
            }

            if (size == 64) {
                return ~0ULL;
            }

            const uint64_t on_bits = (1ULL << size) - 1;
            const uint64_t result = on_bits << (free_width - size);
            return result;
        }

        [[nodiscard]] uint64_t pack(const std::vector<uint64_t> &values) const {
            assert(
                values.size() == m_sizes.size() &&
                "BytecodeEncoder::pack: Input vector size must match field configuration.");

            uint64_t data = 0;
            uint64_t used_bits = 0;

            for (size_t i = 0; i < m_sizes.size(); i++) {
                if (values[i] > ~0ULL >> (64 - m_sizes[i])) {
                    throw std::runtime_error(
                        "BytecodeEncoder::pack: A value is too large for its specified field width.");
                }

                const uint64_t shift = width - m_sizes[i] - used_bits;
                const uint64_t shifted = values[i] << shift;
                data |= shifted;
                used_bits += m_sizes[i];
            }

            return data;
        }

        [[nodiscard]] std::vector<uint64_t> unpack(std::uint64_t packed) const {
            uint64_t free_width = width;
            std::vector<uint64_t> result;
            result.reserve(m_sizes.size());

            for (const auto size: m_sizes) {
                const uint64_t shift = free_width - size;
                const std::uint64_t val = (packed & get_mask(free_width, size)) >> shift;
                result.push_back(val);
                free_width -= size;
            }

            return result;
        }
    };
};
