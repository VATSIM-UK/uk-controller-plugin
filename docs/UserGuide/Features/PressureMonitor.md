# Pressure Monitor

The plugin provides functionality for aerodrome controllers to monitor changes in pressure. This allows controllers to
be notified of important pressure changes so that they can then update pilots.

## Configuration

Configuration for the Pressure Monitor is under the "General Settings" option of the `OP` menu.

## Pressure Update Notifications

Pressure update notifications are displayed to controllers in the `UKCP_PRESSURE_MONITOR` chat handler.

These messages will display the QNH/QFE before and after. Messages will not be sent for the first time a pressure is
updated, or if the pressure does not change between METARs.

## Querying Pressure

Users may query the plugin to provide the current QNH and QFE at any airfield they wish.

This is achieved by typing a command into the EuroScope chat box, as follows:

`.ukcp pressure EGKK`

The plugin will then respond in the `UKCP_PRESSURE_MONITOR` with the current QNH and QFE at the given airfield.
