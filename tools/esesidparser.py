import json
readFile = open("./input/UK.ese", "r")
readDataLines = readFile.readlines()
readFile.close()

sidsstarsdictionary = {}

sidstarsline = "[SIDSTARS]"
inSidStars = 0
for line in readDataLines :
	# Wait until we find the SIDs
	if (inSidStars == 0) :
		if sidstarsline in line :
			inSidStars = 1
			continue

	# End of section, stop
	if (inSidStars == 1 and line[0] == "[") :
		break

	# When we do, process the sid line
	lineSplit = line.split(":")
	if (lineSplit[0] == "SID"):
		dictKeys = sidsstarsdictionary.keys()
		# Create the key if we need to
		if (lineSplit[1] not in dictKeys) :
			sidsstarsdictionary[lineSplit[1]] = {}

		sidsstarsdictionary[lineSplit[1]][lineSplit[3]] = 0


outFile = open("./output/initialaltitudes.json", "w")
outFile.truncate()
outFile.write(json.dumps(sidsstarsdictionary, indent=4, sort_keys=True))
outFile.close()


