import re

functionList = []

with open("eq_functions.h", "rt") as inFile:
    lines = inFile.readlines()
    for index, line in enumerate(lines):

        line = line.replace('\n', '')

        if line.startswith("inline") == 1:
            functionList.append(line)

with open("eq_functions_h.txt", "w") as outFile:
    for functionName in functionList:
        outFile.write(functionName + ";\n")
