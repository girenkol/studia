slownik = {'q':0,'w':0,'e':0,'r':0,'t':0,'y':0,'u':0,'i':0,'o':0,'p':0,'a':0,'s':0,'d':0,'f':0,'g':0,'h':0,'j':0,'k':0,'l':0,'z':0,'x':0,'c':0,'v':0,'b':0,'n':0,'m':0}

def ilosc_znakow(n):
    linijka = f.readline()
    dlugosc=0
    while linijka:
        linijka = f.readline()
        for i in range(len(n)):
            dlugosc = dlugosc + len(n[i])
    return dlugosc

def literki(tekst):
    for i in range(26):
        for j in tekst:
            print(tekst[j] + "  " + slownik.keys([i]))
            if tekst[j] == slownik.keys([i]):
                slownik[i] = slownik[i] + 1
    return True

f = open("lorem.txt", "r")
linijka = f.readline()
LicznikLinijek=0
while linijka:
    print(linijka)
    linijka = f.readline()
    LicznikLinijek=LicznikLinijek+1

print(ilosc_znakow(f))
tekst=f.read()
literki(tekst)
print(slownik)

f.close()