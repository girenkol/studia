a = [1003216,4,5,3,4,2,13,62,643222,11]

print(a)

for i in range(len(a)):
    for j in range(0, len(a)-i-1):
        if a[j]>a[j+1]:
            a[j], a[j+1] = a[j+1], a[j]
            print(a)
print("sor: ", a)