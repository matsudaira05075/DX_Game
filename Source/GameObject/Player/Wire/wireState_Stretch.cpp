#include "wireState.h"

void WireState_Stretch::Update(Wire* wire)
{
	wire->m_flyDistance += 0.5f;

	wire->m_position.x += wire->m_flyDistance;
	wire->m_scale.x += 1.0f;

		// 一定時間削除(現在の時間-生成時間が一定量を超えていたら削除)
		if (wire->m_scale.x >= 20.0f)
		{
			//SetDestroy();
			wire->m_state = wire->m_stateShrink;
		}
}