# Missed Approaches

The UK Controller Plugin provides functionality to be able to signal to adjacent controllers that an aircraft
has executed a missed approach, without having to through other coordination methods such as TeamSpeak.

## Who can trigger a missed approach?

Only controllers that are logged into a recognised, active, Tower (TWR) position may trigger a missed approach.

## When can a missed approach be triggered?

A missed approach can only be triggered when the aircraft is within a certain distance from its arrival
airfield. Once a missed approach is triggered, another may not be triggered for the next three minutes.

## How do I trigger a missed approach?

There are two methods by which to trigger a missed approach.

- The "Missed Approach Button" on the screen
- The "Trigger Missed Approach" TAG function

When either of these methods are used, the plugin will ask for confirmation and, if confirmed, will trigger
the missed approach.

The first two settings are explained in

## The missed approach button

### How do I trigger the button?

The button may be toggled on and off using the "Toggle Missed Approach Button" option in the `OP` menu.

### How does the button work?

To trigger a missed approach using the button, select an aircraft and then click the button.

If the button is `greyed out`, a missed approach cannot be triggered at this time. This may be due
to the aircrafts distance from the airfield or the position you are controlling.

If the button is `green`, a missed approach may be triggered.

If the button is `red`, then a missed approach is active for the selected aircraft.

## How do I know if a missed approach has happened?

There are multiple ways to know if a missed approach has happened:

- An audio alert
- A visual cue on the radar display
- A TAG item

## TAG Item

There is a TAG item named "Missed Approach Indicator". This will display "GO AROUND" if the aircraft in question
has an active missed approach.

## Audio Alert and Visual Cues

There are a number of configurable options for missed approaches in the plugin. These are all configurable
via the "Configure Missed Approaches" option of the `OP` menu.

## General settings

These settings are global, and not specific to a particular radar screen.

### Play missed approach alert sound

When a missed approach occurs that is relevant to you, selecting this option will cause the plugin to play
a short alarm sound, to alert you to the fact that a missed approach has occurred.

### Trigger missed approach alert when I initiate

This option controls whether or not you wish to receive the missed approach alert when it is
you that has initiated the missed approach.

### Service provisions

You may only be interested in missed approaches when you are providing a particular type of
service, e.g. Final Approach. You may select which services you wish to be alerted for,

### Airfields

Only airfields selected from the list will have their missed approaches alerted to you.

## ASR-specific settings

These settings are configurable on a per-ASR basis.

### Draw circle

When this option is selected, a red circle will be drawn around the aircraft performing a missed approach.

### Draw line

When this option is selected, a red line will be drawn from the centre of the screen to the aircraft performing a missed approach.

### Draw duration

This option controls how long any drawings will remain on screen following a missed approach.

## Acknowledging a missed approach

To acknowledge a missed approach, you must be providing Final Approach services at the given aircraft's destination airfield.

### TAG Function

Acknowledging a missed approach is achieved via the "Acknowledge Missed Approach" TAG function. Activate this TAG function and in the following dialog, enter any instructions or remarks that you have for the controller who triggered the approach.

### Receiving instructions

When a controller acknowledges a missed approach, they may append remarks or instructions for the responsible controller to carry out.

These remarks will be displayed to the controller in the `MISSED_APPROACH` message handler of the Euroscope chat box.
