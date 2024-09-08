# Cpp-api

This is a living document, it will change as the project progresses.

## Branching rules

- The main branch is `main`. Direct push to main is not allowed. On the main there is only a stable version of the application that is covered with unit tests.
- The `development` branch is the branch for developing the application. This branch contains an application with complete features, and after completing a certain set of features, this branch is merged into the `main` branch. The `development` branch must not be merged with the `main` branch until all the code is covered with unit tests.
- The `readme` branch is the branch where the documentation is written. This branch is merged with the main branch after every documentation update.
- Feature branches have a name in the format `FE-feature_number: Feature name`. After the implementation of a certain feature, this branch is merged into the `development` branch. After merging the feature branch into the `development` branch, the mentioned feature branch should be deleted.

## FEATURE LIST:

### FE-1: HTTP(S) Server

Missing description

### FE-2: Logger

The `Logger` class provides a thread-safe mechanism for logging messages with various levels of severity. It is designed to handle log messages safely in a multithreaded environment and supports multiple logging levels.

#### Key Features

- **Thread-Safety**:

  - Uses `std::mutex` to ensure that logging operations are thread-safe.
  - Prevents race conditions and ensures consistent log output even when multiple threads are writing logs simultaneously.

- **Log Levels**:

  - Supports multiple log levels: `Debug`, `Info`, `Warning`, and `Error`.
  - Allows filtering of log messages based on their importance, helping you control the verbosity of logging output.

- **Timestamping**:
  - Each log message is prefixed with a timestamp.
  - Provides context for when the message was logged, aiding in debugging and tracking application behavior over time.
