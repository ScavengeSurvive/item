# Item

A complex and flexible script to replace the use of pickups as a means of displaying objects that the player can pick up and use. Item offers picking up, dropping and even giving items to other players. Items in the game world consist of static objects combined with buttons from SIF/Button to provide a means of interacting.

Item aims to be an extremely flexible script offering a callback for almost every action the player can do with an item. The script also allows the ability to add the standard GTA:SA weapons as items that can be dropped, given and anything else you script items to do.

When picked up, items will appear on the character model bone specified in the item definition. This combines the visible aspect of weapons and items that are already in the game with the scriptable versatility of server created and scriptable entities.

## Usage

Simply add to your `pawn.json` and include:

```json
{
    "dependencies": ["ScavengeSurvive/LIBRARY_NAME"]
}
```

```pawn
#include <LIBRARY_NAME>

#include "your-library.inc"
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```

Then connect to `localhost:7777`.
