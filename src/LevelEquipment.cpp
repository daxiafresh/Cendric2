#include "LevelEquipment.h"
#include "LevelMainCharacter.h"

LevelEquipment::~LevelEquipment() {
	g_resourceManager->deleteResource(m_texturePath);
}

void LevelEquipment::calculatePositionAccordingToMainChar(sf::Vector2f& position) const {
	sf::Vector2f mainCharPosition(m_mainChar->getPosition().x + (m_mainChar->getBoundingBox()->width / 2), m_mainChar->getPosition().y);
	sf::Vector2f offset(-60.f, -20.f);
	if (!m_mainChar->getIsFacingRight())
		offset.x = -offset.x - getBoundingBox()->width;
	if (m_mainChar->getIsUpsideDown())
		offset.y = m_mainChar->getBoundingBox()->height - offset.y - getBoundingBox()->height;

	position.x = (mainCharPosition + offset).x;
	position.y = (mainCharPosition + offset).y;
}

void LevelEquipment::update(const sf::Time& frameTime) {
	AnimatedGameObject::update(frameTime);
	GameObjectState newState = m_mainChar->getState();
	if (newState == GameObjectState::Dead) {
		setDisposed();
		return;
	}
	bool newFacingRight = m_mainChar->getIsFacingRight();
	if (m_state != newState || newFacingRight != m_isFacingRight) {
		m_state = newState;
		m_isFacingRight = newFacingRight;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
	if (m_mainChar->getIsUpsideDown() != m_animatedSprite.isFlippedY()) {
		m_animatedSprite.setFlippedY(m_mainChar->getIsUpsideDown());
	}

	// TODO: use level main char color!

	sf::Vector2f newPosition;
	calculatePositionAccordingToMainChar(newPosition);
	setPosition(newPosition);
}

void LevelEquipment::loadEquipment(LevelMainCharacter* mainChar) {
	m_mainChar = mainChar;

	sf::Vector2f position;
	calculatePositionAccordingToMainChar(position);
	setPosition(position);
}

void LevelEquipment::setTexturePath(const std::string& texturePath) {
	m_texturePath = texturePath;
}

GameObjectType LevelEquipment::getConfiguredType() const {
	return GameObjectType::_LevelEquipment;
}
