def util_string_get_between(subject, first, last):
    try:
        begin = subject.index(first) + len(first)
        end = subject.index(last, begin)
        return subject[begin:end]
    except ValueError:
        return ""

outputFile = open("ExecuteCommand_output.txt", "a")

with open("ExecuteCommand.txt") as inputFile:
    commandID = 0
    for line in inputFile:
        if line.find("'") != -1:
            commandName = util_string_get_between(line, "'", "'")
            if commandName == "GREES" or commandName == "REES" or commandName == "EES":
                continue
            outputFile.write("const uint32_t " + commandName + " = " + str(commandID) + ";\n")
            commandID = commandID + 1

outputFile.close()
