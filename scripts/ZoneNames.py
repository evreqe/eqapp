outFile = open("ZoneNames_output.txt", "a")

with open('ZoneNames.txt') as inFile:
    for line in inFile:
        if line.find('ZONE^LONG_NAME') != -1:
            continue
        line = line.replace('\n', '')
        tokens = line.split('^')
        outFile.write('{' + tokens[0] + ', "' + tokens[1] + '"},\n')

outFile.close()