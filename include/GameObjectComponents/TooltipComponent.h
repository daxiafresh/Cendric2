#pragma once

#include "global.h"
#include "GameObjectComponents/GameObjectComponent.h"
#include "GUI/BitmapText.h"
#include "AnimatedGameObject.h"

// A game object component that holds a tooltip text
class TooltipComponent : public virtual GameObjectComponent {
public:
	TooltipComponent(std::string tooltip, AnimatedGameObject* parent, bool useInteractiveColor = true);
	virtual ~TooltipComponent() {}

	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;
	void onParentMouseOver() override;

protected:
	BitmapText m_tooltipText;
	sf::Time m_tooltipTime = sf::Time::Zero;
	AnimatedGameObject* m_animatedParent;
	bool m_useInteractiveColor;
	
	static const sf::Time TOOLTIP_TIME;
	static const float TOOLTIP_HEIGHT;
};