class Graf:
    def __init__(self, wierzcholki, krawedzie):
        # Nazwy wierzcholkow nie powtarzaja sie
        if(len(wierzcholki) != len(set(wierzcholki))):
            print("Nazwy wierzcholkow powtarzaja sie")
        
        # krawedzie musza laczyc wierzcholki grafu
        for krawedz in krawedzie:
            if not krawedz[0] in wierzcholki or not krawedz[1] in wierzcholki:
                raise Exception("Nie da sie utworzyc takiego grafu")

        self.wierzcholki = wierzcholki
        self.krawedzie = krawedzie

    # funkcja zwracajaca sasiadow danego wierzcholka
    def sasiedzi(self, wierzcholek):
        sasiady = []
        for krawedz in self.krawedzie:
            if krawedz[0] == wierzcholek:
                sasiady.append(krawedz[1])
            elif krawedz[1] == wierzcholek:
                sasiady.append(krawedz[0])
        return set(sasiady)

    #sprawdza czy nie ma petli
    def CzyJestemDrzewem(self):
        wierzcholki = [set([i]) for i in self.wierzcholki]
        for krawedz in self.krawedzie:
            zbior_wierzcholka_A = None
            zbior_wierzcholka_B = None
            for zbior in wierzcholki:
                if krawedz[0] in zbior:
                    zbior_wierzcholka_A = zbior
                if krawedz[1] in zbior:
                    zbior_wierzcholka_B = zbior
            if zbior_wierzcholka_A == zbior_wierzcholka_B:
                return False
            else:
                zbior_wierzcholka_A.update(zbior_wierzcholka_B)
                wierzcholki.remove(zbior_wierzcholka_B)
        return True

if __name__ == "__main__":
    g = Graf(['a','b','c'],[('a','b'),('b','c')])
    print(g.sasiedzi('a'))

    if g.CzyJestemDrzewem():
        print("drzewuje")
    else:
        print("nie drzewuje")
