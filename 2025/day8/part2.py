import itertools as it

import networkx as nx
import numpy as np


def main():
    with open("input.in") as f:
        lines = f.readlines()

    nodes = [tuple(map(int, line.strip().split(","))) for line in lines]
    edges = [
        (u, v, np.linalg.norm(np.array(u) - np.array(v)))
        for u, v in list(it.combinations(nodes, 2))
    ]

    graph = nx.Graph()
    graph.add_nodes_from(nodes)

    edges.sort(key=lambda x: x[2])
    num_components = len(nodes)

    for u, v, _ in edges:
        graph.add_edge(u, v)
        num_cc = len(list(nx.connected_components(graph)))

        if num_cc == 1:
            print(u, v, u[0] * v[0])
            break
        elif num_cc == num_components:
            graph.remove_edge(u, v)

        num_components = num_cc

    # graph = nx.Graph()
    # graph.add_nodes_from(nodes)
    # graph.add_weighted_edges_from(edges)
    # mst = nx.minimum_spanning_tree(graph)

    # print(max(mst.edges(data=True)))


if __name__ == "__main__":
    main()
