
#include "PlayerObj.h"
#include "StateCmds.h"
#include "Cmds.h"
#include "messages.h"
#include "TimeHandler.h"

namespace Planet
{

	void PlayerObj::UpdateData::interExtraPolate(int tick1, const UpdateData &data1, int tick2, const UpdateData &data2, Tickf resultTick, UpdateData &resultData)
	{
		standardInterExtraPolate(tick1, data1, tick2, data2, resultTick, resultData);
		resultData.nextShootTick = resultTick;

		if (resultTick >= tick2) resultData.ammoSupply = data2.ammoSupply;
		else resultData.ammoSupply = data1.ammoSupply;
	}


	const float PlayerObj::FORWARD_BACKWARD_SPEED = 100.0f / 1000.0f;
	const float PlayerObj::ROTATE_SPEED = 2.0f / 1000.0f;
	const float PlayerObj::STRAFE_SPEED = 100.0f / 1000.0f;
	const float PlayerObj::RECTANGLE_SIZE = 20.0f;

	PlayerObj::PlayerObj(PlayerId playerId, const Color &playerColor, const Pos &pos, const Pos &aimPos, size_t nHistoryTicks, int tick, PlanetBody *planetBody)
		: historyList(nHistoryTicks, UpdateData::interExtraPolate), 
			ship(playerColor, pos, aimPos, planetBody),
			health(100),
		 nextShootTick(static_cast<Tickf>(tick))
	{
		
		
		setAmmoSupply(0);
		if (N_WEAPONS >= 2) ammoSupply[1] = 10;

		// insert data into history list
		UpdateData firstTickData(getPos(),
									getAimPos(),
									nextShootTick,
									ammoSupply
									);
		historyList.setDefaultData(firstTickData);
		historyList.setData(tick, firstTickData);

		userCmd.clear(playerId, tick);

		assert(isConsistent(tick));
	}

	void PlayerObj::tickInit(PlayerId playerId, int tick)
	{
		this->nextShootTick = static_cast<Tickf>(tick);
		userCmd.clear(playerId, tick);
	}

	void PlayerObj::setAmmoSupply(int seed)
	{		
		for(int i=0; i<N_WEAPONS; ++i)
		{			
			if (((seed % N_WEAPONS) == 0) || ((seed % (N_WEAPONS + 2)) == 0))
			{
				ammoSupply[i] = 200 * ((seed % 4)+1) / Projectile::getDirectDamage(i);
			}
			else
			{
				ammoSupply[i] = 0;
			}
		}
		ammoSupply[0] = 10000;
	}

	void PlayerObj::hurt(int damage)
	{
		health -= damage;
	}

	Projectile::Type PlayerObj::getNextWeapon(Projectile::Type weapon)
	{
		for(int i=1; i<=N_WEAPONS; ++i)
		{
			weapon = (weapon + 1) % N_WEAPONS;
			if (ammoSupply[weapon] > 0) break;
		}
		return weapon;
	}

	bool PlayerObj::setTickDataAndCompare(int tick, const UpdatePlayerObj *updatePlayerObj)
	{
		UpdateData storedData;
		historyList.getData(tick, storedData);
		
		UpdateData data(updatePlayerObj->pos, updatePlayerObj->aimPos, updatePlayerObj->nextShootTick, updatePlayerObj->ammoSupply);
		historyList.setData(tick, data);

		return data != storedData;
	}

	void PlayerObj::updateToTickData(int tick)
	{
		UpdateData data;
		historyList.getData(tick, data);

		ship.setState(data.pos, data.aimPos);

		this->nextShootTick = data.nextShootTick;
		
		ammoSupply = data.ammoSupply;
		
		assert(data.nextShootTick >= tick);
	}

	void PlayerObj::updateToTickData(Tickf tick)
	{
		UpdateData data;
		historyList.getData(tick, data);

		ship.setState(data.pos, data.aimPos);

		this->nextShootTick = data.nextShootTick;

		for(int i=0; i<Projectile::N_TYPES; ++i)
		{
			this->ammoSupply[i] = data.ammoSupply[i];
		}
	}

	void PlayerObj::setUserCmd(const UserCmd *userCmd)
	{
		this->userCmd = *userCmd;
		ship.aimPos = userCmd->aimPos;
	}

	int PlayerObj::getNTickShots(Projectile::Type weapon, int currentTick)
	{ 
		assert(nextShootTick >= currentTick);

		Tickf nextTickf = static_cast<Tickf>(currentTick + 1);
		Tickf shootInterval = Projectile::getShootInterval(weapon);
		
		Tickf currentShootTick = nextShootTick;
		int nShots = 0;
		while(currentShootTick < nextTickf)
		{			
			++nShots;
			currentShootTick = nextShootTick + nShots * shootInterval;
		}
		return nShots;
	}

	void PlayerObj::updateNextShootTick(int currentTick)
	{
		assert(userCmd.isConsistent(currentTick));

		Tickf currentTickf = static_cast<Tickf>(currentTick);
		
		assert((nextShootTick >= currentTickf) || (userCmd.nShots == 0));
		nextShootTick = getShotTick(currentTick, userCmd.nShots);
		
		// advance to next tick, and update nextShootTick according to that tick
		++currentTick;
		currentTickf = static_cast<Tickf>(currentTick);

		if (nextShootTick < currentTickf) nextShootTick = currentTickf;
	}

	Tickf PlayerObj::getShotTick(int currentTick, int shotN)
	{
		assert(nextShootTick >= currentTick);
		assert(userCmd.isConsistent(currentTick));
		
		Tickf startShootTick = tmax(nextShootTick, userCmd.firstShotTick);

		return startShootTick + Projectile::getShootInterval(userCmd.weapon) * static_cast<Tickf>(shotN);
	}

	bool PlayerObj::isConsistent() const
	{
		assert((ship.position.x > -1000000.0f) && (ship.position.x < 1000000.0f));
		return historyList.isConsistent();
	}

	bool PlayerObj::isConsistent(int currentTick) const
	{
		if (!isConsistent()) return false;		
		bool consistent = (this->nextShootTick >= static_cast<int>(currentTick));
		assert(consistent);
		return consistent;
	}

}

