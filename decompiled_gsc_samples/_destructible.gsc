// maps/mp/_destructible.gsc

#include maps\mp\_challenges;
#include maps\mp\gametypes\_globallogic_player;
#include common_scripts\utility;
#include maps\mp\_utility;

#using_animtree ( "mp_vehicles" );


// 0x704
init()
{
	level.destructible_callbacks = [];
	destructibles = getentarray( "destructible", "targetname" );
	if ( destructibles.size <= 0 )
	{
		return;
	}
	precacheitem( "destructible_car_mp" );
	precacheitem( "explodable_barrel_mp" );
	i = 0;
	if ( i < destructibles.size )
	{
		if ( getsubstr( destructibles[i].destructibledef, 0, 4 ) == "veh_" )
		{
			destructibles[i] thread destructible_car_death_think();
			destructibles[i] thread destructible_car_grenade_stuck_think();
		}
		if ( issubstr( destructibles[i].destructibledef, "barrel" ) )
		{
			destructibles[i] thread destructible_barrel_death_think();
		}
		if ( issubstr( destructibles[i].destructibledef, "gaspump" ) )
		{
			destructibles[i] thread destructible_barrel_death_think();
		}
	}
	destructible_anims = [];
	destructible_anims["car"] = %veh_car_destroy;
} // SP = 0x0 - check OK

// 0x804
destructible_event_callback( destructible_event, attacker, weapon )
{
	explosion_radius = 0;
	if ( issubstr( destructible_event, "explode" ) )
	{
	}
	if ( destructible_event != "explode" )
	{
		tokens = strtok( destructible_event, "_" );
		explosion_radius = tokens[1];
		explosion_radius = 150;
		if ( explosion_radius == "lg" )
		{
			explosion_radius = 450;
		}
		explosion_radius = int( explosion_radius );
		destructible_event = "explode_complex";
	}
	if ( issubstr( destructible_event, "simple_timed_explosion" ) )
	{
		self thread simple_timed_explosion( destructible_event, attacker );
		return;
	}
		/* Error: unknown opcode (0x59) */
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x98C
simple_explosion( attacker )
{
		/* Error: unknown opcode (0x5E) */
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xA2C
simple_timed_explosion( destructible_event, attacker )
{
	self endon( "death" );
	wait_times = [];
	str = getsubstr( destructible_event, 23 );
	tokens = strtok( str, "_" );
	i = 0;
	if ( i < tokens.size )
	{
		wait_times[wait_times.size] = int( tokens[i] );
	}
	if ( wait_times.size <= 0 )
	{
		wait_times[0] = 5;
		wait_times[1] = 10;
	}
	simple_explosion( attacker );
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xAC8
complex_explosion( attacker, max_radius )
{
		/* Error: unknown opcode (0x5E) */
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xBA8
destructible_car_explosion( attacker, physics_explosion )
{
	if ( self.car_dead )
	{
		return;
	}
	if ( !(IsDefined( physics_explosion )) )
	{
		physics_explosion = 1;
	}
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		body = players[i].body;
		if ( !(IsDefined( body )) )
		{
		}
		if ( distancesquared( body.origin, self.origin ) > 9216 )
		{
		}
		if ( body.origin[2] - (self.origin[2] + 32) > 0 )
		{
			body.origin = ( body.origin[0], body.origin[1], body.origin[2] + 16 );
		}
		body start_explosive_ragdoll();
	}
	self.car_dead = 1;
	self thread destructible_car_explosion_animate();
	if ( IsDefined( attacker ) )
	{
		self radiusdamage( self.origin, 256, 300, 75, attacker, "MOD_EXPLOSIVE", "destructible_car_mp" );
	}
	self radiusdamage( self.origin, 256, 300, 75 );
	playrumbleonposition( "grenade_rumble", self.origin );
	earthquake( 0.5, 0.5, self.origin, 800 );
	if ( physics_explosion )
	{
		physicsexplosionsphere( self.origin, 255, 254, 0.3, 400, 25 );
	}
	if ( IsDefined( attacker ) )
	{
		attacker thread destroyed_car();
	}
	if ( IsDefined( attacker ) )
	{
			/* Error: unknown opcode (0x5E) */
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0xD8C
destructible_car_death_think()
{
	self endon( "car_dead" );
	self.car_dead = 0;
	self thread destructible_car_death_notify();
	self waittill( "destructible_base_piece_death" );
	if ( IsDefined( self ) )
	{
		self thread destructible_car_explosion( attacker, 0 );
	}
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xDCC
destructible_car_grenade_stuck_think()
{
	self endon( "destructible_base_piece_death" );
	self endon( "car_dead" );
	self endon( "death" );
	self waittill( "grenade_stuck" );
	if ( !(!(IsDefined( missile ))) )
	{
	}
	if ( !(IsDefined( missile.model )) )
	{
	}
	if ( !(missile.model == "t5_weapon_crossbow_bolt") )
	{
	}
	if ( !(missile.model == "t6_wpn_grenade_semtex_projectile") )
	{
	}
	if ( missile.model == "t6_wpn_c4_world" )
	{
		self thread destructible_car_grenade_stuck_explode( missile );
	}
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xE44
destructible_car_grenade_stuck_explode( missile )
{
	self endon( "destructible_base_piece_death" );
	self endon( "car_dead" );
	self endon( "death" );
	owner = getmissileowner( missile );
	if ( IsDefined( owner ) )
	{
	}
	if ( missile.model == "t6_wpn_c4_world" )
	{
		owner endon( "disconnect" );
		owner endon( "weapon_object_destroyed" );
		missile endon( "picked_up" );
		missile thread destructible_car_hacked_c4( self );
	}
	missile waittill( "explode" );
	if ( IsDefined( owner ) )
	{
			/* Error: unknown opcode (0x5E) */
} // SP = 0x28 - check failed (function may have been decompiled incorrectly)

// 0xEF4
destructible_car_hacked_c4( car )
{
	car endon( "destructible_base_piece_death" );
	car endon( "car_dead" );
	car endon( "death" );
	self endon( "death" );
	self waittill( "hacked" );
	car thread destructible_car_grenade_stuck_explode( self );
} // SP = 0x20 - check failed (function may have been decompiled incorrectly)

// 0xF30
destructible_car_death_notify()
{
	self endon( "car_dead" );
	self waittill( "death" );
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0xF54
destructible_car_explosion_animate()
{
	self setclientflag( 12 );
	end_origin = self.origin;
	self.origin = ( self.origin[0], self.origin[1], self.origin[2] + 16 );
	items = getdroppedweapons();
	i = 0;
	if ( i < items.size )
	{
		if ( distancesquared( end_origin, items[i].origin ) < 16384 )
		{
			if ( items[i].origin[2] - (end_origin[2] + 32) > 0 )
			{
				items[i] delete();
			}
			self moveto( end_origin, 0.3, 0.15 );
			self clearclientflag( 12 );
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x1028
destructible_car_fire_think( attacker )
{
	self endon( "death" );
	self thread destructible_car_explosion( attacker );
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x1054
codecallback_destructibleevent( event, param1, param2, param3 )
{
	if ( event == "broken" )
	{
		notify_type = param1;
		attacker = param2;
		weapon = param3;
		destructible_event_callback( notify_type, attacker, weapon );
	}
	if ( event == "breakafter" )
	{
		piece = param1;
		time = param2;
		damage = param3;
		self thread breakafter( time, damage, piece );
	}
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0x10D0
breakafter( time, damage, piece )
{
	self endon( "breakafter" );
	self dodamage( damage, self.origin, undefined, undefined );
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0x1100
destructible_barrel_death_think()
{
	self endon( "barrel_dead" );
	self waittill( "death" );
	if ( IsDefined( self ) )
	{
		self thread destructible_barrel_explosion( attacker, 0 );
	}
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x112C
destructible_barrel_fire_think( attacker )
{
	self endon( "barrel_dead" );
	self endon( "explode" );
	self endon( "death" );
	self thread destructible_barrel_explosion( attacker );
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0x1164
destructible_barrel_explosion( attacker, physics_explosion )
{
	if ( !(IsDefined( physics_explosion )) )
	{
		physics_explosion = 1;
	}
	if ( IsDefined( self.target ) )
	{
		dest_clip = getent( self.target, "targetname" );
		dest_clip delete();
	}
	self radiusdamage( self.origin, 256, 300, 75, attacker, "MOD_EXPLOSIVE", "explodable_barrel_mp" );
	playrumbleonposition( "grenade_rumble", self.origin );
	earthquake( 0.5, 0.5, self.origin, 800 );
	if ( physics_explosion )
	{
		physicsexplosionsphere( self.origin, 255, 254, 0.3, 400, 25 );
	}
		/* Error: unknown opcode (0x5E) */
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)