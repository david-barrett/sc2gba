//Copyright Paul Reiche, Fred Ford. 1992-2002

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "gba.h"
#include "dispcnt.h"
#include "sc.h"


s16 TurnShip (pPlayer ai, s16 angle);
extern pAsteroid asteroids;
extern s16 planet_screenx,planet_screeny;

void Pursue (pPlayer ShipPtr, pObject obj);
void Entice (pPlayer ShipPtr, pObject obj);



//#include "starcon.h"

inline s16 GetVelocityTravelAngle(s32 x,s32 y)
{
	return FindAngle(0,0,x,y);
}

COUNT
PlotIntercept (pPlayer ElementPtr0, pObject ElementPtr1, COUNT
		max_turns, COUNT margin_of_error)
{
	s32 x1 = ElementPtr0->xpos+ElementPtr0->xspeed;
	s32 y1 = ElementPtr0->ypos+ElementPtr0->yspeed;
	s32 x2 = ElementPtr1->xpos+ElementPtr1->xspeed;
	s32 y2 = ElementPtr1->ypos+ElementPtr1->yspeed;
	s16 size = (ElementPtr0->offset+ElementPtr1->size)/2+margin_of_error;
	for (s16 i=1;i<max_turns;i++)
	{
		if (distanceBetweenPoints(x1,y1,x2,y2)<size)
			return i;
		x1+=ElementPtr0->xspeed;
		y1+=ElementPtr0->yspeed;
		x2+=ElementPtr1->xspeed;
		y1+=ElementPtr1->yspeed;
	}
	/*
	register SIZE dy;
	register SIZE time_y_0, time_y_1;
	POINT dst[2];
	RECT r0, r1;
	SIZE dx_0, dy_0, dx_1, dy_1;

	if ((ElementPtr0->state_flags | ElementPtr1->state_flags) & FINITE_LIFE)
	{
		if (!(ElementPtr0->state_flags & FINITE_LIFE))
		{
			if (ElementPtr1->life_span < max_turns)
				max_turns = ElementPtr1->life_span;
		}
		else if (!(ElementPtr1->state_flags & FINITE_LIFE))
		{
			if (ElementPtr0->life_span < max_turns)
				max_turns = ElementPtr0->life_span;
		}
		else
		{
			if (ElementPtr0->life_span < max_turns)
				max_turns = ElementPtr0->life_span;
			if (ElementPtr1->life_span < max_turns)
				max_turns = ElementPtr1->life_span;
		}
	}

	dst[0] = ElementPtr0->current.location;
	GetCurrentVelocityComponents (&ElementPtr0->velocity, &dx_0, &dy_0);
	dx_0 = (SIZE)VELOCITY_TO_WORLD ((long)dx_0 * (long)max_turns);
	dy_0 = (SIZE)VELOCITY_TO_WORLD ((long)dy_0 * (long)max_turns);

	dst[1] = ElementPtr1->current.location;
	GetCurrentVelocityComponents (&ElementPtr1->velocity, &dx_1, &dy_1);
	dx_1 = (SIZE)VELOCITY_TO_WORLD ((long)dx_1 * (long)max_turns);
	dy_1 = (SIZE)VELOCITY_TO_WORLD ((long)dy_1 * (long)max_turns);

	if (margin_of_error)
	{
		dst[1].y -= margin_of_error;
		time_y_0 = 1;
		time_y_1 = margin_of_error << 1;
	}
	else
	{
		GetFrameRect (
				ElementPtr0->IntersectControl.IntersectStamp.frame,
				&r0);
		GetFrameRect (
				ElementPtr1->IntersectControl.IntersectStamp.frame,
				&r1);

		dst[0].y += DISPLAY_TO_WORLD (r0.corner.y);
		dst[1].y += DISPLAY_TO_WORLD (r1.corner.y);
		time_y_0 = DISPLAY_TO_WORLD (r0.extent.height);
		time_y_1 = DISPLAY_TO_WORLD (r1.extent.height);
	}

	dy = dst[1].y - dst[0].y;
	time_y_0 = dy - time_y_0 + 1;
	time_y_1 = dy + time_y_1 - 1;
	dy = dy_0 - dy_1;

	if ((time_y_0 <= 0 && time_y_1 >= 0)
			|| (time_y_0 > 0 && dy >= time_y_0)
			|| (time_y_1 < 0 && dy <= time_y_1))
	{
		register SIZE dx;
		register SIZE time_x_0, time_x_1;

		if (margin_of_error)
		{
			dst[1].x -= margin_of_error;
			time_x_0 = 1;
			time_x_1 = margin_of_error << 1;
		}
		else
		{
			dst[0].x += DISPLAY_TO_WORLD (r0.corner.x);
			dst[1].x += DISPLAY_TO_WORLD (r1.corner.x);
			time_x_0 = DISPLAY_TO_WORLD (r0.extent.width);
			time_x_1 = DISPLAY_TO_WORLD (r1.extent.width);
		}

		dx = dst[1].x - dst[0].x;
		time_x_0 = dx - time_x_0 + 1;
		time_x_1 = dx + time_x_1 - 1;
		dx = dx_0 - dx_1;

		if ((time_x_0 <= 0 && time_x_1 >= 0)
				|| (time_x_0 > 0 && dx >= time_x_0)
				|| (time_x_1 < 0 && dx <= time_x_1))
		{
			if (dx == 0 && dy == 0)
				time_y_0 = time_y_1 = 0;
			else
			{
				SIZE t;
				long time_beg, time_end, fract;

				if (time_y_1 < 0)
				{
					t = time_y_0;
					time_y_0 = -time_y_1;
					time_y_1 = -t;
				}
				else if (time_y_0 <= 0)
				{
					if (dy < 0)
						time_y_1 = -time_y_0;
					time_y_0 = 0;
				}
				if (dy < 0)
					dy = -dy;
				if (dy < time_y_1)
					time_y_1 = dy;

				if (time_x_1 < 0)
				{
					t = time_x_0;
					time_x_0 = -time_x_1;
					time_x_1 = -t;
				}
				else if (time_x_0 <= 0)
				{
					if (dx < 0)
						time_x_1 = -time_x_0;
					time_x_0 = 0;
				}
				if (dx < 0)
					dx = -dx;
				if (dx < time_x_1)
					time_x_1 = dx;

				if (dx == 0)
				{
					time_beg = time_y_0;
					time_end = time_y_1;
					fract = dy;
				}
				else if (dy == 0)
				{
					time_beg = time_x_0;
					time_end = time_x_1;
					fract = dx;
				}
				else
				{
					long time_x, time_y;

					time_x = (long)time_x_0 * (long)dy;
					time_y = (long)time_y_0 * (long)dx;
					time_beg = time_x < time_y ? time_y : time_x;

					time_x = (long)time_x_1 * (long)dy;
					time_y = (long)time_y_1 * (long)dx;
					time_end = time_x > time_y ? time_y : time_x;

					fract = (long)dx * (long)dy;
				}

				if ((time_beg *= max_turns) < fract)
					time_y_0 = 0;
				else
					time_y_0 = (SIZE)(time_beg / fract);

				if (time_end >= fract) // just in case of overflow
					time_y_1 = max_turns - 1;
				else
					time_y_1 = (SIZE)((time_end * max_turns) / fract);
			}

			if (time_y_0 <= time_y_1)
			{
				if (margin_of_error != 0)
					return ((COUNT)time_y_0 + 1);
				else
				{
					POINT Pt0, Pt1;
					VELOCITY_DESC Velocity0, Velocity1;
					INTERSECT_CONTROL Control0, Control1;

					Pt0 = ElementPtr0->current.location;
					Velocity0 = ElementPtr0->velocity;
					Control0 = ElementPtr0->IntersectControl;

					Pt1 = ElementPtr1->current.location;
					Velocity1 = ElementPtr1->velocity;
					Control1 = ElementPtr1->IntersectControl;

					if (time_y_0)
					{
						GetNextVelocityComponents (&Velocity0, &dx_0, &dy_0, time_y_0);
						Pt0.x += dx_0;
						Pt0.y += dy_0;
						Control0.EndPoint.x = WORLD_TO_DISPLAY (Pt0.x);
						Control0.EndPoint.y = WORLD_TO_DISPLAY (Pt0.y);

						GetNextVelocityComponents (&Velocity1, &dx_1, &dy_1, time_y_0);
						Pt1.x += dx_1;
						Pt1.y += dy_1;
						Control1.EndPoint.x = WORLD_TO_DISPLAY (Pt1.x);
						Control1.EndPoint.y = WORLD_TO_DISPLAY (Pt1.y);
					}

					do
					{
						TIME_VALUE when;

						++time_y_0;

						GetNextVelocityComponents (&Velocity0, &dx_0, &dy_0, 1);
						Pt0.x += dx_0;
						Pt0.y += dy_0;

						GetNextVelocityComponents (&Velocity1, &dx_1, &dy_1, 1);
						Pt1.x += dx_1;
						Pt1.y += dy_1;

						Control0.IntersectStamp.origin = Control0.EndPoint;
						Control0.EndPoint.x = WORLD_TO_DISPLAY (Pt0.x);
						Control0.EndPoint.y = WORLD_TO_DISPLAY (Pt0.y);

						Control1.IntersectStamp.origin = Control1.EndPoint;
						Control1.EndPoint.x = WORLD_TO_DISPLAY (Pt1.x);
						Control1.EndPoint.y = WORLD_TO_DISPLAY (Pt1.y);
						when = DrawablesIntersect (&Control0,
								&Control1, MAX_TIME_VALUE);
						if (when)
						{
							if (when == 1
									&& time_y_0 == 1
									&& ((ElementPtr0->state_flags
									| ElementPtr1->state_flags) & APPEARING))
							{
								when = 0;
								Control0.EndPoint.x = WORLD_TO_DISPLAY (Pt0.x);
								Control0.EndPoint.y = WORLD_TO_DISPLAY (Pt0.y);

								Control1.EndPoint.x = WORLD_TO_DISPLAY (Pt1.x);
								Control1.EndPoint.y = WORLD_TO_DISPLAY (Pt1.y);
							}

							if (when)
								return ((COUNT)time_y_0);
						}
					} while (time_y_0 < time_y_1);
				}
			}
		}
	}
*/
	return (0);
}
/*
STARSHIPPTR CyborgDescPtr;

#ifdef DEBUG
static COUNT turn_counter[NUM_PLAYERS];
#endif // DEBUG
*/
static void
InitCyborg (pPlayer StarShipPtr)
{
	COUNT Index, Divisor;

#ifdef DEBUG
	//turn_counter[which_ship] = 0;
#endif // DEBUG=30;

	Index = StarShipPtr->maxspeed
			* StarShipPtr->accel_inc;
	if ((Divisor = StarShipPtr->turn_wait
			+ StarShipPtr->thrust_wait) > 0)
		Index /= Divisor;
	else
		Index >>= 1;
	StarShipPtr->ManeuverabilityIndex = Index;
#ifdef DEBUG
	//turn_counter[which_ship] = 0;
#endif // DEBUG
}

static void
ship_movement (pPlayer ShipPtr, pObject obj)
{
print("\nship movement");
	switch (obj->MoveState)
	{
		case PURSUE:
		print(" pursue");
			Pursue (ShipPtr, obj);
			break;
		case AVOID:
#ifdef NOTYET
			Avoid (ShipPtr, EvalDescPtr);
			break;
#endif // NOTYET
		case ENTICE:
		print(" entice");
			Entice (ShipPtr,obj);
			break;
		case NO_MOVEMENT:
		print(" nowt");
			break;
	}
}


s16
ship_weapons (pPlayer ShipPtr, pObject obj,  COUNT		margin_of_error)
//ELEMENTPTR OtherPtr, COUNT		margin_of_error)
{
//	SIZE delta_x, delta_y;
//	COUNT n, num_weapons;
//	ELEMENT Ship;
//	HELEMENT Weapon[6];
//	STARSHIPPTR StarShipPtr;

//	if (OBJECT_CLOAKED (OtherPtr))
	// if (ship && opp->cloak>0)
	//	margin_of_error +=  (40);

//	Ship = *ShipPtr;
//	GetNextVelocityComponents (&Ship.velocity,
//			&delta_x, &delta_y, 1);
//	Ship.next.location.x =
//			Ship.current.location.x + delta_x;
//	Ship.next.location.y =
//			Ship.current.location.y + delta_y;
//	Ship.current.location = Ship.next.location;
//
//	GetElementStarShip (&Ship, &StarShipPtr);
//	num_weapons =
//			(*StarShipPtr->RaceDescPtr->init_weapon_func) (&Ship, Weapon);
/*
	if ((n = num_weapons))
	{
		HELEMENT *WeaponPtr, w;
		//STARSHIPPTR StarShipPtr;
		ELEMENTPTR EPtr;

		WeaponPtr = &Weapon[0];
		do
		{
			w = *WeaponPtr;
			if (w)
			{
				EPtr = LockElement (w, &EPtr);
				if (EPtr->state_flags & APPEARING)
				{
					EPtr->next = EPtr->current;
					InitIntersectStartPoint (EPtr);
					InitIntersectEndPoint (EPtr);
					InitIntersectFrame (EPtr);
				}
*/
				//wrong wrong wrong
				//need to use weapon speed,off set etc
				pPlayer opp=(pPlayer)ShipPtr->opp;
				s16 angle = FindAngle(ShipPtr->xpos,ShipPtr->ypos,opp->xpos,opp->ypos);
			//	if (PlotIntercept (ShipPtr, (pPlayer)ShipPtr->opp,
			//			ShipPtr->range, margin_of_error))
				if ((TurnAngle(angle,ShipPtr->angle,45)==0)&&InRange(ShipPtr->xpos,ShipPtr->ypos,opp->xpos,opp->ypos,ShipPtr->range))
				{
					return 1;
					//UnlockElement (w);
					//break;
				}
/*
				UnlockElement (w);
				FreeElement (w);
			}
			++WeaponPtr;
		} while (--n);

		if ((num_weapons = n))
		{
			do
			{
				w = *WeaponPtr++;
				if (w)
					FreeElement (w);
			} while (--n);
		}
	}

	return (num_weapons > 0);*/
	return 0;
}

void
ship_intelligence (pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter)
//(ELEMENTPTR ShipPtr, EVALUATE_DESCPTR ObjectsOfConcern,		COUNT ConcernCounter)
{
	s16 ShipMoved, ShipFired;
	COUNT margin_of_error;
	//STARSHIPPTR StarShipPtr;

	//GetElementStarShip (ShipPtr, &StarShipPtr);
print("\n ship intel");
if (ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState == PURSUE)
				print("PURSE");
				else if (ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState == ENTICE)
				print("ENTICE");
				else
				print("NOTHING");

	ShipMoved = 1;
	if (ShipPtr->turn_turn == 0)
		ShipMoved = 0;
	if (ShipPtr->thrust_turn== 0)
		ShipMoved = 0;

	ShipFired = 1;
	if (ShipPtr->weapon_turn == 0)
	{
		ShipPtr->ship_input_state &= ~WEAPON;
		if (!(ShipPtr->ship_flags & SEEKING_WEAPON))
			ShipFired = 0;
	}

	if (ShipPtr->ai== AWESOME)
		margin_of_error = 0;
	else if (ShipPtr->ai== GOOD)
		margin_of_error = 20;
	else // if (PlayerControl[cur_player] & STANDARD_RATING)
		margin_of_error = 40;

	ObjectsOfConcern += ConcernCounter;
	while (ConcernCounter--)
	{
		print("\n counter");
		print(ConcernCounter);
		--ObjectsOfConcern;
	//if 	(ObjectsOfConcern->ObjectPtr)
	//	{
			if (ObjectsOfConcern->MoveState ==NO_MOVEMENT)
				print("no move");
			else if (ObjectsOfConcern->MoveState ==ENTICE)
				print("enticve");
			else if (ObjectsOfConcern->MoveState ==PURSUE)
				print("pursue");
			else if (ObjectsOfConcern->MoveState ==AVOID)
				print("avoid");

			if (!ShipMoved && ObjectsOfConcern->MoveState !=NO_MOVEMENT
					&& (ConcernCounter != ENEMY_WEAPON_INDEX ||
					 ObjectsOfConcern->MoveState == PURSUE
					|| (ObjectsOfConcern->type==CREW)
					|| ShipPtr->ManeuverabilityIndex >= MEDIUM_SHIP))
			{
					print("\n obj x=");
						print(ObjectsOfConcern->xpos+ObjectsOfConcern->xspeed);
					print("\n obj y=");
		print(ObjectsOfConcern->ypos+ObjectsOfConcern->yspeed);
				ship_movement (ShipPtr, ObjectsOfConcern);
				ShipMoved = 1;
			}
			if (!ShipFired
				&& (ConcernCounter == ENEMY_SHIP_INDEX
				|| (ConcernCounter == ENEMY_WEAPON_INDEX
				&& ObjectsOfConcern->MoveState != AVOID)))
				{
				ShipFired = ship_weapons (ShipPtr,
				ObjectsOfConcern, margin_of_error);
				if (ShipFired)
					ShipPtr->ship_input_state |= WEAPON;
			}
		//}
	}
}

s16
TurnShip (pPlayer ai, s16 angle)
{/*
	COUNT f, ship_delta_facing;
	//STARSHIPPTR StarShipPtr;

	//GetElementStarShip (ShipPtr, &StarShipPtr);
	f = ShipPtr->angle;
	ship_delta_facing = ModifyAngle(angle ,f*-1);
	if (ship_delta_facing)
	{
		if (ship_delta_facing == 180))
					(Ran(0,1) == 1 ?
					TurnLeft(ShipPtr)) :
					TurnRight(ShipPtr)));

		if (ship_delta_facing < 180)
		{
			TurnLeft(ShipPtr))
		}
		else
		{
			StarShipPtr->ship_input_state |= LEFT;
			--f;
			ShipPtr->next.image.frame =
					DecFrameIndex (ShipPtr->current.image.frame);
		}

#ifdef NOTYET
		if (((StarShipPtr->ship_input_state & (LEFT | RIGHT))
				^ (StarShipPtr->cur_status_flags & (LEFT | RIGHT))) == (LEFT | RIGHT))
			StarShipPtr->ship_input_state &= ~(LEFT | RIGHT);
		else
#endif // NOTYET
		{
			StarShipPtr->ShipFacing = NORMALIZE_FACING (f);

			return (TRUE);
		}
	}

*/
//	print("\n angle=");
//	print(angle);
	int ret=TurnAngle(angle,ai->angle,15);
	if (ret<0)
		ai->ship_input_state |= LEFT;
	else if (ret>0)
		ai->ship_input_state |= RIGHT;

}

s16
ThrustShip (pPlayer ShipPtr, s16 angle)
{
	s16 ShouldThrust=1;
//	STARSHIPPTR StarShipPtr;

//	GetElementStarShip (ShipPtr, &StarShipPtr);
	print("enter thrust");
	if (ShipPtr->ship_input_state & THRUST)
		ShouldThrust = 1;
	else if (ShipPtr->angle==GetVelocityTravelAngle (ShipPtr->xspeed,ShipPtr->yspeed)
			//&& (StarShipPtr->cur_status_flags
			&&(ShipPtr->speed>=ShipPtr->maxspeed))//& (SHIP_AT_MAX_SPEED | SHIP_BEYOND_MAX_SPEED))
		//	&& !(StarShipPtr->cur_status_flags & SHIP_IN_GRAVITY_WELL))
		ShouldThrust = 0;
	else
	{
		s16 ship_delta_facing;

		//ship_delta_facing = ModifyAngle(angle,45
			//	- ShipPtr->angle );
		if ((TurnAngle(angle,ShipPtr->angle,45)==0)
				|| ((ShipPtr->speed>=ShipPtr->maxspeed)
				&& (TurnAngle(angle,ShipPtr->angle,180)==0)))
			ShouldThrust = 1;
		else
			ShouldThrust = 0;
	}

	if (ShouldThrust)
	{
		ShipPtr->ship_input_state |= THRUST;
		print("should thrust");
	}
	else
	 	print("not thrust");


	return (ShouldThrust);
}
/*
void
PursueWeapon (pPlayer ShipPtr, pWeapon w)
{

	s16 desired_thrust_angle = FindAngle(ShipPtr->xpos,ShipPtr->ypos,w->xpos,w->ypos);

	if (ShipPtr->turn_turn==0)
		TurnShip (ShipPtr, desired_thrust_angle);

	if (ShipPtr->thrust_turn==0)
		ThrustShip (ShipPtr, desired_thrust_angle);
}
*/
void
Pursue (pPlayer ShipPtr, pObject obj)
{

	s16 desired_thrust_angle = FindAngle(ShipPtr->xpos,ShipPtr->ypos,obj->xpos,obj->ypos);
	s16 desired_turn_angle = ModifyAngle(desired_thrust_angle,180);
	// other player's ship

	if (obj->type==PLAYER)
	{
		pPlayer enemyShipPtr = (pPlayer)obj->parent;
		//GetElementStarShip (ShipPtr, &StarShipPtr);
		//GetElementStarShip (OtherObjPtr, &EnemyStarShipPtr);
		if ((ShipPtr->ManeuverabilityIndex >= FAST_SHIP
				&& ShipPtr->range > CLOSE_RANGE_WEAPON)
				|| (obj->which_turn >= 24
				&& (ShipPtr->maxspeed * 2 / 3 <	enemyShipPtr->maxspeed
				|| (enemyShipPtr->speed>enemyShipPtr->maxspeed))))
		{
			//UWORD ship_flags;
			print("here");
			//ship_flags = EnemyStarShipPtr->RaceDescPtr->ship_info.ship_flags;
						// you're maneuverable
			if (ShipPtr->ManeuverabilityIndex >= MEDIUM_SHIP)
			{
				print("\nhere2");
				//UWORD fire_flags;
				//COUNT facing;

				//for (fire_flags = FIRES_FORE, facing = EvalDescPtr->facing;
				//		fire_flags <= FIRES_LEFT;
				//		fire_flags <<= 1, facing += QUADRANT)
				//{
					if
					(
							// he's dangerous in this direction
						//(ship_flags & fire_flags) && //asume only forward fire enimies
							// he's facing direction you want to go
							(TurnAngle(desired_turn_angle,enemyShipPtr->angle,90)==0)

						&& (
							// he's moving
						(enemyShipPtr->xspeed != 0 || enemyShipPtr->yspeed != 0)
						&&
							// he's coasting backwards
						TurnAngle(enemyShipPtr->angle,GetVelocityTravelAngle (enemyShipPtr->xspeed,enemyShipPtr->yspeed),180)==0)
					//	ModifyAngle (
					//	(GetVelocityTravelAngle (enemyStartShipPtr->xspeed,enemyStartShipPtr->yspeed),
					//	+ 180 - enemyStartShipPtr->angle + (45 + (45 >> 1)))
					//	<= ((45 + (45 >> 1)) << 1))
					)
					{
							// catch him on the back side
						desired_thrust_angle = desired_turn_angle;
						print("reverse");
					//	break;
					}
				//}
			}

			if (desired_thrust_angle != desired_turn_angle
					&& (enemyShipPtr->xspeed || enemyShipPtr->yspeed)
				//	&& EvalDescPtr->which_turn >= 24
					&& (TurnAngle (desired_thrust_angle,GetVelocityTravelAngle (enemyShipPtr->xspeed,enemyShipPtr->yspeed),90)==0)
					&& (TurnAngle(
						GetVelocityTravelAngle (enemyShipPtr->xspeed,enemyShipPtr->yspeed),
					GetVelocityTravelAngle (enemyShipPtr->xspeed,enemyShipPtr->yspeed),45)
					==0)
					&& ((ShipPtr->speed>=ShipPtr->maxspeed)
					&& (ShipPtr->speed<enemyShipPtr->maxspeed))
				//	|| (ship_flags & DONT_CHASE))
					|| (TurnAngle(desired_turn_angle,ShipPtr->angle,45)==0))
					{
						print("\n sec");
				desired_thrust_angle = desired_turn_angle;
			}
		}

print("\n angle=");
print(desired_thrust_angle);
	}//if player
	if (ShipPtr->turn_turn==0)
		TurnShip (ShipPtr, desired_thrust_angle);

	if (ShipPtr->thrust_turn==0)
		ThrustShip (ShipPtr, desired_thrust_angle);


}

void
Entice (pPlayer ShipPtr, pObject obj)//, EVALUATE_DESCPTR EvalDescPtr)
{
	pPlayer opp=(pPlayer)ShipPtr->opp;
	//INPUT_STATE maneuver_state;
	COUNT desired_thrust_angle, desired_turn_angle;
	COUNT cone_of_fire, travel_angle;
	s16  delta_x, delta_y;
	s16  ship_delta_x, ship_delta_y;
	s16  other_delta_x, other_delta_y;
//	ELEMENTPTR OtherObjPtr;
	s16 ShipVelocity, OtherVelocity;
//	STARSHIPPTR StarShipPtr;
//	RACE_DESCPTR RDPtr;
/*
	ShipVelocity = ShipPtr->speed;
	GetNextVelocityComponents (&ShipVelocity,
			&ship_delta_x, &ship_delta_y, EvalDescPtr->which_turn);
	ShipPtr->next.location.x =
			ShipPtr->current.location.x + ship_delta_x;
	ShipPtr->next.location.y =
			ShipPtr->current.location.y + ship_delta_y;

	OtherObjPtr = (ELEMENTPTR)EvalDescPtr->ObjectPtr;
	OtherVelocity = OtherObjPtr->velocity;
	GetNextVelocityComponents (&OtherVelocity,
			&other_delta_x, &other_delta_y, EvalDescPtr->which_turn);

	delta_x = (OtherObjPtr->current.location.x + other_delta_x)
			- ShipPtr->next.location.x;
	delta_y = (OtherObjPtr->current.location.y + other_delta_y)
			- ShipPtr->next.location.y;
	delta_x = WRAP_DELTA_X (delta_x);
	delta_y = WRAP_DELTA_Y (delta_y);

	desired_thrust_angle = ARCTAN (delta_x, delta_y);
	*/
	desired_thrust_angle = FindAngle(ShipPtr->xpos+ShipPtr->xspeed,ShipPtr->ypos+ShipPtr->yspeed,
		obj->xpos+obj->xspeed,obj->ypos+obj->yspeed);

		print("\ndesired thrust angle");
		print(desired_thrust_angle);
		print("\n ship x=");
		print(ShipPtr->xpos+ShipPtr->xspeed);
		print("\n ship y=");
		print(ShipPtr->ypos+ShipPtr->yspeed);
		print("\n obj x=");
		print(obj->xpos+obj->xspeed);
		print("\n obj y=");
		print(obj->ypos+obj->yspeed);



	s8 maneuver_state = 0;
	if (ShipPtr->turn_wait == 0)
		maneuver_state |= LEFT | RIGHT;
	if (ShipPtr->thrust_wait == 0)
		maneuver_state |= THRUST;

/*
	delta_x = ship_delta_x - other_delta_x;
	delta_y = ship_delta_y - other_delta_y;
	travel_angle = ARCTAN (delta_x, delta_y);
	*/
	travel_angle=FindAngle(ShipPtr->xpos+ShipPtr->xspeed,ShipPtr->ypos+ShipPtr->yspeed,
		opp->xpos,opp->ypos);
	desired_turn_angle = ModifyAngle (desired_thrust_angle,180);

	//GetElementStarShip (ShipPtr, &StarShipPtr);
	//RDPtr = StarShipPtr->RaceDescPtr;



	if (obj->MoveState == AVOID)
	{
		desired_turn_angle = ModifyAngle(desired_turn_angle,-1*obj->angle);

		TurnAngle(desired_turn_angle,ShipPtr->angle,0)<0?desired_thrust_angle = LEFT:desired_thrust_angle = RIGHT;
		/*
		if (NORMALIZE_FACING (ANGLE_TO_FACING (desired_turn_angle)))
		{
			if (desired_turn_angle <= HALF_CIRCLE)
				desired_thrust_angle = RIGHT;
			else // if (desired_turn_angle > HALF_CIRCLE)
				desired_thrust_angle = LEFT;
		}
		else
		{
			desired_turn_angle = NORMALIZE_ANGLE (
					FACING_TO_ANGLE (StarShipPtr->ShipFacing)
					- EvalDescPtr->facing
					);
			if ((desired_turn_angle & (HALF_CIRCLE - 1)) == 0)
				desired_thrust_angle = TFB_Random () & 1 ? RIGHT : LEFT;
			else
				desired_thrust_angle = desired_turn_angle < HALF_CIRCLE ? RIGHT : LEFT;
		}
		*/
		if (desired_thrust_angle == LEFT)//looks wrong to me
		{

			desired_thrust_angle = ModifyAngle(obj->angle,-45);

		}
		else
		{

			desired_thrust_angle = ModifyAngle(obj->angle,45);
		}

		//desired_thrust_angle = desired_thrust_angle;
	}
	//else if (GRAVITY_MASS (OtherObjPtr->mass_points))
	else if (obj->type==PLANET)
	{
		COUNT planet_facing,awayfacing;

		planet_facing = desired_thrust_angle;
		cone_of_fire = ModifyAngle(planet_facing,90-ShipPtr->angle);

		awayfacing=ModifyAngle(planet_facing,180);



		if (ShipPtr->accel_inc !=
				ShipPtr->maxspeed)
			maneuver_state &= ~THRUST;

				// if not pointing towards planet
		if (TurnAngle(ShipPtr->angle,awayfacing,90)==0)
			desired_turn_angle = desired_thrust_angle;
				// if pointing directly at planet
		else if (TurnAngle(ShipPtr->angle,planet_facing,90)==0)
			desired_turn_angle = travel_angle;
		else /*if (cone_of_fire == 0
				|| cone_of_fire == ANGLE_TO_FACING (QUADRANT << 1)
				|| (!(maneuver_state & THRUST)
				&& (cone_of_fire < ANGLE_TO_FACING (OCTANT)
				|| cone_of_fire > ANGLE_TO_FACING ((QUADRANT << 1) - OCTANT))))*/
		{
			desired_turn_angle = (ShipPtr->angle);
			if (TurnAngle(desired_turn_angle,travel_angle,180)!=0)
				desired_turn_angle = travel_angle;
			if (ShipPtr->thrust_wait == 0)
				maneuver_state |= THRUST;
		}

		desired_thrust_angle = desired_turn_angle;
	}

	else
	{
		COUNT WRange;

		WRange = ShipPtr->range; //this this is player

		//cone_of_fire = ModifyAngle (desired_turn_angle,
		//		-1* EvalDescPtr->facing) + 45;
		if (obj->type==PLAYER)
		{
			print("\n enticing other ship");
			//UWORD fire_flags, ship_flags;
			//COUNT facing;
		//	STARSHIPPTR EnemyStarShipPtr;

		//	GetElementStarShip (OtherObjPtr, &EnemyStarShipPtr);
		//	ship_flags = EnemyStarShipPtr->RaceDescPtr->ship_info.ship_flags;
		//	for (fire_flags = FIRES_FORE, facing = EvalDescPtr->facing;
		//			fire_flags <= FIRES_LEFT;
		//			fire_flags <<= 1, facing += QUADRANT)
		//	{
				if
				(
						// he's dangerous in this direction
				//	(ship_flags & fire_flags)&&
						// he's facing direction you want to go
					 //(cone_of_fire = NORMALIZE_ANGLE (
					//desired_turn_angle - facing + OCTANT
					//)) <= QUADRANT
					(TurnAngle(desired_turn_angle,opp->angle,45)==0)
						// he's moving
					&& ((opp->xspeed != 0 || opp->yspeed != 0)
						// he's coasting backwards
					&&TurnAngle(opp->angle,GetVelocityTravelAngle (opp->xspeed,opp->yspeed),180)==0)
					//&& NORMALIZE_ANGLE (
					//(GetVelocityTravelAngle (&OtherVelocity) + HALF_CIRCLE)
					//- facing + OCTANT) <= QUADRANT)
				)
				{
						// need to be close for a kill
					if (WRange < LONG_RANGE_WEAPON)
							//&& EvalDescPtr->which_turn <= 32)
					{
						// catch him on the back side
						desired_thrust_angle = desired_turn_angle;
						print("\nturning away from opp");
						goto DoManeuver;
					}

		//			break;
				}
		//	}
/* taken out otherwise wont veer off
			if (obj->which_turn <= 8
					&& ShipPtr->maxspeed <=
					opp->maxspeed)
				{
				print("speed");
				goto DoManeuver;
				}
*/

		}

		if
		(

					// not at full speed
			 (ShipPtr->speed>=ShipPtr->maxspeed)
			&& (PlotIntercept (
					ShipPtr, obj, 40, CLOSE_RANGE_WEAPON *2
					)

			)
		)
		{
			print("A");
			if
			(
					// pointed straight at him
				(TurnAngle (desired_thrust_angle,ShipPtr->angle,45)==0)

					// or not exposed to business end
					//TODO
				//|| cone_of_fire > QUADRANT
			)
			{
				print("B");
				desired_thrust_angle = desired_turn_angle;
			}
			else
			{
				print("C");

				desired_thrust_angle = ShipPtr->angle;
				desired_turn_angle = desired_thrust_angle;
			}
		}
		else if ((cone_of_fire = PlotIntercept (
				ShipPtr, obj, 10, WRange- (WRange >> 2)	)))
		{
			if (ShipPtr->accel_inc !=ShipPtr->maxspeed

							// and already at full speed
				&&(ShipPtr->speed>=ShipPtr->maxspeed)
							// and facing away from enemy
					&& ((TurnAngle(desired_turn_angle,ShipPtr->angle,180)==0)
							// or not on collision course
					|| !PlotIntercept (
							ShipPtr, obj, 30, CLOSE_RANGE_WEAPON << 1
							)))
				{
				print("D");
				maneuver_state &= ~THRUST;
				}
					// veer off
			else if (cone_of_fire == 1
					|| ShipPtr->maxspeed !=
					ShipPtr->accel_inc)
			{
				if (maneuver_state & (LEFT | RIGHT))
				{
					print("E");
					TurnShip (ShipPtr, desired_turn_angle);
					maneuver_state &= ~(LEFT | RIGHT);
				}

				if (TurnAngle(desired_thrust_angle,ShipPtr->angle,45)==0)
				{
					print("F");
					desired_thrust_angle = ShipPtr->angle;
				}
				else
				{
					print("G");
					desired_thrust_angle = desired_turn_angle;
				}
			}
		}
	}

DoManeuver:
print("do maneuver");
	if (ShipPtr->turn_turn==0)
		TurnShip (ShipPtr, desired_thrust_angle);

	if (ShipPtr->thrust_turn==0)
		ThrustShip (ShipPtr, desired_thrust_angle);
}
/*
void
Avoid (ELEMENTPTR ShipPtr, EVALUATE_DESCPTR EvalDescPtr)
{
	(void) ShipPtr;  // Satisfying compiler (unused parameter)
	(void) EvalDescPtr;  // Satisfying compiler (unused parameter)
}
*/

//BATTLE_INPUT_STATE

s16
tactical_intelligence (pPlayer StarShipPtr )
{

	//ELEMENTPTR ShipPtr;
	//ELEMENT Ship;
	COUNT ShipFacing;
	//HELEMENT hElement, hNextElement;
	COUNT ConcernCounter;
	Object ObjectsOfConcern[10],gameObjs[19];
	int ShipMoved, UltraManeuverable;
	//STARSHIPPTR StarShipPtr, EnemyStarShipPtr;
	//RACE_DESCPTR RDPtr, EnemyRDPtr;

	if (StarShipPtr->ManeuverabilityIndex == 0)
		InitCyborg (StarShipPtr);

	ShipMoved = 1;
	// think this means std will never do specials
	//if (StarShipPtr->ai==STANDARD)
	//	++StarShipPtr->special_counter;

	ShipFacing = StarShipPtr->angle;
print("\n tact intel");
//need object
	for (ConcernCounter = 0;
			ConcernCounter <= FIRST_EMPTY_INDEX; ++ConcernCounter)
	{
		ObjectsOfConcern[ConcernCounter].parent = 0;
		ObjectsOfConcern[ConcernCounter].MoveState = NO_MOVEMENT;
		ObjectsOfConcern[ConcernCounter].which_turn = (COUNT)~0;
	}
	--ConcernCounter;

	UltraManeuverable = (
			StarShipPtr->accel_inc ==
			StarShipPtr->maxspeed
			&& StarShipPtr->ManeuverabilityIndex >= MEDIUM_SHIP
			);

	if (StarShipPtr->turn_turn == 0)
	{
		ShipMoved = 0;
		StarShipPtr->ship_input_state &= ~(LEFT | RIGHT);
	}
	if (StarShipPtr->thrust_turn == 0)
	{
		ShipMoved = 0;
		StarShipPtr->ship_input_state &= ~THRUST;
	}

	/* going to cheat here - we should be able to make this global - much easier
	contrust list of all objects in the game
	0 = opp
	1 = planet
	2-6 = asteroids
	7-18 = opp weapons
	perhaps should consider own weapons??
	*/

	pPlayer opp = (pPlayer)StarShipPtr->opp;

	Object tmp_obj;

	tmp_obj.type=PLAYER;
	tmp_obj.parent=(void*)StarShipPtr;
	tmp_obj.xpos=StarShipPtr->xpos;
	tmp_obj.ypos=StarShipPtr->ypos ;
	tmp_obj.xscreen=StarShipPtr->xscreen;
	tmp_obj.yscreen=StarShipPtr->yscreen;
	tmp_obj.angle=StarShipPtr->angle;
	tmp_obj.xspeed=StarShipPtr->xspeed;
	tmp_obj.yspeed=StarShipPtr->yspeed;
	tmp_obj.life=-1;
	tmp_obj.life=StarShipPtr->offset;

	gameObjs[0].type=PLAYER;
	gameObjs[0].parent=(void*)opp;
	gameObjs[0].xpos=opp->xpos;
	gameObjs[0].ypos=opp->ypos ;
	gameObjs[0].xscreen=opp->xscreen;
	gameObjs[0].yscreen=opp->yscreen;
	gameObjs[0].angle=opp->angle;
	gameObjs[0].xspeed=opp->xspeed;
	gameObjs[0].yspeed=opp->yspeed;
	gameObjs[0].life=-1;
	gameObjs[0].life=opp->offset;


	gameObjs[1].type=PLANET;
	gameObjs[1].parent=0; // fix??
	gameObjs[1].xpos=planetx;
	gameObjs[1].ypos=planety;
	gameObjs[1].xscreen=planet_screenx;
	gameObjs[1].yscreen=planet_screeny;
	gameObjs[1].angle=0;
	gameObjs[1].xspeed=0;
	gameObjs[1].yspeed=0;
	gameObjs[1].life=-1;
	gameObjs[1].life=64;

	for (int i=0;i<5;i++)
	{
		gameObjs[i+2].type=ASTEROID;
		gameObjs[i+2].parent=&asteroids[i];
		gameObjs[i+2].xpos=asteroids[i].xpos;
		gameObjs[i+2].ypos=asteroids[i].ypos ;
		gameObjs[i+2].xscreen=asteroids[i].xscreen;
		gameObjs[i+2].yscreen=asteroids[i].yscreen;
		gameObjs[i+2].angle=asteroids[i].angle;
		gameObjs[i+2].xspeed=asteroids[i].xspeed;
		gameObjs[i+2].yspeed=asteroids[i].yspeed;
		gameObjs[i+2].life=-1;
		gameObjs[i+2].size=16;
	}
	int j=7;
	for (int i=0;i<12;i++)
		{
			if (opp->weapon[i].life>0)
			{
				gameObjs[j].type=opp->weapon[i].type;
				gameObjs[j].parent=&opp->weapon[i];
				gameObjs[j].xpos=opp->weapon[i].xpos;
				gameObjs[j].ypos=opp->weapon[i].ypos ;
				gameObjs[j].xscreen=opp->weapon[i].xscreen;
				gameObjs[j].yscreen=opp->weapon[i].yscreen;
				gameObjs[j].angle=opp->weapon[i].angle;
				gameObjs[j].xspeed=opp->weapon[i].xspeed;
				gameObjs[j].yspeed=opp->weapon[i].yspeed;
				gameObjs[j].life=opp->weapon[i].life;
				gameObjs[j].size=opp->weapon[i].size;
				j++;
			}
	}

	for (int i=0;i<j;i++)
	{
		Object ed=gameObjs[i];



		//LockElement (hElement, &ed.ObjectPtr);
	//	hNextElement = GetSuccElement (ed.ObjectPtr);
	/*
		if (CollisionPossible (ed.ObjectPtr, &Ship))
		{
			SIZE dx, dy;

			dx = ed.ObjectPtr->next.location.x
					- Ship.next.location.x;
			dy = ed.ObjectPtr->next.location.y
					- Ship.next.location.y;
			dx = WRAP_DELTA_X (dx);
			dy = WRAP_DELTA_Y (dy);
			if (GRAVITY_MASS (ed.ObjectPtr->mass_points))
			{
				COUNT maneuver_turn, ship_bounds;
				RECT ship_footprint;

				if (UltraManeuverable)
					maneuver_turn = 16;
				else if (StarShipPtr->ManeuverabilityIndex <= MEDIUM_SHIP)
					maneuver_turn = 48;
				else
					maneuver_turn = 32;

				GetFrameRect (SetAbsFrameIndex (
						Ship.IntersectControl.IntersectStamp.frame, 0
						), &ship_footprint);
				ship_bounds = (COUNT)(ship_footprint.extent.width
						+ ship_footprint.extent.height);

				if (!ShipMoved && (ed.which_turn =
						PlotIntercept (ed.ObjectPtr, &Ship, maneuver_turn,
						DISPLAY_TO_WORLD (30 + (ship_bounds * 3 // << 2 )))))
				{
					if (ed.which_turn > 1
							|| PlotIntercept (ed.ObjectPtr, &Ship, 1,
							DISPLAY_TO_WORLD (35 + ship_bounds))
							|| PlotIntercept (ed.ObjectPtr, &Ship,
							maneuver_turn << 1,
							DISPLAY_TO_WORLD (40 + ship_bounds)) > 1)
					{
						ed.facing = ARCTAN (-dx, -dy);
						if (UltraManeuverable)
							ed.MoveState = AVOID;
						else
							ed.MoveState = ENTICE;

						ObjectsOfConcern[GRAVITY_MASS_INDEX] = ed;
					}
					else if (!UltraManeuverable)
					{
						++Ship.thrust_wait;
						if (Ship.turn_wait)
							ShipMoved = TRUE;
					}
				}
			}
			else */ if (ed.type==PLAYER)
			{

			print("\nanalysing other player");
				if (opp->ManeuverabilityIndex == 0)
					InitCyborg (opp);
/*
				ed.which_turn = WORLD_TO_TURN (
						square_root ((long)dx * dx + (long)dy * dy)
						);
				if (ed.which_turn > ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn)
				{
					UnlockElement (hElement);
					continue;
				}
				else if (ed.which_turn == 0)
					ed.which_turn = 1;
*/
				ObjectsOfConcern[ENEMY_SHIP_INDEX] = ed;
				//ObjectsOfConcern[ENEMY_SHIP_INDEX].angle = opp->angle;
				//ObjectsOfConcern[ENEMY_SHIP_INDEX].angle = opp->angle;
				int tmp=PlotIntercept(StarShipPtr,&ed,20,0);

				ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn = tmp==0?100:tmp;


				if (ShipMoved
				//		|| ed.ObjectPtr->mass_points > MAX_SHIP_MASS
						|| (opp->range < LONG_RANGE_WEAPON
						&& (opp->range <= CLOSE_RANGE_WEAPON
						|| (opp->range >= LONG_RANGE_WEAPON
						&& (opp->ship_flags & SEEKING_WEAPON))
						|| (StarShipPtr->maxspeed < opp->maxspeed
						&& StarShipPtr->range <	opp->range))))
					ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState = PURSUE;
				else
					ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState = ENTICE;

				if ((opp->ship_flags & IMMEDIATE_WEAPON)
					 && ship_weapons (opp, &tmp_obj, 0))//can be hit

				{
					ed.which_turn = 1;
					ed.MoveState = AVOID;
					//ed.facing = ObjectsOfConcern[ENEMY_SHIP_INDEX].facing;

					ObjectsOfConcern[ENEMY_WEAPON_INDEX] = ed;
				}
				if (ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState == PURSUE)
				print("PURSE");
				else if (ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState == ENTICE)
				print("ENTICE");
				else
				print("NOTHING");
			}//if player
			else if (ed.type==ASTEROID||ed.type==PLANET)//if (ed->pParent == 0)
			{
				//if (ed.type==ASTEROID||ed.type==PLANET)
				//{

					ed.which_turn = distanceBetweenPoints(StarShipPtr->xpos,
						StarShipPtr->xpos,ed.xpos,ed.ypos);

					if (ed.which_turn <
							ObjectsOfConcern[FIRST_EMPTY_INDEX].which_turn)
					{
						ed.MoveState = PURSUE;
						//ed.facing = GetVelocityTravelAngle (
						//		&ed.ObjectPtr->velocity
							//	);

						ObjectsOfConcern[FIRST_EMPTY_INDEX] = ed;

					}
				//}
			}
			else if /*((ed.ObjectPtr->state_flags & (GOOD_GUY | BAD_GUY)) !=
					(Ship.state_flags & (GOOD_GUY | BAD_GUY))
					&& ed.ObjectPtr->preprocess_func != crew_preprocess
					&&*/( ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn > 1
					&& ed.life > 0)

			{
				if (((opp->ship_flags & SEEKING_WEAPON)
					//	&& ed.ObjectPtr->next.image.farray !=
					//	EnemyRDPtr->ship_data.special)
						|| ((opp->ship_flags & SEEKING_SPECIAL)
					//	&& ed.ObjectPtr->next.image.farray ==
					//	EnemyRDPtr->ship_data.special))
					)))
				{
					if ((!(ed.life!=-1 ||ed.type==CREW)
							&& StarShipPtr->maxspeed > 8 )
							|| TurnAngle(GetVelocityTravelAngle (ed.xspeed,ed.yspeed),
							GetVelocityTravelAngle (StarShipPtr->xspeed,StarShipPtr->yspeed),180)==0)
					{

						//ed.which_turn = 0;
					}
					else
					{
						ed.which_turn = distanceBetweenPoints(StarShipPtr->xpos,
							StarShipPtr->xpos,ed.xpos,ed.ypos);

						ed.MoveState = ENTICE;
						if (UltraManeuverable)
						{
							if (ed.which_turn == 0)
								ed.which_turn = 1;
							else if (ed.which_turn > 16)
								ed.which_turn = 0;
						}
						else if (ed.which_turn == 0)
							ed.which_turn = 1;
						else if (ed.which_turn > 16
								|| (StarShipPtr->ManeuverabilityIndex > MEDIUM_SHIP
								&& ed.which_turn > 8))
							ed.which_turn = 0;
					}
				}//end if seeking
				else if (!(StarShipPtr->ai==AWESOME))
					ed.which_turn = 0;
				else
				{
					ed.which_turn =
						PlotIntercept (StarShipPtr,&ed, ed.life,40);
					ed.MoveState = AVOID;
				}

				if (ed.which_turn > 0
						&& (ed.which_turn <
						ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn
						|| (ed.which_turn ==
						ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn
						&& ed.MoveState == AVOID)))
				{
					ObjectsOfConcern[ENEMY_WEAPON_INDEX] = ed;
				}
			}
			else if ((ed.type==CREW)
					//&& ((!(ed.ObjectPtr->state_flags & IGNORE_SIMILAR)
					//&& (ed.ObjectPtr->state_flags & (GOOD_GUY | BAD_GUY)) ==
					//(Ship.state_flags & (GOOD_GUY | BAD_GUY)))
					//|| ed.ObjectPtr->preprocess_func == crew_preprocess)
					&& ObjectsOfConcern[CREW_OBJECT_INDEX].which_turn > 1)
			{
				ed.which_turn = distanceBetweenPoints(StarShipPtr->xpos,
							StarShipPtr->xpos,ed.xpos,ed.ypos);

				if (ed.which_turn == 0)
					ed.which_turn = 1;

				if (ObjectsOfConcern[CREW_OBJECT_INDEX].which_turn >
						ed.which_turn
						&& (ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn > 32
						|| (ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn > 8
						//&& StarShipPtr->hShip == ed.ObjectPtr->hTarget)))
						)))
				{
					ed.MoveState = PURSUE;
					//ed.facing = 0;
					ObjectsOfConcern[CREW_OBJECT_INDEX] = ed;
				}
			}
		//} //dont like to remove this

	}//for loop


	// TO DO Call AI Func
	(StarShipPtr->aifunc)(StarShipPtr, ObjectsOfConcern, ConcernCounter);

/* not sure if I need this?

	StarShipPtr->angle = angle;
	{
		s16 InputState;

		InputState = 0;
		if (StarShipPtr->ship_input_state & LEFT)
			InputState |= BATTLE_LEFT;
		else if (StarShipPtr->ship_input_state & RIGHT)
			InputState |= BATTLE_RIGHT;
		if (StarShipPtr->ship_input_state & THRUST)
			InputState |= BATTLE_THRUST;
		if (StarShipPtr->ship_input_state & WEAPON)
			InputState |= BATTLE_WEAPON;
		if (StarShipPtr->ship_input_state & SPECIAL)
			InputState |= BATTLE_SPECIAL;
		return (InputState);
	}
	*/


}


