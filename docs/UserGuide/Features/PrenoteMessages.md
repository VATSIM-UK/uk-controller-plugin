# Prenote

Part of controlling Tower positions at most if not all UK airports is sending a prenote message, that is, letting an above controller such an Radar or Area Control unit know that an aircraft is starting and will be coming their way shortly.

As an alternative to coordinating this via TeamSpeak, the UK Controller Plugin provides a way to achieve this electronically.

## Who Can Send Prenotes?

To be able to send a prenote, you must be recognised by the plugin as an active controller.

## Who Can Be Targeted For A Prenote?

Only positions of APP and above can be selected as a target controller for a prenote. Tower, Ground and Delivery
will not appear in the target list.

## How Do I send a prenote?

A prenote for a given aircraft can be requested using the `Open Prenote Controller Selection Menu` TAG function (see below).
Activate this using the aircrafts TAG or a EuroScope list, select the controller you wish to target from the dropdown and
click request.

Prenote messages are active for five minutes, before automatically expiring.

## Can I Have Multiple Prenotes At Once?

For a given aircraft, there may be one "active" prenote for a single controller position at any given time.
In this context, "active" is defined to mean not expired. It is therefore possible to send a prenote to several controller positions at once.

## How Can I Check The Status Of My Prenotes?

There are two options for viewing the status of prenotes for a given aircraft. You may either use the Status Indicator
TAG item described below, or you can use the `Open Prenote Message Status View` TAG function to popup
a temporary display for a given aircrafts prenotes.

The `Open Prenote Message Status View` displays the callsigns of controllers that have been targeted for prenote and
whether they have acknowledged receipt of the message.

The controller callsign indicates the status of the prenote:

- Green = Approved
- White = Pending

## How Do I Respond To A Prenote?

There are two ways to respond to a prenote, if you are the target controller.

Firstly, you can use the `Departure Coordination List` to acknowledge the message.
This may be toggled on a per-ASR basis using the `OP` menu. A prenote will have `Pre` in the "Type" column.

If there is a pending prenote for you, the header bar of the list will flash *_Red_*. You will also receive
an audible ping to let you know that something has come in.

To acknowledge the prenote and remove it from the aforementioned list, left click on the aircrafts callsign.

This may also be achieved using the `Acknowledge Prenote` TAG function (see below).

## TAG Items

Several TAG items are provided for prenotes.

### Prenote Status Indicator

This TAG item displays the current status of any active prenotes for a given aircraft. It does this by
displaying in the format of `X/Y` where `X` is the number of acknowledged prenotes and `Y` is the total number of active prenotes
requests.

The indicator will show as *_Green_* if all active prenotes have been acknowledged.


## TAG Functions

Several TAG functions are available for prenotes.

### Open Prenote Controller Selection Menu

This TAG function triggers the controller selection menu, which allows you to send a prenote to a specified
controller.

### Open Prenote Message Status View

This TAG function toggles (for a brief period) a small view of the outstanding prenotes for a given
aircraft. It displays the target controller, and whether the prenote has been acknowledged.

### Open Cancel Prenote Message Menu

This TAG allows you to select a controller for which to cancel a prenote message. Using this will allow you to send
another prenote to the same controller immediately.

### Acknowledge Prenote

This TAG function allows a controller to acknowledge a prenote for a given aircraft directly through the EuroScope
lists.
