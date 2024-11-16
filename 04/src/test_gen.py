from random import *

TEST_COUNT = 1
TEST_SIZE = 100
MAX_NUM_SIZE = 100


def rand_num():
    # num_size = randint(1, MAX_NUM_SIZE)
    num_size = MAX_NUM_SIZE
    num = ""
    for i in range(num_size):
        num = num + chr(ord('0') + randint(0, 9))
    return num


def bin_pow(x, y):
    z = 1
    while y > 0:
        if y % 2 > 0:
            z = z * x
        x = x * x
        y = y // 2
    return z


operators = ["*"]

for i in range(1, TEST_COUNT + 1):
    testFile = open("tests/" + str(i) + ".in", "w")
    ansFile = open("tests/" + str(i) + ".out", "w")
    for j in range(TEST_SIZE):
        op = operators[randint(0, len(operators) - 1)]
        num1 = rand_num()
        num2 = rand_num()

        testFile.write(num1 + "\n")
        testFile.write(num2 + "\n")
        testFile.write(op + "\n")

        if op == "+":
            ansFile.write(str(int(num1) + int(num2)) + "\n")
        if op == "*":
            ansFile.write(str(int(num1) * int(num2)) + "\n")
        if op == "^":
            if int(num1) == 0 and int(num2) == 0:
                ansFile.write("Error\n")
            else:
                ansFile.write(str(bin_pow(int(num1), int(num2))) + "\n")
        if op == "/":
            if int(num2) == 0:
                ansFile.write("Error\n")
            else:
                ansFile.write(str(int(num1) // int(num2)) + "\n")
        if op == "-":
            if int(num1) < int(num2):
                ansFile.write("Error\n")
            else:
                ansFile.write(str(int(num1) - int(num2)) + "\n")
        if op == "<":
            if int(num1) < int(num2):
                ansFile.write("true\n")
            else:
                ansFile.write("false\n")
        if op == ">":
            if int(num1) > int(num2):
                ansFile.write("true\n")
            else:
                ansFile.write("false\n")
        if op == "=":
            if int(num1) == int(num2):
                ansFile.write("true\n")
            else:
                ansFile.write("false\n")
    testFile.close()
    ansFile.close()
