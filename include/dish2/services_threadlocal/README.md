Actions that are performed on each ThreadWorld.
Each ThreadWorld iterates through all registered services and, based on the result of their static `ShouldRun` method, runs them.
For a jump start on writing and hooking up a custom service, use a [project search on an existing service](https://github.com/mmore500/dishtiny/search?q=DiversityMaintenanceService) to see where its defined and plugged in to the rest of the source.
