import json
from json import encoder
encoder.FLOAT_REPR = lambda o: format(o, '.3f')
readFile = open("./input/UK.ese", "r")
readDataLines = readFile.readlines()
readFile.close()

positionsdictionary = {}
identifiercallsigndictionary = {}

sectionLine = "[POSITIONS]"
inPositions = 0
# Parse the positions
for line in readDataLines :
	# Wait until we find the positions section
    if (inPositions == 0 and sectionLine in line) :
        inPositions = 1
        continue

    # Ignore comment lines or blank lines
    if (line[0] == ";" or line[0] == "\n") :
        continue

	# End of section, stop
    if (inPositions == 1 and line[0] == "[") :
        break

	# When we find positions, process lines. Only interested in UK positions that have a primary frequency
    # and are NOT considered to be ATIS. In this case, we want the position identifier.
    lineSplit = line.split(":")
    if (("LON" in lineSplit[0] or "LTC" in lineSplit[0] or "SCO" in lineSplit[0] or "STC" in lineSplit[0] or lineSplit[0][0:2] == "EG") and
       lineSplit[2] != "199.998" and lineSplit[6] != "ATIS") :
        positionsdictionary[lineSplit[0]] = {'frequency' : float(lineSplit[2]), 'top-down' : []}
        identifiercallsigndictionary[lineSplit[3]] = lineSplit[0]

ownershipdictionary = {}
sectionLine = "[AIRSPACE]"
inownership = 0
insector = 0
waitingForAirfield = 0
currentTopdown = []

# Parse the ownership
for line in readDataLines :
	# Wait until we find the positions section
    if (inownership == 0 and sectionLine in line) :
        inownership = 1
        continue

    # Ignore comment lines or blank lines
    if (line[0] == ";" or line[0] == "\n") :
        continue

	# End of section, stop
    if (inownership == 1 and line[0] == "[") :
        break

    # This signifies the start of a TWR sector definition (DEP is used for LL/CC)
    if (insector == 0 and line[:7] == "SECTOR:" and "Only" not in line and ("TWR" in line or "DEP" in line)) :
        insector = 1
        continue

    # Once we're in the sector part, the next line is ownership. Get the top down order
    if (insector == 1) :
        insector = 0
        waitingForAirfield = 1
        currentTopdown = line.split(":")[1:]
        currentTopdown[len(currentTopdown) - 1] = currentTopdown[len(currentTopdown) - 1].strip("\n")

    # When we find the DEPAPT part, we know the airfield we're doing top down for
    if (waitingForAirfield == 1 and "DEPAPT" in line) :
        waitingForAirfield = 0
        airfield = line.split(":")[1][:-1]

        # Add DEL and GND to the list, because they don't typically "own" sectors
        delpos = airfield[2:] + "D"
        gndpos = airfield[2:] + "G"
        newTopDown = [delpos, gndpos] + currentTopdown
        topDownPositions = []

        # Match positions to the defined positions list to get the ICAO
        for position in newTopDown:
            if (position in identifiercallsigndictionary.keys()):
                topDownPositions = topDownPositions + [identifiercallsigndictionary[position]]

                # Add the airport to the positions "interested" list for the position.
                positionsdictionary[identifiercallsigndictionary[position]]['top-down'].append(airfield);

        ownershipdictionary[airfield] = topDownPositions

outFile = open("./output/controller-positions.json", "w")
outFile.truncate()
outFile.write(json.dumps(positionsdictionary, indent=4, sort_keys=True))
outFile.close()


