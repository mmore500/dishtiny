Classes to manage global simulation state.
The `dish2::ProcWorld` class is just a factory to pump out `dish2::ThreadWorld`s.
Worker threads call `Update` on `dish2::ThreadWorld` objects to actually perform simulation.
