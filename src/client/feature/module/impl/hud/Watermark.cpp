#include "pch.h"
#include "Watermark.h"
#include "client/Latite.h"

Watermark::Watermark() : TextModule("Watermark", LocalizeString::get("client.textmodule.watermark.name"),
                                      LocalizeString::get("client.textmodule.watermark.desc"), HUD) {
    this->prefix = TextValue(L"SLIME");
}
