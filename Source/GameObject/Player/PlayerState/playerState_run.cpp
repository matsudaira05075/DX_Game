#include "playerState.h"

void PlayerState_Run::Update(Player* player)
{
	player->UpdateMove();

	if (Input::GetKeyTrigger(VK_SPACE)) {
		player->m_state = player->m_stateJump;
		player->ActionJump();
	}
}
