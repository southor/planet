#ifndef __projectile_h__
#define __projectile_h__

#include "Line.h"
#include "basic.h"
#include "GameObjId.h"
#include "HistoryList.inl"
#include "TimeHandler.h"

namespace Planet
{
	class Projectile : public Line
	{
	private:

		HistoryList<Pos, Projectile*> historyList;

	public:

		struct RenderLagMods
		{			
			double shooterObjLagMod;
			double viewerObjLagMod;
			
			RenderLagMods(double shooterObjLagMod, double viewerObjLagMod)
				: shooterObjLagMod(shooterObjLagMod), viewerObjLagMod(viewerObjLagMod)
			{}

			inline Tickf getRenderTick(Tickf currentTick, Projectile *projectile, int viewerObjLag) const
			{
				Tickf lag = static_cast<Tickf>(static_cast<double>(projectile->getObjLag()) * shooterObjLagMod + static_cast<double>(viewerObjLag) * viewerObjLagMod);
				return currentTick - lag;
			}
		};

		static const RenderLagMods RENDER_LAG_MODS;

		struct Properties
		{
			float speed; // distance units per milliseconds
			int directDamage;
			int blastDamage;
			float blastDistance;
			int shootInterval; // time interval in milliseconds
		};

		static const int N_TYPES = 2;
		typedef int Type;
		enum
		{			
			BULLET,
			ROCKET
		};
		static const Type DEFAULT_TYPE = BULLET;

		Projectile(Type type, const Pos &pos, const Vec &ext, GameObjId shooterId, size_t nHistoryTicks, Tickf shootTick, int objLag);

		~Projectile()											{}

		inline Pos getPos() const								{ return pos; }
		inline void setPos(const Pos &pos)						{ this->pos = pos; }
		inline Type getType() const								{ return type; }
		inline Vec getExt() const								{ return ext; }
		inline Tickf getShootTick() const						{ return shootTick; }

		const Line& getLine() const								{ return *this; }

		
		Pos getPosAtTick(Tickf currentTick, Tickf resultTick);
		static void getPosAtTick(int tick, const Pos &pos, Tickf resultTick, Pos &resultPos, Projectile *projectile);
		
		// @return distance units per tick
		inline float getSpeed() const							{ return properties[type].speed * static_cast<float>(TimeHandler::TICK_DELTA_TIME); }
		
		inline int getObjLag() const							{ return objLag; }
		inline int getDirectDamage() const						{ return properties[type].directDamage; }
		static inline int getDirectDamage(Type type)			{ return properties[type].directDamage; }
		
		// time interval in ticks
		static inline Tickf getShootInterval(Type type)			{ return static_cast<Tickf>(properties[type].shootInterval) / static_cast<Tickf>(TimeHandler::TICK_DELTA_TIME); }
		
		// @param blastPos The d-position along the line, 0.0 is beginning of line, 1.0 is end of line
		int getBlastDamage(float blastPos, const Pos &targetPos) const;
		
		
		GameObjId getShooterId() const							{ return shooterId; }

		inline void setTickData(int tick, const Pos &pos)		{ historyList.setData(tick, pos); }
		inline void storeToTickData(int tick)					{ historyList.setData(tick, pos); }
		inline void updateToTickData(int tick)					{ historyList.getData(tick, pos); }
		void updateToTickData(Tickf tick)						{ historyList.getData(tick, pos); }

		bool render;

	private:
		static const Properties properties[N_TYPES];

		Type type;
		
		GameObjId shooterId;
		Tickf shootTick; // the exact moment when it is shot.
		int objLag; // The client object lag that this projectile was shot with
	};
};

#endif

