L = ['ania', 'kania', 'bania', 'lania', 'frania', 'drania']

while True:
    print("lista: ", L)
    wybor = input("dodac(d) czy usunac(u) czy wyszukac(s) czy zamienic po indexie(zi) czy zamienic po elementach(ze): ")
    
    match wybor:
        case "d":
            print("wybrano dodawanie\n")
            d=input("podaj element: ")
            L.append(d)
        case "u":
            print("wybrano usuwanie\n")
            u=int(input("podaj numer elementu: "))
            print("usunieto element - ", L.pop(u))
        case "s":
            print("wybrano szukanie\n")
            s=input("podaj element: ")
            print(L.index(s))
        case "zi":
            print("wybrano zamiane\n")
            a=int(input("podaj numer pierwszego elementu: "))
            b=int(input("podaj numer drugiego elementu: "))
            L[a], L[b] = L[b], L[a]
        case "ze":
            print("wybrano zamiane\n")
            a=L.index(input("podaj pierwszy element: "))
            b=L.index(input("podaj drugi element: "))
            L[a], L[b] = L[b], L[a]
        case _:
            print("nie umiesz wpisywac!!")
