Utilities and event specifications event-driven computation with SignalGP-Lite.
These source files implement simulation-managed events that were dispatched on virtual CPUs.
Each event is defined as a class with a static `Test` function that examines a `dish2::Cardinal` and decides if the event's trigger should be dispatched on that `dish2::Cardinal`.
Several events may be defined using the same class via template instantiations of the `Test` function over a range of `SeriesIdx` integer values.

In addition to a program, each genome contains an array of tags --- one for each event.
When an event's criteria is met in the simulation, the genome's corresponding tag is used to dispatch a module in the program and launch a core executing that module.

All events are also exposed to the cell as a corresponding input sensor.
The state of the event is stored in the sensor register using a truthy encoding and is then available during virtual CPU execution.
Events are triggered based on the reading of that sensor register (not by re-reading the underlying simulation state).
This means that experimental perturbations that perturb sensor input also disrupted event-handling, allowing the state interface complexity metric to measure both event-driven and sensor-based behaviors.
