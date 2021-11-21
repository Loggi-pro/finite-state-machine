# State machine implementation

This is a header-only library that implements a **common** finite state machine (`FSM`) and **hierarchical** finite state machine (`HSM`).

This implementation is based on the book _"Practical Statecharts in C/C++: Quantum Programming for Embedded Systems"_ and _Quantum Platform Framework_.

## Folders:

- **/naive** - naive (not optimal) different FSM implementations, presented for education purpose only;
- **/fsm** - library source code;
- **/static_fsm** - fsm implemented by variants;
- **/examples** - some examples and FSM\HSM patterns.

## Files:

- **fsm/fsm_types.h** - base types of state machine. This file contains declaration of base `Signal`'s and `Event`'s;
- **fsm/fsm.h** - implementation of `Fsm` class. Base class that implements `Signal` processing and transition between states;
- **fsm/hsm.h** - implementation of `Hsm` class. Base class that implements _hierarchical_ `Signals` processing and transition between states;
- **[optional]fsm/\<platform\>/event_queue.h** - implementation of the event queue for specified platform. This queue is used for the asynchronous event calls, such as defer event and other patterns. Library provides an example of queue implementation for the win32 platform.

# Naive implementations of FSM

## Implementation based on enum switch

File: **dumb_state_machine.h**

Pros:

- simple;
- small memory footprint (only state variable).

Cons:

- signals and states must be enumerated;
- not reusable: all states must be coded for all events;
- dispatch time is `log(N)`, where N - number of cases. Not constant dispatch time in case of switches.

## Implementation based on the table

File: **table_state_machine.h**

Pros:

- maps directly to state table representation;
- reusable: dispatch logic can be encapsulated by generic event processor class `StateTable`;
- constant dispatch time `O(const)`.

Cons:

- signals and states must be enumerated;
- excessive memory usage: class must store large state table;
- require larget amount of function representing actions;
- complicated for adding new states (need to add a new row to table).

## Implementation based on state pattern

File: **state_pattern_machine.h**

Pros:

- no enumerating;
- every new state is partitioned (separated in classes), logic localized in separate classes;
- allows to customize the signature of each event handler;
- moderate memory footprint: requires memory only for a vtable;
- constant dispatch time (virtual/double dispatch).

Cons:

- not practically reusable: transition methods move to general class;
- not effectively portable: this method uses virtual functions, which may not be available on some platforms (embedded).

# Finite state machine

## Basic concepts

`Fsm` class implements basic transition logic. Every state is localized and represented by a simple class method. Transition is happening when dispatching `Signal`.

There are several types of standard signals:

- `SIG_EMPTY`: used by internal FSM logic;
- `SIG_ENTRY`: happens on the transition to a state;
- `SIG_EXIT`: happens on the transition out of a state;
- `SIG_USER`: the first signal that can be used in user code;

`Event` may contain arguments\parameters and wraps every `Signal`. The User can create custom Events, inherited from the base `Event` class.

`Fsm` class contain two methods:

- `void init()` - empty method. The User can override it to set up an initial state and variables values;
- `void dispatch(const Event& ev)` - dispatches an `Event` and makes a transition to a certain state depending on a particular FSM logic.

Pros:

- is simple and has enumerated `Signals`;
- every state is localized in a function;
- reusable, transition logic is localized in the base class
- low memory footprint: requires memory only on state pointer.

Cons:

- don't allow customization of a signature for each event handler;
- has moderate dispatching time: there is only one switch.

## Usage

1. Create class inherited from `Fsm` class:

```c++
class MyFsm : public fsm::Fsm {}
```

2. Create custom `Signal's` or `Event's`:

```c++
enum MySignals {
    SIG_CUSTOM = fsm::ReservedSignals::SIG_USER,
    SIG_CUSTOM2, SIG_CUSTOM3
};
struct MyEvent : fsm::Event { //event with parameters
	uint8_t param;
};
```

3. Code every state as a method with a signature `void <state-name>(const fsm::Event& ev)`. Inside the method you can use protected method `void transition(DerivedState target)` to change state of `fsm` to `target` state:

```c++
void MyFsm::myState(const fsm::Event& ev) {
    switch (ev.sig) {
    case fsm::SIG_EMPTY: break;

    case fsm::SIG_ENTRY: break;

    case fsm::SIG_EXIT: break;

    case SIG_CUSTOM:
        transition(&MyFsm::otherState);
        break;
    }
}
```

4. Using `fsm`:

```c++
MyFsm fsm;
fsm.init();
fsm.dispatch({SIG_CUSTOM});
fsm.dispatch(MyEvent{SIG_CUSTOM_2,1});
```

# Hierarchical finite state machine

## Basic concepts

`Hsm` class implements hierarchical transition logic. Every state is localized and represented by a simple class method. Transition happens when dispatching the `Signal`.

There are several types of standard signals:

- `SIG_EMPTY`: is used by internal FSM logic;
- `SIG_ENTRY`: happens on the transition to a state;
- `SIG_EXIT`: happens on the transition out of a state;
- `SIG_INIT`: happens on the first transition to a state;
- `SIG_USER`: the first signal that can be used in user code;

`Event` may contain arguments\parameters and wraps every `Signal`. The User can create custom Events, inherited from the base `Event` class.

`Hsm` class contain three methods:

- `void init(const Event& e)` - dispatches the event and does a transition to a state with enter logic
- `void dispatch(const Event& ev)` - dispatch `Event` and make transition to some state based on concrete hsm logic;
- `bool isInState(StateHandlerDerived state)` - returns `true` if state machine is in passed state (state hierarchy taken into account).

Pros:

- is simple and has enumerated `Signals`;
- every state is localized in a function;
- reusable, transition logic is localized in the base class
- low memory footprint: requires memory only on state pointer.
- hierarchical

Cons:

- don't allow signature customization for each event handler;
- has moderate dispatch time: there is only one switch.

## Usage

1. Create class inherited from `Hsm` class:

```c++
class MyHsm : public hsm::Hsm {}
```

2. Create custom `Signals` or `Events`:

```c++
enum MySignals {
    SIG_CUSTOM = hsm::ReservedSignals::SIG_USER,
    SIG_CUSTOM2, SIG_CUSTOM3
};
struct MyEvent : hsm::Event { //event with parameters
	uint8_t param;
};
```

3. Code every state as method with signature `hsm::EventResult <state-name>(const hsm::Event& ev)`.

   3.1. On signal handler: when transition needed this method must return `eventTransition(&<state>)`.

   3.2. On signal handler: if no transition needed, method must return `eventHandled()`.

   3.3. In other cases method must return parent state, or `eventSuper(&Hsm::top)` if no parent state exists.

```c++
hsm::EventResult MyHsm::code(const hsm::Event& e) { //state handler
    switch (e.sig) {
        case SIG_CUSTOM: return eventTransition(&MyHsm::otherState);
        case SIG_CUSTOM_2: return eventHandled();
        case SIG_CUSTOM_3: return eventHandled();
    }
    return eventSuper(&Hsm::top);
}
```

4. Using `hsm`:

```c++
MyHsm hsm;
hsm.init();
hsm.dispatch({SIG_CUSTOM});
hsm.dispatch(MyEvent{SIG_CUSTOM_2,1});
```

# Event queue

Sometimes, `HSM` or `FSM` must handle the new `Event` asynchronously. In this case, `Event` must be placed in the event queue. The background task must continuously poll the event queue and dispatch events taken from it.

As an example, event queue is implemented for win32 platform in **event_queue.h** by `HsmEventQueue` class.

> This class stores only **reference** to `Events` (Event itself must be stored by calling side).

Queue contains two methods:

- method `void putEvent(const fsm::Event& ev, hsm::Hsm& obj)` puts `Event ev` to the queue.
- method `void checkQueue()` pops the first event from the queue, and calls `dispatch` on the corresponging `hsm`.

# Patterns

Some patterns to be used with the state machine:

- **Ultimate hook** (examples/ultimate_hook.h) - example shows how to make a derived class with overridden methods, without using of virtual utilities;
- **Reminder** (examples/reminder.h) - example uses the event queue to fire asynchronous event on timeout (when alarm happened);
- **Deffered event** (examples/deffered_event.h) - example implement server which try to handle `Events`. If the server is busy, it defers `Events` in the event queue. When the server gets free, it recalls `Events` from the event queue;
- **Orthogonal** (examples/orthogonal.h) - example shows how to combine nested `HSM`. There are two `HSM`, one of which handles time representation, while the other handles time management;
- **Transition to history** (examples/transition_to_history.h) - is an example showing how to save current state and return back to this state programmaticaly.

# Examples

- **fsm parser** (examples/fsm_parser.h) - is c/c++ style block comment parser based on flat `fsm`;
- **hsm parser** (examples/hsm_parser.h) - is c/c++ style block comment parser based on hierarchical `hsm`;
- **kitchen timer** (examples/kitchen_timer.h) - is an example of a kitchen timer, that can be disarmed by entering a code.
