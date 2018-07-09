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
already in the game with the scriptable versatility of server created entities.

## Overview

- See the [docs](https://github.com/ScavengeSurvive/item/tree/master/docs) for
  in-depth information on extending the library and advanced topics.

- See the [code](https://github.com/ScavengeSurvive/item/tree/master/item.inc)
  for function reference and documentation.

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
like TDM.

Another note regarding the design behind the way things work: this library has
been used in production and refined since 2012. During this time, the design of
the library has evolved and matured to a point where features adhere to a sort
of guideline (almost a spec, although it's in my head!). Because of this, there
will be very few changes. The library serves a purpose and is very extensible --
almost every facet of how the interface works can be customised or disabled in
some way. It's a very powerful set of tools and basically underpins not only
most of the Scavenge and Survive gameplay mechanics but also the fundamental
design philosophies behind how users interact with the game.

On the topic of extensions, another benefit to this library is there are a ton
of existing libraries to save you development time, including:

- [inventories](https://github.com/ScavengeSurvive/inventory)
- [inventory dialogs](https://github.com/ScavengeSurvive/inventory-dialog)
- [item storage containers](https://github.com/ScavengeSurvive/container)
- [container dialogs](https://github.com/ScavengeSurvive/container-dialog)
- [turn items into weapons](https://github.com/ScavengeSurvive/weapons)
- [use items as wearable storage containers](https://github.com/ScavengeSurvive/itemtype-bag)
- [store arbitrary amounts of data with each item](https://github.com/ScavengeSurvive/item-array-data)
- [use an item to change skin](https://github.com/ScavengeSurvive/itemtype-clothes)
- [combine items together to make new items](https://github.com/ScavengeSurvive/craft)

## Usage

(A quick-start tutorial is in the works!)

## Installation

Simply install to your project:

```bash
sampctl package install ScavengeSurvive/item
```

Include in your code and begin using the library:

```pawn
#include <item>
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```

And connect to `localhost:7777` to test.
