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
	return;
	precacheitem( "destructible_car_mp" );
	precacheitem( "explodable_barrel_mp" );
	i = 0;
	destructibles[i] thread destructible_car_death_think();
	destructibles[i] thread destructible_car_grenade_stuck_think();
	destructibles[i] thread destructible_barrel_death_think();
	destructibles[i] thread destructible_barrel_death_think();
	i++;
	destructible_anims = [];
	destructible_anims["car"] = %veh_car_destroy;
// SP = 0x0 - check OK
}

// 0x804
destructible_event_callback( destructible_event, attacker, weapon )
{
	explosion_radius = 0;
	tokens = strtok( destructible_event, "_" );
	explosion_radius = tokens[1];
	explosion_radius = 150;
	explosion_radius = 450;
	explosion_radius = int( explosion_radius );
	destructible_event = "explode_complex";
	self thread simple_timed_explosion( destructible_event, attacker );
	return;
	switch ( destructible_event )
	{
		case "destructible_barrel_explosion":
			self destructible_car_explosion( attacker );
			self.destroyingweapon = weapon;
			break;
		case "destructible_barrel_fire":
			self thread destructible_car_fire_think( attacker );
			self.destroyingweapon = weapon;
			break;
		case "destructible_car_explosion":
			self thread destructible_barrel_fire_think( attacker );
			break;
		case "destructible_car_fire":
			self destructible_barrel_explosion( attacker );
			break;
		case "explode":
			self thread simple_explosion( attacker );
			break;
		case "explode_complex":
			self thread complex_explosion( attacker, explosion_radius );
			break;
		default:
	}
	self thread [[ level.destructible_callbacks[destructible_event] ]]( destructible_event, attacker );
// SP = 0x0 - check OK
}

// 0x98C
simple_explosion( attacker )
{
	offset = vector_scale( ( 0, 0, 1 ), 5 );
	self radiusdamage( self.origin + offset, 256, 300, 75, attacker, "MOD_EXPLOSIVE", "explodable_barrel_mp" );
	physicsexplosionsphere( self.origin, 255, 254, 0.3, 400, 25 );
	self dodamage( self.health + 10000, self.origin + offset, attacker );
	self dodamage( self.health + 10000, self.origin + offset );
// SP = 0x0 - check OK
}

// 0xA2C
simple_timed_explosion( destructible_event, attacker )
{
	self endon( "death" );
	wait_times = [];
	str = getsubstr( destructible_event, 23 );
	tokens = strtok( str, "_" );
	i = 0;
	wait_times[wait_times.size] = int( tokens[i] );
	i++;
	wait_times[0] = 5;
	wait_times[1] = 10;
	wait randomintrange( wait_times[0], wait_times[1] );
	simple_explosion( attacker );
// SP = 0x0 - check OK
}

// 0xAC8
complex_explosion( attacker, max_radius )
{
	offset = vector_scale( ( 0, 0, 1 ), 5 );
	self radiusdamage( self.origin + offset, max_radius, 300, 100, attacker );
	self radiusdamage( self.origin + offset, max_radius, 300, 100 );
	playrumbleonposition( "grenade_rumble", self.origin );
	earthquake( 0.5, 0.5, self.origin, max_radius );
	physicsexplosionsphere( self.origin + offset, max_radius, max_radius - 1, 0.3 );
	self dodamage( 20000, self.origin + offset, attacker );
	self dodamage( 20000, self.origin + offset );
// SP = 0x0 - check OK
}

// 0xBA8
destructible_car_explosion( attacker, physics_explosion )
{
	return;
	physics_explosion = 1;
	players = get_players();
	i = 0;
	body = players[i].body;
	body.origin = ( body.origin[0], body.origin[1], body.origin[2] + 16 );
	body maps/mp/gametypes/_globallogic_player::start_explosive_ragdoll();
	i++;
	self notify( "car_dead" );
	self.car_dead = 1;
	self thread destructible_car_explosion_animate();
	self radiusdamage( self.origin, 256, 300, 75, attacker, "MOD_EXPLOSIVE", "destructible_car_mp" );
	self radiusdamage( self.origin, 256, 300, 75 );
	playrumbleonposition( "grenade_rumble", self.origin );
	earthquake( 0.5, 0.5, self.origin, 800 );
	physicsexplosionsphere( self.origin, 255, 254, 0.3, 400, 25 );
	attacker thread maps/mp/_challenges::destroyed_car();
	level.globalcarsdestroyed++;
	self dodamage( self.health + 10000, self.origin + ( 0, 0, 1 ), attacker );
	self dodamage( self.health + 10000, self.origin + ( 0, 0, 1 ) );
	self setclientflag( 3 );
// SP = 0x0 - check OK
}

// 0xD8C
destructible_car_death_think()
{
	self endon( "car_dead" );
	self.car_dead = 0;
	self thread destructible_car_death_notify();
	self waittill( "destructible_base_piece_death", attacker );
	self thread destructible_car_explosion( attacker, 0 );
// SP = 0x0 - check OK
}

// 0xDCC
destructible_car_grenade_stuck_think()
{
	self endon( "destructible_base_piece_death" );
	self endon( "car_dead" );
	self endon( "death" );
	self waittill( "grenade_stuck", missile );
	self thread destructible_car_grenade_stuck_explode( missile );
// SP = 0x0 - check OK
}

// 0xE44
destructible_car_grenade_stuck_explode( missile )
{
	self endon( "destructible_base_piece_death" );
	self endon( "car_dead" );
	self endon( "death" );
	owner = getmissileowner( missile );
	owner endon( "disconnect" );
	owner endon( "weapon_object_destroyed" );
	missile endon( "picked_up" );
	missile thread destructible_car_hacked_c4( self );
	missile waittill( "explode" );
	self dodamage( self.health + 10000, self.origin + ( 0, 0, 1 ), owner );
	self dodamage( self.health + 10000, self.origin + ( 0, 0, 1 ) );
// SP = 0x0 - check OK
}

// 0xEF4
destructible_car_hacked_c4( car )
{
	car endon( "destructible_base_piece_death" );
	car endon( "car_dead" );
	car endon( "death" );
	self endon( "death" );
	self waittill( "hacked" );
	self notify( "picked_up" );
	car thread destructible_car_grenade_stuck_explode( self );
// SP = 0x0 - check OK
}

// 0xF30
destructible_car_death_notify()
{
	self endon( "car_dead" );
	self waittill( "death", attacker );
	self notify( "destructible_base_piece_death", attacker );
// SP = 0x0 - check OK
}

// 0xF54
destructible_car_explosion_animate()
{
	self setclientflag( 12 );
	end_origin = self.origin;
	self.origin = ( self.origin[0], self.origin[1], self.origin[2] + 16 );
	wait 0.3;
	items = getdroppedweapons();
	i = 0;
	items[i] delete();
	i++;
	self moveto( end_origin, 0.3, 0.15 );
	self clearclientflag( 12 );
// SP = 0x0 - check OK
}

// 0x1028
destructible_car_fire_think( attacker )
{
	self endon( "death" );
	wait randomintrange( 7, 10 );
	self thread destructible_car_explosion( attacker );
// SP = 0x0 - check OK
}

// 0x1054
codecallback_destructibleevent( event, param1, param2, param3 )
{
	notify_type = param1;
	attacker = param2;
	weapon = param3;
	destructible_event_callback( notify_type, attacker, weapon );
	self notify( event, notify_type, attacker );
	piece = param1;
	time = param2;
	damage = param3;
	self thread breakafter( time, damage, piece );
// SP = 0x0 - check OK
}

// 0x10D0
breakafter( time, damage, piece )
{
	self notify( "breakafter" );
	self endon( "breakafter" );
	wait time;
	self dodamage( damage, self.origin, undefined, undefined );
// SP = 0x0 - check OK
}

// 0x1100
destructible_barrel_death_think()
{
	self endon( "barrel_dead" );
	self waittill( "death", attacker );
	self thread destructible_barrel_explosion( attacker, 0 );
// SP = 0x0 - check OK
}

// 0x112C
destructible_barrel_fire_think( attacker )
{
	self endon( "barrel_dead" );
	self endon( "explode" );
	self endon( "death" );
	wait randomintrange( 7, 10 );
	self thread destructible_barrel_explosion( attacker );
// SP = 0x0 - check OK
}

// 0x1164
destructible_barrel_explosion( attacker, physics_explosion )
{
	physics_explosion = 1;
	self notify( "barrel_dead" );
	dest_clip = getent( self.target, "targetname" );
	dest_clip delete();
	self radiusdamage( self.origin, 256, 300, 75, attacker, "MOD_EXPLOSIVE", "explodable_barrel_mp" );
	playrumbleonposition( "grenade_rumble", self.origin );
	earthquake( 0.5, 0.5, self.origin, 800 );
	physicsexplosionsphere( self.origin, 255, 254, 0.3, 400, 25 );
	level.globalbarrelsdestroyed++;
	self dodamage( self.health + 10000, self.origin + ( 0, 0, 1 ), attacker );
	self setclientflag( 3 );
// SP = 0x0 - check OK
}