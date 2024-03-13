class Samochod:

    NumerySeryjne = 1

    def __init__(self, NumerRejestracyjny):
        self.MojNumerRejestracyjny = NumerRejestracyjny
        self.MojNumerSeryjny = Samochod.NumerySeryjne
        Samochod.NumerySeryjne += 1

    def UruchomSilnik(self):
        print("Silnik samochodu o numrze seryjnym {0} i numerze rejestracyjnym {1} zostal uruchomiony".format(self.MojNumerSeryjny,self.MojNumerRejestracyjny))

if __name__ == "__main__":

    s1 = Samochod("XD 666")
    s1.UruchomSilnik()
    s2 = Samochod("DUP 4")
    s2.UruchomSilnik()
