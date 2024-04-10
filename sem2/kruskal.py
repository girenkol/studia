class Graf():
    def __init__(self, wierzcholki, krawedzie):
        self.wierzcholki = set(wierzcholki)
        self.krawedzie = krawedzie
        self.sasiedzi = []
        self.wybrane_krawedzie = []  # Nowa lista do przechowywania wybranych krawędzi
    
    def dodaj_krawedz(self, krawedz):
        self.wybrane_krawedzie.append(krawedz)

def sorter_wagowy(G_kraw_kg):
    return sorted(G_kraw_kg, key=lambda x: x[1])

def utw_zbior(elem):
    return set([elem])

def znaj_elem(a, A):
    for zbior in A:
        if a in zbior:
            return zbior
    return None

def Kruskal(graf, Waga):  # Zmieniamy nazwę parametru z "Graf" na "graf", aby nie kolidować z nazwą klasy
    A = []
    B = []
    for elem in graf.wierzcholki:  # Używamy parametru "graf" zamiast nazwy klasy "Graf"
        B.append(utw_zbior(elem))  
    Waga_sort = sorter_wagowy(Waga) 
    for i, j in Waga_sort:
        zb_a = znaj_elem(i, B) 
        zb_b = znaj_elem(j, B)
        if zb_a != zb_b:
            A.append((i, j))
            B.append(zb_a.union(zb_b))
            B.remove(zb_b)
            B.remove(zb_a)
            graf.dodaj_krawedz((i, j))  # Dodanie wybranej krawędzi do obiektu graf
    return A

if __name__ == "__main__":
    G_wierzch = ["A", "B", "C", "D", "E", "F", "G", "H"]
    G_kraw_kg =[(("A","B"), 2), (("A","C"),9), (("A","D"),10), (("B","C"),7),(("B","E"),8),(("B","F"),14),(("C","E"),21),(("C","D"),2),(("D","G"),11),(("E","G"),5),(("E","H"),7),(("E","F"),14),(("G","H"),9),(("H","F"),10) ]

    print(sorter_wagowy(G_kraw_kg))
    graf = Graf(G_wierzch, G_kraw_kg)
    A = Kruskal(graf, G_kraw_kg)
    print("Wybrane krawędzie:")
    print(graf.wybrane_krawedzie)
