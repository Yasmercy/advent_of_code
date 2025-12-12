from collections import defaultdict


def num_paths_to(dst, forward, bckward):
    # consider the sub-DAG that can reach the dst
    queue = [dst]
    reachable = set()
    while queue:
        node = queue.pop()
        if node in reachable:
            continue
        reachable.add(node)
        queue.extend(bckward[node])

    # now, do the dp to find number of paths
    num_paths = defaultdict(int)
    degrees = {node: len(set(nbr) & reachable) for node, nbr in forward.items()}
    queue = [dst]
    while queue:
        node = queue.pop()
        if node == dst:
            num_paths[node] = 1
        else:
            num_paths[node] = sum(num_paths[v] for v in forward[node] if v in reachable)

        for nbr in bckward[node]:
            if nbr not in reachable:
                continue
            degrees[nbr] -= 1
            if degrees[nbr] == 0:
                queue.append(nbr)
    return num_paths


def main():
    with open("input.in") as f:
        lines = [line.strip().replace(":", "") for line in f.readlines()]

    nodes = {node for line in lines for node in line.split(" ")}
    for_adjac = {line.split(" ")[0]: line.split(" ")[1:] for line in lines}
    bck_adjac = {node: [] for node in nodes}
    for line in lines:
        split = line.split(" ")
        dst = split[0]
        for src in split[1:]:
            bck_adjac[src].append(dst)

    num_paths_out = num_paths_to("out", for_adjac, bck_adjac)
    num_paths_dac = num_paths_to("dac", for_adjac, bck_adjac)
    num_paths_fft = num_paths_to("fft", for_adjac, bck_adjac)

    s = "svr"
    dac_first = num_paths_dac[s] * num_paths_fft["dac"] * num_paths_out["fft"]
    fft_first = num_paths_fft[s] * num_paths_dac["fft"] * num_paths_out["dac"]

    print(dac_first + fft_first)


if __name__ == "__main__":
    main()
