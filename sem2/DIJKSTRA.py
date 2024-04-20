import math

class Graf():
    def __init__(self, wierzcholki, krawedzie):
        if len(wierzcholki) != len(set(wierzcholki)):
            raise Exception("Nazwy wierzchołków powtarzają się")
        for krawedz in krawedzie:
            if not krawedz[0][0] in wierzcholki or not krawedz[0][1] in wierzcholki:
                raise Exception("Nieprawidłowe krawędzie")

        self.wierzcholki = wierzcholki
        self.krawedzie = krawedzie
        self.graf = self.stworz_graf()

    def stworz_graf(self):
        graf = {}
        for w in self.wierzcholki:
            graf[w] = {}
        for (w1, w2), waga in self.krawedzie:
            graf[w1][w2] = waga
        return graf
    
    def znajdz_sciezke(self, p, wierzcholek):
        sciezka = []
        while wierzcholek is not None:
            sciezka.insert(0, wierzcholek)
            wierzcholek = p[wierzcholek]
        return sciezka

    def dijkstra(self, startowy):
        d = {w: math.inf for w in self.wierzcholki}
        p = {w: None for w in self.wierzcholki}
        d[startowy] = 0
        Q = set(self.wierzcholki)

        while Q:
            u = min(Q, key=lambda x: d[x])
            Q.remove(u)
            for v, waga in self.graf[u].items():
                alt = d[u] + waga
                if alt < d[v]:
                    d[v] = alt
                    p[v] = u

        wyniki = {}
        for wierzcholek in self.wierzcholki:
            sciezka = self.znajdz_sciezke(p, wierzcholek)
            koszt = d[wierzcholek]
            wyniki[wierzcholek] = (sciezka, koszt)

        return wyniki

if __name__ == "__main__":
    g_wierzcholki = ["s", "u", "x", "v", "y"]
    g_krawedzie = [(("s", "x"), 5), (("s", "u"), 10), (("u", "v"), 1), (("u", "x"), 3), (("x", "u"), 2),
                   (("x", "y"), 2), (("x", "v"), 9), (("v", "y"), 4), (("y", "s"), 7), (("y", "v"), 6)]
    g = Graf(g_wierzcholki, g_krawedzie)
    wyniki_dijkstra = g.dijkstra("s")
    for wierzcholek, (sciezka, koszt) in wyniki_dijkstra.items():
        print(f"{wierzcholek}: {' '.join(sciezka)} koszt: {koszt}")
