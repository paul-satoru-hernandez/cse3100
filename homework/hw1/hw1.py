n = int(input("n = "))
i = 0
sum = 0.0

while (i < n):
    print(16 ** i)
    sum += ((4 / (8*i + 1)) - (2 / (8*i + 4)) - (1 / (8*i + 5)) - (1 / (8*i + 6))) * (1 / (16 ** i))
    i += 1

print("PI = {}".format(sum))
