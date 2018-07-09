# Extensibility Patterns

The primary role of this library is to provide a rich API to easily build simple
or complex interaction systems. Knowing how to extend the library in a
future-proof way is key to using it effectively. The majority of the time, you
do not want to script events for specific items but instead for specific item
_types_. This minimises code repetition and enables you to build a rich gallery
of item types for all kinds of things. From keys for houses and cars, to weapon
modifications.

## Writing Good ItemType Code

If you write your item type code in a specific way, it can be shared and used by
others. On top of this, building an ItemType script in this way will make it
easier to debug, test, isolate and extend.

In a gamemode, there will be several (or hundreds) of **Item Type Definitions**.
They look like this:

```pawn
new ItemType:Medkit;
public OnGameModeInit() {
    Medkit = DefineItemType("Medkit", "Medkit", 11736, 1, 0.0, 0.0, 0.0, 0.004, 0.197999, 0.038000, 0.021000, 79.700012, 0.000000, 90.899978);
}
```

This is a simple Medical supplies ItemType. Say you wanted to make it heal the
player on use, you could write:

```pawn
hook OnPlayerUseItem(playerid, Item:itemid) {
    if(GetItemType(itemid) == Medkit) {
        SetPlayerHealth(playerid, 100.0);
        DestroyItem(itemid);
    }
}
```

This is simple but it works. The item is destroyed and the player is given full
health.

However this code is far too coupled to the definition of the `Medkit` ItemType.
If you moved this code to a new gamemode, or changed the `Medkit` ItemType to a
new name, you would also need to change every check on the `Medkit` variable.

There is a much better way, and that is to provide **ItemType Bindings**.

This pattern simply requires adding a function to your Medkit script named
`DefineItemTypeMedkit` with the first parameter as an `ItemType:` tag. The goal
of this function is to store the ItemType that the script wants to use _locally_
to that script, rather than relying on a globally declared variable.

```pawn
static ItemType:MedkitItemType;
stock DefineItemTypeMedkit(ItemType:itemType) {
    MedkitItemType = itemtype;
}
```

And at the item declaration, you simply call the binding function and remove the
need for a global variable:

```pawn
public OnGameModeInit() {
    new ItemType:medkit = DefineItemType("Medkit", "Medkit", 11736, 1, 0.0, 0.0, 0.0, 0.004, 0.197999, 0.038000, 0.021000, 79.700012, 0.000000, 90.899978);
    DefineItemTypeMedkit(medkit);
}
```

Using this pattern, you reduce the amount of global variables, reduce the
coupling between the gamemode and the medkit script and make your medkit script
easily sharable and usable by someone else. Which is great news for the open
source community!
