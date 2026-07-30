#include "item.inc"

#include <YSI_Core\y_testing>
#include <test-boilerplate>
#include <zcmd>


// -
// Unit tests
// -


Test:DefineItemType() {
	new ItemType:tmp = DefineItemType(
		"Medkit", "DefineItemTypeMedkit", 11736, 1,
		0.0, 0.0, 0.0, 0.004,
		0.197999, 0.038000, 0.021000,
		79.700012, 0.000000, 90.899978,
		.maxhitpoints = 1);

	ASSERT(IsValidItemType(tmp));
}

Test:CreateDestroyItem() {
	new ItemType:tmp = DefineItemType(
		"Medkit", "CreateDestroyItemMedkit", 11736, 1,
		0.0, 0.0, 0.0, 0.004,
		0.197999, 0.038000, 0.021000,
		79.700012, 0.000000, 90.899978,
		.maxhitpoints = 1);

	new Item:itemid = CreateItem(tmp);

	dumpItemInfo(itemid);

	ASSERT(IsValidItem(itemid));
	DestroyItem(itemid);
	ASSERT(!IsValidItem(itemid));
}

stock dumpItemInfo(Item:itemid) {
	new
		Float:x,
		Float:y,
		Float:z,
		Float:rx,
		Float:ry,
		Float:rz,
		data,
		nameExtra[MAX_ITEM_TEXT];

	GetItemPos(itemid, x, y, z);
	GetItemRot(itemid, rx, ry, rz);
	GetItemExtraData(itemid, data);
	GetItemNameExtra(itemid, nameExtra);

	new objectID;GetItemObjectID(itemid, objectID);
	new Button:buttonID;GetItemButtonID(itemid, buttonID);
	new type = _:GetItemType(itemid);
	new world;GetItemWorld(itemid, world);
	new interior;GetItemInterior(itemid, interior);
	new hitPoints = GetItemHitPoints(itemid);

	Logger_Log("item basic info",
		Logger_I("objectID", objectID),
		Logger_I("buttonID", _:buttonID),
		Logger_I("type", type),
		Logger_I("world", world),
		Logger_I("interior", interior),
		Logger_I("hitPoints", hitPoints));

	Logger_Log("item positional info",
		Logger_F("x", x),
		Logger_F("y", y),
		Logger_F("z", z),
		Logger_F("rx", rx),
		Logger_F("ry", ry),
		Logger_F("rz", rz));

	Logger_Log("item additional",
		Logger_I("data", data),
		Logger_S("nameExtra", nameExtra));
}

new ItemType:item_Medkit;


// -
// Demo tests
// -


main() {
	item_Medkit = DefineItemType(
		"Medkit", "Medkit", 11736, 1,
		0.0, 0.0, 0.0, 0.004,
		0.197999, 0.038000, 0.021000,
		79.700012, 0.000000, 90.899978,
		.maxhitpoints = 1);

	// GetItemTypeCount(ItemType:itemtype);
}

new Item:lastItem = INVALID_ITEM_ID;

CMD:ci(playerid, params[]) {
	new Item:nextItemID = GetNextItemID();
	
	new
		Float:x,
		Float:y,
		Float:z;
	GetPlayerPos(playerid, x, y, z);

	lastItem = CreateItem(item_Medkit, x, y + 1, z - 0.8);
	ASSERT(lastItem == nextItemID);

	return 1;
}

CMD:gwitp(playerid, params[]) {
	lastItem = CreateItem(item_Medkit);
	Logger_Log("CreateItem",
		Logger_I("ret", _:lastItem));

	new ret = GiveWorldItemToPlayer(playerid, lastItem);
	Logger_Log("GiveWorldItemToPlayer",
		Logger_I("ret", ret));

	return 1;
}

CMD:rci(playerid, params[]) {
	new Item:ret = RemoveCurrentItem(playerid);
	Logger_Log("RemoveCurrentItem",
		Logger_I("ret", _:ret));
	return 1;
}

CMD:rifw(playerid, params[]) {
	new ret = RemoveItemFromWorld(lastItem);
	Logger_Log("RemoveItemFromWorld",
		Logger_I("ret", ret));
	return 1;
}

// AllocNextItemID(ItemType:type, geid[] = "");
// CreateItem_ExplicitID(itemid, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, world = 0, interior = 0, label = 1, applyrotoffsets = 1, virtual = 0, hitpoints = -1);

// todo: command to set item label
// SetItemLabel(itemid, text[], colour = 0xFFFF00FF, Float:range = 10.0);

// todo: command to simply set item pos back to spawn point
// SetItemPos(itemid, Float:x, Float:y, Float:z);

// todo: command to invert Z rotation
// SetItemRot(itemid, Float:rx, Float:ry, Float:rz, bool:offsetfromdefaults = false);

// todo: command to set item and player world
// SetItemWorld(itemid, world);

// todo: command to set item and player interior
// SetItemInterior(itemid, interior);

// todo: command to set item hitpoints
// SetItemHitPoints(itemid, hitpoints);

// todo: command to set item extra data
// SetItemExtraData(itemid, data);

// todo: command to set item name extra
// SetItemNameExtra(itemid, string[]);

// todo: command to display player item ID
// GetPlayerItem(playerid);

// todo: command to
// GetPlayerNearbyItems(playerid, list[]);



// bool:IsValidItemType(ItemType:itemtype);
// GetItemTypeName(ItemType:itemtype, string[]);
// GetItemTypeUniqueName(ItemType:itemtype, string[]);
// ItemType:GetItemTypeFromUniqueName(string[], bool:ignorecase = false);
// GetItemTypeModel(ItemType:itemtype);
// GetItemTypeSize(ItemType:itemtype);
// GetItemTypeRotation(ItemType:itemtype, &Float:rx, &Float:ry, &Float:rz);
// bool:IsItemTypeCarry(ItemType:itemtype);
// GetItemTypeColour(ItemType:itemtype, &colour);
// GetItemTypeBone(ItemType:itemtype);
// bool:IsItemTypeLongPickup(ItemType:itemtype);

// bool:IsItemDestroying(itemid);
// GetItemHolder(itemid);
// bool:IsItemInWorld(itemid);
// GetItemFromButtonID(buttonid);
// GetItemName(itemid, string[]);
// GetNextItemID();
// GetItemsInRange(Float:x, Float:y, Float:z, Float:range = 300.0, items[], maxitems = sizeof(items));

public OnItemTypeDefined(uname[]) {
	Logger_Log("OnItemTypeDefined(uname[])",
		Logger_S("uname", uname));
}

public OnItemCreate(Item:itemid) {
	Logger_Log("OnItemCreate(itemid)",
		Logger_I("itemid", _:itemid));
}

public OnItemCreated(Item:itemid) {
	Logger_Log("OnItemCreated(itemid)",
		Logger_I("itemid", _:itemid));
}

public OnItemDestroy(Item:itemid) {
	Logger_Log("OnItemDestroy(itemid)",
		Logger_I("itemid", _:itemid));
}

public OnItemDestroyed(Item:itemid) {
	Logger_Log("OnItemDestroyed(itemid)",
		Logger_I("itemid", _:itemid));
}

public OnItemCreateInWorld(Item:itemid) {
	Logger_Log("OnItemCreateInWorld(itemid)",
		Logger_I("itemid", _:itemid));
}

public OnItemRemoveFromWorld(Item:itemid) {
	Logger_Log("OnItemRemoveFromWorld(itemid)",
		Logger_I("itemid", _:itemid));
}

public OnPlayerUseItem(playerid, Item:itemid) {
	Logger_Log("OnPlayerUseItem(playerid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerUseItemWithItem(playerid, Item:itemid, Item:withitemid) {
	Logger_Log("OnPlayerUseItemWithItem(playerid, itemid, withitemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid),
		Logger_I("withitemid", _:withitemid));
}

public OnPlayerUseItemWithButton(playerid, Button:buttonid, Item:itemid) {
	Logger_Log("OnPlayerUseItemWithButton(playerid, buttonid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("buttonid", _:buttonid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerRelButtonWithItem(playerid, Button:buttonid, Item:itemid) {
	Logger_Log("OnPlayerRelButtonWithItem(playerid, buttonid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("buttonid", _:buttonid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerPickUpItem(playerid, Item:itemid) {
	Logger_Log("OnPlayerPickUpItem(playerid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerPickedUpItem(playerid, Item:itemid) {
	Logger_Log("OnPlayerPickedUpItem(playerid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerGetItem(playerid, Item:itemid) {
	Logger_Log("OnPlayerGetItem(playerid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerDropItem(playerid, Item:itemid) {
	Logger_Log("OnPlayerDropItem(playerid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerDroppedItem(playerid, Item:itemid) {
	Logger_Log("OnPlayerDroppedItem(playerid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerGiveItem(playerid, targetid, Item:itemid) {
	Logger_Log("OnPlayerGiveItem(playerid, targetid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("targetid", targetid),
		Logger_I("itemid", _:itemid));
}

public OnPlayerGivenItem(playerid, targetid, Item:itemid) {
	Logger_Log("OnPlayerGivenItem(playerid, targetid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("targetid", targetid),
		Logger_I("itemid", _:itemid));
}

public OnItemRemovedFromPlayer(playerid, Item:itemid) {
	Logger_Log("OnItemRemovedFromPlayer(playerid, itemid)",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}

public OnItemNameRender(Item:itemid, ItemType:itemtype) {
	Logger_Log("OnItemNameRender(itemid, ItemType:itemtype)",
		Logger_I("itemid", _:itemid),
		Logger_I("itemtype", _:itemtype));
}

public OnItemHitPointsUpdate(Item:itemid, oldvalue, newvalue) {
	Logger_Log("OnItemHitPointsUpdate(itemid, oldvalue, newvalue)",
		Logger_I("itemid", _:itemid),
		Logger_I("oldvalue", oldvalue),
		Logger_I("newvalue", newvalue));
}
