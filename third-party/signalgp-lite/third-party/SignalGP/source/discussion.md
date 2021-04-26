# Code Review Notes

## 11/19 - Notes

- Decay value in matchbin
  - When to update/decrement?
    - Current: user controlled
    - Do we want to shove into SingleProcess?
    - DecayRegulators function to canonical function
- MatchBin
  - Regulator struct
- Best way to handle regulation?
- Biased instruction representations
  - e.g., don't want certain types of instructions to overwhelm basic GP functionality
  - Achieves virtual copy count
  - Maybe goes into instruction library
  - In mutator? In random generation?
- Should event library be a reference?
  - Yes, but with documentation that event libraries are shared
  - Or, use emp::Ptr<const event_lib>
- Should cur_thread_id be std::optional<size_t> or a custom struct?
  - Probably
- API
  - KillCurThread
  - Kill a thread
  - Clear all threads
- ActivatePending
  - Max heap for Pending, min heap for running; bail when you get a tie
- BaseSignalGP::Reset { ResetImpl(); [inline BaseReset()]; }
  - virtual ResetImpl = 0;
- BaseSignalGP
  - using module_id_t = size_t
- BaseSignalGP
  - priority override in SpawnThreadID

## Notes

- Directory structure
  - Hardware
    - SignalGP
      - SignalGPBase
      - EventLibrary
      - utils
        - IntructionLibrary
        - LinearProgram
        - MemoryModel
      - impls
        - SignalGPToy
        ...

## Discussion Points

- Goal: solidify interface. Want to start using. Okay for us to improve internals
  as we go.

- How should SignalGP virtual hardware be named?
- How should namespaces/file structure/etc be named & organized?
  - e.g., signalgp namespace? instruction implementations for a particular
    hardware loadout?
- Should implementations of functions be at bottom of file? Pro: cleans up class
  definition. Con: implementation signatures are DISGUSTING (because of template madness).

- Getting into Empirical master
  - Do we even want to? Would it be better to have a SignalGP stand-alone repository?
    - It's nice to have it packaged with Empirical. But, hardware directory will
      get really gross.
  - What's the status on MatchBin? Can we get it merged in?
  - What to do with EventDrivenGP? => Move to OLD? Leave alone? Conflicts with updated
    versions of things like

- Implementation details
  - BaseSignalGP
    - Big downside: Template hell.
    - Hard cap on total thread space (pending + active) threads
    - Where do we want configurable lambdas/emp::Signals (for data collect, modification, etc)?
      - think emp::World signals like, OnBirth, OnPlacement, etc
    - CUSTOM_COMPONENT_T
      - Arg for including it in Base class: want all SignalGP classes to have capacity
        for this.
      - Arg against: would clean up templates ever so slightly. Derived classes
        can make this decision for themselves (but this is sort of annoying).
    - struct Thread
      - Nested in BaseSignalGP (don't really want the type to bleed out of scope)
    - Thread management
      - Data structures for each.
      - Management interface.
      - Management visibility.
      - Management consolidation.

    - Can concepts be used to clean anything up?