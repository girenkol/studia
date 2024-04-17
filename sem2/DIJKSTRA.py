class Graf():

    def __init__(self, wierzcholki, krawedzie):
        if len(wierzcholki) != len(set(wierzcholki)):
            raise Exception("Nazwy wierzcholkow powtarzaja sie")
        for krawedz in krawedzie:
            if not krawedz[0][0] in wierzcholki or not krawedz[0][1] in wierzcholki:
                raise Exception("Nieprawidłowe krawędzie")

        self.wierzcholki = wierzcholki
        self.krawedzie = krawedzie

    def dijkstra(self, startowy):
        d = {w: float('inf') for w in self.wierzcholki}
        d[startowy] = 0
        p = {w: None for w in self.wierzcholki}
        QS = set()

        while QS != set(self.wierzcholki):
            u = min((w for w in self.wierzcholki if w not in QS), key=lambda w: d[w])
            QS.add(u)
            for v, waga in self.krawedzie:
                if v[0] == u and v[1] not in QS and d[u] + waga < d[v[1]]:
                    d[v[1]] = d[u] + waga
                    p[v[1]] = u

        wyniki = {}
        for w in self.wierzcholki:
            sciezka = []
            u = w
            while u is not None:
                sciezka.insert(0, u)
                u = p[u]
            wyniki[w] = (sciezka, d[w])
        return wyniki

if __name__ == "__main__":
    g_wierzcholki = ["s", "u", "x", "v", "y"]
    g_krawedzie = [(("s","x"), 5), (("s","u"), 10), (("u","v"), 1), (("u","x"), 3), (("x","u"), 2), (("x","y"), 2), (("x","v"), 9), (("v","y"), 4), (("y","s"), 7), (("y","v"), 6)] 
    g = Graf(g_wierzcholki, g_krawedzie)
    wyniki_dijkstra = g.dijkstra("A")
    for wierzcholek, (sciezka, koszt) in wyniki_dijkstra.items():
        print(f"{wierzcholek}: {' '.join(sciezka)} koszt: {koszt}")
