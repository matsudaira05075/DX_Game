#include "playerState.h"

void PlayerState_Idle::Update(Player* player)
{
	player->UpdateMove();

	if (Input::GetKeyTrigger(VK_SPACE)) {
		player->m_state = player->m_stateJump;
		player->ActionJump();
	}
	if (Input::OnMouseUp(Left)) {
		player->m_state = player->m_stateJump;
		player->ActionJump();
	}

}
