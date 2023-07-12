# Hold Manager

During periods of busy traffic, it may be prudent to hold aircraft in a holding pattern prior to commencing an approach
in order to manage workload and runway use. Aircraft in holds can become difficult to manage as a sea of tags converges
over a single location. The hold manager in the UK Controller Plugin attempts to reduce this workload by providing
a convenient display of the holding pattern.

## Displaying holds

First of all, whether or not to display the hold manager may be configured on a per-display basis. This is achieved
by ticking (or unticking) the `Display Holds` item in the `OP` menu.

Which holds are active can be configured via the `Configure Holds` item, where you can select from any of the holding
navaids that the plugin knows about.

## Synchronisation

The hold manager is synchronised between controllers, so teams of controllers staffing related controller
positions can be situationally aware of what others are doing.

## Adding and removing aircraft from the hold

There are several ways to add or remove aircraft from a given hold.

### The Commandline

When it's busy, you may not be able to find an aircraft in your lists or on the screen. For this purpose, the plugin
provides a number of commands for managing holds. If the optional callsign is omitted, the currently selected aircraft
will be added.

#### Adding an aircraft

To add an aircraft to the hold, simply type:

`.ukcp hold <NAVAID> <OPTIONAL_CALLSIGN>`

For example:

`.ukcp hold LAM BAW51A`

#### Removing an aircraft

To remove an aircraft from the hold, simply type:

`.ukcp nohold <OPTIONAL_CALLSIGN>`

For example:

`.ukcp nohold EZY4211`

### TAG Item and Function

The plugin has a TAG item for the currently selected hold, as well as a function for
triggering the hold selection list. You may add this to the TAGs or any EuroScope list
to manage the holds.

### Hold Display Add Button

The `ADD` button on the hold display provides two ways to add ways to aircraft to the hold.

Left clicking on the button adds the currently selected aircraft.

Right clicking will show a list of all aircraft that the user is tracking and are not currently assigned
to the hold. Selecting an aircraft will add it to the hold.

## The Hold Display

The hold display is highly configurable to the situation at hand.

### Changing which levels are displayed

The default levels displayed on the hold display are derived from any published holds that may exist at that navaid.

The top menu bar provides an options `o` button. Clicking on this allows you to select the minimum and maximum levels to
display.

If you want to add more levels quickly, you can use the large `+` and `-` buttons on the display to add or remove them
one at a time.

Clicking the large `ALL` button will return the hold display to its default state.

### Viewing published hold information

To view details of any published holds at a given navaid, you can click on the `i` button, which will display the details
instead of the standard hold display. Clicking the button again will switch back to the main hold display view.

### Aircraft Display

Aircraft are displayed alongside the level that they occupy. Each row displays (from left to right:

- Callsign
- Current level, with an indicator to show if the aircraft is climbing or descending.
- The current cleared level, in orange.
- How many minutes the aircraft has been in the hold.

Clicking the callsign triggers the hold selection list, where you can remove the aircraft
from the hold or assign it to another one.

Clicking the current cleared level triggers the EuroScope cleared level selection list.

### Multiple aircraft at the same level

If multiple aircraft are occupy the same level in the hold, they will show in the display
inside a green box.

### Restricted Levels

In some published holds, certain levels are restricted from use, often due to factors
such as the Minimum Stack Level, runway in use or the flow of traffic.

Levels that are not available for use in the current situation are displayed against
a slightly lighter background. Aircraft at these levels will still show on the display.

### Proximity Aircraft

Whilst holding, it is important to be aware of any other aircraft may affect your traffic.

If an aircraft that is not assigned to the hold comes within close proximity of the navaid, at a level
that is already occupied by an aircraft that is assigned to the hold, then the proximity aircraft
will be inserted into the hold display.

However, if both aircraft are within the confines of published holds at their respective
navaids that are deemed separated from each other, the proximity aircraft will only be
inserted into the display if it starts to come into conflict with the other aircraft.
