import itertools as it

import networkx as nx
import numpy as np


def main():
    with open("input.in") as f:
        lines = f.readlines()

    nodes = [tuple(map(int, line.strip().split(","))) for line in lines]
    edges = list(it.combinations(nodes, 2))
    edges.sort(key=lambda x: np.linalg.norm(np.array(x[0]) - np.array(x[1])))
    edges = edges[:1000]

    graph = nx.Graph()
    graph.add_nodes_from(nodes)
    graph.add_edges_from(edges)
    cc = nx.connected_components(graph)

    sizes = [len(c) for c in cc]
    sizes.sort()

    print(sizes[-1] * sizes[-2] * sizes[-3])


if __name__ == "__main__":
    main()
