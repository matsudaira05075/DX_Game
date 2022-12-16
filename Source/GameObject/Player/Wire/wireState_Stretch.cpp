#include "wireState.h"

void WireState_Stretch::Update(Wire* wire)
{
	wire->m_flyDistance += 0.5f;

	wire->m_position.x += wire->m_flyDistance;
	wire->m_scale.x += 1.0f;

		// ˆê’èŠÔíœ(Œ»İ‚ÌŠÔ-¶¬ŠÔ‚ªˆê’è—Ê‚ğ’´‚¦‚Ä‚¢‚½‚çíœ)
		if (wire->m_scale.x >= 20.0f)
		{
			//SetDestroy();
			wire->m_state = wire->m_stateShrink;
		}
}