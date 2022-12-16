#include "wireState.h"

void WireState_Shrink::Update(Wire* wire)
{
	wire->m_flyDistance -= 0.5f;

	wire->m_position.x += wire->m_flyDistance;
	wire->m_scale.x -= 1.0f;

	if (wire->m_scale.x <= 0.3f)
	{
		//SetDestroy();
		wire->m_flyDistance = 1.0f;
		wire->m_state = wire->m_stateStrech;
	}
}