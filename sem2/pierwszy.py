def kwadrat(n):
    return n*n

def silnia(n):
    a=1
    for i in range(n):
        a=a*(i+1)
    return int(a)

if __name__ == "__main__":
    print("ta liczba to: " + str(17) + " czemu?\nbo tak")
    print("Niech zyje nam {0:.2f} lat!".format(kwadrat(1.4243)))
    print("silnia " + str(silnia(10)))