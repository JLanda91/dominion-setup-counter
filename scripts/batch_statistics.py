from itertools import pairwise


class LogLine:
    def __init__(self, line: str):
        split_line = line.split(';')
        self.thread_num = int(split_line[0])
        self.combination_modifiers = split_line[1]
        self.num_action_treasure = int(split_line[2])
        self.time_ns = int(split_line[3])

    def __repr__(self):
        return f"LogLine(thread_num={self.thread_num}, combination_modifiers={self.combination_modifiers}, num_action_treasure={self.num_action_treasure}, time_ns={self.time_ns})"


def read_batch(n: int):
    file_name = f"../build/release-gcc-14/batch_{n}.txt"
    with open(file_name, 'r') as file:
        lines = file.readlines()
    return [LogLine(line.rstrip('\n')) for line in lines]

def basic_stats(batch_data: list):
    stats = dict()
    for log_line in batch_data:
        key = (log_line.combination_modifiers, log_line.num_action_treasure)
        stats.setdefault(key, list())
        stats[key].append(log_line.time_ns)

    for stat_values in stats.values():
        stat_values.sort()

    for (cm, num_at), stat_values in stats.items():
        n = len(stat_values)
        avg_time = sum(stat_values) / n
        min_time = stat_values[0]
        max_time = stat_values[-1]
        median_time = stat_values[n // 2]
        q1_time = stat_values[n // 4]
        q3_time = stat_values[3 * n // 4]
        print(f"{cm};{num_at};{n};{avg_time};{min_time};{q1_time};{median_time};{q3_time};{max_time}")


def main(n: int):
    print(f"Basic statistics of batch {n}:")
    batch_data = read_batch(n)
    basic_stats(batch_data)
    print()


if __name__ == '__main__':
    main(75)
    main(76)
    main(192)
    main(25000)
