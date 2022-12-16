#pragma once
#include "../../../Main/main.h"
#include "../../../System/manager.h"
#include "../../../System/input.h"
#include "../../Camera/camera.h"
#include "wire.h"

class Wire;

class WireState
{
public:
	WireState() {}
	virtual ~WireState() {}

	virtual void Update(Wire* wire) = 0;
};


class WireState_Stretch : public WireState
{
public:
	void Update(Wire* player) override;
};

class WireState_Shrink : public WireState
{
public:
	void Update(Wire* player) override;
};

class WireState_Back : public WireState
{
public:
	void Update(Wire* player) override;
};

class WireState_Atack : public WireState
{
public:
	void Update(Wire* player) override;
};