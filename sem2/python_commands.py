L = [i*j*k for i in range(10) for j in range(10) for k in range(10)]
[i*i+1 for i in range(0,10,2)]
L[3:2:-1]
//Liste można trzymać w liście, można trzymać różne typy zmiennych

//dla takiego czegoś
K = (0,1) - krotka
nie można zrobić K[0]=9

licznik znakow w txt
txt = "Ala ma kota, a kot ma Ale."
[(i, txt.count(i)) for i in set (txt)]

macierz jednostkowa
[(i,j,1 if i==j else 0) for i in range(3) for j in range(3)]

to są wskaźniki więc jak zmieniam wartość w a to na kopi b też sie to zmieni
>>> a = [0,1,2]
>>> b=a
>>> b
[0, 1, 2]
>>> a[2]=4
>>> b
[0, 1, 4]
>>> a
[0, 1, 4]
