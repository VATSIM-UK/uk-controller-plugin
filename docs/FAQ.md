# Frequently Asked Questions

Below is a non-exhaustive list of the most frequently asked questions and common sticking points about the UK Controller Plugin and the answers to them.

## Where can I find the logs for the plugin?

The plugin logs everything it does to `<EuroScope folder in My Documents>/ukcp/logs/eventlog.txt`

## When I start the plugin, I get an error saying an "Unknown Exception" has occured

This is often caused when an Anti Virus program prevents the plugin from writing to its log files. It may be fixed by adding the `ukcp` folder within the `EuroScope` documents folder to
the list of exempt folders.

## When I start the plugin, I get an error saying that the plugin is unable to authorise itself with the API and that core functionality is disabled

When the plugin starts, it contacts the web API to check a few things:

- That your account on the API is active, is not banned and your API key is not valid.
- That the version of the plugin you are using is still allowed for use and if updates are available.

If the plugin cannot connect to the API, some functionality that requires you to be authenticated (e.g. squawk assignments) is disabled. This is to protect other
users of the VATSIM network from a rougue plugin, as there have been issues in the past where plugins have been arbitrarily assigning squawks all over the world.

This issue can sometimes be caused by a slow internet connection that takes too long to connect to the API to perform these checks. It is also possible that the API itself is down
and thus not accepting connections - to check this you can access `ukcp.vatsim.uk` in your web browser. If you see an access denied message, then the API is working as expected.

## When the plugin starts, it asks me to go to the website and download my personal access token

This error occurs when you have either not installed your personal access token or it has expired. You can regenerate your personal access token by visiting
`vatsim.uk/ukcp` and logging in when prompted.

## Squawks aren't being assigned automatically

The plugin assigns squawks automatically provided that you have successfully authenticated with the API on startup and one of the following conditions are met:

- For aircraft on the ground, that you "own" the particular airfield
- For aircraft in the air, that you are "tracking" the aircraft in EuroScope

To check if you own a particular airfield, you can run one of the following commands. Note, these are case sensitive:

`.ukcp owner EGLL` - To check which controller owns EGLL
`.ukcp owned LON_S_CTR` - To check which airfields are owned by LON_S_CTR
