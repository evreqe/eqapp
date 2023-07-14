import re

addressList = []

addressIndex = 0

with open("eq_addresses.h", "rt") as inFile:
    lines = inFile.readlines()
    for index, line in enumerate(lines):

        if line.find("uint32_t EQ_ADDRESS_") != -1:
            if line.find("// calculated") == -1:
                matches = re.findall("uint32_t (EQ_ADDRESS_[0-9A-Za-z_]+) = ", line)
                if matches:
                    addressList.append(matches[0])

for address in addressList:
    print(address)

with open("eqapp_addresses_fix.h", "w") as outFile:
    outFile.write("#pragma once\n")
    outFile.write("\n")
    addressIndex = 0
    for address in addressList:
        outFile.write("EQAPP_FixAddress(" + address + ");    // Index: " + str(addressIndex) + "\n")
        addressIndex = addressIndex + 1
