import matplotlib.pyplot as plt

data = [3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]

graph = plt.bar(range(len(data)), data)
plt.bar_label(graph, padding=1, fontsize=12)
plt.xticks(range(len(data)), range(len(data)))

plt.xlabel("Valor Backlog")
plt.ylabel("Conexões estabelecidas")
plt.title("Conexões simultâneas por valor de backlog")

plt.tight_layout()
plt.savefig("ex2/backlog_concurrency.png")
plt.show()
