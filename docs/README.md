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
- Download and build [Boost](https://www.boost.org/users/download/)
- Set your `BOOST_ROOT` and `BOOST_LIBRARYDIR` environment variables in Windows.
- Download [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html)
- Set your `OPENSSL_ROOT` AND `OPENSSL_LIBRARYDIR` environment variables in Windows.

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

To contribute to the project, please have a look at the [Contributing Guide](CONTRIBUTING.md).
