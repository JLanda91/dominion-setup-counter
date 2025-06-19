#include <algorithm>
#include <chrono>
#include <numeric>
#include <thread>

#include <cub/cub.cuh>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <fmt/format.h>

#define CUDA_CHECK(expr, msg, ...) if (cudaError err = expr; err != cudaSuccess) { fmt::println(stderr, msg __VA_OPT__(,) __VA_ARGS__); exit(1); }

constexpr uint64_t binomial(uint8_t n, uint8_t k){
    if(k > n){
        return 0;
    }
    if(k == n || k == 0) {
        return 1;
    }
    uint64_t result = 1;
    const int max_j = k > n/2 ? n-k: k;
    for(auto j = 0; j < max_j; ++j){
        result *= (n - j);
        result /= j+1;
    }
    return result;
}

struct __align__(32) uint256_t {
    uint64_t limbs[4]{};  // Little-endian: limbs[0] is least significant

    // Default constructor (POD-compatible)
    __host__ __device__ __forceinline__
    uint256_t() = default;

    // Constructor from uint64_t
    __host__ __device__ __forceinline__
    uint256_t(uint64_t value) {
        limbs[0] = value;
        limbs[1] = 0;
        limbs[2] = 0;
        limbs[3] = 0;
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

    // operator+=
    __host__ __device__ __forceinline__
    uint256_t& operator+=(const uint256_t& rhs) {
        uint64_t carry = 0;
        for (int i = 0; i < 4; ++i) {
            limbs[i] = add_carry(limbs[i], rhs.limbs[i], carry);
        }
        return *this;
    }

    // operator+
    __host__ __device__ __forceinline__
    uint256_t operator+(const uint256_t& rhs) const {
        uint256_t result = *this;
        result += rhs;
        return result;
    }

    // operator*=(uint64_t)
    __host__ __device__ __forceinline__
    uint256_t& operator*=(uint64_t rhs) {
        uint64_t result[4] = {0};
        uint64_t carry = 0;

        for (int i = 0; i < 4; ++i) {
            uint64_t lo, hi;
            mul_64x64(limbs[i], rhs, lo, hi);

            // Add carry from previous limb
            uint64_t new_carry = 0;
            lo = add_carry(lo, carry, new_carry);
            hi += new_carry;

            result[i] = lo;
            carry = hi;
        }

        limbs[0] = result[0];
        limbs[1] = result[1];
        limbs[2] = result[2];
        limbs[3] = result[3];

        return *this;
    }

    // operator*(uint64_t)
    __host__ __device__ __forceinline__
    uint256_t operator*(uint64_t rhs) const {
        uint256_t result = *this;
        result *= rhs;
        return result;
    }

    // operator*=
    __host__ __device__ __forceinline__
    uint256_t& operator*=(const uint256_t& rhs) {
        uint256_t result(0);

        for (int i = 0; i < 4; ++i) {
            uint64_t carry = 0;

            for (int j = 0; j + i < 4; ++j) {
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
    uint256_t operator*(const uint256_t& rhs) const {
        uint256_t result = *this;
        result *= rhs;
        return result;
    }
};

struct uint256_t_reduce {

    __host__ __device__ __forceinline__
    uint256_t operator()(const uint256_t& lhs, const uint256_t& rhs) const {
        return lhs + rhs;
    }

    __host__ __device__ __forceinline__
    uint256_t operator()(const uint256_t& lhs, const uint64_t& rhs) const {
        return lhs + rhs;
    }

    __host__ __device__ __forceinline__
    uint256_t operator()(const uint64_t& lhs, const uint256_t& rhs) const {
        return rhs + lhs;
    }
};

template<>
struct fmt::formatter<uint256_t> : formatter<std::string> {
private:
    static uint64_t divide_by_ten(uint64_t* limbs) {
        __uint128_t remainder = 0;
        for (ssize_t i = 3; i >= 0; --i) {
            const __uint128_t current = (remainder << 64) | limbs[i];
            limbs[i] = static_cast<uint64_t>(current / 10);
            remainder = current % 10;
        }
        return static_cast<uint64_t>(remainder);
    }

    static bool is_zero(const uint64_t* limbs) {
        return std::all_of(limbs, limbs + 4, [](uint64_t limb) { return limb == 0; });
    }

    static std::string big_number_to_decimal(const uint64_t* limbs) {
        if (is_zero(limbs)) {
            return "0";
        }

        uint64_t tmp[4];
        std::copy_n(limbs, 4, tmp);
        std::string result;
        result.reserve(79);

        do {
            uint64_t digit = divide_by_ten(tmp);
            result.push_back('0' + digit);
        } while (!is_zero(tmp));

        std::reverse(result.begin(), result.end());
        return result;
    }

public:
    auto format(const uint256_t& obj, format_context& ctx) const {
        return formatter<std::string>::format(big_number_to_decimal(obj.limbs), ctx);
    }
};

namespace card_data::kingdom {
    enum class CardType : uint8_t {
        YoungWitch = 0,
        Knights = 1,
        Druid = 2,
        Ferryman = 3,
        Riverboat = 4,
        Looter = 5,
        Fate = 6,
        Doom = 7,
        Liaison = 8,
        Omen = 9,
        Loot = 10,
        None = 11,
    };

    // static constexpr std::size_t NUM_CARD_TYPES = 12;

    struct MembershipMask {
        bool is_young_witch_region : 1;
        bool is_approaching_army_region : 1;
        bool is_way_of_the_mouse_region : 1;
        bool is_ferryman_region : 1;
        bool is_riverboat_region : 1;
        bool is_obelisk_region : 1;
        bool is_action_or_treasure_region : 1;

        static constexpr MembershipMask FromUnsigned(uint8_t bit_pack) noexcept {
            return MembershipMask{
                .is_young_witch_region = (bit_pack & (1u << 0u)) > 0,
                .is_approaching_army_region = (bit_pack & (1u << 1u)) > 0,
                .is_way_of_the_mouse_region = (bit_pack & (1u << 2u)) > 0,
                .is_ferryman_region = (bit_pack & (1u << 3u)) > 0,
                .is_riverboat_region = (bit_pack & (1u << 4u)) > 0,
                .is_obelisk_region = (bit_pack & (1u << 5u)) > 0,
                .is_action_or_treasure_region = (bit_pack & (1u << 6u)) > 0
            };
        }

        static constexpr uint8_t ToUnsigned(const MembershipMask& m) noexcept {
            return
                (m.is_young_witch_region << 0uz) +
                (m.is_approaching_army_region << 1uz) +
                (m.is_way_of_the_mouse_region << 2uz) +
                (m.is_ferryman_region << 3uz) +
                (m.is_riverboat_region << 4uz) +
                (m.is_obelisk_region << 5uz) +
                (m.is_action_or_treasure_region << 6uz) ;
        }
    };

    // static constexpr std::size_t NUM_MEMBERSHIP_REGIONS = 7;

    struct Nonzero{
        MembershipMask mask;
        CardType card_type;
        uint8_t amount;
    };

    struct AmountIndexedNonzero {
        MembershipMask mask;
        CardType card_type;
        uint8_t n_index;
    };

    static constexpr std::size_t NUM_NNZ = 53;

    // using nonzero_array_t = std::array<Nonzero, NUM_NNZ>;
    static constexpr std::array<Nonzero, NUM_NNZ> NONZEROS = {{
        { MembershipMask::FromUnsigned(106u),	CardType::YoungWitch,	1 },
        { MembershipMask::FromUnsigned(114u),	CardType::Knights,	1 },
        { MembershipMask::FromUnsigned(101u),	CardType::Druid,	1 },
        { MembershipMask::FromUnsigned(112u),	CardType::Ferryman,	1 },
        { MembershipMask::FromUnsigned(109u),	CardType::Riverboat,	1 },
        { MembershipMask::FromUnsigned(104u),	CardType::Looter,	1 },
        { MembershipMask::FromUnsigned(106u),	CardType::Looter,	1 },
        { MembershipMask::FromUnsigned(114u),	CardType::Looter,	1 },
        { MembershipMask::FromUnsigned(66u),	CardType::Fate,	1 },
        { MembershipMask::FromUnsigned(101u),	CardType::Fate,	2 },
        { MembershipMask::FromUnsigned(104u),	CardType::Fate,	2 },
        { MembershipMask::FromUnsigned(109u),	CardType::Fate,	1 },
        { MembershipMask::FromUnsigned(112u),	CardType::Fate,	1 },
        { MembershipMask::FromUnsigned(2u),	CardType::Doom,	1 },
        { MembershipMask::FromUnsigned(106u),	CardType::Doom,	1 },
        { MembershipMask::FromUnsigned(109u),	CardType::Doom,	1 },
        { MembershipMask::FromUnsigned(112u),	CardType::Doom,	1 },
        { MembershipMask::FromUnsigned(114u),	CardType::Doom,	2 },
        { MembershipMask::FromUnsigned(64u),	CardType::Liaison,	1 },
        { MembershipMask::FromUnsigned(65u),	CardType::Liaison,	1 },
        { MembershipMask::FromUnsigned(101u),	CardType::Liaison,	1 },
        { MembershipMask::FromUnsigned(104u),	CardType::Liaison,	1 },
        { MembershipMask::FromUnsigned(109u),	CardType::Liaison,	3 },
        { MembershipMask::FromUnsigned(112u),	CardType::Liaison,	2 },
        { MembershipMask::FromUnsigned(96u),	CardType::Omen,	1 },
        { MembershipMask::FromUnsigned(104u),	CardType::Omen,	3 },
        { MembershipMask::FromUnsigned(112u),	CardType::Omen,	1 },
        { MembershipMask::FromUnsigned(114u),	CardType::Omen,	1 },
        { MembershipMask::FromUnsigned(64u),	CardType::Loot,	2 },
        { MembershipMask::FromUnsigned(65u),	CardType::Loot,	1 },
        { MembershipMask::FromUnsigned(98u),	CardType::Loot,	1 },
        { MembershipMask::FromUnsigned(101u),	CardType::Loot,	1 },
        { MembershipMask::FromUnsigned(112u),	CardType::Loot,	1 },
        { MembershipMask::FromUnsigned(0u),	CardType::None,	8 },
        { MembershipMask::FromUnsigned(1u),	CardType::None,	2 },
        { MembershipMask::FromUnsigned(2u),	CardType::None,	1 },
        { MembershipMask::FromUnsigned(8u),	CardType::None,	6 },
        { MembershipMask::FromUnsigned(9u),	CardType::None,	5 },
        { MembershipMask::FromUnsigned(64u),	CardType::None,	26 },
        { MembershipMask::FromUnsigned(65u),	CardType::None,	5 },
        { MembershipMask::FromUnsigned(66u),	CardType::None,	2 },
        { MembershipMask::FromUnsigned(72u),	CardType::None,	9 },
        { MembershipMask::FromUnsigned(73u),	CardType::None,	5 },
        { MembershipMask::FromUnsigned(96u),	CardType::None,	43 },
        { MembershipMask::FromUnsigned(98u),	CardType::None,	12 },
        { MembershipMask::FromUnsigned(101u),	CardType::None,	37 },
        { MembershipMask::FromUnsigned(103u),	CardType::None,	2 },
        { MembershipMask::FromUnsigned(104u),	CardType::None,	93 },
        { MembershipMask::FromUnsigned(106u),	CardType::None,	13 },
        { MembershipMask::FromUnsigned(109u),	CardType::None,	65 },
        { MembershipMask::FromUnsigned(111u),	CardType::None,	8 },
        { MembershipMask::FromUnsigned(112u),	CardType::None,	88 },
        { MembershipMask::FromUnsigned(114u),	CardType::None,	26 }
    }};

    static constexpr std::size_t NUM_UNIQUE_NNZ_AMOUNTS = 15u;

    class BinomialData {
    public:
        BinomialData() {
            std::transform(NONZEROS.cbegin(), NONZEROS.cend(), unique_nnz_amounts_.begin(), [](const Nonzero& nnz) {return nnz.amount; });
            std::sort(unique_nnz_amounts_.begin(), unique_nnz_amounts_.end());
            unique_nnz_amounts_.resize(std::distance(unique_nnz_amounts_.begin(), std::unique(unique_nnz_amounts_.begin(), unique_nnz_amounts_.end())));
            std::transform(NONZEROS.cbegin(), NONZEROS.cend(), amount_indexed_nonzeros_.begin(), [&](const Nonzero& nnz) {
                return AmountIndexedNonzero{
                    .mask = nnz.mask,
                    .card_type = nnz.card_type,
                    .n_index = static_cast<uint8_t>(std::distance(unique_nnz_amounts_.cbegin(), std::find(unique_nnz_amounts_.cbegin(), unique_nnz_amounts_.cend(), nnz.amount)))
                };
            });
            binom_data_.resize(unique_nnz_amounts_.size() * 11u);
            for (auto i = 0u; i < unique_nnz_amounts_.size(); ++i) {
                for (auto j = 0u; j < 11u; ++j) {
                    binom_data_[11 * i + j] = binomial(unique_nnz_amounts_[i], j);
                }
            }
        }

        [[nodiscard]] auto unique_nnz_amounts() const -> const thrust::host_vector<uint64_t>& {
            return unique_nnz_amounts_;
        }

        [[nodiscard]] auto amount_indexed_nonzeros() const -> const thrust::host_vector<AmountIndexedNonzero>& {
            return amount_indexed_nonzeros_;
        }

        [[nodiscard]] auto binom_data() const -> const thrust::host_vector<uint64_t>& {
            return binom_data_;
        }

    private:
        thrust::host_vector<uint64_t> unique_nnz_amounts_ = thrust::host_vector<uint64_t>(NUM_NNZ);
        thrust::host_vector<AmountIndexedNonzero> amount_indexed_nonzeros_ = thrust::host_vector<AmountIndexedNonzero>(NUM_NNZ);
        thrust::host_vector<uint64_t> binom_data_{};
    };

    static constexpr std::size_t DATA_SIZE = NUM_NNZ * 11u;

    static const std::array<uint64_t, DATA_SIZE> SEARCH_TABLE = [] {
        std::array<uint64_t, DATA_SIZE> result{};
        result.back() = 1ul;
        std::size_t i = NUM_NNZ - 1u;
        do {
            i--;
            const auto max_amount = std::min(NONZEROS[i+1].amount, static_cast<uint8_t>(10u));
            for (auto j = 0; j < 11uz; ++j) {
                uint64_t v = 0ul;
                for (auto k = 0; k <= max_amount && j + k < 11uz; ++k) {
                    v += result[11*(i+1) + j + k];
                }
                result[11*i + j] = v;
            }
        } while (i > 0);
        return result;
    }();

    static constexpr std::size_t MAX_ITER = SEARCH_TABLE[0] + SEARCH_TABLE[1];
}

template<typename T>
constexpr T div_ceil(T p, T q) {
    return (p + q - 1) / q;
}

static constexpr std::size_t BLOCK_SIZE = 256;
static constexpr std::size_t GRID_SIZE = 912;

__device__ __forceinline__ uint64_t binom_product(std::size_t n,
                                         const card_data::kingdom::AmountIndexedNonzero* __restrict__ nnz,
                                         const uint64_t* __restrict__ search_table,
                                         const uint64_t* __restrict__ binom_products,
                                         const uint64_t* __restrict__ unique_nnz_amounts) {
    uint8_t s = 0;
    uint64_t binom_product = 1ul;

    const std::size_t final_offset = nnz[card_data::kingdom::NUM_NNZ - 1].n_index * 11;

    for (auto i = 0u; i < card_data::kingdom::NUM_NNZ - 1u; ++i) {

        const auto n_index = nnz[i].n_index;
        const auto max_amount = unique_nnz_amounts[n_index];
        const uint64_t* binom_row = &binom_products[n_index * 11]; // padded version
        const uint64_t* search_table_row = &search_table[11 * i + s]; // padded version

        for (uint8_t j = 0; (j <= max_amount) && (s + j <= 10u); ++j) {
            const auto v = search_table_row[j];
            if (v > n) {
                binom_product *= binom_row[j];
                s += j;
                break;
            }
            n -= v;
        }
    }

    binom_product *= binom_products[final_offset + static_cast<uint8_t>(10u - s)];

    return binom_product;
}

__global__
void compute(uint256_t* __restrict__ block_out,
            const card_data::kingdom::AmountIndexedNonzero* __restrict__ nnz,
            const uint64_t* __restrict__ search_table,
            const uint64_t* __restrict__ binoms,
            const uint64_t* __restrict__ unique_nnz_amounts) {
    using BlockReduce = cub::BlockReduce<uint256_t, BLOCK_SIZE>;
    __shared__ BlockReduce::TempStorage s_temp_storage;
    __shared__ card_data::kingdom::AmountIndexedNonzero s_nnz[card_data::kingdom::NUM_NNZ];
    __shared__ uint64_t s_search_table[card_data::kingdom::NUM_NNZ * 11]; // added padding
    __shared__ uint64_t s_binoms[card_data::kingdom::NUM_UNIQUE_NNZ_AMOUNTS * 11]; // added padding
    __shared__ uint64_t s_unique_nnz_amounts[card_data::kingdom::NUM_UNIQUE_NNZ_AMOUNTS];

    for (auto j = threadIdx.x; j < card_data::kingdom::NUM_NNZ; j += BLOCK_SIZE) { s_nnz[j] = nnz[j]; }
    for (auto j = threadIdx.x; j < 11 * card_data::kingdom::NUM_NNZ; j += BLOCK_SIZE) { s_search_table[j] = search_table[j]; }
    for (auto j = threadIdx.x; j < 11 * card_data::kingdom::NUM_UNIQUE_NNZ_AMOUNTS; j += BLOCK_SIZE) { s_binoms[j] = binoms[j]; }
    for (auto j = threadIdx.x; j < card_data::kingdom::NUM_UNIQUE_NNZ_AMOUNTS; j += BLOCK_SIZE){s_unique_nnz_amounts[j] = unique_nnz_amounts[j];}

    __syncthreads();

    uint256_t result = 0;

    for (auto n = blockIdx.x * BLOCK_SIZE + threadIdx.x; n < card_data::kingdom::MAX_ITER; n += GRID_SIZE * BLOCK_SIZE) {
        result += binom_product(n, s_nnz, s_search_table, s_binoms, s_unique_nnz_amounts);
    }

    uint256_t block_sum = BlockReduce(s_temp_storage).Sum(result);
    if (threadIdx.x == 0) {
        block_out[blockIdx.x] = block_sum;
    }
}

__global__
void reduce(const uint256_t* __restrict__ block_out, uint256_t* __restrict__ scalar_out) {
    using BlockReduce = cub::BlockReduce<uint256_t, BLOCK_SIZE>;
    __shared__ BlockReduce::TempStorage temp_storage;

    const std::size_t i = blockIdx.x * BLOCK_SIZE + threadIdx.x;
    uint256_t result = 0;

    for(unsigned n = i; n < GRID_SIZE; n += BLOCK_SIZE) {
        result += block_out[n];
    }

    uint256_t block_sum = BlockReduce(temp_storage).Sum(result);
    if (threadIdx.x == 0) {
        *scalar_out = block_sum;
    }
}

template<typename T>
class Scalar {
public:
    Scalar() {
        CUDA_CHECK(cudaMallocHost(&h_data_, sizeof(T)), "Scalar cudaMallocHost failed");
        CUDA_CHECK(cudaMalloc(&d_data_, sizeof(T)), "Scalar cudaMalloc failed");
        *h_data_ = 0;
    }

    void dev_to_host(cudaStream_t& stream) const {
        CUDA_CHECK(cudaMemcpyAsync(h_data_, d_data_, sizeof(T), cudaMemcpyDeviceToHost, stream), "Scalar cudaMemcpyAsync failed");
    }

    [[nodiscard]] auto dev_ptr() const -> T* {
        return d_data_;
    }

    [[nodiscard]] auto value() const -> const T& {
        return *h_data_;
    }

    ~Scalar() {
        CUDA_CHECK(cudaFreeHost(h_data_), "DeviceVector cudaFreeHost failed");
        CUDA_CHECK(cudaFree(d_data_), "DeviceVector cudaFree failed");
    }

protected:
    T* h_data_ = nullptr;
    T* d_data_ = nullptr;
};

class AutoCudaStream {
public:
    explicit AutoCudaStream() {
        CUDA_CHECK(cudaStreamCreate(&stream_), "Failed to create cuda stream");
    }

    cudaStream_t& operator()() {
        return stream_;
    }

    ~AutoCudaStream() {
        CUDA_CHECK(cudaStreamDestroy(stream_), "Failed to destroy cuda stream");
    }

private:
    cudaStream_t stream_{};
};

int main()
{
    fmt::println("Iteration size: {}", card_data::kingdom::MAX_ITER);
    AutoCudaStream acs{};
    thrust::device_vector<uint256_t> compute_buffer = thrust::device_vector<uint256_t>(GRID_SIZE);
    const card_data::kingdom::BinomialData binom_data{};
    const thrust::device_vector<card_data::kingdom::AmountIndexedNonzero> d_nnz = binom_data.amount_indexed_nonzeros();
    const thrust::device_vector<uint64_t> d_search_table(card_data::kingdom::SEARCH_TABLE.cbegin(), card_data::kingdom::SEARCH_TABLE.cend());
    const thrust::device_vector<uint64_t> d_binom_products = binom_data.binom_data();
    const thrust::device_vector<uint64_t> d_unique_nnz_amounts = binom_data.unique_nnz_amounts();
    Scalar<uint256_t> result{};
    cudaGraph_t graph;
    cudaGraphExec_t graph_exec;

    cudaStreamBeginCapture(acs(), cudaStreamCaptureModeGlobal);
    compute<<<GRID_SIZE, BLOCK_SIZE, 0, acs()>>>(compute_buffer.data().get(), d_nnz.data().get(), d_search_table.data().get(), d_binom_products.data().get(), d_unique_nnz_amounts.data().get());
    reduce<<<1, BLOCK_SIZE, 0, acs()>>>(compute_buffer.data().get(), result.dev_ptr());
    result.dev_to_host(acs());
    cudaStreamEndCapture(acs(), &graph);
    cudaGraphInstantiate(&graph_exec, graph, nullptr, nullptr, 0);

    const auto t1 = std::chrono::high_resolution_clock::now();
    CUDA_CHECK(cudaGraphLaunch(graph_exec, acs()), "Graph launch failed: {}", cudaGetErrorString(err));
    CUDA_CHECK(cudaStreamSynchronize(acs()), "Awaiting graph complete failed: {}", cudaGetErrorString(err));
    const auto t2 = std::chrono::high_resolution_clock::now();

    const auto& host_result = result.value();
    fmt::println("Result:     {}", host_result);
    fmt::println("Test value: 236066834400084673986");
    fmt::println("Time (ms): {}", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
    return 0;
}