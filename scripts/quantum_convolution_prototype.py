from enum import IntFlag, auto
from math import comb, ceil, log2
from itertools import product
import numpy as np
import time
import sys


class EffectiveCardType(IntFlag):
    Loot = auto(),
    Omen = auto(),
    Knights = auto(),
    Druid = auto(),
    Looter = auto(),
    Fate = auto(),
    Doom = auto(),
    Liaison = auto(),
    Empty = auto(),
    YoungWitch = auto(),
    Ferryman = auto(),
    Riverboat = auto(),


class BinomData:
    def __init__(self, effective_card_type: EffectiveCardType, size: int, num_action_treasure: int):
        self.effective_card_type = effective_card_type
        self.size = size
        self.num_action_treasure = num_action_treasure
        self.data = list(comb(size - num_action_treasure, n-k)*comb(num_action_treasure, k) for n, k in product(range(11), repeat=2) if k <= n)

    def __repr__(self):
        return f"BinomData({self.effective_card_type}, {self.size}, {self.num_action_treasure})"


class EquivalenceClass:
    def __init__(self, vdrm: int, effective_card_type: EffectiveCardType, size: int, num_action_treasure: int):
        self.vdrm = vdrm
        self.binom_data = BinomData(effective_card_type, size, num_action_treasure)

    def __repr__(self):
        return f"EquivalenceClass({self.vdrm}, {self.binom_data})"


EC_DATA = [
    EquivalenceClass(0, EffectiveCardType.Empty, 34, 26),
    EquivalenceClass(0, EffectiveCardType.Liaison, 1, 1),
    EquivalenceClass(0, EffectiveCardType.Loot, 2, 2),
    EquivalenceClass(1, EffectiveCardType.Empty, 7, 5),
    EquivalenceClass(1, EffectiveCardType.Liaison, 1, 1),
    EquivalenceClass(1, EffectiveCardType.Loot, 1, 1),
    EquivalenceClass(2, EffectiveCardType.Doom, 1, 0),
    EquivalenceClass(2, EffectiveCardType.Empty, 3, 2),
    EquivalenceClass(2, EffectiveCardType.Fate, 1, 1),
    EquivalenceClass(8, EffectiveCardType.Empty, 15, 9),
    EquivalenceClass(9, EffectiveCardType.Empty, 10, 5),
    EquivalenceClass(32, EffectiveCardType.Empty, 43, 43),
    EquivalenceClass(32, EffectiveCardType.Omen, 1, 1),
    EquivalenceClass(34, EffectiveCardType.Empty, 12, 12),
    EquivalenceClass(34, EffectiveCardType.Loot, 1, 1),
    EquivalenceClass(37, EffectiveCardType.Druid, 1, 1),
    EquivalenceClass(37, EffectiveCardType.Empty, 37, 37),
    EquivalenceClass(37, EffectiveCardType.Fate, 2, 2),
    EquivalenceClass(37, EffectiveCardType.Liaison, 1, 1),
    EquivalenceClass(37, EffectiveCardType.Loot, 1, 1),
    EquivalenceClass(39, EffectiveCardType.Empty, 2, 2),
    EquivalenceClass(40, EffectiveCardType.Empty, 93, 93),
    EquivalenceClass(40, EffectiveCardType.Fate, 2, 2),
    EquivalenceClass(40, EffectiveCardType.Liaison, 1, 1),
    EquivalenceClass(40, EffectiveCardType.Looter, 1, 1),
    EquivalenceClass(40, EffectiveCardType.Omen, 3, 3),
    EquivalenceClass(42, EffectiveCardType.Doom, 1, 1),
    EquivalenceClass(42, EffectiveCardType.Empty, 13, 13),
    EquivalenceClass(42, EffectiveCardType.Looter, 1, 1),
    EquivalenceClass(42, EffectiveCardType.YoungWitch, 1, 1),
    EquivalenceClass(45, EffectiveCardType.Doom, 1, 1),
    EquivalenceClass(45, EffectiveCardType.Empty, 65, 65),
    EquivalenceClass(45, EffectiveCardType.Fate, 1, 1),
    EquivalenceClass(45, EffectiveCardType.Liaison, 3, 3),
    EquivalenceClass(45, EffectiveCardType.Riverboat, 1, 1),
    EquivalenceClass(47, EffectiveCardType.Empty, 8, 8),
    EquivalenceClass(48, EffectiveCardType.Doom, 1, 1),
    EquivalenceClass(48, EffectiveCardType.Empty, 88, 88),
    EquivalenceClass(48, EffectiveCardType.Fate, 1, 1),
    EquivalenceClass(48, EffectiveCardType.Ferryman, 1, 1),
    EquivalenceClass(48, EffectiveCardType.Liaison, 2, 2),
    EquivalenceClass(48, EffectiveCardType.Loot, 1, 1),
    EquivalenceClass(48, EffectiveCardType.Omen, 1, 1),
    EquivalenceClass(50, EffectiveCardType.Doom, 2, 2),
    EquivalenceClass(50, EffectiveCardType.Empty, 26, 26),
    EquivalenceClass(50, EffectiveCardType.Knights, 1, 1),
    EquivalenceClass(50, EffectiveCardType.Looter, 1, 1),
    EquivalenceClass(50, EffectiveCardType.Omen, 1, 1),
]

EC_DATA.sort(key=lambda x: x.binom_data.effective_card_type)
EC_DATA_TYPE_OFFSETS = tuple(next((i for i, ec in enumerate(EC_DATA) if ec.binom_data.effective_card_type == ect), None) for ect in EffectiveCardType) + (len(EC_DATA), )


def binom_index(n: int, k: int):
    return n*(n+1)//2 + k


def convolve(arr: np.ndarray, binom_data: BinomData) -> None:
    ec_in_range = binom_data.effective_card_type < 2**8
    if not ec_in_range:
        for i_mask in (EffectiveCardType(i) for i in range(256)):
            for n in reversed(range(11)): # voor alle output n
                tmp = np.zeros(n+1, dtype=object)
                for prev_n in range(n+1): #
                    ec_n = n - prev_n
                    for prev_k, ec_k in product(range(prev_n+1), range(ec_n+1)):
                        k = prev_n + ec_k
                        tmp[k] += binom_data.data[binom_index(ec_n, ec_k)] * arr[i_mask, binom_index(prev_n, prev_k)]
                arr[i_mask, binom_index(n, 0):binom_index(n+1,0) ] = tmp
    else:
        for i_mask in (EffectiveCardType(i) for i in range(256)):
            if binom_data.effective_card_type in i_mask:
                i_mask_no_type = i_mask ^ binom_data.effective_card_type
                for n in reversed(range(11)):  # voor alle output n
                    tmp = np.zeros(n + 1, dtype=object)
                    for prev_n in range(n + 1):  #
                        ec_n = n - prev_n
                        for prev_k, ec_k in product(range(prev_n + 1), range(ec_n + 1)):
                            k = prev_k + ec_k
                            tmp[k] += binom_data.data[binom_index(ec_n, ec_k)] * arr[i_mask, binom_index(prev_n, prev_k)]
                            if ec_n > 0:
                                tmp[k] += binom_data.data[binom_index(ec_n, ec_k)] * arr[i_mask_no_type, binom_index(prev_n, prev_k)]
                    arr[i_mask, binom_index(n, 0):binom_index(n + 1, 0)] = tmp


def intra_type_convolve(arr: list, binom_data: BinomData) -> None:
    for n in reversed(range(11)):
        tmp = np.zeros(n + 1, dtype=object)
        for prev_n in range(n+1):
            ec_n = n - prev_n
            for prev_k, ec_k in product(range(prev_n + 1), range(ec_n + 1)):
                k = prev_k + ec_k
                tmp[k] += binom_data.data[binom_index(ec_n, ec_k)] * arr[binom_index(prev_n, prev_k)]
        arr[binom_index(n, 0):binom_index(n + 1, 0)] = tmp


def log_result(result: np.ndarray) -> None :
    np.set_printoptions(threshold=sys.maxsize)
    print(result)

    i_max, j_max = np.unravel_index(np.argmax(result), result.shape)
    num_cards = next((n for n in reversed(range(11)) if n*(n+1)//2 <= j_max), None)
    num_action_treasure_cards = j_max - num_cards*(num_cards+1)//2
    max_val = result[i_max, j_max]
    print("Max number found at:")
    print(f"\tmask:                         {EffectiveCardType(i_max)}")
    print(f"\tnum cards:                    {num_cards}")
    print(f"\tnum action or treasure cards: {num_action_treasure_cards}")
    print(f"\tvalue:                        {max_val} ({int(ceil(log2(max_val))) + 1} bits)")


def main():
    result = np.zeros((2**8, 66), dtype=object)
    result[0, 0] = 1
    t1 = time.time()
    for i, ec in enumerate(EC_DATA):
        convolve(result, ec.binom_data)
    t2 = time.time()
    print(f"Convolution: {t2-t1:.6f}s")

    # log_result(result)

    return result


def main2():
    t1 = time.time()
    per_type_result = list(BinomData(ect, 0, 0) for ect in EffectiveCardType)
    for ect_i, ect in enumerate(EffectiveCardType):
        ec_slice = EC_DATA[EC_DATA_TYPE_OFFSETS[ect_i] : EC_DATA_TYPE_OFFSETS[ect_i+1]]
        per_type_slice = per_type_result[ect_i].data
        for ec in ec_slice:
            intra_type_convolve(per_type_slice, ec.binom_data)
    t3 = time.time()

    result = np.zeros((2 ** 8, 66), dtype=object)
    result[0, 0] = 1
    for i, binom_data in enumerate(per_type_result):
        convolve(result, binom_data)
    t2 = time.time()
    print(f"Intra-type only:    {t3 - t1:.6f}s")
    print(f"All:                {t2 - t1:.6f}s")
    # log_result(result)
    return result


if __name__ == '__main__':
    print(*EC_DATA, sep='\n')
    print()
    print("Max starting binom: ", max(max(ec.binom_data.data) for ec in EC_DATA))

    r1 = main()
    print()
    r2 = main2()

    print(np.all(r1 == r2))
    np.set_printoptions(threshold=sys.maxsize)
    for mask in range(2**8):
        print(r2[mask, :])