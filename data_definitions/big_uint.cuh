#pragma once

#include <cstdint>



namespace data_definitions {

    template<std::size_t NumLimbs>
    struct alignas(8) big_uint {
        static_assert(NumLimbs > 1);

        uint64_t limbs[NumLimbs]{};  // Little-endian: limbs[0] is least significant

        // Default constructor (POD-compatible)
        __host__ __device__ __forceinline__
        big_uint() = default;

        // Constructor from uint64_t
        __host__ __device__ __forceinline__
        explicit big_uint(uint64_t value) {
            limbs[0] = value;
            for (auto i = 1u; i < NumLimbs; ++i)
                limbs[i] = 0;
        }

        big_uint(uint64_t l0, uint64_t l1) {
            static_assert(NumLimbs >= 2);
            limbs[0] = l0;
            limbs[1] = l1;
        }

        // Addition with carry
        __host__ __device__ __forceinline__
        static uint64_t add_carry(uint64_t a, uint64_t b, uint64_t& carry) {
            uint64_t result = a + b + carry;
#if defined(__CUDA_ARCH__)
            carry = (result < a || (carry && result == a)) ? 1 : 0;
#else
            carry = ((uint64_t(result) < a) || ((carry != 0) && result == a)) ? 1 : 0;
#endif
            return result;
        }

        // Multiplication helper
        __host__ __device__ __forceinline__
        static void mul_64x64(uint64_t a, uint64_t b, uint64_t& lo, uint64_t& hi) {
#if defined(__CUDA_ARCH__)
            lo = a * b;
            hi = __umul64hi(a, b);
#else
            __uint128_t product = static_cast<__uint128_t>(a) * static_cast<__uint128_t>(b);
            lo = static_cast<uint64_t>(product);
            hi = static_cast<uint64_t>(product >> 64);
#endif
        }

        // operator+= (uint64_t)
        __host__ __device__ __forceinline__
        big_uint& operator+=(uint64_t rhs) {
            uint64_t carry = rhs;
            for (int i = 0; i < NumLimbs; ++i) {
                limbs[i] = add_carry(limbs[i], 0ull, carry);  // add carry into limb
            }
            return *this;
        }

        // operator+ (uint64_t)
        __host__ __device__ __forceinline__
        big_uint operator+(uint64_t rhs) const {
            big_uint result = *this;
            result += rhs;
            return result;
        }

        // operator+=
        __host__ __device__ __forceinline__
        big_uint& operator+=(const big_uint& rhs) {
            uint64_t carry = 0;
            for (int i = 0; i < NumLimbs; ++i) {
                limbs[i] = add_carry(limbs[i], rhs.limbs[i], carry);
            }
            return *this;
        }

        // operator+
        __host__ __device__ __forceinline__
        big_uint operator+(const big_uint& rhs) const {
            big_uint result = *this;
            result += rhs;
            return result;
        }

        // operator*=(uint64_t)
        __host__ __device__ __forceinline__
        big_uint& operator*=(uint64_t rhs) {
            uint64_t result[NumLimbs] = {};
            uint64_t carry = 0;

            for (int i = 0; i < NumLimbs; ++i) {
                uint64_t lo, hi;
                mul_64x64(limbs[i], rhs, lo, hi);

                // Add carry from previous limb
                uint64_t new_carry = 0;
                lo = add_carry(lo, carry, new_carry);
                hi += new_carry;

                result[i] = lo;
                carry = hi;
            }

            for (auto i = 0u; i < NumLimbs; ++i)
                limbs[i] = result[i];

            return *this;
        }

        // operator*(uint64_t)
        __host__ __device__ __forceinline__
        big_uint operator*(uint64_t rhs) const {
            big_uint result = *this;
            result *= rhs;
            return result;
        }

        // operator*=
        __host__ __device__ __forceinline__
        big_uint& operator*=(const big_uint& rhs) {
            big_uint result(0);

            for (int i = 0; i < NumLimbs; ++i) {
                uint64_t carry = 0;

                for (int j = 0; j + i < NumLimbs; ++j) {
                    uint64_t lo, hi;
                    mul_64x64(limbs[i], rhs.limbs[j], lo, hi);

                    lo = add_carry(lo, result.limbs[i + j], carry);
                    hi = hi + carry;

                    result.limbs[i + j] = lo;
                    carry = hi;
                }
            }

            *this = result;
            return *this;
        }

        // operator*
        __host__ __device__ __forceinline__
        big_uint operator*(const big_uint& rhs) const {
            big_uint result = *this;
            result *= rhs;
            return result;
        }

        __host__ __device__ __forceinline__
        bool operator==(const big_uint& rhs) const {
            for (int i = 0; i < NumLimbs; ++i) {
                if (limbs[i] != rhs.limbs[i]) {
                    return false;
                }
            }
            return true;
        }
    };

    using u128_t = big_uint<2>;
    using u256_t = big_uint<4>;

    // template<std::size_t N>
    // struct big_uint_reduce {
    //
    //     __host__ __device__ __forceinline__
    //     big_uint<N> operator()(const big_uint<N>& lhs, const big_uint<N>& rhs) const {
    //         return lhs + rhs;
    //     }
    //
    //     __host__ __device__ __forceinline__
    //     big_uint<N> operator()(const big_uint<N>& lhs, const uint64_t& rhs) const {
    //         return lhs + rhs;
    //     }
    //
    //     __host__ __device__ __forceinline__
    //     big_uint<N> operator()(const uint64_t& lhs, const big_uint<N>& rhs) const {
    //         return rhs + lhs;
    //     }
    // };
    //
    // using u128_reduce_t = big_uint_reduce<2>;
    // using u256_reduce_t = big_uint_reduce<4>;

    // template<std::size_t N>
    // struct fmt::formatter<big_uint<N>> : formatter<std::string> {
    // private:
    //     static uint64_t divide_by_ten(uint64_t* limbs) {
    //         __uint128_t remainder = 0;
    //         for (ssize_t i = N-1; i >= 0; --i) {
    //             const __uint128_t current = (remainder << 64) | limbs[i];
    //             limbs[i] = static_cast<uint64_t>(current / 10);
    //             remainder = current % 10;
    //         }
    //         return static_cast<uint64_t>(remainder);
    //     }
    //
    //     static bool is_zero(const uint64_t* limbs) {
    //         return std::all_of(limbs, limbs + N, [](uint64_t limb) { return limb == 0; });
    //     }
    //
    //     static std::string big_number_to_decimal(const uint64_t* limbs) {
    //         if (is_zero(limbs)) {
    //             return "0";
    //         }
    //
    //         uint64_t tmp[N];
    //         std::copy_n(limbs, N, tmp);
    //         std::string result;
    //         result.reserve(21 * N);
    //
    //         do {
    //             uint64_t digit = divide_by_ten(tmp);
    //             result.push_back('0' + digit);
    //         } while (!is_zero(tmp));
    //
    //         std::reverse(result.begin(), result.end());
    //         return result;
    //     }
    //
    // public:
    //     auto format(const big_uint<N>& obj, format_context& ctx) const {
    //         return formatter<std::string>::format(big_number_to_decimal(obj.limbs), ctx);
    //     }
    // };

}
