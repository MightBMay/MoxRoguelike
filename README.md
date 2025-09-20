# MoxRogue and MoxEngine
## MoxEngine
MoxEngine is a basic game engine i am making for more experiance with game engines, and c++ in general. it uses SFML 3.0 for most system related things (graphics, window creation, etc), as well as FMOD for more versitile audio.

## Moxrogue
Moxrouge is an idea i had for quite a while of a simple roguelike game, akin to vampire survivors, with a heavy theme around a DND character i made, which is a sentient cardboard box.


# for class ( INFR-3110U)

May (marcus) Yorke, 100874913

## In Lab Assignment 1:
    I use the singleton pattern in many places in my game. I use it in places where it would make logical sense to only ever have one of a class at any time, and global access may be benificial.
    For example in GameObject.h/cpp, in my GameObjectManager class,i use the singleton by deleting all functions allowing copying and external creation (constructor for the class is private), and have a static instance within my getInstance() method.

    I do this as it allows me to access the gameobject manager globally, and because my engine is laid out to only ever have one of these at a time ( the class handles updating gameobjects, and drawing them. multiple copies would cause many issues). 
    this is benificial because it insures in the other aspects of my game where i may need to manually do something that wouldn't be easy/efficient to do in a normal gameobject or component on a gameobject, 
    that i can easily access the needed methods, as well as garenteeing all code works with the same object. 

    the most common example of the above, is with sprites and rendering objects. i have the option to create a gameobject with no sprite, as some times i must assign a sprite to the object at a later time.
    To avoid trying to draw any gameobject that does not have a valid sprite assigned, i only add them to the vector of things to render if a valid sprite ptr is found. thus to change a sprite, or add one to a spriteless object, i have to manually add to that vector.
    this can be seen in the remove/addExternalRenderable() method, which allows me to add renderables which may not be attached to a gameobject ( commonly used to contain multiple sprites for ui all in one component on one gameobject, instead of like 10 different objects). 

    Diagram:
    [Diagram of my most common use case for my GameObjectManager's singleton pattern](Diagrams/ILA1_Diagram.png)