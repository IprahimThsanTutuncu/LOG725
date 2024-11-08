#include "pch.h"

#include "State.h"
#include "thsan/Game.h"
#include "thsan/util/helper.h"
#include "thsan/config/ControlSetting.h"

State::State(Game* parent):
	m_parent(parent)
{
	id = pointer_to_string(this);
}

std::string State::getId()
{
	return id;
}
