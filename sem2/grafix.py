# zdefiniować class graf która bęzdie przetwarzac wierzchołki i krawędzie

class Graf:
        def __init__(self, wierzcholki, krawedzie):
            #Nazwy wierzcholkow nie powtarzaja sie
            if(len(wierzcholki) != len(set(wierzcholki))):
                print("Nazwy wierzcholkow powtarzaja sie")
            
            # krawedzie musza laczyc wierzcholki grafu
            for krawedz in krawedzie:
                if not krawedz[0] in wierzcholki or not krawedz[1] in wierzcholki:
                    raise Exception("Nie da sie utworzyc takiego grafu")

            self.wierzcholki = wierzcholki
            self.krawedzie = krawedzie

        #funkcja zwracajaca sasiadow danego wierzcholka
        def sasiedzi(self, wierzcholek):
            sasiady = []
            for krawedz in self.krawedzie:
                if krawedz[0] == wierzcholek:
                    sasiady.append(krawedz[1])
                elif krawedz[1] == wierzcholek:
                    sasiady.append(krawedz[0])
            return set(sasiady)

        
if __name__ == "__main__":
    g = Graf(['a','b','c','d'],[('b','c'),('a','c'),('a','d'),('a','d'),('a','a')])
    a = 'a'
    print(g.sasiedzi(a))
