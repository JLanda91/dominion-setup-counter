from functools import lru_cache


@lru_cache(maxsize=None)
def num_tuples(t: tuple, i: int = 0, s: int = 10):
    if i == len(t):
        return 1 if s == 0 else 0
    max_k = max(t[i], s)
    return sum(num_tuples(t, i+1, s-k) for k in range(max_k+1))


if __name__ == '__main__':
    loots = (2, 1, 1, 1, 1)
    nones = (8, 2, 1, 6, 5, 26, 5, 2, 9, 5, 43, 12, 37, 2, 93, 13, 65, 8, 88, 26)
    print(num_tuples(loots + nones))
