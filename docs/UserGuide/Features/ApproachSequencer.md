# Approach Sequencer

One of the big challenges facing controllers, especially during big events, is making the most efficent use of space and
positioning aircraft at optimal intervals on the final approach track. The UK Controller Plugin provides a tool in order
to aid in approach planning.

## How To Toggle

The `OP` menu contains an option for displaying the Approach Sequencer window. The window can be closed using the `X`
button on the title bar, as well as collapsible content via the up/down arrow button.

## Airfield Defaults

Above the sequencing list, a number of default values are provided. This allows you to set:

- The default "target" to aim for, either Wake Turbulence or a specified minimum distance.
- The airfields separation requirement, which will override the target where applicable or when there is no applicable
  wake turbulence interval.

## Airfield Selection

To select an airfield, click on the airfield code (default `--`) and pick one from the list.

## Adding An Aircraft

To add an aircraft, click on the `Add Aircraft` button and select it from the dropdown list. Only aircraft that are in
the air and destined for the selected airfield can be selected.

## The Display

Each aircraft is displayed on a single row in the display, in order of priority.

### Details On Each Row

Each row contains:

- The position in the sequence of the aircraft
- The aircrafts callsign
- The user-selected target (wake, or a specific distance). This is how far behind the previous aircraft that the user
  wishes this aircraft to be.
- The calculated actual target. If the user has selected a target that is not allowable - e.g. lower than required wake
  separation, then this may be different.

### Action Buttons

There are a number of buttons with which to perform actions on each aircraft

#### Changing The Order

Aircraft can be moved up or down in the order via the up and down arrows adjacent to their row.

#### Removing An Aircraft

An aircraft can be removed from the sequence via the delete button on its corresponding row.

#### Displaying The Target Radius

By toggling the circle button on and off, it will toggle whether the target radius is displayed around the radar target
for the aircraft.
