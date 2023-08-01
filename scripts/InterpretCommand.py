numHotBars = 11
numHotButtonsPerHotBar = 12

outputFile = open("InterpretCommand_output.txt", "a")

for i in range(numHotBars):
    for j in range(numHotButtonsPerHotBar):
        outputFile.write('if (commandText == "//Hot' + str(i + 1) + '_' + str(j + 1) + '")\n')
        outputFile.write('{\n')
        outputFile.write('    EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT' + str(i + 1) + '_' + str(j + 1) + ');\n')
        outputFile.write('    return true;\n')
        outputFile.write('}\n')
        outputFile.write('\n')

outputFile.close()