# Item

A feature-rich item library with an massively extensive API.

A complex and flexible script to replace the use of pickups as a means of
displaying objects that the player can pick up and use. Item offers picking up,
dropping and even giving items to other players. Items in the game world consist
of static objects combined with buttons from SIF/Button to provide a means of
interacting.

Item aims to be an extremely flexible script offering a callback for almost
every action the player can do with an item. The script also allows the ability
to add the standard GTA:SA weapons as items that can be dropped, given and
anything else you script items to do.

When picked up, items will appear on the character model bone specified in the
item definition. This combines the visible aspect of weapons and items that are
already in the game with the scriptable versatility of server created and
scriptable entities.

Documentation is in the source code.

## Installation

Simply install to your project:

```bash
sampctl package install ScavengeSurvive/item
```

Include in your code and begin using the library:

```pawn
#include <item>
```

## Overview

This library provides a rich set of tools for interactive entities in the world
that have real, physical and tangible existence. This original idea was to
counter the typical video-game-ness of spinning pickups or glowing orbs. This
means that items are represented using in-game objects. Another major difference
is that players interact with these objects _actively_ rather than _passively_.
What does this mean? When you walk through a typical pickup created with
`CreatePickup`, the effect is realised immediately, with no negotiation from the
player. This may have uses in some gamemodes but this library takes a different
approach. Items in this library require pressing a key to pick up, pressing a
key to use and pressing a key to drop. This is probably better suited to more
realistic gamemodes such as roleplay rather than more arcade style gamemodes
like deathmatch.

Another note regarding the design behind the way things work: this library has
been used in production and refined since 2012. During this time, the design of
the library has evolved and matured to a point where features adhere to a sort
of guideline (almost a spec, although it's in my head!). Because of this, there
will be very few changes. The library serves a purpose and is very extensible --
almost every facet of how the interface works can be customised or disabled in
some way. It's a very powerful set of tools and basically underpins not only
most of the Scavenge and Survive gameplay mechanics but also the fundamental
design philosophies behind how users interact with the game.

### Concepts

With all that out of the way, the underlying concepts of this library are very
simple. I would recommend reading through this section before diving into the
code or examples.

#### `ItemType`

Before items even exist, there must be types. An item type describes a
particular type of item, like a template.

`ItemType:` is actually a tag name used on item type identifiers. This library
makes use of tags to ensure data correctness at compile time.

You don't set the object model of each indiviual item, you set the object model
of an item _type_ then create items _of that type_.

This same concept goes for a multitude of properties, including:

- The attachment offsets that control how the model is oriented when the player
  picks it up.
- The theoretical size in "slots" - an abstract concept used primarily for
  inventory management but also may be used to determine the value of an item.
- Which animation to use while carrying the item - generally speaking,
  physically larger items (such as wooden boxes) should use the two-handed pick
  up, carry and drop animations whereas smaller items that could be held in one
  hand (such as a mobile phone) use standard animations.
- Hit points of the item - this is a simple convenience variable that, when
  reduced to zero, will result in the item automatically being destroyed.

There are many more properties that you can control for item types, see the
forward declaration for `DefineItemType` in the source code for full
documentation.

#### `Item`

Individual items can be thought of as "instances" of `ItemType`s - it's not
quite object-oriented but thinking of it in that way will help.

Items can have three physical states:

- "In World" - they exist in the game world as objects
- "Held" - a player is holding the item, this means the item is _not_ "In World"
- "Virtual" - the item is nowhere and has no physical presence

So when an item is "In World" it has coordinates, it has an Object
(`CreateDynamicObject`) and it has a Button (`CreateButton`) (as well as the
other standard geometrical properties in the game such as an interior and a
virtual world).

When an item is held by a player, this means the player has picked the item up
and is holding the item in their hand. The item is no longer in the game world,
it has no Button and no Object associated with it.

The third state may be confusing. This state is useful for describing an item
that may be stored inside a vehicle or a box (see the Container package). This
means the item is not in the world (`!IsItemInWorld()`) and the item has no
"owner" who is holding it (`GetItemHolder() == INVALID_PLAYER_ID`). The
functions `RemoveItemFromWorld` and `RemoveCurrentItem` can be used together to
achieve this state.

Items don't just need to represent things that players can pick up and drop.
Items can be used for furniture, street signs, trash cans and even houses. You
will see in the next sections how the API can be used to describe almost
anything as an "item".

### Events

Almost every change to the state of an item has an event associated with it.

Furthermore, most of those events provide ways of controlling items via the
return functions. This means you can hook a callback and return 1 for one item
type and 0 for another and that would change how those items work.

This is a fundamental element of the design of this library. Event returns are
incredibly powerful for controlling how items work. After reading this section,
you should have an understanding of how flexible this library is for creating
all sorts of entities.

There are 21 events in total, I won't list them all here because they are
documented in the source code in the `Events` section of the forward
declarations. I will however go over a few of the more interesting ones and
explain how they can be used to achieve very different outcomes.

A few events use tenses to describe when they happened in relation to their
origin. Often, the present-tense version of an event can be used to stop
something from happening but the past-tense version indicates that it has
already happened and there's nothing that can be done at that point.

#### `OnItemCreate`, `OnItemCreated`, `OnItemCreateInWorld` and `OnItemRemoveFromWorld`

These are all called during creation of a brand new item, called by
`CreateItem`. `OnItemCreate` is the present-tense, it does not have any return
values so you can not block an item from being created. This event is called
_before_ an item is placed into the world - in other words when this event is
called, the item has no physical presence yet. It's valid to call `DestroyItem`
on this event which will block the rest of `CreateItem` from working. The reason
this isn't controlled via a return statement is 1. legacy reasons and 2. item
creation is a rather important element of this whole library so controlling this
with a return value would be passing too much responsibility to such a small
line of code that could easily be a typo.

`OnItemCreated` is the past-tense version of `OnItemCreate` and as you can
guess, this is called at the very end of the brand new item creation process.
This signifies that the item has been created, _may_ be in the world (depending
on how it was created - see note about the null-point below) and it is safe to
use any API functions against the item's identifier.

`OnItemCreateInWorld` is called in two situations:

1.  When an brand new item is created - `OnItemCreateInWorld` is called after
    `OnItemCreate` (given that the item wasn't destroyed in that callback). This
    indicates that the item now has physical presence and the geometrical get
    functions (such as position, rotation, virtual world and interior) will
    return valid values.
2.  When an existing item is added to the world - remember how items have three
    states? Items can be removed from the world (when a player picks them up or
    via `RemoveItemFromWorld`). This callback is called **alone** if the item
    already existed but is simply added to the world. This means `OnItemCreate`
    and `OnItemCreated` are _not_ called because the item isn't a _brand new_
    item, it's an existing item that was just added to the world.

However it is _not_ called if the `CreateItem` position arguments (x, y, z)
described the null-point `(0.0, 0.0, 0.0)` of the world. This implicitly turns
the item into a "virtual" item that has no physical existence.

Again, this event has no return controls, it's merely to signal to the script
that an item has entered the world.

Do not call `DestroyItem` on this callback, the item _will_ be destroyed safely
however `OnItemCreated` will still be called with an invalid item identifier.
This behaviour may change in future.

Finally, `OnItemRemoveFromWorld` is called when an item is removed from the game
world. This is called when `RemoveItemFromWorld` is used which also includes
when an item is picked up by a player. This is useful for cases where
`OnPlayerPickUpItem` is too specific and you need to run some code for whenever
an item is removed from the world regardless of how it happened.

#### `OnPlayerPickUpItem` and `OnPlayerPickedUpItem`

This is a tense-pair of events that are called as a player picks up an item from
the game world.

### Extensibility Patterns

(todo)

## Testing

To test, simply run the package:

```bash
sampctl package run
```

And connect to `localhost:7777` to test.
