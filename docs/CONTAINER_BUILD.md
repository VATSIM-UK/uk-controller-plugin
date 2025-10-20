# Building the UKCP using containers

The project provides both a Dockerfile and Dev Container pre-configured for
building the UKCP, to help contributors on different platforms run the build
process easily, and to ensure reproducible builds.

Building can be done using a plain Docker container, or through Visual Studio
Code's Dev Containers feature (recommended for most contributors).

## Setting up a container

### Option 1: Docker container

Install [Docker](https://docs.docker.com/desktop/). Linux users may prefer to
install [Docker Engine](https://docs.docker.com/engine/) only.

After setting up the project locally, build and launch the Docker container with
the "dockerfile" file in the project root, either through Docker Desktop or via
the Docker CLI:

```sh
docker run -it ./dockerfile
```

The process of building the container may take up to 20 minutes, depending on
Internet connection speed.

### Option 2: Dev Container

Install [VS Code](https://code.visualstudio.com/), and
[Dev Containers](https://code.visualstudio.com/docs/devcontainers/containers).

After setting up the project locally, open it in VS Code. Press <kbd>Ctrl</kbd>
<kbd>Shift</kbd> <kbd>P</kbd> to open the command palette. Search for "Dev
Containers: Reopen in Container", and execute it by pressing <kbd>Enter</kbd>.

The process of building the container may take up to 20 minutes, depending on
Internet connection speed.

## Building

To configure the build, first run:

```sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN -G Ninja -B build
```

Then, to execute the build, run:

```sh
ninja -C build UKControllerPluginCore
```

The built DLL is written to "build/bin/UKControllerPluginCore.dll".

When making changes to the code, only the second command need be run to rebuild.

<!-- TODO: debug builds (currently broken), testing, Curl problems -->

## Troubleshooting

Ensure that all submodules are fully cloned before attempting any build
processes.

Ensure that the line endings in all files are preserved; particularly, ensure
that the line endings in the Dockerfile are LF, and are not converted to CRLF.

By default, Git is not installed within the container. Git operations should be
performed on the host machine, or Git installed in the container.
