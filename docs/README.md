# uk-controller-plugin

The VATSIM UK Controller Plugin for EuroScope 3.2 only. This project works in cooperation with
the Plugin API to offer useful functionality to controllers of UK positions on the VATSIM network.

[![Build](https://github.com/VATSIM-UK/uk-controller-plugin/workflows/Build/badge.svg)](https://github.com/VATSIM-UK/uk-controller-plugin/actions)
[![semantic-release](https://img.shields.io/badge/%20%20%F0%9F%93%A6%F0%9F%9A%80-semantic--release-e10079.svg)](https://github.com/semantic-release/semantic-release)
[![Commitizen friendly](https://img.shields.io/badge/commitizen-friendly-brightgreen.svg)](http://commitizen.github.io/cz-cli/)

## Setting Up The Project Locally

- Fork the repo
- Clone the project
- Open the project, we suggest you either use Visual Studio Community Edition or Visual Studio Code (both free)
- You will need to ensure that you have the libraries containing mitigation for the Spectre exploit, information on how to install them may be found in the [installation guide](https://docs.microsoft.com/en-us/cpp/build/reference/qspectre?view=vs-2017)

### Compiling

There are two build configurations available:

- Debug: Compile with all debugging symbols for development. Results in decreased performance and an inflated DLL.
- Release: Compile in release mode, with compiler optimisations.

Compiling may be achieved by:

- Running CMake followed by nmake - see the workflow file for GitHub Actions.
	- This can be done most easily by using the provided Dockerfile - see the [instructions](CONTAINER_BUILD.md).
- Using your favourite IDE (e.g. Visual Studio / CLion) to build the project.

### Running Tests

Tests are powered by Google Test and Google Mock and may be run by compiling and running the three "Test" projects.

Most code editors offer test runner adapters for GoogleTest, to integrate the running of tests
into the development environment.

### Code Style

Every pull request for this repository is checked using `clang-format`. Clang format may be configured in your code editor of choice to run locally, as well!

### Contributing

To contribute to the project, please have a look at the [Contributing Guide](CONTRIBUTING.md).

## Design Rationale

There are a number of technical decisions that have been made during the development of the plugin to improve the development experience and also to make the code clean with separation of concerns.

### Event Driven

EuroScope is a highly event-driven program, with events being sent to the plugin for everything from flights being updated, to controllers logging on, to radar blips moving across the screen.

The plugin is therefore built around the idea of "event handlers" - classes that follow a particular interface and process events that are passed to them. Each type of event will have a collection of handlers that is called, which allows us to avoid having all the event handling logic directly in the methods that EuroScope calls when an event occurs!

### Asynchronous for long running operations

EuroScope plugins run on a single thread, the same thread that most of EuroScope uses, therefore it is necessary to ensure that long-running operations do not block this thread.

Calls to the web API are handled on a separate thread, as well as other HTTP requests to external resources and integrations with other plugins and programs.

### Interfaces around External Classes

EuroScope provides a number of classes that represent data within the program, such as Flightplans and Controller Positions. Most of these classes are not directly instantiable and thus are difficult to write tests with. Therefore, the plugin provides interfaces and wrappers for these external classes, allowing them to be mocked for the purposes of testing.

### Everything gets a test

Unlike a web application, a program that runs on a VATSIM members machine is much harder to debug when something goes wrong, and everyone's setup is slightly different. Therefore, every class in the plugin, as far as practicable, has an automated test to ensure that nothing gets broken when we change something.
