#include "Screen.h"
#include "client/Latite.h"
#include "client/event/Eventing.h"
#include "ScreenManager.h"
#include "client/event/impl/ClickEvent.h"
#include "client/event/impl/RenderOverlayEvent.h"
#include "util/Util.h"
#include "util/DxContext.h"
#include "sdk/common/client/game/ClientInstance.h"

Screen::Screen(std::string const& name) : name(name) {
	arrow = LoadCursorA(Latite::get().dllInst, MAKEINTRESOURCEA(IDC_ARROW));
	hand = LoadCursorA(Latite::get().dllInst, MAKEINTRESOURCEA(IDC_HAND));
	ibeam = LoadCursorA(Latite::get().dllInst, MAKEINTRESOURCEA(IDC_IBEAM));

	Eventing::get().listen<RenderGameEvent>(this, (EventListenerFunc)&Screen::onRenderGame, 0);
	Eventing::get().listen<RenderOverlayEvent>(this, (EventListenerFunc)&Screen::onRenderOverlay, 0);
	Eventing::get().listen<ClickEvent>(this, (EventListenerFunc)&Screen::onClick, 3);
}

void Screen::onRenderGame(Event& ev) {
	switch (cursor) {
	case Cursor::Arrow:
		SetCursor(arrow);
		break;
	case Cursor::Hand:
		SetCursor(hand);
		break;
	case Cursor::IBeam:
		SetCursor(ibeam);
		break;
	}

	sdk::ClientInstance::get()->releaseCursor();
}

void Screen::close() {
	Latite::getScreenManager().exitCurrentScreen();
}

void Screen::playClickSound() {
	util::PlaySoundUI("random.click");
}

void Screen::onClick(Event& evGeneric) {
	auto& ev = reinterpret_cast<ClickEvent&>(evGeneric);
	if (ev.getMouseButton() < 4) {
		if (ev.isDown())
			this->mouseButtons[ev.getMouseButton() - 1] = ev.isDown();
	}
	if (ev.getMouseButton() > 0) ev.setCancelled(true);
}

void Screen::onRenderOverlay(Event& ev) {
	for (size_t i = 0; i < justClicked.size(); i++) {
		justClicked[i] = this->mouseButtons[i];
		this->mouseButtons[i] = false;
	}

	if (this->tooltip.has_value()) {
		DXContext dc;
		Vec2& mousePos = sdk::ClientInstance::get()->cursorPos;
		dc.setFont(Renderer::FontSelection::Regular);
		d2d::Rect textRect = dc.getTextRect(this->tooltip.value(), 15.f, 5.f);
		textRect.setPos(mousePos);
		auto height = textRect.getHeight() * 0.9f;
		textRect.top -= height;
		textRect.bottom -= height;
		textRect.left += 5.f;
		textRect.right += 5.f;
		dc.fillRectangle(textRect, d2d::Color(0.f, 0.f, 0.f, 0.6f));
		dc.drawText(textRect, this->tooltip.value(), d2d::Color(1.f, 1.f, 1.f, 0.8f), 15.f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	this->tooltip = std::nullopt;
}
