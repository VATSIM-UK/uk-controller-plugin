# Handoffs

The UK division has the potential to open multiple, uncommon area controller positions during large, busy events such as
Cross the Pond. The plugin provides a convenient item for the departure list to make it easier for aerodrome controllers
to select the correct controller to handoff to on departure.

## TAG Item

The plugin provides the following TAG item: `Departure handoff controller`.

This will display the frequency of the next controller after departure, or, Unicom if the next controller is not online
or is the current user.

## How it works

The TAG item will primarily try to match an aircraft to a controller based on its filed SID.

If there is no SID for the flight, or no controllers are online in that handoff order, it will next try the "default"
order for the given airfield, which is, in most cases, the airfields top-down order.
