import zstandard as zstd

from dataclasses import dataclass
from itertools import batched

import ec53


@dataclass
class MultiplierTypeATDistribution:
    data: tuple[int, ...]
    mask: ec53.MultiplierType
    num_action_or_treasure: int
    continuation_no: int = 0

    def __init__(self, data: tuple[int, ...]):
        self.data = data
        self.mask = ec53.MultiplierType(0)
        self.num_action_or_treasure = 0
        for n, kingdom_type_at_aggr in zip(data, ec53.KINGDOM_DATA_TYPE_AT_GROUPED):
            if n > 0 and kingdom_type_at_aggr.multiplier_card_type != ec53.MultiplierType.Other:
                self.mask |= kingdom_type_at_aggr.multiplier_card_type
            if kingdom_type_at_aggr.is_action_or_treasure:
                self.num_action_or_treasure += n

    def __lt__(self, other):
        return (self.mask.value, self.num_action_or_treasure, self.data) < \
            (other.mask, other.num_action_or_treasure, other.data)

    def mask_num_at_tuple(self):
        return self.mask, self.num_action_or_treasure


def generate_distributions(ecd_output: list = None, i : int = 0, s : int = 0, result = tuple()):
    if ecd_output is None:
        ecd_output = list()

    remainder = 10 - s

    if i == len(ec53.KINGDOM_DATA_TYPE_AT_GROUPED):
        if remainder == 0:
            ecd_output.append(MultiplierTypeATDistribution(result))
        return

    max_ecd_amount = ec53.KINGDOM_DATA_TYPE_AT_GROUPED[i].total
    max_assignable_amount = min(remainder, max_ecd_amount) + 1
    for k in range(max_assignable_amount):
        generate_distributions(ecd_output, i+1, s + k, result + (k, ))

NUM_BYTES_PER_DISTRIBUTION = 6

def pack_bytes(distributions: list[MultiplierTypeATDistribution]) -> bytes:
    """
    Writes 6 bytes per item
    - Byte 0: data[0], data[1]
    - Byte 1: data[2], data[3]
    - Byte 2: data[4], data[5]
    - Byte 3: data[6], data[7]
    - Byte 4: data[8], data[9]
    - Byte 5: data[10], continuation_no
    """

    buf = bytearray(NUM_BYTES_PER_DISTRIBUTION * len(distributions))

    offset = 0
    for distribution in distributions:
        buf[offset] = distribution.data[0] | (distribution.data[1] << 4)
        buf[offset + 1] = distribution.data[2] | (distribution.data[3] << 4)
        buf[offset + 2] = distribution.data[4] | (distribution.data[5] << 4)
        buf[offset + 3] = distribution.data[6] | (distribution.data[7] << 4)
        buf[offset + 4] = distribution.data[8] | (distribution.data[9] << 4)
        buf[offset + 5] = distribution.data[10] | (distribution.continuation_no << 4)
        offset += 6

    return bytes(buf)


def write_zstd(distribs_bytes):
    cctx = zstd.ZstdCompressor(level=10)
    with open("../data/mask_num_at_reduction.zstd", "wb") as f:
        with cctx.stream_writer(f) as compressor:
            compressor.write(distribs_bytes)


def read_zstd():
    dctx = zstd.ZstdDecompressor()
    with open("../data/mask_num_at_reduction.zstd", "rb") as f:
        with dctx.stream_reader(f) as reader:
            return reader.read()


def print_first(data: bytes, n: int):
    for distrib_bytes in batched(data[: n*NUM_BYTES_PER_DISTRIBUTION], NUM_BYTES_PER_DISTRIBUTION):
        data = list()
        data.append(distrib_bytes[0] & 0xF)
        data.append(distrib_bytes[0] >> 4)
        data.append(distrib_bytes[1] & 0xF)
        data.append(distrib_bytes[1] >> 4)
        data.append(distrib_bytes[2] & 0xF)
        data.append(distrib_bytes[2] >> 4)
        data.append(distrib_bytes[3] & 0xF)
        data.append(distrib_bytes[3] >> 4)
        data.append(distrib_bytes[4] & 0xF)
        data.append(distrib_bytes[4] >> 4)
        data.append(distrib_bytes[5] & 0xF)
        continuation_no = distrib_bytes[5] >> 4

        print(f"{{ {{ {', '.join(f'{x:>2}' for x in data)} }}, {continuation_no:>2} }}")




def main():
    print(*ec53.KINGDOM_DATA_TYPE_AT_GROUPED, sep='\n')
    distribs = list()

    generate_distributions(distribs)

    distribs.sort()
    print(f"Found {len(distribs)} distributions\n")

    last_mask_num_at_segment = None
    continuation_no = 0

    for batch in batched(distribs, 32):
        if batch[0].mask_num_at_tuple() != last_mask_num_at_segment:
            batch[0].continuation_no = 1
            continuation_no = 1
        else:
            batch[0].continuation_no = continuation_no + 1
            continuation_no += 1
        last_mask_num_at_segment = batch[0].mask_num_at_tuple()
        for elem in batch[1:]:
            if elem.mask_num_at_tuple() != last_mask_num_at_segment:
                last_mask_num_at_segment = elem.mask_num_at_tuple()
                elem.continuation_no = 1
                continuation_no = 1

    # print("Histogram of continuation numbers:")
    # max_continuation_no = max(x.continuation_no for x in distribs)
    # for c in range(1, max_continuation_no + 1):
    #     print(f"{c:>2}: {sum(1 for x in distribs if x.continuation_no == c):>4}")
    # print()
    #
    # print("Max continuation number per mask:")
    # print('{', end='')
    # for m in range(256):
    #     # print(f"{m:>3}: {max(x.continuation_no for x in distribs if x.mask.value == m):>2}")
    #     print(f"{max(x.continuation_no for x in distribs if x.mask.value == m):>2}, ", end='')
    # print('}')

    print(f"AS IS:\n{'='*10}")
    for elem in distribs[-100 : ]:
        print(f"{{ {{ {', '.join(f'{x:>2}' for x in elem.data)} }}, {elem.continuation_no:>2} }},")

    write_zstd(pack_bytes(distribs))





if __name__ == '__main__':
    main()