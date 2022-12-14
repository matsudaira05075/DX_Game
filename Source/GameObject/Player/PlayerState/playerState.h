#pragma once
#include "../../../Main/main.h"
#include "../../../System/manager.h"
#include "../../../System/input.h"
#include "../player.h"

class Player;

class PlayerState
{
public:
	PlayerState() {}
	virtual ~PlayerState() {}

	virtual void Update(Player* player) = 0;
};


class PlayerState_Idle : public PlayerState
{
public:
	void Update(Player* player) override;
};

class PlayerState_Run : public PlayerState
{
public:
	void Update(Player* player) override;
};

class PlayerState_Jump : public PlayerState
{
public:
	void Update(Player* player) override;
};

class PlayerState_Fall : public PlayerState
{
public:
	void Update(Player* player) override;
};