#include "GUI/SpellSlot.h"

#include "ResourceManager.h"

using namespace std;

const float SpellSlot::SIZE = 74.f;
const float SpellSlot::ICON_OFFSET = 12.f;

SpellSlot::SpellSlot(SpellType type) {
	m_spellType = type;
	m_spellID = SpellID::VOID;
	m_isChopSlot = false;
	m_inputKeyID = Key::VOID;
	m_isEmpty = true;

	m_iconTextureRect = sf::IntRect(250, (static_cast<int>(m_spellType) - 1) * 50, 50, 50);

	initSpellSlot();
}

SpellSlot::SpellSlot(SpellID id) {
	m_spellID = id;
	const SpellData& bean = SpellData::getSpellData(id);
	m_iconTextureRect = bean.iconTextureRect;
	m_spellType = bean.spellType;
	m_isChopSlot = (id == SpellID::Chop);
	m_inputKeyID = Key::VOID;
	m_isEmpty = false;

	initSpellSlot();
}

SpellSlot::SpellSlot(const SpellData& bean) {
	m_spellID = bean.id;
	m_iconTextureRect = bean.iconTextureRect;
	m_spellType = bean.spellType;
	m_isChopSlot = (bean.id == SpellID::Chop);
	m_inputKeyID = bean.inputKey;
	m_isEmpty = false;

	initSpellSlot();
}

void SpellSlot::initSpellSlot() {
	m_inputKey.setString(m_inputKeyID != Key::VOID ?
		EnumNames::getKeyboardKeyName(g_resourceManager->getConfiguration().mainKeyMap[m_inputKeyID]) :
		"");
	m_inputKey.setCharacterSize(16);
	if (m_inputKey.getLocalBounds().width > SIZE - 10.f) m_inputKey.setCharacterSize(8);
	m_inputKey.setColor(CENDRIC_COLOR_WHITE);

	m_borderTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_spell);
	m_borderTextureSelected = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_spell_selected);

	m_highlightTexture = g_resourceManager->getTexture(ResourceID::Texture_GUI_slot_highlight);

	if (m_spellID != SpellID::VOID) {
		m_iconTexture = m_isChopSlot ?
			g_resourceManager->getTexture(ResourceID::Texture_items) :
			g_resourceManager->getTexture(ResourceID::Texture_spellicons);
	}
	else {
		m_iconTexture = g_resourceManager->getTexture(ResourceID::Texture_spellicons);
	}

	m_cooldownRect.setSize(ICON_SIZE, ICON_SIZE);
	m_cooldownRect.setFillColor(sf::Color(200, 200, 200, 128));

	initSlot();
}

void SpellSlot::setPosition(const sf::Vector2f& pos) {
	Slot::setPosition(pos);
	sf::Vector2f positionOffset(SpellSlot::SIZE / 2.f - m_inputKey.getLocalBounds().width / 2.f, SpellSlot::SIZE - 10.f);
	m_inputKey.setPosition(pos + positionOffset);
	m_cooldownRect.setPosition(pos.x + ICON_OFFSET, pos.y + ICON_OFFSET);
}

void SpellSlot::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_backgroundRect);
	renderTarget.draw(m_iconRect);
	renderTarget.draw(m_overlayRect);
	if (m_animating) {
		renderTarget.draw(m_cooldownRect);
	}
	renderTarget.draw(m_borderRect);
	renderTarget.draw(m_inputKey);
}

void SpellSlot::playAnimation(const sf::Time &cooldown) {
	if (!m_isEmpty) {
		m_cooldown = cooldown;
		m_animationTime = sf::Time::Zero;
		m_animating = true;
	}
}

void SpellSlot::update(const sf::Time& frameTime) {
	float flashTime = 0.05f;	// seconds

	if (m_animating) {
		float t = m_animationTime.asSeconds() / m_cooldown.asSeconds();
		float angle = lerp(t, 0.f, 360.f);
		m_cooldownRect.setAngle(angle);

		m_animationTime += frameTime;

		if (m_animationTime.asSeconds() > m_cooldown.asSeconds()) {
			if (m_animationTime.asSeconds() < m_cooldown.asSeconds() + flashTime) {
				m_cooldownRect.setAngle(0.f);
			}
			else {
				m_animationTime = sf::Time::Zero;
				m_cooldownRect.setAngle(0.f);
				m_animating = false;
			}

		}
	}
	m_isClicked = false;
	m_isRightClicked = false;
	GameObject::update(frameTime);
}

void SpellSlot::select() {
	if (m_isSelected || m_isEmpty) return;
	Slot::select();
	m_inputKey.setColor(sf::Color::Red);
}

void SpellSlot::deselect() {
	if (!m_isSelected || m_isEmpty) return;
	Slot::deselect();
	m_inputKey.setColor(sf::Color::White);
}
