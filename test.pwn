#define RUN_TESTS

#include "item.inc"

#include <YSI\y_testing>
#include <test-boilerplate>
#include <zcmd>

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

	new itemid = CreateItem(tmp);

	dumpItemInfo(itemid);

	ASSERT(IsValidItem(itemid));
	DestroyItem(itemid);
	ASSERT(!IsValidItem(itemid));
}

dumpItemInfo(itemid) {
	new
		Float:x,
		Float:y,
		Float:z,
		Float:rx,
		Float:ry,
		Float:rz,
		data,
		nameExtra[ITM_MAX_TEXT];

	GetItemPos(itemid, x, y, z);
	GetItemRot(itemid, rx, ry, rz);
	GetItemExtraData(itemid, data);
	GetItemNameExtra(itemid, nameExtra);

	log("item basic info",
		_i("objectID", GetItemObjectID(itemid)),
		_i("buttonID", GetItemButtonID(itemid)),
		_i("type", _:GetItemType(itemid)),
		_i("world", GetItemWorld(itemid)),
		_i("interior", GetItemInterior(itemid)),
		_i("hitPoints", GetItemHitPoints(itemid)));

	log("item positional info",
		_f("x", x),
		_f("y", y),
		_f("z", z),
		_f("rx", rx),
		_f("ry", ry),
		_f("rz", rz));

	log("item additional",
		_i("data", data),
		_s("nameExtra", nameExtra));
}

new ItemType:item_Medkit;

main() {
	item_Medkit = DefineItemType(
		"Medkit", "Medkit", 11736, 1,
		0.0, 0.0, 0.0, 0.004,
		0.197999, 0.038000, 0.021000,
		79.700012, 0.000000, 90.899978,
		.maxhitpoints = 1);

	// GetItemTypeCount(ItemType:itemtype);
}

new lastItem = INVALID_ITEM_ID;

CMD:ci(playerid, params[]) {
	new nextItemID = GetNextItemID();
	
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
	log("CreateItem",
		_i("ret", lastItem));

	new ret = GiveWorldItemToPlayer(playerid, lastItem);
	log("GiveWorldItemToPlayer",
		_i("ret", ret));

	return 1;
}

CMD:rci(playerid, params[]) {
	new ret = RemoveCurrentItem(playerid);
	log("RemoveCurrentItem",
		_i("ret", ret));
	return 1;
}

CMD:rifw(playerid, params[]) {
	new ret = RemoveItemFromWorld(lastItem);
	log("RemoveItemFromWorld",
		_i("ret", ret));
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
	log("OnItemTypeDefined(uname[])",
		_s("uname", uname));
}

public OnItemCreate(itemid) {
	log("OnItemCreate(itemid)",
		_i("itemid", itemid));
}

public OnItemCreated(itemid) {
	log("OnItemCreated(itemid)",
		_i("itemid", itemid));
}

public OnItemDestroy(itemid) {
	log("OnItemDestroy(itemid)",
		_i("itemid", itemid));
}

public OnItemDestroyed(itemid) {
	log("OnItemDestroyed(itemid)",
		_i("itemid", itemid));
}

public OnItemCreateInWorld(itemid) {
	log("OnItemCreateInWorld(itemid)",
		_i("itemid", itemid));
}

public OnItemRemoveFromWorld(itemid) {
	log("OnItemRemoveFromWorld(itemid)",
		_i("itemid", itemid));
}

public OnPlayerUseItem(playerid, itemid) {
	log("OnPlayerUseItem(playerid, itemid)",
		_i("playerid", playerid),
		_i("itemid", itemid));
}

public OnPlayerUseItemWithItem(playerid, itemid, withitemid) {
	log("OnPlayerUseItemWithItem(playerid, itemid, withitemid)",
		_i("playerid", playerid),
		_i("itemid", itemid),
		_i("withitemid", withitemid));
}

public OnPlayerUseItemWithButton(playerid, buttonid, itemid) {
	log("OnPlayerUseItemWithButton(playerid, buttonid, itemid)",
		_i("playerid", playerid),
		_i("buttonid", buttonid),
		_i("itemid", itemid));
}

public OnPlayerRelButtonWithItem(playerid, buttonid, itemid) {
	log("OnPlayerRelButtonWithItem(playerid, buttonid, itemid)",
		_i("playerid", playerid),
		_i("buttonid", buttonid),
		_i("itemid", itemid));
}

public OnPlayerPickUpItem(playerid, itemid) {
	log("OnPlayerPickUpItem(playerid, itemid)",
		_i("playerid", playerid),
		_i("itemid", itemid));
}

public OnPlayerPickedUpItem(playerid, itemid) {
	log("OnPlayerPickedUpItem(playerid, itemid)",
		_i("playerid", playerid),
		_i("itemid", itemid));
}

public OnPlayerGetItem(playerid, itemid) {
	log("OnPlayerGetItem(playerid, itemid)",
		_i("playerid", playerid),
		_i("itemid", itemid));
}

public OnPlayerDropItem(playerid, itemid) {
	log("OnPlayerDropItem(playerid, itemid)",
		_i("playerid", playerid),
		_i("itemid", itemid));
}

public OnPlayerDroppedItem(playerid, itemid) {
	log("OnPlayerDroppedItem(playerid, itemid)",
		_i("playerid", playerid),
		_i("itemid", itemid));
}

public OnPlayerGiveItem(playerid, targetid, itemid) {
	log("OnPlayerGiveItem(playerid, targetid, itemid)",
		_i("playerid", playerid),
		_i("targetid", targetid),
		_i("itemid", itemid));
}

public OnPlayerGivenItem(playerid, targetid, itemid) {
	log("OnPlayerGivenItem(playerid, targetid, itemid)",
		_i("playerid", playerid),
		_i("targetid", targetid),
		_i("itemid", itemid));
}

public OnItemRemovedFromPlayer(playerid, itemid) {
	log("OnItemRemovedFromPlayer(playerid, itemid)",
		_i("playerid", playerid),
		_i("itemid", itemid));
}

public OnItemNameRender(itemid, ItemType:itemtype) {
	log("OnItemNameRender(itemid, ItemType:itemtype)",
		_i("itemid", itemid),
		_i("itemtype", _:itemtype));
}

public OnItemHitPointsUpdate(itemid, oldvalue, newvalue) {
	log("OnItemHitPointsUpdate(itemid, oldvalue, newvalue)",
		_i("itemid", itemid),
		_i("oldvalue", oldvalue),
		_i("newvalue", newvalue));
}
