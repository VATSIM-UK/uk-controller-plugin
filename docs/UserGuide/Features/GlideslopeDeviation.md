# Glideslope Deviation

The glideslope deviation TAG item displays the aircrafts current deviation from the glideslope. It does this by looking at the aircraft current position
along the localiser (by taking a perpendicular line from the aircraft to the localiser) and then calculating the difference between the aircrafts altitude
and the altitude of the glideslope at that point.

## TAG Item

The "Glideslope Deviation" TAG item displays the current deviation from the glideslope in the format of `+/-XXX` where `XXX` is the deviation in feet. It will display in
green when the aircraft is within a few hundred feet of or below the glideslope, and red when the aircraft is above the glideslope. If the aircraft is massively above or below the
glideslope, the deviation will be displayed as `>/<1k`.
