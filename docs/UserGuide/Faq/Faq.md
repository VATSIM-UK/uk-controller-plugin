# Frequently Asked Questions

1. [Local Files](#local-files)
2. [Event Logs](#logging)
3. [API Availability Warnings](#api-connectivity)
4. [Displays Are Hidden](#windows-hidden)

## Where can I find the local files that the plugin uses?<a name="local-files"></a>

The plugin stores data in the Local Application Data folder on your machine, in a folder named UKControllerPlugin.

For example: `C:\Users\YourUser\AppData\Local\UKControllerPlugin`

## Where can I find the event log that the plugin generates<a name="logging"></a>

The plugin maintains a log of events that have taken place, which can be used for troubleshooting purposes. If you report a bug with the system, you may be
asked to provide this file.

The file is located within the `logs` subfolder of the local files.

## When I load up the plugin, I get a warning about API availability <a name="api-connectivity"></a>

The UK Controller Plugin ecosystem is made up of two components: the plugin that you have downloaded and a central web service that is managed by VATSIM UK. When the plugin starts up
it communicates with this service to check for version updates and download data.

If the web service is not available, these checks will fail and therefore the plugin will disable some automated functionality in order to ensure that broken versions are not
used on the VATSIM network.

If you see this warning, wait for a few moments and try restarting EuroScope, it may be a temporary blip.

## I can't see some of the display windows, despite turning them on!<a name="windows-hidden"></a>

Try running the `.ukcp resetvisuals` command in EuroScope - this will move everything back to default positions.
