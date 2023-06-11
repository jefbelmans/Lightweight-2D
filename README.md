# Lightweight 2D
Lightweight 2D is a lightweight 2D game engine developed by Jef Belmans at Digital Arts and Entertainment. It is designed to provide a flexible and efficient framework for developing 2D games. This README provides an overview of the engine's key features and design patterns (Explenation courtesy of [Robert Nystrom](https://gameprogrammingpatterns.com/)).

## Architecture
The engine's architecture takes inspiration from the Unity engine, utilizing concepts such as GameObjects and Components. Each scene in Lightweight 2D consists of multiple GameObjects, where each GameObject represents a distinct object in the game world. The behavior and visual representation of these objects are defined by the Components attached to them.

## Event Pattern
The engine incorporates the event pattern, inspired by UnityEvent, to facilitate effective communication and interaction between components and systems. The event pattern follows a publish-subscribe model, allowing components to subscribe to specific events and receive notifications when those events occur.

By implementing the event pattern, Lightweight 2D achieves a decoupled and flexible approach to inter-component communication. This enables different game elements, such as entities, systems, and UI elements, to communicate without direct dependencies, promoting modularity and extensibility.

The event pattern involves two main elements: event invokers and event listeners. Event invokers are responsible for triggering events, while event listeners register themselves to receive notifications when specific events occur. This separation promotes loose coupling between components, enabling efficient communication and facilitating modular design.

## Finite State Machine (FSM) with Blackboard
The FSM with Blackboard is another powerful feature implemented in Lightweight 2D. This feature provides a flexible and efficient way of managing game logic and behavior, inspired by the concept of Finite State Machines.

A Finite State Machine (FSM) is a computational model that represents the behavior of an entity or system through states, transitions, and associated actions. In Lightweight 2D, the FSM is implemented with the concept of a Blackboard, which serves as a shared memory space for data sharing.

The FSM manages the current state of an entity or system and executes actions associated with that state. The Blackboard enables efficient communication and coordination between states by providing a central storage system for shared data. This eliminates complex dependencies and direct coupling between states.

### Benefits and Features
- Modularity and Flexibility: The FSM allows easy addition and removal of states, making it highly modular and adaptable to various gameplay scenarios.
- Dynamic Transitions: States can define conditions for transitioning to other states, enabling dynamic and context-aware behavior.
- Actions and Behaviors: Each state can define a set of actions, such as movement, animations, and sound effects, allowing for diverse game-specific behaviors.
- Blackboard Data Sharing: The Blackboard facilitates data sharing between states, promoting effective communication and coordination between different parts of the game.
- Simplified Logic: The FSM with Blackboard breaks down complex game logic into manageable states, enhancing readability, maintainability, and extensibility.

## Singleton Pattern and Service Locator Pattern
In addition to the event pattern and FSM with Blackboard, Lightweight 2D also utilizes the Singleton Pattern and Service Locator Pattern to provide global access to services in a decoupled manner.

### Singleton Pattern
The Singleton Pattern ensures that only one instance of a class is created throughout the runtime of the game engine. It provides a global point of access to this instance, allowing components and systems to access the service without coupling them to the concrete class that implements it.

In Lightweight 2D, the Singleton Pattern is employed to manage essential engine services, such as the input system and resource manager. By using singletons, these services can be easily accessed from anywhere in the codebase without the need for direct dependencies.

### Service Locator Pattern
The Service Locator Pattern is a design pattern that allows components and systems to obtain services without explicitly knowing the concrete classes that implement them. It acts as a registry or directory of services, providing a global point of access to retrieve the desired service by using a common interface.

In Lightweight 2D, the Service Locator Pattern is implemented through a centralized Service Locator object. This object maintains a collection of registered services and provides methods to retrieve them (like the SoundSystem). Components and systems can request a service through the Service Locator without having explicit knowledge of the implementation details.

By using the Service Locator Pattern, Lightweight 2D achieves loose coupling between components and services, further promoting modularity and flexibility. It allows for easy swapping of service implementations and facilitates testing and mocking of services during development.

# Example Project: Pac-Man
The Lightweight 2D engine comes with an example project that demonstrates the capabilities of the engine by recreating Pac-Man. The example project is structured as two projects within the Visual Studio solution.

## Engine Library (Static Lib)
The engine library project is a static library that contains the core functionality of the Lightweight 2D engine. It provides the foundation for building 2D games, including features such as the event pattern, FSM with Blackboard, Singleton Pattern, and Service Locator Pattern. The engine library can be utilized as a dependency for other projects, enabling developers to leverage the engine's capabilities in their own games.

## Pac-Man Project
The Pac-Man project showcases the usage of the Lightweight 2D engine by implementing a fully playable Pac-Man game. It demonstrates how the engine's components, systems, and design patterns can be utilized to create a simple but complete game experience.

Within the Pac-Man project, you will find:
- Game-specific logic: The project contains game-specific scripts, components, and systems that define the behavior of the Pac-Man characters, ghosts, pellets, and other elements of the game.
- Asset integration: The project demonstrates how to integrate assets, such as sprites, sounds, and level data (binary and JSON), into the game using the Lightweight 2D engine's resource management system.
- Scene setup: The Pac-Man game project includes pre-built scenes that set up the game environment, including the maze layout, character placement, and initial game state.
- Input handling: The project showcases how the engine's input system can be utilized to capture player input (Keyboard and Controller) for controlling the different characters.
- Game loop and update logic: The example project demonstrates how the engine's game loop and update loop can be utilized to drive the game's logic, handle collisions, update AI behavior, and manage game state transitions.

To run the Pac-Man project, simply build and run the Pac-Man project within the Visual Studio solution. You can customize and extend the game by modifying the classes, components, and systems provided in the project.

Enjoy exploring the capabilities of the Lightweight 2D engine and have fun playing Pac-Man!
