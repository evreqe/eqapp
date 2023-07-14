output_file = open("ZoneData_output.txt", "a")

with open('ZoneData.txt') as input_file:
    for line in input_file:
        tokens = line.split('^')
        output_file.write('const uint32_t ' + tokens[1] + ' = ' + tokens[0] + ';\n')

output_file.write('--------------------------------------------------\n')

with open('ZoneData.txt') as input_file:
    for line in input_file:
        tokens = line.split('^')
        output_file.write('{' + tokens[0] + ', "' + tokens[1] + '"},\n')

output_file.write('--------------------------------------------------\n')

with open('ZoneData.txt') as input_file:
    for line in input_file:
        tokens = line.split('^')
        output_file.write('{' + tokens[0] + ', "' + tokens[2] + '"},\n')

output_file.close()