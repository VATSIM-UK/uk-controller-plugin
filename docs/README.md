# uk-controller-plugin

The VATSIM UK Controller Plugin for EuroCcope 3.2 only. This project works in cooperation with
the Plugin API to offer useful functionality to controllers of UK positions on the VATSIM network.

## Feature List

- Open Source Plugin and API Code
- Integration with Core
- Automatic update notifications and version control
- Initial altitude assignment
- Intention codes based on sector exit points
- General and Local squawk assignment
- History Trails
- Minimum Stack Levels
- Countdown Timer with more granular times
- Variable data (Initial Altitudes, Controller Positions, SIDs) updated by VATSIM UK's Sector File Team and downloaded
dynamically to the plugin.
- Notification of required pre-notes
- Expected off-block times
- Actual off-block times
- Estimated departure times
- UK Wake Categories

## Setting Up The Project Locally

- Fork the repo
- Clone the project
- Open the project, we suggest you either use Visual Studio Community Edition or Visual Studio Code (both free)
- You will need to ensure that you have the libaries containing mitigations for the Spectre exploit, information on how to install them may be found in the [installation guide](https://docs.microsoft.com/en-us/cpp/build/reference/qspectre?view=vs-2017)

### Compiling

There are two build configurations available:

- Debug: Compile with all debugging symbols for development. Results in decreased performance and an inflated DLL
- Release: Compile in release mode

Both Visual Studio and Visual Studio Code have build tasks set up for each of the above configurations.

### Running Tests

Tests are powered by Google Test and Google Mock and may be run by compiling and running the `UKControllerPluginTest` project.

Both Visual Studio and Visual Studio Code offer test runner adapters for GoogleTest, to integrate the running of tests
into the development environment.

### Contributing

To contribute to the project, please have a look through our [Contributing Guide](CONTRIBUTING.md).
