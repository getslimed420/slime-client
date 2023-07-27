#include "MinecraftGame.h"
#include "sdk/signature/storage.h"

bool sdk::MinecraftGame::isCursorGrabbed() {
    if (sdk::internalVers == VLATEST)
        return util::directAccess<bool>(this, Signatures::Offset::MinecraftGame_cursorGrabbed.result);
    // v1_18_12
    if (sdk::internalVers == V1_19_51) return util::directAccess<bool>(this, 0xD8);
    return util::directAccess<bool>(this, 0x318);
}
