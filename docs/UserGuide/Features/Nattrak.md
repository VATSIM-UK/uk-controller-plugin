# Nattrak

Nattrak is a VATSIM platform for managing the process of requesting clearance and transiting the North Atlantic Track System. Pilots can request their oceanic clearances here.

The UK Controller Plugin integrates with Nattrak in order to provide controllers with up-to-date information regarding the clearance status of aircraft on the network, which is invaluable
during busy, high-profile events such as Cross The Pond.

## Update frequency

UKCP polls Nattrak for clearance updates every 60 seconds.

## Viewing the full oceanic clearance

UKCP provides a TAG function - Oceanic Clearance Dialog. Using this function on any aircraft will open a dialog that displays
the full oceanic clearance from Nattrak.

## Tag items

### Oceanic Clearance Indicator

The Oceanic Clearance indicator shows the clearance status of a particular aircraft and whether the controller needs to make any amendments to
the aircrafts profile to meet the clearance.

The clearance indicator displays as "CLR" in the TAG item if a clearance is registered in Nattrak (blank otherwise), with colours differing depending on situation.

Pink: Clearance request is pending approval.
Green: Clearance has been approved and the controller needs to take no action.
Orange: Clearance has been approved, but the aircrafts currently assigned flight-level (or cruise level, if not set) does not match the cleared level for oceanic.

### Oceanic Clearance Flight Level

Displays the level at which the aircraft has been cleared to cross the ocean. The green and orange colours are used to indicate mismatches, as per the clearance indicator.

### Oceanic Clearance Mach Number

The mach number at which the aircraft has been cleared to cross the ocean.

### Oceanic Clearance Entry Point

The oceanic entry point.
