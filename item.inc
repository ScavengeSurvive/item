// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_item
	#undef _inc_item
#endif
// custom include-guard to ensure we don't duplicate
#if defined _item_included
	#endinput
#endif
#define _item_included

#include <a_samp>
#include <logger>
#include <YSI\y_iterate>
#include <YSI\y_timers>
#include <streamer>
#include <geid>
#include <button>
#include <mathutil>
#include <tick-difference>
#include <action-text>
#include <personal-space>

#include <YSI\y_hooks>


/*==============================================================================

	Constant Definitions, Function Declarations and Documentation

==============================================================================*/


// Maximum amount of items that can be created.
#if !defined MAX_ITEM
	#define MAX_ITEM (8192)
#endif

// Maximum amount of item types that can be defined.
#if !defined MAX_ITEM_TYPE
	#define MAX_ITEM_TYPE (ItemType:256)
#endif

// Maximum string length for item type names.
#if !defined MAX_ITEM_NAME
	#define MAX_ITEM_NAME (32)
#endif

// Maximum string length for item specific extra text.
#if !defined MAX_ITEM_TEXT
	#define MAX_ITEM_TEXT (32)
#endif

// Item attachment index for SetPlayerAttachedObject native.
#if !defined ITEM_ATTACH_INDEX
	#define ITEM_ATTACH_INDEX (0)
#endif

// Key code for interact key
#if !defined ITEM_KEY_INTERACT
	#define ITEM_KEY_INTERACT (16)
#endif

// Key code for dropping items
#if !defined ITEM_KEY_DROP_ITEM
	#define ITEM_KEY_DROP_ITEM (KEY_NO)
#endif

// Game text symbol for interact key
#if !defined ITEM_GAMETEXT_INTERACT
	#define ITEM_GAMETEXT_INTERACT "~k~~VEHICLE_ENTER_EXIT~"
#endif

// Game text symbol for dropping items
#if !defined ITEM_GAMETEXT_DROP_ITEM
	#define ITEM_GAMETEXT_DROP_ITEM "~k~~CONVERSATION_NO~"
#endif


// Offset from player Z coordinate to floor Z coordinate
#define ITEM_FLOOR_OFFSET (0.96)

// Item validity check constant
#define INVALID_ITEM_ID (-1)

// Item type validity check constant
#define INVALID_ITEM_TYPE (ItemType:-1)


// Functions


forward CreateItem(ItemType:type, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1, virtual = 0, geid[] = "", hitpoints = -1);
/*
# Description
Creates an item in the game world at the specified coordinates with the
specified rotation with options for world, interior and whether or not to
display a 3D text label above the item.

# Parameters
- type: An item type defined with DefineItemType.
- x, y, z: The position to create the object and button of the item.
- rx, ry, rz: The rotation value of the object, overrides item type data.
- world: The virtual world in which the object, button and label will appear.
- interior: Interior world, same as above but for interior worlds.
- label: True to show a label with the item name at the item.
- applyrotoffsets: False to make rx, ry, rz rotation values absolute.
- virtual: When true, the item doesn't have an in-world existence.
- geid: Allows specific GEID to be set instead of generating one.
- hitpoints: -1 by default, if -1 then item type maxhitpoints value is used.

# Returns
Item ID handle of the newly created item or INVALID_ITEM_ID If the item index is
full and no more items can be created.
*/

forward DestroyItem(itemid, &indexid = -1, &worldindexid = -1);
/*
# Description
Destroys an item.

# Returns
Boolean to indicate success or failure.
*/

forward ItemType:DefineItemType(name[], uname[], model, size, Float:rotx = 0.0, Float:roty = 0.0, Float:rotz = 0.0, Float:modelz = 0.0, Float:attx = 0.0, Float:atty = 0.0, Float:attz = 0.0, Float:attrx = 0.0, Float:attry = 0.0, Float:attrz = 0.0, bool:usecarryanim = false, colour = -1, boneid = 6, bool:longpickup = false, Float:buttonz = ITEM_FLOOR_OFFSET, maxhitpoints = 5);
/*
# Description
Defines a new item type with the specified name and model. Item types are the
fundamental pieces of data that give items specific characteristics. At least
one item definition must exist or CreateItem will have no data to use.

# Parameters
- name: The name of the item, that will be displayed on item labels.
- uname: The unique name of the item to identify the item type via string.
- model: The GTA:SA model id to use when the item is visible in the game world.
- size: An arbitrary size value (has no effect in this module).
- rotx, roty, rotz: The default rotation the item object will have when dropped.
- modelz: Z offset from the item world position to create item model.
- attx, atty, attz, attrx, attry, attrz: Player holding attachment coordinates.
- usecarryanim: When true, player will use a two-handed carry animation.
- colour: Item model texture colour.
- boneid: The attachment bone to use, by default this is the right hand (6).
- longpickup: When true, requires long press to pick up, tap results in using.
- buttonz: Z offset from the item world position to create item button.
- maxhitpoints: maximum and default hitpoints for items of this type.

# Returns
Item Type ID handle of the newly defined item type. INVALID_ITEM_TYPE If the
item type definition index is full and no more item types can be defined.
*/

forward PlayerPickUpItem(playerid, itemid);
/*
# Description
A function to directly make a player pick up an item, regardless of whether he
is within the button range.
*/

forward PlayerDropItem(playerid);
/*
# Description
Force a player to drop their currently held item.

# Returns
1 If the function was called successfully 0 If the player isn't holding an item
or if the function was stopped by a return of 1 in OnPlayerDropItem.
*/

forward PlayerGiveItem(playerid, targetid, call = true);
/*
# Description

# Returns
1 If the give was successful. 0 If the player isn't holding an item, of the
function was stopped by a return of 1 in OnPlayerGiveItem. -1 If the target
player was already holding an item.
*/

forward PlayerUseItem(playerid);
/*
# Description
Forces a player to use their current item, resulting in a call to OnPlayerUseItem.
*/

forward GiveWorldItemToPlayer(playerid, itemid, call = 1);
/*
# Description
Give a world item to a player.

# Parameters
- playerid: The player to give the item to.
- itemid: The ID handle of the item to give to the player.
- call: Determines whether OnPLayerPickUpItem is called.

# Returns
Boolean to indicate success or failure.
*/

forward RemoveCurrentItem(playerid);
/*
# Description
Removes the player's currently held item and places it in the world.

# Returns
INVALID_ITEM_ID If the player ID is invalid or the player isn't holding an item.
*/

forward RemoveItemFromWorld(itemid);
/*
# Description
Removes an item from the world. Deletes all physical elements but keeps the item
in memory with a valid ID and removes the ID from the world index. Effectively
makes the item a "virtual" item, as in it still exists in the server memory but
it doesn't exist physically in the game world.

# Returns
Boolean to indicate success or failure.
*/

forward AllocNextItemID(ItemType:type, geid[] = "");
/*
# Description
Preallocates an item ID for a specific item type. This doesn't actually create
an item but it makes the ID valid so item related functions can be called on it
to set various pieces of data before the item is created. Useful for setting
data that needs to be valid for when OnItemCreate(InWorld) is called.

# Returns
The allocated ID of the item or INVALID_ITEM_ID If there are no more free item
slots or -2 If the specified type is invalid.
*/

forward CreateItem_ExplicitID(itemid, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1, virtual = 0, hitpoints = -1);
/*
# Description
Creates an item using an ID allocated from AllocNextItemID. This is the only
function that can create items that were preallocated.

# Parameters:
Apart from the explicit item ID, parameters are the same as CreateItem.

# Returns
1 On success or 0 if the ID is invalid or is destroyed in OnItemCreate.
*/

forward bool:IsValidItem(itemid);
/*
# Description
Returns whether the entered value is a valid item ID handle.
*/

forward GetItemObjectID(itemid);
/*
# Description
Returns the streamed object ID for a world item. Cannot be a virtual item.
*/

forward GetItemButtonID(itemid);
/*
# Description
Returns the button ID of a world item. Cannot be a virtual item.
*/

forward SetItemLabel(itemid, text[], colour = 0xFFFF00FF, Float:range = 10.0);
/*
# Description
Creates or updates a 3D text label above the item. This is actually the label
which is associated with the button for the item, so you could just call
GetItemButtonID then use SetButtonLabel but this is just here for convenience.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemTypeCount(ItemType:itemtype);
/*
# Description
Returns the amount of created items of the given type.
*/

forward ItemType:GetItemType(itemid);
/*
# Description
Returns the item type of an item.
*/

forward GetItemPos(itemid, &Float:x, &Float:y, &Float:z);
/*
# Description
Returns the position of a world item. If used on a non-world item such as an
item being held by a player, it will return the last position of the item.

# Returns
Boolean to indicate success or failure.
*/

forward SetItemPos(itemid, Float:x, Float:y, Float:z);
/*
# Description
Changes the position of an item. This includes the associated object and button.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemRot(itemid, &Float:rx, &Float:ry, &Float:rz);
/*
# Description
Returns the rotation of a world item.

# Returns
Boolean to indicate success or failure.
*/

forward SetItemRot(itemid, Float:rx, Float:ry, Float:rz, bool:offsetfromdefaults = false);
/*
# Description
Sets the rotation of a world item object.

# Parameters
- offsetfromdefaults: If true, parameters are offsets from item type defaults.

# Returns
Boolean to indicate success or failure.
*/

forward SetItemWorld(itemid, world);
/*
# Description
Sets an item's virtual world.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemWorld(itemid);
/*
# Description
Returns an item's virtual world.
*/

forward SetItemInterior(itemid, interior);
/*
# Description
Sets an item's interior ID.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemInterior(itemid);
/*
# Description
Returns an item's interior ID.
*/

forward SetItemHitPoints(itemid, hitpoints);
/*
# Description
Sets an item's hitpoint value, destroys the item if 0.
*/

forward GetItemHitPoints(itemid);
/*
# Description
Returns an item's hitpoint value.
*/

forward SetItemExtraData(itemid, data);
/*
# Description
Sets the item's extra data field, this is one cell of data space allocated for
each item, this value can be a simple value or point to a cell in a more complex
set of data to act as extra characteristics for items.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemExtraData(itemid, &data);
/*
# Description
Retrieves the integer assigned to the item set with SetItemExtraData.
*/

forward SetItemNameExtra(itemid, string[]);
/*
# Description
Gives the item a unique string of text to be appended to the name in brackets.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemNameExtra(itemid, string[]);
/*
# Description
Retrieves the unique string of text assigned to an item.

# Returns
Boolean to indicate success or failure.
*/

forward bool:IsValidItemType(ItemType:itemtype);
/*
# Description
Checks whether a value is a valid item type.
*/

forward GetItemTypeName(ItemType:itemtype, string[]);
/*
# Description
Retrieves the name of an item type.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemTypeUniqueName(ItemType:itemtype, string[]);
/*
# Description
Gets the unique name for an item type.

# Returns
Boolean to indicate success or failure.
*/

forward ItemType:GetItemTypeFromUniqueName(string[], bool:ignorecase = false);
/*
# Description
Returns an item type for the specified unique item type name via lookup.

# Returns
Boolean to indicate success or failure.
*/

forward GetItemTypeModel(ItemType:itemtype, &model);
/*
# Description
Returns the model assigned to an item type.
*/

forward GetItemTypeSize(ItemType:itemtype, &size);
/*
# Description
Returns the defined size of an item type.

# Returns
0 if itemtype is invalid.
*/

forward GetItemTypeRotation(ItemType:itemtype, &Float:rx, &Float:ry, &Float:rz);
/*
# Description
Returns the rotation offsets defined in DefineItemType for an item type.

# Returns
0 if itemtype is invalid.
*/

forward bool:IsItemTypeCarry(ItemType:itemtype);
/*
# Description
Returns true if the itemtype uses two-handed carry animations set via the
usecarryanim parameter of DefineItemType.
*/

forward GetItemTypeColour(ItemType:itemtype, &colour);
/*
# Description
Returns the default colour of an item type.
*/

forward GetItemTypeBone(ItemType:itemtype);
/*
# Description
Returns the bone that an item type will attach the mesh to.
*/

forward bool:IsItemTypeLongPickup(ItemType:itemtype);
/*
# Description
Returns true if the item requires holding the pick-up key in order to pick up.
*/

forward bool:IsItemDestroying(itemid);
/*
# Description
Returns true in OnItemDestroy calls, use to prevent unwanted recursion.
*/

forward GetItemHolder(itemid);
/*
# Description
Returns the ID of the player who is holding an item.

# Returns
INVALID_PLAYER_ID if the item isn't being held by anyone.
*/

forward GetPlayerItem(playerid);
/*
# Description
Returns the item ID handle of the item a player is holding.

# Returns
INVALID_ITEM_ID if the player isn't holding an item.
*/

forward bool:IsItemInWorld(itemid);
/*
# Description
Checks if an item is in the game world regardless of whether or not the item
exists at all (in other words, the function returning false gives no indication
of whether or not the item is not in the world or just doesn't exist)
*/

forward GetItemFromButtonID(buttonid);
/*
# Description
Returns the item ID associated with the specified button ID (if any).

# Returns
INVALID_ITEM_ID if the button is not associated with any item.
*/

forward GetItemName(itemid, string[]);
/*
# Description
Returns the name of the type of the specified item ID and appends the unique
text assigned to the item to the end.

# Returns
Boolean to indicate success or failure.
*/

forward GetPlayerInteractingItem(playerid);
/*
# Description
Returns the ID handle of the item that <playerid> is interacting with. This
means either picking up, dropping or giving.
*/

forward GetPlayerNearbyItems(playerid, list[]);
/*
# Description:
Stores a list of items the player is within interaction range of into <list>.
*/

forward GetNextItemID();
/*
# Description:
Returns the next item ID in the index that is unused. Useful for determining
what ID an item will have before calling CreateItem and thus OnItemCreated.
*/

forward GetItemsInRange(Float:x, Float:y, Float:z, Float:range = 300.0, items[], maxitems = sizeof(items));
/*
# Description:
Returns a list of items in range of the specified point. Uses streamer cells so
the range is limited and will only list items in the surrounding cells.
*/


// Events


forward OnItemTypeDefined(uname[]);
/*
# Called
After an item type is defined.
*/

forward OnItemCreate(itemid);
/*
# Called
As an item is created.
*/

forward OnItemCreated(itemid);
/*
# Called
After an item is created.
*/

forward OnItemDestroy(itemid);
/*
# Called
Before an item is destroyed, the itemid is still valid and existing.
*/

forward OnItemDestroyed(itemid);
/*
# Called
After an item is destroyed, itemid is now invalid.
*/

forward OnItemCreateInWorld(itemid);
/*
# Called
After an existing (already created with CreateItem) item is created in the game world (for instance, after a player drops the item, or directly after it is created with CreateItem)
*/

forward OnItemRemoveFromWorld(itemid);
/*
# Called
After an item is removed from the world either by being given to a player or by calling RemoveItemFromWorld
*/

forward OnPlayerUseItem(playerid, itemid);
/*
# Called
When a player presses F/Enter while holding an item.
*/

forward OnPlayerUseItemWithItem(playerid, itemid, withitemid);
/*
# Called
When a player uses a held item with an item in the world.
*/

forward OnPlayerUseItemWithButton(playerid, buttonid, itemid);
/*
# Called
When a player uses an item while in the area of a button from an item that is in the game world.
*/

forward OnPlayerRelButtonWithItem(playerid, buttonid, itemid);
/*
# Called
When a player releases the interact key after calling OnPlayerUseItemWithButton.
*/

forward OnPlayerPickUpItem(playerid, itemid);
/*
# Called
When a player presses the button to pick up an item.

# Returns
1 To cancel the pickup request, no animation will play.
*/

forward OnPlayerPickedUpItem(playerid, itemid);
/*
# Called
When a player finishes the picking up animation.

# Returns
1 To cancel giving the item ID to the player.
*/

forward OnPlayerGetItem(playerid, itemid);
/*
# Called
When a player acquires an item from any source.
*/

forward OnPlayerDropItem(playerid, itemid);
/*
# Called
When a player presses the button to drop an item.

# Returns
1 To cancel the drop, no animation will play and the player will keep their item.
*/

forward OnPlayerDroppedItem(playerid, itemid);
/*
# Called
When a player finishes the animation for dropping an item.
*/

forward OnPlayerGiveItem(playerid, targetid, itemid);
/*
# Called
When a player presses the button to give an item to another player.

# Returns
1 To cancel the give request, no animations will play.
*/

forward OnPlayerGivenItem(playerid, targetid, itemid);
/*
# Called
When a player finishes the animation for giving an item to another player.

# Returns
1 To cancel removing the item from the giver and the target receiving the item.
*/

forward OnItemRemovedFromPlayer(playerid, itemid);
/*
# Called
When an item is removed from a player's hands without him dropping it (through a script action)
*/

forward OnItemNameRender(itemid, ItemType:itemtype);
/*
# Called
When the function GetItemName is called, so an additional piece of text can be added to items giving more information unique to that specific item.
*/

forward OnItemHitPointsUpdate(itemid, oldvalue, newvalue);
/*
# Called
When an item's hitpoints value is changed.
*/


/*==============================================================================

	Setup

==============================================================================*/


enum E_ITEM_TYPE_DATA {
	itm_name[MAX_ITEM_NAME],
	itm_uname[MAX_ITEM_NAME],
	itm_model,
	itm_size,

	Float:itm_offsetRotX,
	Float:itm_offsetRotY,
	Float:itm_offsetRotZ,
	Float:itm_zModelOffset,
	Float:itm_zButtonOffset,

	Float:itm_attachPosX,
	Float:itm_attachPosY,
	Float:itm_attachPosZ,

	Float:itm_attachRotX,
	Float:itm_attachRotY,
	Float:itm_attachRotZ,
	bool:itm_useCarryAnim,

	itm_colour,
	itm_attachBone,
	bool:itm_longPickup,
	itm_maxHitPoints
}

enum E_ITEM_DATA {
	itm_objId,
	itm_button,
	ItemType:itm_type,

	Float:itm_posX,
	Float:itm_posY,
	Float:itm_posZ,
	Float:itm_rotX,
	Float:itm_rotY,
	Float:itm_rotZ,
	itm_world,
	itm_interior,
	itm_hitPoints,

	itm_exData,
	itm_nameEx[MAX_ITEM_TEXT],
	itm_geid[GEID_LEN]
}


static
	itm_TypeData[MAX_ITEM_TYPE][E_ITEM_TYPE_DATA],
	itm_TypeCount[MAX_ITEM_TYPE],
	itm_TypeTotal;

static
	itm_Data[MAX_ITEM][E_ITEM_DATA],
	bool:itm_Destroying[MAX_ITEM],
	itm_Interactor[MAX_ITEM],
	itm_Holder[MAX_ITEM];

new
	Iterator:itm_Index<MAX_ITEM>,
	Iterator:itm_WorldIndex<MAX_ITEM>,
	itm_ButtonIndex[BTN_MAX];

static
	itm_Holding[MAX_PLAYERS],
	itm_LongPickupTick[MAX_PLAYERS],
	itm_Interacting[MAX_PLAYERS],
	itm_CurrentButton[MAX_PLAYERS],
	Timer:itm_InteractTimer[MAX_PLAYERS],
	Timer:itm_LongPickupTimer[MAX_PLAYERS];


/*==============================================================================

	Zeroing

==============================================================================*/


hook OnScriptInit() {
	for(new i; i < MAX_PLAYERS; i++) {
		itm_Holding[i] = INVALID_ITEM_ID;
		itm_Interacting[i] = INVALID_ITEM_ID;
	}

	for(new i; i < MAX_ITEM; i++) {
		itm_Holder[i] = INVALID_PLAYER_ID;
	}

	for(new i; i < BTN_MAX; i++) {
		itm_ButtonIndex[i] = INVALID_ITEM_ID;
	}

	return 1;
}

hook OnPlayerConnect(playerid) {
	itm_Holding[playerid] = INVALID_ITEM_ID;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	itm_CurrentButton[playerid] = INVALID_BUTTON_ID;
	stop itm_InteractTimer[playerid];
	stop itm_LongPickupTimer[playerid];
}

hook OnPlayerDisconnect(playerid, reason) {
	stop itm_InteractTimer[playerid];
	stop itm_LongPickupTimer[playerid];
}


/*==============================================================================

	Core Functions

==============================================================================*/


stock CreateItem(ItemType:type, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1, virtual = 0, geid[] = "", hitpoints = -1) {
	new itemid = Iter_Free(itm_Index);

	if(itemid == INVALID_ITERATOR_SLOT) {
		err("MAX_ITEM limit reached");
		return INVALID_ITEM_ID;
	}

	if(!IsValidItemType(type)) {
		err("Item creation with undefined typeid failed.",
			_i("type", _:type));
		return INVALID_ITEM_ID;
	}


	Iter_Add(itm_Index, itemid);

	if(geid[0] == EOS) {
		mkgeid(itemid, itm_Data[itemid][itm_geid]);
	} else {
		strcat(itm_Data[itemid][itm_geid], geid, GEID_LEN);
	}

	itm_Data[itemid][itm_type] = type;
	itm_Data[itemid][itm_hitPoints] = hitpoints == -1 ? itm_TypeData[itm_Data[itemid][itm_type]][itm_maxHitPoints] : hitpoints;
	itm_TypeCount[type]++;

	CallLocalFunction("OnItemCreate", "d", itemid);

	if(!Iter_Contains(itm_Index, itemid)) {
		return INVALID_ITEM_ID;
	}

	if(x == 0.0 && y == 0.0 && z == 0.0) {
		virtual = 1;
	}

	if(!virtual) {
		CreateItemInWorld(itemid, x, y, z, rx, ry, rz, world, interior, label, applyrotoffsets, hitpoints);
	}

	CallLocalFunction("OnItemCreated", "d", itemid);

	return itemid;
}

stock DestroyItem(itemid, &indexid = -1, &worldindexid = -1) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	if(itm_Destroying[itemid]) {
		return 2;
	}

	itm_Destroying[itemid] = true;

	CallLocalFunction("OnItemDestroy", "d", itemid);

	if(itm_Holder[itemid] != INVALID_PLAYER_ID) {
		if(itm_TypeData[itm_Data[itemid][itm_type]][itm_useCarryAnim]) {
			SetPlayerSpecialAction(itm_Holder[itemid], SPECIAL_ACTION_NONE);
		}

		RemovePlayerAttachedObject(itm_Holder[itemid], ITEM_ATTACH_INDEX);
		itm_Holding[itm_Holder[itemid]] = INVALID_ITEM_ID;
		itm_Interacting[itm_Holder[itemid]] = INVALID_ITEM_ID;
		stop itm_InteractTimer[itm_Holder[itemid]];
	} else if(Iter_Contains(itm_WorldIndex, itemid)) {
		DestroyDynamicObject(itm_Data[itemid][itm_objId]);
		DestroyButton(itm_Data[itemid][itm_button]);
		itm_ButtonIndex[itm_Data[itemid][itm_button]] = INVALID_BUTTON_ID;
	}

	itm_TypeCount[itm_Data[itemid][itm_type]]--;

	itm_Data[itemid][itm_objId] = -1;
	itm_Data[itemid][itm_button] = INVALID_BUTTON_ID;
	itm_Holder[itemid] = INVALID_PLAYER_ID;
	itm_Interactor[itemid] = INVALID_PLAYER_ID;

	Iter_SafeRemove(itm_Index, itemid, indexid);
	Iter_SafeRemove(itm_WorldIndex, itemid, worldindexid);

	CallLocalFunction("OnItemDestroyed", "d", itemid);

	itm_Destroying[itemid] = false;

	return 0;
}

stock ItemType:DefineItemType(name[], uname[], model, size, Float:rotx = 0.0, Float:roty = 0.0, Float:rotz = 0.0, Float:modelz = 0.0, Float:attx = 0.0, Float:atty = 0.0, Float:attz = 0.0, Float:attrx = 0.0, Float:attry = 0.0, Float:attrz = 0.0, bool:usecarryanim = false, colour = -1, boneid = 6, bool:longpickup = false, Float:buttonz = ITEM_FLOOR_OFFSET, maxhitpoints = 5) {
	new ItemType:id = ItemType:itm_TypeTotal;

	if(id == MAX_ITEM_TYPE) {
		fatal("MAX_ITEM_TYPE limit reached");
		return INVALID_ITEM_TYPE;
	}

	// Check if any other items have this uname
	for(new i = _:id - 1; i >= 0; i--) {
		if(!strcmp(uname, itm_TypeData[ItemType:i][itm_uname], true)) {
			err("attempt to define two item types with the same unique name",
				_s("uname", uname));
			return INVALID_ITEM_TYPE;
		}
	}

	itm_TypeTotal++;

	format(itm_TypeData[id][itm_name], MAX_ITEM_NAME, name);
	format(itm_TypeData[id][itm_uname], MAX_ITEM_NAME, uname);
	itm_TypeData[id][itm_model]			= model;
	itm_TypeData[id][itm_size]			= size;

	itm_TypeData[id][itm_offsetRotX]	= rotx;
	itm_TypeData[id][itm_offsetRotY]	= roty;
	itm_TypeData[id][itm_offsetRotZ]	= rotz;
	itm_TypeData[id][itm_zModelOffset]	= modelz;
	itm_TypeData[id][itm_zButtonOffset]	= buttonz;

	itm_TypeData[id][itm_attachPosX]	= attx;
	itm_TypeData[id][itm_attachPosY]	= atty;
	itm_TypeData[id][itm_attachPosZ]	= attz;

	itm_TypeData[id][itm_attachRotX]	= attrx;
	itm_TypeData[id][itm_attachRotY]	= attry;
	itm_TypeData[id][itm_attachRotZ]	= attrz;
	itm_TypeData[id][itm_useCarryAnim]	= usecarryanim;

	itm_TypeData[id][itm_colour]		= colour;
	itm_TypeData[id][itm_attachBone]	= boneid;
	itm_TypeData[id][itm_longPickup]	= longpickup;
	itm_TypeData[id][itm_maxHitPoints]	= maxhitpoints;

	CallLocalFunction("OnItemTypeDefined", "s", uname);

	return id;
}

stock PlayerPickUpItem(playerid, itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	if(Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return 2;
	}

	new
		Float:x,
		Float:y,
		Float:z;

	GetPlayerPos(playerid, x, y, z);

	ClearAnimations(playerid);
	SetPlayerPos(playerid, x, y, z);
	SetPlayerFacingAngle(playerid, GetAngleToPoint(x, y, itm_Data[itemid][itm_posX], itm_Data[itemid][itm_posY]));

	if((z - itm_Data[itemid][itm_posZ]) < 0.3) { // If the height between the player and the item is below 0.5 units
		if(itm_TypeData[itm_Data[itemid][itm_type]][itm_useCarryAnim]) {
			ApplyAnimation(playerid, "CARRY", "liftup105", 5.0, 0, 0, 0, 0, 400);
		} else {
			ApplyAnimation(playerid, "CASINO", "SLOT_PLYR", 4.0, 0, 0, 0, 0, 0);
		}
		itm_InteractTimer[playerid] = defer PickUpItemDelay(playerid, itemid, 1);
	} else {
		if(itm_TypeData[itm_Data[itemid][itm_type]][itm_useCarryAnim]) {
			ApplyAnimation(playerid, "CARRY", "liftup", 5.0, 0, 0, 0, 0, 400);
		} else {
			ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 5.0, 0, 0, 0, 0, 450);
		}
		itm_InteractTimer[playerid] = defer PickUpItemDelay(playerid, itemid, 0);
	}

	itm_Interacting[playerid] = itemid;
	itm_Interactor[itemid] = playerid;

	return 0;
}

stock PlayerDropItem(playerid) {
	if(!Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return 1;
	}

	if(CallLocalFunction("OnPlayerDropItem", "dd", playerid, itm_Holding[playerid])) {
		return 2;
	}

	if(itm_TypeData[itm_Data[itm_Holding[playerid]][itm_type]][itm_useCarryAnim]) {
		ApplyAnimation(playerid, "CARRY", "putdwn", 5.0, 0, 0, 0, 0, 0);
	} else {
		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 5.0, 1, 0, 0, 0, 450);
	}

	itm_InteractTimer[playerid] = defer DropItemDelay(playerid);

	return 0;
}

stock PlayerGiveItem(playerid, targetid, call = true) {
	if(!(0 <= playerid < MAX_PLAYERS)) {
		return 1;
	}

	if(!Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return 2;
	}

	new itemid = itm_Holding[playerid];

	if(call) {
		if(CallLocalFunction("OnPlayerGiveItem", "ddd", playerid, targetid, itemid)) {
			return 3;
		}
	}

	if(!Iter_Contains(itm_Index, itemid)) {
		return 4;
	}

	if(Iter_Contains(itm_Index, itm_Holding[targetid])) {
		return 5;
	}

	new
		Float:x1,
		Float:y1,
		Float:z1,
		Float:x2,
		Float:y2,
		Float:z2,
		Float:angle;

	GetPlayerPos(targetid, x1, y1, z1);
	GetPlayerPos(playerid, x2, y2, z2);

	angle = GetAngleToPoint(x2, y2, x1, y1);

	SetPlayerFacingAngle(playerid, angle);
	SetPlayerFacingAngle(targetid, angle+180.0);

	if(!itm_TypeData[itm_Data[itemid][itm_type]][itm_useCarryAnim]) {
		ApplyAnimation(playerid, "CASINO", "SLOT_PLYR", 4.0, 0, 0, 0, 0, 450);
		ApplyAnimation(targetid, "CASINO", "SLOT_PLYR", 4.0, 0, 0, 0, 0, 450);
	} else {
		SetPlayerSpecialAction(targetid, SPECIAL_ACTION_CARRY);
	}

	itm_Interacting[playerid]	= targetid;
	itm_Interacting[targetid]	= playerid;
	itm_Holder[itemid]			= playerid;

	itm_InteractTimer[playerid] = defer _item_giveItem(playerid, targetid);

	return 0;
}

stock PlayerUseItem(playerid) {
	return _item_onPlayerUseItem(playerid, itm_Holding[playerid]);
}

stock GiveWorldItemToPlayer(playerid, itemid, call = 1) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	RemoveItemFromWorld(itemid);
	RemoveCurrentItem(GetItemHolder(itemid));

	new ItemType:type = itm_Data[itemid][itm_type];

	itm_Data[itemid][itm_posX]		= 0.0;
	itm_Data[itemid][itm_posY]		= 0.0;
	itm_Data[itemid][itm_posZ]		= 0.0;

	itm_Holding[playerid]			= itemid;
	itm_Holder[itemid]				= playerid;
	itm_Interacting[playerid]		= INVALID_ITEM_ID;
	itm_Interactor[itemid]			= INVALID_PLAYER_ID;

	SetPlayerAttachedObject(
		playerid, ITEM_ATTACH_INDEX, itm_TypeData[type][itm_model], itm_TypeData[type][itm_attachBone],
		itm_TypeData[type][itm_attachPosX], itm_TypeData[type][itm_attachPosY], itm_TypeData[type][itm_attachPosZ],
		itm_TypeData[type][itm_attachRotX], itm_TypeData[type][itm_attachRotY], itm_TypeData[type][itm_attachRotZ],
		.materialcolor1 = itm_TypeData[type][itm_colour], .materialcolor2 = itm_TypeData[type][itm_colour]);

	if(itm_TypeData[type][itm_useCarryAnim]) {
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	}

	if(call) {
		if(CallLocalFunction("OnPlayerGetItem", "dd", playerid, itemid)) {
			return 2;
		}

		if(!Iter_Contains(itm_Index, itemid)) {
			return 3;
		}
	}

	return 0;
}

stock RemoveCurrentItem(playerid) {
	if(!(0 <= playerid < MAX_PLAYERS)) {
		return INVALID_ITEM_ID;
	}

	if(!Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return INVALID_ITEM_ID;
	}

	new itemid = itm_Holding[playerid];

	RemovePlayerAttachedObject(playerid, ITEM_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	itm_Holding[playerid] = INVALID_ITEM_ID;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	itm_Holder[itemid] = INVALID_PLAYER_ID;
	itm_Interactor[itemid] = INVALID_PLAYER_ID;

	CallLocalFunction("OnItemRemovedFromPlayer", "dd", playerid, itemid);

	return itemid;

}

stock RemoveItemFromWorld(itemid) {
	if(!Iter_Contains(itm_Index, _:itemid)) {
		return 1;
	}

	if(!Iter_Contains(itm_WorldIndex, _:itemid)) {
		return 2;
	}

	if(itm_Holder[itemid] != INVALID_PLAYER_ID) {
		err("RemoveItemFromWorld unexpected state player was holding item that was in-world.",
			_i("playerid", itm_Holder[itemid]),
			_i("itemid", itemid));

		RemoveCurrentItem(itm_Holder[itemid]);
	}

	DestroyDynamicObject(itm_Data[itemid][itm_objId]);

	DestroyButton(itm_Data[itemid][itm_button]);

	itm_ButtonIndex[itm_Data[itemid][itm_button]] = INVALID_BUTTON_ID;
	itm_Data[itemid][itm_objId] = -1;
	itm_Data[itemid][itm_button] = INVALID_BUTTON_ID;

	Iter_Remove(itm_WorldIndex, itemid);

	CallRemoteFunction("OnItemRemoveFromWorld", "d", itemid);

	return 0;
}

stock AllocNextItemID(ItemType:type, geid[] = "") {
	new itemid = Iter_Free(itm_Index);

	if(itemid == -1) {
		err("MAX_ITEM limit reached");
		return INVALID_ITEM_ID;
	}

	if(!IsValidItemType(type)) {
		err("Item creation with undefined typeid failed.",
			_i("type", _:type));
		return INVALID_ITEM_ID;
	}

	itm_Data[itemid][itm_type] = type;
	Iter_Add(itm_Index, itemid);

	if(geid[0] == EOS) {
		mkgeid(itemid, itm_Data[itemid][itm_geid]);
	} else {
		strcat(itm_Data[itemid][itm_geid], geid, GEID_LEN);
	}

	return itemid;
}

stock CreateItem_ExplicitID(itemid, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1, virtual = 0, hitpoints = -1) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	itm_TypeCount[itm_Data[itemid][itm_type]]++;

	CallLocalFunction("OnItemCreate", "d", itemid);

	if(!Iter_Contains(itm_Index, itemid)) {
		return 2;
	}

	if(x == 0.0 && y == 0.0 && z == 0.0) {
		virtual = 1;
	}

	itm_Data[itemid][itm_hitPoints] = hitpoints == -1 ? itm_TypeData[itm_Data[itemid][itm_type]][itm_maxHitPoints] : hitpoints;

	if(!virtual) {
		CreateItemInWorld(itemid, x, y, z, rx, ry, rz, world, interior, label, applyrotoffsets, hitpoints);
	}

	CallLocalFunction("OnItemCreated", "d", itemid);

	return 0;
}

stock bool:IsValidItem(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return false;
	}

	return true;
}

// itm_objId
stock GetItemObjectID(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return INVALID_OBJECT_ID;
	}

	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return INVALID_OBJECT_ID;
	}

	return itm_Data[itemid][itm_objId];
}

// itm_button
stock GetItemButtonID(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return INVALID_BUTTON_ID;
	}

	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return INVALID_BUTTON_ID;
	}

	return itm_Data[itemid][itm_button];
}

stock SetItemLabel(itemid, text[], colour = 0xFFFF00FF, Float:range = 10.0) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	SetButtonLabel(itm_Data[itemid][itm_button], text, colour, range);

	return 0;
}

stock GetItemTypeCount(ItemType:itemtype) {
	if(!IsValidItemType(itemtype)) {
		return -1;
	}

	return itm_TypeCount[itemtype];
}

// itm_type
stock ItemType:GetItemType(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return INVALID_ITEM_TYPE;
	}

	return itm_Data[itemid][itm_type];
}

// itm_posX
// itm_posY
// itm_posZ
stock GetItemPos(itemid, &Float:x, &Float:y, &Float:z) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	x = itm_Data[itemid][itm_posX];
	y = itm_Data[itemid][itm_posY];
	z = itm_Data[itemid][itm_posZ];

	return 0;
}

stock SetItemPos(itemid, Float:x, Float:y, Float:z) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	itm_Data[itemid][itm_posX] = x;
	itm_Data[itemid][itm_posY] = y;
	itm_Data[itemid][itm_posZ] = z;

	SetButtonPos(itm_Data[itemid][itm_button], x, y, z + itm_TypeData[itm_Data[itemid][itm_type]][itm_zButtonOffset]);
	SetDynamicObjectPos(itm_Data[itemid][itm_objId], x, y, z + itm_TypeData[itm_Data[itemid][itm_type]][itm_zModelOffset]);

	return 0;
}

stock GetItemRot(itemid, &Float:rx, &Float:ry, &Float:rz) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	rx = itm_Data[itemid][itm_rotX];
	ry = itm_Data[itemid][itm_rotY];
	rz = itm_Data[itemid][itm_rotZ];

	return 0;
}

stock SetItemRot(itemid, Float:rx, Float:ry, Float:rz, bool:offsetfromdefaults = false) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return 2;
	}

	if(offsetfromdefaults) {
		SetDynamicObjectRot(itm_Data[itemid][itm_objId],
			itm_TypeData[itm_Data[itemid][itm_type]][itm_offsetRotX] + rx,
			itm_TypeData[itm_Data[itemid][itm_type]][itm_offsetRotY] + ry,
			itm_TypeData[itm_Data[itemid][itm_type]][itm_offsetRotZ] + rz);
	} else {
		SetDynamicObjectRot(itm_Data[itemid][itm_objId], rx, ry, rz);
	}

	itm_Data[itemid][itm_rotX] = rx;
	itm_Data[itemid][itm_rotY] = ry;
	itm_Data[itemid][itm_rotZ] = rz;

	return 0;
}

// itm_world
stock SetItemWorld(itemid, world) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return 2;
	}

	SetButtonWorld(itm_Data[itemid][itm_button], world);
	itm_Data[itemid][itm_world] = world;

	return 0;
}

stock GetItemWorld(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return -1;
	}

	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return -2;
	}

	return itm_Data[itemid][itm_world];
}

// itm_interior
stock SetItemInterior(itemid, interior) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return 2;
	}

	SetButtonInterior(itm_Data[itemid][itm_button], interior);
	itm_Data[itemid][itm_interior] = interior;

	return 0;
}

stock GetItemInterior(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return -1;
	}

	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return -2;
	}

	return itm_Data[itemid][itm_interior];
}

// itm_hitPoints
stock SetItemHitPoints(itemid, hitpoints) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	new old = itm_Data[itemid][itm_hitPoints];

	itm_Data[itemid][itm_hitPoints] = hitpoints;

	CallLocalFunction("OnItemHitPointsUpdate", "ddd", itemid, old, hitpoints);

	if(itm_Data[itemid][itm_hitPoints] <= 0) {
		DestroyItem(itemid);
	}

	return 0;
}

stock GetItemHitPoints(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return -1;
	}

	return itm_Data[itemid][itm_hitPoints];
}

// itm_exData
stock SetItemExtraData(itemid, data) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	itm_Data[itemid][itm_exData] = data;

	return 0;
}

stock GetItemExtraData(itemid, &data) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	data = itm_Data[itemid][itm_exData];

	return 0;
}

// itm_nameEx
stock SetItemNameExtra(itemid, string[]) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	itm_Data[itemid][itm_nameEx][0] = EOS;
	strcat(itm_Data[itemid][itm_nameEx], string, MAX_ITEM_TEXT);

	return 0;
}

stock GetItemNameExtra(itemid, string[]) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	string[0] = EOS;
	strcat(string, itm_Data[itemid][itm_nameEx], MAX_ITEM_TEXT);

	return 0;
}

// itm_geid
stock GetItemGEID(itemid, string[]) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	string[0] = EOS;
	strcat(string, itm_Data[itemid][itm_geid], GEID_LEN);

	return 0;
}

stock bool:IsValidItemType(ItemType:itemtype) {
	if(ItemType:0 <= itemtype < ItemType:itm_TypeTotal) {
		return true;
	}

	return false;
}

// itm_name
stock GetItemTypeName(ItemType:itemtype, string[]) {
	if(!IsValidItemType(itemtype)) {
		return 1;
	}

	string[0] = EOS;
	strcat(string, itm_TypeData[itemtype][itm_name], MAX_ITEM_NAME);

	return 0;
}

// itm_uname
stock GetItemTypeUniqueName(ItemType:itemtype, string[]) {
	if(!IsValidItemType(itemtype)) {
		return 1;
	}

	string[0] = EOS;
	strcat(string, itm_TypeData[itemtype][itm_uname], MAX_ITEM_NAME);

	return 0;
}

stock ItemType:GetItemTypeFromUniqueName(string[], bool:ignorecase = false) {
	if(isnull(string)) {
		return INVALID_ITEM_TYPE;
	}

	for(new i; i < itm_TypeTotal; i++) {
		if(!strcmp(string, itm_TypeData[ItemType:i][itm_uname], ignorecase)) {
			return ItemType:i;
		}
	}

	return INVALID_ITEM_TYPE;
}

// itm_model
stock GetItemTypeModel(ItemType:itemtype, &model) {
	if(!IsValidItemType(itemtype)) {
		return 1;
	}

	model = itm_TypeData[itemtype][itm_model];
	return 0;
}

// itm_size
stock GetItemTypeSize(ItemType:itemtype, &size) {
	if(!IsValidItemType(itemtype)) {
		return 1;
	}

	size = itm_TypeData[itemtype][itm_size];
	return 0;
}

// itm_offsetRotX
// itm_offsetRotY
// itm_offsetRotZ
stock GetItemTypeRotation(ItemType:itemtype, &Float:rx, &Float:ry, &Float:rz) {
	if(!IsValidItemType(itemtype)) {
		return 1;
	}

	rx = itm_TypeData[itemtype][itm_offsetRotX];
	ry = itm_TypeData[itemtype][itm_offsetRotY];
	rz = itm_TypeData[itemtype][itm_offsetRotZ];

	return 0;
}

// itm_useCarryAnim
stock bool:IsItemTypeCarry(ItemType:itemtype) {
	if(!IsValidItemType(itemtype)) {
		return false;
	}

	return itm_TypeData[itemtype][itm_useCarryAnim];
}

// itm_colour
stock GetItemTypeColour(ItemType:itemtype, &colour) {
	if(!IsValidItemType(itemtype)) {
		return 1;
	}

	colour = itm_TypeData[itemtype][itm_colour];
	return 0;
}

// itm_attachBone
stock GetItemTypeBone(ItemType:itemtype) {
	if(!IsValidItemType(itemtype)) {
		return -1;
	}

	return itm_TypeData[itemtype][itm_attachBone];
}

// itm_longPickup
stock bool:IsItemTypeLongPickup(ItemType:itemtype) {
	if(!IsValidItemType(itemtype)) {
		return false;
	}

	return itm_TypeData[itemtype][itm_longPickup];
}

// itm_maxHitPoints
stock GetItemTypeMaxHitPoints(ItemType:itemtype) {
	if(!IsValidItemType(itemtype)) {
		return -1;
	}

	return itm_TypeData[itemtype][itm_maxHitPoints];
}

// itm_Destroying
stock bool:IsItemDestroying(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return false;
	}

	return itm_Destroying[itemid];
}

// itm_Holder
stock GetItemHolder(itemid) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return INVALID_PLAYER_ID;
	}

	return itm_Holder[itemid];
}

// itm_Holding
stock GetPlayerItem(playerid) {
	if(!Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return INVALID_ITEM_ID;
	}

	if(!(0 <= playerid < MAX_PLAYERS)) {
		return INVALID_ITEM_ID;
	}

	return itm_Holding[playerid];
}

stock bool:IsItemInWorld(itemid) {
	if(!Iter_Contains(itm_WorldIndex, itemid)) {
		return false;
	}

	return true;
}

// itm_ButtonIndex
stock GetItemFromButtonID(buttonid) {
	if(!IsValidButton(buttonid)) {
		return INVALID_ITEM_ID;
	}

	return itm_ButtonIndex[buttonid];
}

stock GetItemName(itemid, string[]) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	string[0] = EOS;
	strcat(string, itm_TypeData[itm_Data[itemid][itm_type]][itm_name], MAX_ITEM_NAME + MAX_ITEM_TEXT);

	CallLocalFunction("OnItemNameRender", "dd", itemid, _:itm_Data[itemid][itm_type]);

	if(!isnull(itm_Data[itemid][itm_nameEx])) {
		strcat(string, " (", MAX_ITEM_NAME + MAX_ITEM_TEXT);
		strcat(string, itm_Data[itemid][itm_nameEx], MAX_ITEM_NAME + MAX_ITEM_TEXT);
		strcat(string, ")", MAX_ITEM_NAME + MAX_ITEM_TEXT);
	}

	return 0;
}

stock GetPlayerInteractingItem(playerid) {
	if(!IsPlayerConnected(playerid)) {
		return INVALID_ITEM_ID;
	}

	return itm_Interacting[playerid];
}

stock GetPlayerNearbyItems(playerid, list[]) {
	new
		buttons[BTN_MAX_INRANGE],
		buttoncount,
		itemcount;

	GetPlayerButtonList(playerid, buttons, buttoncount, true);

	for(new i; i < buttoncount; ++i) {
		if(Iter_Contains(itm_Index, itm_ButtonIndex[buttons[i]])) {
			list[itemcount++] = itm_ButtonIndex[buttons[i]];
		}
	}

	return itemcount;
}

stock GetNextItemID() {
	return Iter_Free(itm_Index);
}

stock GetItemsInRange(Float:x, Float:y, Float:z, Float:range = 300.0, items[], maxitems = sizeof(items)) {
	new
		streamer_items[256],
		streamer_count,
		data[2],
		itemid,
		count;

	streamer_count = Streamer_GetNearbyItems(x, y, z, STREAMER_TYPE_AREA, streamer_items, .range = range);

	for(new i; i < streamer_count && count < maxitems; ++i) {
		Streamer_GetArrayData(STREAMER_TYPE_AREA, streamer_items[i], E_STREAMER_EXTRA_ID, data);

		if(data[0] != BTN_STREAMER_AREA_IDENTIFIER) {
			continue;
		}

		itemid = GetItemFromButtonID(data[1]);

		if(IsValidItem(itemid)) {
			items[count++] = itemid;
		}
	}

	return count;
}


/*==============================================================================

	Internal Functions and Hooks

==============================================================================*/


CreateItemInWorld(
	itemid,
	Float:x = 0.0, Float:y = 0.0, Float:z = 0.0,
	Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0,
	world = 0, interior = 0, label = 1, applyrotoffsets = 1, hitpoints = -1
) {
	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	if(Iter_Contains(itm_WorldIndex, itemid)) {
		return 2;
	}

	new ItemType:itemtype = itm_Data[itemid][itm_type];

	if(!IsValidItemType(itemtype)) {
		return 3;
	}

	itm_Data[itemid][itm_posX] = x;
	itm_Data[itemid][itm_posY] = y;
	itm_Data[itemid][itm_posZ] = z;
	itm_Data[itemid][itm_rotX] = rx;
	itm_Data[itemid][itm_rotY] = ry;
	itm_Data[itemid][itm_rotZ] = rz;
	itm_Data[itemid][itm_world] = world;
	itm_Data[itemid][itm_interior] = interior;
	itm_Data[itemid][itm_hitPoints] = hitpoints == -1 ? itm_TypeData[itm_Data[itemid][itm_type]][itm_maxHitPoints] : hitpoints;

	if(itm_Holder[itemid] != INVALID_PLAYER_ID) {
		RemovePlayerAttachedObject(itm_Holder[itemid], ITEM_ATTACH_INDEX);
		SetPlayerSpecialAction(itm_Holder[itemid], SPECIAL_ACTION_NONE);

		itm_Holding[itm_Holder[itemid]] = INVALID_ITEM_ID;
		itm_Interacting[itm_Holder[itemid]] = INVALID_ITEM_ID;
	}

	itm_Interactor[itemid] = INVALID_PLAYER_ID;
	itm_Holder[itemid] = INVALID_PLAYER_ID;

	if(applyrotoffsets) {
		itm_Data[itemid][itm_objId] = CreateDynamicObject(itm_TypeData[itemtype][itm_model],
			x, y, z + itm_TypeData[itemtype][itm_zModelOffset],
			rx + itm_TypeData[itemtype][itm_offsetRotX],
			ry + itm_TypeData[itemtype][itm_offsetRotY],
			rz + itm_TypeData[itemtype][itm_offsetRotZ],
			world, interior, .streamdistance = 100.0);
	} else {
		itm_Data[itemid][itm_objId] = CreateDynamicObject(itm_TypeData[itemtype][itm_model],
			x, y, z + itm_TypeData[itemtype][itm_zModelOffset], rx, ry, rz,
			world, interior, .streamdistance = 100.0);
	}

	itm_Data[itemid][itm_button] = CreateButton(x, y, z + itm_TypeData[itemtype][itm_zButtonOffset], "Press ~k~~VEHICLE_ENTER_EXIT~ to pick up", world, interior, 1.0, .testlos = false);

	if(itm_Data[itemid][itm_button] == INVALID_BUTTON_ID) {
		err("invalid button ID during CreateItemInWorld",
			_i("itemid", itemid));
		return -3;
	}

	itm_ButtonIndex[itm_Data[itemid][itm_button]] = itemid;

	if(itm_TypeData[itemtype][itm_colour] != -1) {
		SetDynamicObjectMaterial(itm_Data[itemid][itm_objId], 0, itm_TypeData[itemtype][itm_model], "invalid", "invalid", itm_TypeData[itemtype][itm_colour]);
	}

	if(label) {
		SetButtonLabel(itm_Data[itemid][itm_button], itm_TypeData[itemtype][itm_name], .range = 2.0, .testlos = false);
	}

	Iter_Add(itm_WorldIndex, itemid);

	CallLocalFunction("OnItemCreateInWorld", "d", itemid);

	return 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if(IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING) {
		return Y_HOOKS_CONTINUE_RETURN_1;
	}

	// Pressed the drop key
	if(newkeys & ITEM_KEY_DROP_ITEM) {
		_item_handleDropKeyDown(playerid);
	}

	// Pressed the interact key
	if(newkeys & ITEM_KEY_INTERACT && !(oldkeys & ITEM_KEY_INTERACT)) {
		_item_handleUseKeyDown(playerid);
	}

	// Released the interact key
	if(oldkeys & ITEM_KEY_INTERACT && !(newkeys & ITEM_KEY_INTERACT)) {
		_item_handleUseKeyUp(playerid);
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}

_item_handleDropKeyDown(playerid) {
	new animidx = GetPlayerAnimationIndex(playerid);

	if(!_item_isIdleAnimation(animidx)) {
		return 1;
	}

	if(itm_Interacting[playerid] != INVALID_ITEM_ID) {
		return 2;
	}

	if(!Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return 3;
	}

	new
		targetid = INVALID_PLAYER_ID,
		players[MAX_PLAYERS],
		totalPlayers;

	totalPlayers = GetPlayersNextToPlayer(playerid, players);

	for(new i; i < totalPlayers; ++i) {
		if(itm_Holding[i] != INVALID_ITEM_ID)
			continue;

		if(itm_Interacting[i] != INVALID_ITEM_ID)
			continue;

		if(IsPlayerInAnyVehicle(i))
			continue;

		targetid = i;
		break;
	}

	if(IsPlayerConnected(targetid)) {
		PlayerGiveItem(playerid, targetid, 1);
	} else {
		PlayerDropItem(playerid);
	}

	return 0;
}

_item_handleUseKeyDown(playerid) {
	new animidx = GetPlayerAnimationIndex(playerid);

	if(!_item_isIdleAnimation(animidx)) {
		return 1;
	}

	if(itm_Interacting[playerid] != INVALID_ITEM_ID) {
		return 2;
	}

	if(!Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return 3;
	}

	return PlayerUseItem(playerid);
}

_item_handleUseKeyUp(playerid) {
	stop itm_LongPickupTimer[playerid];

	if(itm_Interacting[playerid] == INVALID_ITEM_ID) {
		if(itm_CurrentButton[playerid] != INVALID_BUTTON_ID) {
			CallLocalFunction("OnPlayerRelButtonWithItem", "ddd", playerid, itm_CurrentButton[playerid], itm_Holding[playerid]);
			itm_CurrentButton[playerid] = INVALID_BUTTON_ID;
		}

		return 1;
	}

	// If the item the player is interacting with is not a long-press pickup
	// type, ignore the next part of code since it's not relavent.
	if(!itm_TypeData[itm_Data[itm_Interacting[playerid]][itm_type]][itm_longPickup]) {
		return 2;
	}

	// Time since player interact keydown event
	new interval = GetTickCountDifference(itm_LongPickupTick[playerid], GetTickCount());

	// If the interval is below 200 it's a tap event, counts as using an item.
	if(interval < 200) {
		CallLocalFunction("OnPlayerUseItem", "dd", playerid, itm_Interacting[playerid]);
	}

	itm_LongPickupTick[playerid] = 0;
	itm_Interacting[playerid] = INVALID_ITEM_ID;

	return 0;
}

hook OnPlayerEnterPlayerArea(playerid, targetid) {
	if(Iter_Contains(itm_Index, itm_Holding[playerid])) {
		ShowActionText(playerid, "Press N to give item");
	}

	return 1;
}

hook OnPlayerLeavePlayerArea(playerid, targetid) {
	if(Iter_Contains(itm_Index, itm_Holding[playerid])) {
		HideActionText(playerid);
	}

	return 1;
}

_item_onPlayerUseItem(playerid, itemid) {
	new buttonid = GetPlayerButtonID(playerid);

	if(buttonid != -1) {
		itm_CurrentButton[playerid] = buttonid;

		if(CallLocalFunction("OnPlayerUseItemWithButton", "ddd", playerid, buttonid, itm_Holding[playerid])) {
			return 1;
		}
	}

	return CallLocalFunction("OnPlayerUseItem", "dd", playerid, itemid);
}


hook OnButtonPress(playerid, buttonid) {
	if(itm_Interacting[playerid] != INVALID_ITEM_ID) {
		return Y_HOOKS_CONTINUE_RETURN_0;
	}

	if(itm_ButtonIndex[buttonid] == INVALID_ITEM_ID) {
		return Y_HOOKS_CONTINUE_RETURN_0;
	}

	if(!Iter_Contains(itm_Index, itm_ButtonIndex[buttonid])) {
		return Y_HOOKS_CONTINUE_RETURN_0;
	}

	new itemid = itm_ButtonIndex[buttonid];

	if(itm_Holder[itemid] != INVALID_PLAYER_ID) {
		return Y_HOOKS_CONTINUE_RETURN_0;
	}

	if(itm_Interactor[itemid] != INVALID_PLAYER_ID) {
		return Y_HOOKS_CONTINUE_RETURN_0;
	}

	if(Iter_Contains(itm_Index, itm_Holding[playerid])) {
		return CallLocalFunction("OnPlayerUseItemWithItem", "ddd", playerid, itm_Holding[playerid], itemid);
	}

	if(itm_TypeData[itm_Data[itemid][itm_type]][itm_longPickup]) {
		_item_doLongPickup(playerid, itemid);
		return Y_HOOKS_BREAK_RETURN_1;
	}

	if(CallLocalFunction("OnPlayerPickUpItem", "dd", playerid, itemid)) {
		return Y_HOOKS_BREAK_RETURN_0;
	}

	PlayerPickUpItem(playerid, itemid);

	return Y_HOOKS_BREAK_RETURN_1;
}

_item_doLongPickup(playerid, itemid) {
	itm_LongPickupTick[playerid] = GetTickCount();
	itm_Interacting[playerid] = itemid;

	stop itm_LongPickupTimer[playerid];
	itm_LongPickupTimer[playerid] = defer _LongPickupItemDelay(playerid, itemid);
}

timer _LongPickupItemDelay[500](playerid, itemid) {
	if(CallLocalFunction("OnPlayerPickUpItem", "dd", playerid, itemid)) {
		return;
	}

	itm_LongPickupTick[playerid] = 0;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	PlayerPickUpItem(playerid, itemid);
}

timer PickUpItemDelay[400](playerid, id, animtype) {
	if(animtype == 0) {
		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.0, 0, 0, 0, 0, 0);
	}

	HideActionText(playerid);
	itm_Interacting[playerid] = INVALID_ITEM_ID;

	if(CallLocalFunction("OnPlayerPickedUpItem", "dd", playerid, id)) {
		return 1;
	}

	GiveWorldItemToPlayer(playerid, id, 1);

	return 1;
}

timer DropItemDelay[400](playerid) {
	new
		itemid = itm_Holding[playerid],
		Float:x,
		Float:y,
		Float:z,
		Float:r;

	if(!Iter_Contains(itm_Index, itemid)) {
		return 1;
	}

	RemovePlayerAttachedObject(playerid, ITEM_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	itm_Holding[playerid] = INVALID_ITEM_ID;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	itm_Holder[itemid] = INVALID_PLAYER_ID;
	itm_Interactor[itemid] = INVALID_PLAYER_ID;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, r);

	CreateItemInWorld(
		itemid,
		x + (0.5 * floatsin(-r, degrees)),
		y + (0.5 * floatcos(-r, degrees)),
		z - ITEM_FLOOR_OFFSET,
		0.0,
		0.0,
		r,
		GetPlayerVirtualWorld(playerid),
		GetPlayerInterior(playerid),
		1
	);

	Streamer_Update(playerid);
	ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.0, 0, 0, 0, 0, 0);
	CallLocalFunction("OnPlayerDroppedItem", "dd", playerid, itemid);
	CallLocalFunction("OnItemRemovedFromPlayer", "dd", playerid, itemid);

	return 1;
}

timer _item_giveItem[500](playerid, targetid) {
	// In case the giver quits
	if(!IsPlayerConnected(playerid)) {
		return;
	}

	// In case the receiver quits
	if(!IsPlayerConnected(targetid)) {
		return;
	}

	if(Iter_Contains(itm_Index, itm_Holding[targetid])) {
		return;
	}

	new
		itemid,
		ItemType:type;

	itemid = itm_Holding[playerid];

	if(itemid == -1) {
		return;
	}

	type = itm_Data[itemid][itm_type];

	itm_Holding[playerid] = INVALID_ITEM_ID;
	itm_Interacting[playerid] = INVALID_ITEM_ID;
	itm_Interacting[targetid] = INVALID_ITEM_ID;
	RemovePlayerAttachedObject(playerid, ITEM_ATTACH_INDEX);

	SetPlayerAttachedObject(
		targetid,
		ITEM_ATTACH_INDEX,
		itm_TypeData[type][itm_model],
		itm_TypeData[type][itm_attachBone],
		itm_TypeData[type][itm_attachPosX],
		itm_TypeData[type][itm_attachPosY],
		itm_TypeData[type][itm_attachPosZ],
		itm_TypeData[type][itm_attachRotX],
		itm_TypeData[type][itm_attachRotY],
		itm_TypeData[type][itm_attachRotZ],
		.materialcolor1 = itm_TypeData[type][itm_colour],
		.materialcolor2 = itm_TypeData[type][itm_colour]
	);

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	itm_Holding[targetid] = itemid;
	itm_Holder[itemid] = targetid;

	CallLocalFunction("OnPlayerGivenItem", "ddd", playerid, targetid, itemid);
	CallLocalFunction("OnItemRemovedFromPlayer", "dd", playerid, itemid);

	return;
}

stock _item_isIdleAnimation(animidx) {
	switch(animidx) {
		case 320, 471, 1164, 1183, 1188, 1189:
			return 1;
	}

	return 0;
}
