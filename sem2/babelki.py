def babelki(n):
    for i in range(len(n)):
        for j in range(len(n)-i-1):
            if n[j] > n[j+1]:
                n[j], n[j+1] = n[j+1], n[j]
                print(n)
    return n

print(babelki([5,4,7123890,77,-321,8246,99,111,2,3,-44]))