
#include "PlayerObj.h"

namespace Prototype
{

	const float PlayerObj::FORWARD_BACKWARD_SPEED = 100.0f / 1000.0f;
	const float PlayerObj::STRAFE_SPEED = 100.0f / 1000.0f;
	const float PlayerObj::RECTANGLE_SIZE = 20.0f;

	//PlayerObj::PlayerObj(size_t playerId, const Pos &pos)
	//	: playerId(playerId), pos(pos), angle(0.0f),
	//	 movingForward(false), movingBackward(false),
	//	 strafingLeft(false), strafingRight(false)
	//{}

	PlayerObj::PlayerObj(const Pos &pos, size_t nHistoryTicks)
		: historyList(nHistoryTicks), pos(pos), angle(Angle::PI/2.0f), health(100),
		 movingForward(false), movingBackward(false),
		 strafingLeft(false), strafingRight(false),
		 currentWeapon(0), nextShootTime(0)
	{
		setAmmoSupply(0);
		if (N_WEAPONS >= 2) ammo[1] = 10;
		
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

	void PlayerObj::switchWeapon()
	{		
		for(int i=1; i<=N_WEAPONS; ++i)
		{
			currentWeapon = (currentWeapon + 1) % N_WEAPONS;
			if (ammo[currentWeapon] > 0) break;
		}
	}
	
	void PlayerObj::shoot(int time)
	{
		assert(ammo[currentWeapon] >= 0);
		ammo[currentWeapon]--;
		nextShootTime = time + Projectile::getShootInterval(currentWeapon);
		if (ammo[currentWeapon] == 0) switchWeapon();		
	}

	void PlayerObj::updateToTickData(int tick)
	{
		UpdateData data(historyList.getData(tick));
		pos = data.pos;
		angle = data.angle;
	}

	void PlayerObj::updateToTickData(Tickf tick)
	{
		UpdateData data;
		historyList.getData(tick, data);
		pos = data.pos;
		angle = data.angle;
	}

}

