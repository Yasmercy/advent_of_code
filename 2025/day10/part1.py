import itertools as it

import networkx as nx


def state_to_int(state):
    return sum(2**i for i, x in enumerate(state) if x == "#")


def button_to_int(button):
    return sum(2**x for x in button)


def search(line):
    split = line.split(" ")
    bits = len(split[0].strip("]["))
    buttons = [
        button_to_int([int(x) for x in split[i].strip(")(").split(",")])
        for i in range(1, len(split) - 1)
    ]

    nodes = list(range(2**bits))
    edges = list(it.chain.from_iterable([(x, x ^ b) for b in buttons] for x in nodes))

    graph = nx.Graph()
    graph.add_nodes_from(nodes)
    graph.add_edges_from(edges)

    start = 0
    target = state_to_int(split[0].strip("]["))
    length = nx.shortest_path_length(graph, source=start, target=target)
    return length


def main():
    with open("input.in") as f:
        lines = f.readlines()

    print(sum(search(line) for line in lines))


if __name__ == "__main__":
    main()
