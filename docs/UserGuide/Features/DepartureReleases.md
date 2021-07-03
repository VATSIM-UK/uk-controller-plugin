# Departure Releases

Part of controlling Tower positions at most if not all UK airports is requesting a release for departure, that is, asking
the above controller such an Radar or Area Control unit for permission to depart an aircraft. Some airfields do this for every
departure, whereas others have a "free flow" agreement, whereby normally a release would not be required except under
special circumstances such as after a missed approach.

As an alternative to coordinating this via TeamSpeak, the UK Controller Plugin provides a way to achieve this electronically.

## Who Can Request Releases?

Only positions of TWR and above can request departure releases. Ground and Delivery positions will not be able to open
the request release dialog. To be able to request a release, you must be recognised by the plugin as an active controller.

## Who Can Be Targeted For A Release Request?

Only positions of APP and above can be selected as a target controller for a departure release. Tower, Ground and Delivery
will not appear in the target list.

## How Do I Request A Release?

A release for a given aircraft can be requested using the `Open Request Departure Release Dialog` TAG function (see below).
Activate this using the aircrafts TAG or a EuroScope list, select the controller you wish to target from the dropdown and
click request.

Release requests are active for five minutes, before automatically expiring.

## Can I Have Multiple Requests At Once?

For a given aircraft, there may be one "active" release request for a single controller position at any given time.
In this context, "active" is defined to mean not rejected or request expired. It is therefore possible to request a release
from several controller positions at once.

## How Can I Check The Status Of My Release Requests?

There are two options for viewing the status of releases that you have requested. You may either use the Status Indicator 
and Countdown TAG items described below, or you can use the `Open Departure Release Status View` TAG function to popup
a temporary display for a given aircrafts releases.

The `Departure Release Status View` displays the callsigns of controllers that have been targeted for release, the time
until the release becomes active, if the release has been approved and has a conditional released at time, and the time until the release
approval expires if the release is approved and has an expiry.

The controller callsign indicates the status of the release:

- Green = Approved
- Red = Rejected
- Orange = Request has expired and another one should be made
- Blue = Acknowledged
- White = Pending

## How Do I Respond To A Release Request?

If you are the target of a release request, you can use the `Departure Release Decision List` to make decisions on individual
releases. This may be toggled on a per-ASR basis using the `OP` menu.

If there is a pending release request for you, the header bar of the list will flash *_Red_*.

To make a decision in the aforementioned list, left click on the aircrafts callsign and select your decision from the menu.

If you click "Approve", you will be presented with another dialog that allows you to specify additional constraints, such
as the "released at" time, as well as an approval expiry time.

Alternatively, you can use the `Open Departure Release Decision Menu` TAG function in aircraft TAGs or EuroScope lists.

## TAG Items

Several TAG items are provided for Departure Releases.

### Departure Release Status Indicator

This TAG item displays the current status of any releases that are in progress for a given aircraft. It does this by
displaying in the format of `X/Y` where `X` is the number of approved releases and `Y` is the total number of active release
requests.

The indicator will show as *_Green_* if all active release requests have been approved and not subject to any "released at"
conditions. If all active release requests have been approved but are subject to a "released at" condition, the indicator
will display in *_Pink_*.

The indicator will show as *_Red_* if any active release requests have been rejected.

The indicator will show as *_Orange_* if any active release requests have been approved but since expired.

### Departure Release Countdown

This TAG item displays two types of countdown.

The first type of countdown is the time until release expiry when all release requests are approved.
This indicates how long it will be before the first active release request expires (if applicable). This timer will start
in *_White_*, transitioning to *_Yellow_* and finally *_Red_* as it gets closer to zero.

If the timer displays in *_Pink_*, it is indicating how long it will be until all the active release requests have met
their "released at" conditions, which give you an indicator of how long it will be until the aircraft can depart.

### Departure Release Requesting Controller

This can be used by controllers that are able to approve departure releases. It will display the callsign of the controller
requesting the release.

## TAG Functions

Several TAG functions are available for departure releases.

### Open Request Departure Release Dialog

This TAG function triggers the request release dialog, through which you can select a controller to ask for
a release.

### Open Departure Release Decision Menu

This TAG function opens up the departure release decision menu. Controllers who are the target of a departure release
can use this to make a decision on the release request.

### Open Departure Release Status View

This TAG function toggles (for a brief period) a small view of the outstanding departure release requests for a given
aircraft. It displays the target controller, a countdown until any "released at" times, as well as a countdown until
any release expiry times.

### Open Departure Release Cancellation Menu

This TAG function opens a menu that allows you to select a target controller for whom to cancel a release request. Use
this if you want to withdraw your request at any time.
