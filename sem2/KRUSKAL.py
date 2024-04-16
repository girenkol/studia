class Graf():

    def __init__(self, wierzcholki, krawedzie):

        if(len(wierzcholki) != len(set(wierzcholki))):
            raise Exception("Nazwy wierzcholkow powtarzaja sie")
        for krawedz in krawedzie:
            if not krawedz[0][0] in wierzcholki or not krawedz[0][1] in wierzcholki:
                raise Exception("nieprawidłowe krawędzie")

        self.wierzcholki = list(wierzcholki)
        self.krawedzie = list(krawedzie)

    def Utworz_zbior(self, element):
        return {element}

    def Sortuj(self, A):
        return [krawedz[0] for krawedz in sorted(A, key=lambda x: x[1])]

    def Znajdz_element(self, a, A):
        for element in A:
            if a in element:
                return element

    def Kruskal(self, Graf, Waga):
        A = []
        B = []
        for element in Graf.wierzcholki:
            B.append(self.Utworz_zbior(element))
        Waga_sortuj = self.Sortuj(Waga)
        for i, j in Waga_sortuj:
            temp_a = self.Znajdz_element(i, B)
            temp_b = self.Znajdz_element(j, B)
            if temp_a != temp_b:
                A.append((i, j))
                B.append(temp_a.union(temp_b))
                B.remove(temp_b)
                B.remove(temp_a)
        return A

if __name__ == "__main__":
    g_wierzcholki = ["A", "B", "C", "D", "E", "F", "G"]
    g_krawedzie = [(("A","B"), 2), (("A","C"),9), (("A","D"),10), (("B","C"),7),(("B","E"),8),(("B","F"),14),(("C","E"),21),(("C","D"),2),(("D","G"),11),(("E","G"),5)]

    g = Graf(g_wierzcholki, g_krawedzie)
    print(g.Kruskal(g, g_krawedzie))
