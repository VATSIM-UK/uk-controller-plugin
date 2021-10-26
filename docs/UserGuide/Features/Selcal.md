# SELCAL

Selective Calling (SELCAL) provides a means by which controllers can attract the attention of a particular
aircraft that they wish to communicate with, without the pilots having to constantly listen for transmissions.

This can be useful in the Oceanic airspace, where aircraft may go hours without receiveing an instruction
from the ground, especially in the modern era of ACARS messages.

## Where Are SELCAL Codes Set?

A SELCAL Code is an alphabetical code containing four out of sixteen possible characters.
The code is split into two parts. No character may be repeated throughout the entire code
and the first character in each part must be "lower than the second", for example:

- ABCD: Valid
- ABBC: Invalid, B is repeated
- ABDC: Invalid, C must come before D

SELCAL Codes are specified in flightplan remarks in the format `SEL/ABCD`.

## TAG Items

The UK Controller Plugin provides two TAG items that parse the SELCAL Code from flightplan remarks
for display in TAGs and lists.

- SELCAL Code: Displays the parsed SELCAL in raw form, e.g. `ABCD`
- SELCAL Code With Separator: Displays the parsed SELCAL with a separator between the two parts, e.g. `AB-CD`

Both TAG items are blank if a SELCAL code cannot be parsed.
