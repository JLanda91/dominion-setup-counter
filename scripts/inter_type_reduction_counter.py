from functools import lru_cache


@lru_cache(maxsize=None)
def count_ecds(ecd_amounts : tuple, i : int = 0, s : int = 0):
    remainder = 10 - s

    if i == len(ecd_amounts):
        return 1 if remainder == 0 else 0

    max_ecd_amount = ecd_amounts[i]
    max_assignable_amount = min(remainder, max_ecd_amount) + 1
    result = 0
    for k in range(max_assignable_amount):
        result += count_ecds(ecd_amounts, i+1, s + k)
    return result


def main():
    ecd_amounts = (459,6,6,1,1,3,7,6,9)
    print(count_ecds(ecd_amounts))


if __name__ == '__main__':
    main()