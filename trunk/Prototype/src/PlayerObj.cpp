
#include "PlayerObj.h"
#include "StateCmds.h"
#include "Cmds.h"
#include "messages.h"
#include "TimeHandler.h"

namespace Prototype
{

	const float PlayerObj::FORWARD_BACKWARD_SPEED = 100.0f / 1000.0f;
	const float PlayerObj::ROTATE_SPEED = 2.0f / 1000.0f;
	const float PlayerObj::STRAFE_SPEED = 100.0f / 1000.0f;
	const float PlayerObj::RECTANGLE_SIZE = 20.0f;

	//PlayerObj::PlayerObj(size_t playerId, const Pos &pos)
	//	: playerId(playerId), pos(pos), angle(0.0f),
	//	 movingForward(false), movingBackward(false),
	//	 strafingLeft(false), strafingRight(false)
	//{}

	PlayerObj::PlayerObj(const Pos &pos, size_t nHistoryTicks, int tick)
		: historyList(nHistoryTicks), pos(pos), angle(Angle::PI/2.0f), health(100),
		 //movingForward(false), movingBackward(false),
		 //strafingLeft(false), strafingRight(false),
		 //currentWeapon(Projectile::DEFAULT_PROJECTILE),
		 nextShootTick(0)
	{
		assert(isConsistent());
		
		setAmmoSupply(0);
		if (N_WEAPONS >= 2) ammo[1] = 10;

		// insert data into history list
		UpdateData firstTickData(pos, angle, nextShootTick);
		historyList.setDefaultData(firstTickData);
		historyList.setData(tick, firstTickData);

		userCmd.clear();
	}

	void PlayerObj::setAmmoSupply(int seed)
	{		
		for(int i=0; i<N_WEAPONS; ++i)
		{			
			if (((seed % N_WEAPONS) == 0) || ((seed % (N_WEAPONS + 2)) == 0))
			{
				ammo[i] = 200 * ((seed % 4)+1) / Projectile::getDirectDamage(i);
			}
			else
			{
				ammo[i] = 0;
			}
		}
		ammo[0] = 10000;
		//ammo[1] = 10000;
	}

	void PlayerObj::getRectangle(Rectangle &rectangle) const
	{
		Vec size(RECTANGLE_SIZE, RECTANGLE_SIZE);
		
		rectangle.pos = this->pos - size/2.0f;
		rectangle.size = size;
	}

	void PlayerObj::hurt(int damage)
	{
		health -= damage;
		//if (health <= 0) nDeaths++;
	}

	void PlayerObj::respawn(const Pos &respawnPos)
	{ 
		assert(isDead());
		this->pos = respawnPos;
		health = 100;
	}

	//void PlayerObj::switchWeapon()
	//{		
	//	for(int i=1; i<=N_WEAPONS; ++i)
	//	{
	//		currentWeapon = (currentWeapon + 1) % N_WEAPONS;
	//		if (ammo[currentWeapon] > 0) break;
	//	}
	//}

	Projectile::Type PlayerObj::getNextWeapon(Projectile::Type weapon)
	{
		for(int i=1; i<=N_WEAPONS; ++i)
		{
			weapon = (weapon + 1) % N_WEAPONS;
			if (ammo[weapon] > 0) break;
		}
		return weapon;
	}
	
	//void PlayerObj::shoot(int time)
	//{
	//	int currentWeapon = getCurrentWeapon();
	//	assert(ammo[currentWeapon] >= 0);
	//	ammo[currentWeapon]--;
	//	nextShootTick = time + Projectile::getShootInterval(currentWeapon);
	//	//if (ammo[currentWeapon] == 0) switchWeapon();		
	//}

	bool PlayerObj::setTickDataAndCompare(int tick, const Pos &pos, Angle angle, Tickf nextShootTick)
	{
		UpdateData storedData;
		historyList.getData(tick, storedData);
		
		UpdateData data(pos, angle, nextShootTick);			
		historyList.setData(tick, data);

		return data != storedData;
	}

	void PlayerObj::updateToTickData(int tick)
	{
		UpdateData data;
		historyList.getData(tick, data);
		this->pos = data.pos;
		this->angle = data.angle;
		this->nextShootTick = data.nextShootTick;
	}

	void PlayerObj::updateToTickData(Tickf tick)
	{
		UpdateData data;
		historyList.getData(tick, data);
		this->pos = data.pos;
		this->angle = data.angle;
		this->nextShootTick = data.nextShootTick;
	}

	void PlayerObj::setUserCmd(const UserCmd *userCmd)
	{
		//StateCmds stateCmds(userCmd->stateCmds);

		//movingForward = stateCmds.getCurrentState(Cmds::FORWARD);
		//movingBackward = stateCmds.getCurrentState(Cmds::BACKWARD);
		//strafingLeft = stateCmds.getCurrentState(Cmds::LEFT);
		//strafingRight = stateCmds.getCurrentState(Cmds::RIGHT);
		this->userCmd = *userCmd;
		angle = userCmd->aimAngle;
	}

	int PlayerObj::getNTickShots(Projectile::Type weapon, int currentTick, bool continuous)
	{
		if (!continuous) std::cout << "      %%      continuous == false" << std::endl;
		Tickf startShootTick = continuous ? nextShootTick : tmax(nextShootTick, static_cast<Tickf>(currentTick)); 
		Tickf totalShootTicks = static_cast<Tickf>(currentTick + 1) - startShootTick;
		//if (!continuous) std::cout << "      %%      totalShootTicks: " << totalShootTicks << std::endl;
		int nShots = (totalShootTicks > 0.0) ? static_cast<int>(totalShootTicks / Projectile::getShootInterval(weapon)) + 1 : 0;
		//assert(nShots >= 0);
		return tmax(0, nShots);
	}

	void PlayerObj::updateNextShootTime(int currentTick)
	{
		assert(userCmd.isConsistent());

		//Tickf oldNextShootTick = nextShootTick;
		nextShootTick = getShotTick(currentTick, userCmd.nShots);

		//if (userCmd.nShots > 0) std::cout << "        ##       nextShootTick is now: " << nextShootTick << std::endl;

		//if (userCmd.shootAction == UserCmd::START_SHOOTING)
		//{
		//	nextShootTick = tmax(nextShootTick, static_cast<Tickf>(currentTick));
		//}
		//nextShootTick += Projectile::getShootInterval(userCmd.weapon) * static_cast<Tickf>(userCmd.nShots);
	}

	Tickf PlayerObj::getShotTick(int currentTick, int shotN)
	{
		assert(userCmd.isConsistent());
		
		Tickf startShootTick;
		if (userCmd.shootAction == UserCmd::START_SHOOTING)
		{
			startShootTick = tmax(nextShootTick, static_cast<Tickf>(currentTick));
		}
		else
		{
			//assert(nextShootTick >= static_cast<Tickf>(currentTick));
			startShootTick = nextShootTick;
		}
		return startShootTick + Projectile::getShootInterval(userCmd.weapon) * static_cast<Tickf>(shotN);
	}

	bool PlayerObj::isConsistent()
	{
		return historyList.isConsistent();
	}

}

