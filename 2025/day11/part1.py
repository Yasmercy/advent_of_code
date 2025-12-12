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

    degrees = {node: len(nbr) for node, nbr in for_adjac.items()}
    num_paths = dict()
    queue = ["out"]
    while queue:
        node = queue.pop()
        if node == "out":
            num_paths[node] = 1
        else:
            num_paths[node] = sum(num_paths[v] for v in for_adjac[node])

        for nbr in bck_adjac[node]:
            degrees[nbr] -= 1
            if degrees[nbr] == 0:
                queue.append(nbr)

    print(num_paths["you"])


if __name__ == "__main__":
    main()
