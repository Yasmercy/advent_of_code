import itertools as it

import networkx as nx
import numpy as np
import matplotlib.pyplot as plt


def get_shapes(rows):
    s1 = np.array([list(r) for r in rows[1:]]) == "#"
    s2 = np.rot90(s1)
    s3 = np.rot90(s2)
    s4 = np.rot90(s3)
    s5 = np.flip(s1, axis=0)
    s6 = np.flip(s2, axis=0)
    s7 = np.flip(s3, axis=0)
    s8 = np.flip(s4, axis=0)

    return [s1, s2, s3, s4, s5, s6, s7, s8]


def check_feasible(m, n, reqs, bitmasks):
    n_rots = 8
    n_shape = len(reqs)
    infty = 1000
    size = 7

    # create the nodes of the graph
    source, target = "s", "t"
    shapes = [s for s in range(n_shape)]
    rots = [(s, r) for s in range(n_shape) for r in range(n_rots)]
    places = [
        (s, r, i, j)
        for s in range(n_shape)
        for r in range(n_rots)
        for i in range(m - 2)
        for j in range(n - 2)
    ]
    cells = [(i, j) for i in range(m) for j in range(n)]

    # create edges between each layer of the graph
    s_to_shape = [(source, shape, req * size) for (shape, req) in zip(shapes, reqs)]
    shape_to_rot = [(shape, (shape, rot), infty) for shape, rot in rots]
    rot_to_place = [((s, r), (s, r, i, j), size) for s, r, i, j in places]
    place_to_cell = [
        ((s, r, i, j), (i + a, j + b), 1)
        for s, r, i, j in places
        for a, b in zip(*np.where(bitmasks[s][r]))
    ]
    cell_to_t = [(c, target, 1) for c in cells]

    # build the graph (with capacities)
    g = nx.DiGraph()
    g.add_node(source)
    g.add_node(target)
    g.add_nodes_from(shapes)
    g.add_nodes_from(rots)
    g.add_nodes_from(places)
    g.add_nodes_from(cells)

    g.add_weighted_edges_from(s_to_shape, weight="capacity")
    g.add_weighted_edges_from(shape_to_rot, weight="capacity")
    g.add_weighted_edges_from(rot_to_place, weight="capacity")
    g.add_weighted_edges_from(place_to_cell, weight="capacity")
    g.add_weighted_edges_from(cell_to_t, weight="capacity")

    flow, _ = nx.maximum_flow(g, source, target)
    # edges = [(u, v, f) for u, d in flow.items() for v, f in d.items() if f > 0]
    # print(*edges, sep="\n")
    return flow == sum(reqs) * size


def main():
    with open("sample.in") as f:
        lines = [line.strip() for line in f.readlines()]

    chunks = [list(group) for k, group in it.groupby(lines, key=bool) if k]
    shapes = [get_shapes(row) for row in chunks[:-1]]

    # for line in chunks[-1]:
    for line in [chunks[-1][2]]:
        dims, reqs = line.split(":")
        m, n = [int(x) for x in dims.strip().split("x")]
        reqs = [int(x) for x in reqs.strip().split(" ")]

        if check_feasible(n, m, reqs, shapes):
            print(line)

        break


if __name__ == "__main__":
    main()
