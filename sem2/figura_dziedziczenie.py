from abc import ABC, abstractmethod

class Figura(ABC): #ABC oznacza dziedziczenie po abstrakycjnek klasie bazowej

    @abstractmethod #to czyni metode abstrakcyjna
    def Obwod(self):
        pass

    @abstractmethod
    def Pole(self):
        pass

class Okrag(Figura): #dziecko figury, dziedziczy po figurze

    def __init__(self, Promien):
        self.Promien = Promien

    def Obwod(self):
        return 2*3.1415 * self.Promien

    def Pole(self):
        return 3.1415 * self.Promien ** 2

class Prostokat(Figura): #dziecko figury, dziedziczy po figurze

    def __init__(self, bokA, bokB):
        self.bokA = bokA
        self.bokB = bokB

    def Obwod(self):
        return 2*(self.bokA+self.bokB)

    def Pole(self):
        return self.bokA*self.bokB

if __name__ == "__main__":
    
    o = Okrag(2)
    print("Okrag ma obwod {0}, pole {1}".format(o.Obwod(), o.Pole()))
    p = Prostokat(3,5)
    print("Okrag ma obwod {0}, pole {1}".format(p.Obwod(), p.Pole()))
