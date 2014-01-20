// maps/mp/zombies/_zm.gsc

#include maps\mp\zombies\_zm_game_module;
#include maps\mp\zombies\_zm_ai_basic;
#include maps\mp\gametypes_zm\_weapons;
#include maps\mp\zombies\_zm_pers_upgrades_system;
#include maps\mp\zombies\_zm_ai_dogs;
#include maps\mp\zombies\_zm_melee_weapon;
#include maps\mp\gametypes_zm\_hud_util;
#include maps\mp\zombies\_zm_pers_upgrades_functions;
#include maps\mp\_demo;
#include maps\mp\gametypes_zm\_zm_gametype;
#include maps\mp\zombies\_zm_pers_upgrades;
#include maps\mp\zombies\_zm_stats;
#include maps\mp\zombies\_zm_tombstone;
#include maps\mp\zombies\_zm_weapons;
#include maps\mp\zombies\_zm_traps;
#include maps\mp\zombies\_zm_timer;
#include maps\mp\zombies\_zm_gump;
#include maps\mp\zombies\_zm_spawner;
#include maps\mp\zombies\_zm_score;
#include maps\mp\zombies\_zm_powerups;
#include maps\mp\zombies\_zm_power;
#include maps\mp\zombies\_zm_playerhealth;
#include maps\mp\zombies\_zm_perks;
#include maps\mp\zombies\_zm_magicbox;
#include maps\mp\zombies\_zm_laststand;
#include maps\mp\zombies\_zm_equipment;
#include maps\mp\zombies\_zm_buildables;
#include maps\mp\zombies\_zm_clone;
#include maps\mp\zombies\_zm_bot;
#include maps\mp\zombies\_zm_blockers;
#include maps\mp\zombies\_zm_audio;
#include maps\mp\zombies\_zm_unitrigger;
#include maps\mp\zombies\_zm_zonemgr;
#include maps\mp\zombies\_zm_devgui;
#include maps\mp\_visionset_mgr;
#include maps\mp\zombies\_zm;
#include maps\mp\zombies\_zm_ffotd;
#include maps\mp\zombies\_zm_utility;
#include maps\mp\_utility;
#include common_scripts\utility;


// 0x8178
init()
{
	level.player_out_of_playable_area_monitor = 1;
	level.player_too_many_weapons_monitor = 1;
	level.player_too_many_weapons_monitor_func = ::player_too_many_weapons_monitor;
	level.player_too_many_players_check = 1;
	level.player_too_many_players_check_func = ::player_too_many_players_check;
	level._use_choke_weapon_hints = 1;
	level._use_choke_blockers = 1;
	level.passed_introscreen = 0;
	if ( !(IsDefined( level.custom_ai_type )) )
	{
		level.custom_ai_type = [];
	}
	level.custom_ai_spawn_check_funcs = [];
	level.spawn_funcs = [];
	level.spawn_funcs["allies"] = [];
	level.spawn_funcs["axis"] = [];
	level.spawn_funcs["team3"] = [];
	level thread main_start();
	level.zombiemode = 1;
	level.revivefeature = 0;
	level.swimmingfeature = 0;
	level.calc_closest_player_using_paths = 0;
	level.zombie_melee_in_water = 1;
	level.put_timed_out_zombies_back_in_queue = 1;
	level.use_alternate_poi_positioning = 1;
	level.zmb_laugh_alias = "zmb_laugh_richtofen";
	level.sndannouncerisrich = 1;
	level.scr_zm_ui_gametype = GetDvar( #"0x41651E" );
	level.scr_zm_ui_gametype_group = GetDvar( #"0x6B64B9B4" );
	level.scr_zm_map_start_location = GetDvar( #"0xC955B4CD" );
	level.curr_gametype_affects_rank = 0;
	gametype = tolower( GetDvar( #"0x4F118387" ) );
	if ( !("zclassic" == gametype) )
	{
	}
	if ( "zsurvival" == gametype )
	{
		level.curr_gametype_affects_rank = 1;
	}
	level.grenade_multiattack_bookmark_count = 1;
	level.rampage_bookmark_kill_times_count = 3;
	level.rampage_bookmark_kill_times_msec = 6000;
	level.rampage_bookmark_kill_times_delay = 6000;
	level thread watch_rampage_bookmark();
	if ( !(IsDefined( level._zombies_round_spawn_failsafe )) )
	{
		level._zombies_round_spawn_failsafe = ::round_spawn_failsafe;
	}
	level.zombie_visionset = "zombie_neutral";
	if ( GetDvar( #"0x5DF80895" ) == "1" )
	{
		level.zombie_anim_intro = 1;
	}
	level.zombie_anim_intro = 0;
	precache_shaders();
	precache_models();
	precacherumble( "explosion_generic" );
	precacherumble( "dtp_rumble" );
	precacherumble( "slide_rumble" );
	precache_zombie_leaderboards();
	level._zombie_gib_piece_index_all = 0;
	level._zombie_gib_piece_index_right_arm = 1;
	level._zombie_gib_piece_index_left_arm = 2;
	level._zombie_gib_piece_index_right_leg = 3;
	level._zombie_gib_piece_index_left_leg = 4;
	level._zombie_gib_piece_index_head = 5;
	level._zombie_gib_piece_index_guts = 6;
	if ( !(IsDefined( level.zombie_ai_limit )) )
	{
		level.zombie_ai_limit = 24;
	}
	if ( !(IsDefined( level.zombie_actor_limit )) )
	{
		level.zombie_actor_limit = 31;
	}
	init();
	init_dvars();
	init_strings();
	init_levelvars();
	init_sounds();
	init_shellshocks();
	init_flags();
	init_client_flags();
	registerclientfield( "world", "zombie_power_on", 1, 1, "int" );
	if ( level.scr_zm_ui_gametype_group == "zclassic" )
	{
	}
	if ( !(level.createfx_enabled) )
	{
		registerclientfield( "allplayers", "navcard_held", 1, 4, "int" );
		level.navcards = [];
		level.navcards[0] = "navcard_held_zm_transit";
		level.navcards[1] = "navcard_held_zm_highrise";
		level thread setup_player_navcard_hud();
	}
	register_offhand_weapons_for_level_defaults();
	level thread drive_client_connected_notifies();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init();
	init_function_overrides();
	level thread last_stand_pistol_rank_init();
	level thread init();
	level thread post_all_players_connected();
	init_utility();
	registerclientsys( "lsm" );
	init();
	initializestattracking();
	if ( get_players().size <= 1 )
	{
		incrementcounter( "global_solo_games", 1 );
	}
	if ( level.systemlink )
	{
		incrementcounter( "global_systemlink_games", 1 );
	}
	if ( GetDvarInt( #"0xE6FF85E6" ) == get_players().size )
	{
		incrementcounter( "global_splitscreen_games", 1 );
	}
	incrementcounter( "global_coop_games", 1 );
	onplayerconnect_callback( ::zm_on_player_connect );
	pers_upgrade_init();
	set_demo_intermission_point();
	level thread main_end();
	level thread track_players_intersection_tracker();
	level thread onallplayersready();
	level thread startunitriggers();
	level thread post_init_gametype();
} // SP = 0x0 - check OK

// 0x8624
post_main()
{
	level thread init_custom_ai_type();
} // SP = 0x0 - check OK

// 0x8638
startunitriggers()
{
	flag_wait_any( "start_zombie_round_logic", "start_encounters_match_logic" );
	level thread main();
} // SP = 0x0 - check OK

// 0x865C
drive_client_connected_notifies()
{
	if ( 1 )
	{
		level waittill( "connected" );
		player reset_rampage_bookmark_kill_times();
		player callback( "on_player_connect" );
	}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x8694
fade_out_intro_screen_zm( hold_black_time, fade_out_time, destroyed_afterwards )
{
	if ( !(IsDefined( level.introscreen )) )
	{
		level.introscreen = newhudelem();
		level.x = 0;
		level.y = 0;
		level.horzalign = "fullscreen";
		level.vertalign = "fullscreen";
		level.foreground = 0;
		level.introscreen setshader( "black", 640, 480 );
		level.immunetodemogamehudsettings = 1;
		level.immunetodemofreecamera = 1;
	}
	level.alpha = 1;
	if ( IsDefined( hold_black_time ) )
	{
	}
	if ( !(IsDefined( fade_out_time )) )
	{
		fade_out_time = 1.5;
	}
	level.introscreen fadeovertime( fade_out_time );
	level.alpha = 0;
	level.passed_introscreen = 1;
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		players[i] setclientuivisibilityflag( "hud_visible", 1 );
		if ( IsDefined( level.host_ended_game ) )
		{
		}
		if ( !(level.host_ended_game) )
		{
			if ( IsDefined( level.player_movement_suppressed ) )
			{
				players[i] freezecontrols( level.player_movement_suppressed );
				println( " Unfreeze controls 4" );
			}
			if ( IsDefined( level.hostmigrationcontrolsfrozen ) )
			{
			}
			if ( !(level.hostmigrationcontrolsfrozen) )
			{
				players[i] freezecontrols( 0 );
				println( " Unfreeze controls 5" );
			}
			if ( destroyed_afterwards == 1 )
			{
				level.introscreen destroy();
			}
			flag_set( "initial_blackscreen_passed" );
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0x8854
onallplayersready()
{
	timeout = GetTime() + 5000;
	if ( getnumexpectedplayers() == 0 )
	{
	}
	if ( GetTime() < timeout )
	{
	}
	println( "ZM >> player_count_expected=" + getnumexpectedplayers() );
	player_count_actual = 0;
	if ( !(getnumconnectedplayers() < getnumexpectedplayers()) )
	{
	}
	if ( player_count_actual != getnumexpectedplayers() )
	{
		players = get_players();
		player_count_actual = 0;
		i = 0;
		if ( i < players.size )
		{
			players[i] freezecontrols( 1 );
			if ( level.sessionstate == "playing" )
			{
			}
		}
		println( "ZM >> Num Connected =" + getnumconnectedplayers() + " Expected : " + getnumexpectedplayers() );
	}
	setinitialplayersconnected();
	println( "ZM >> We have all players - START ZOMBIE LOGIC" );
	if ( 1 == getnumconnectedplayers() )
	{
	}
	if ( GetDvarInt( #"0xB0FB65D0" ) == 1 )
	{
		level thread add_bots();
		flag_set( "initial_players_connected" );
	}
	players = get_players();
	if ( players.size == 1 )
	{
		flag_set( "solo_game" );
		level.solo_lives_given = 0;
		_a374 = players;
		_k374 = GetFirstArrayKey( _a374 );
		if ( IsDefined( _k374 ) )
		{
			player = _a374[_k374];
			level.lives = 0;
			_k374 = GetNextArrayKey( _a374, _k374 );
		}
		level zombiemode_solo_last_stand_pistol();
	}
	flag_set( "initial_players_connected" );
	if ( !(aretexturesloaded()) )
	{
	}
	fade_out_intro_screen_zm( 5, 1.5, 1 );
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0x8A48
start_zombie_logic_in_x_sec( time_to_wait )
{
	flag_set( "start_zombie_round_logic" );
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x8A64
getallotherplayers()
{
	aliveplayers = [];
	i = 0;
	if ( i < level.players.size )
	{
		if ( !(IsDefined( level.players[i] )) )
		{
		}
		player = level.players[i];
		if ( !(level.sessionstate != "playing") )
		{
		}
		if ( player == self )
		{
		}
		aliveplayers[aliveplayers.size] = player;
	}
	return aliveplayers;
} // SP = 0x0 - check OK

// 0x8ACC
getfreespawnpoint( spawnpoints, player )
{
	if ( !(IsDefined( spawnpoints )) )
	{
		iprintlnbold( "ZM >> No free spawn points in map" );
		return undefined;
	}
	if ( !(IsDefined( game["spawns_randomized"] )) )
	{
		i["spawns_randomized"] = 1;
		spawnpoints = array_randomize( spawnpoints );
		random_chance = randomint( 100 );
		if ( random_chance > 50 )
		{
			set_game_var( "side_selection", 1 );
		}
		set_game_var( "side_selection", 2 );
	}
	side_selection = get_game_var( "side_selection" );
	if ( get_game_var( "switchedsides" ) )
	{
		if ( side_selection == 2 )
		{
			side_selection = 1;
		}
		if ( side_selection == 1 )
		{
			side_selection = 2;
		}
		if ( IsDefined( player ) )
		{
		}
		if ( IsDefined( level.team ) )
		{
			i = 0;
			if ( IsDefined( spawnpoints ) )
			{
			}
			if ( i < spawnpoints.size )
			{
				if ( side_selection == 1 )
				{
					if ( level.team != "allies" )
					{
						if ( IsDefined( level.script_int ) )
						{
						}
						if ( level.script_int == 1 )
						{
							arrayremovevalue( spawnpoints, spawnpoints[i] );
							i = 0;
						}
						if ( level.team == "allies" )
						{
							if ( IsDefined( level.script_int ) )
							{
							}
							if ( level.script_int == 2 )
							{
								arrayremovevalue( spawnpoints, spawnpoints[i] );
								i = 0;
							}
							if ( level.team == "allies" )
							{
								if ( IsDefined( level.script_int ) )
								{
								}
								if ( level.script_int == 1 )
								{
									arrayremovevalue( spawnpoints, spawnpoints[i] );
									i = 0;
								}
								if ( level.team != "allies" )
								{
									if ( IsDefined( level.script_int ) )
									{
									}
									if ( level.script_int == 2 )
									{
										arrayremovevalue( spawnpoints, spawnpoints[i] );
										i = 0;
									}
									if ( !(IsDefined( level.playernum )) )
									{
										if ( level.team == "allies" )
										{
											level.playernum = get_game_var( "_team1_num" );
											set_game_var( "_team1_num", level.playernum + 1 );
										}
										level.playernum = get_game_var( "_team2_num" );
										set_game_var( "_team2_num", level.playernum + 1 );
									}
									j = 0;
									if ( j < spawnpoints.size )
									{
										if ( !(IsDefined( level.en_num )) )
										{
											m = 0;
											if ( m < spawnpoints.size )
											{
												level.en_num = m;
											}
											if ( level.en_num == level.playernum )
											{
												return spawnpoints[j];
											}
											return spawnpoints[0];
} // SP = 0x0 - check OK

// 0x8D8C
delete_in_createfx()
{
	exterior_goals = getstructarray( "exterior_goal", "targetname" );
	i = 0;
	if ( i < exterior_goals.size )
	{
		if ( !(IsDefined( level.target )) )
		{
		}
		targets = getentarray( level.target, "targetname" );
		j = 0;
		if ( j < targets.size )
		{
			targets[j] self_delete();
		}
	}
	if ( IsDefined( level.level_createfx_callback_thread ) )
	{
			/* Error: unknown opcode (0x35) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0x8E1C
add_bots()
{
	host = gethostplayer();
	if ( !(IsDefined( host )) )
	{
		host = gethostplayer();
	}
	zbot_spawn();
	setdvar( "bot_AllowMovement", "1" );
	setdvar( "bot_PressAttackBtn", "1" );
	setdvar( "bot_PressMeleeBtn", "1" );
	if ( get_players().size < 2 )
	{
	}
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		players[i] freezecontrols( 0 );
		println( " Unfreeze controls 6" );
	}
	level.numberbotsadded = 1;
	flag_set( "start_zombie_round_logic" );
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0x8F00
zbot_spawn()
{
	player = gethostplayer();
	spawnpoints = getstructarray( "initial_spawn_points", "targetname" );
	spawnpoint = getfreespawnpoint( spawnpoints );
	bot = addtestclient();
	if ( !(IsDefined( bot )) )
	{
		println( "Could not add test client" );
		return;
	}
	level.pers["isBot"] = 1;
	level.equipment_enabled = 0;
	yaw = level.angles[1];
	bot thread zbot_spawn_think( level.origin, yaw );
	return bot;
} // SP = 0x0 - check OK

// 0x8F98
zbot_spawn_think( origin, yaw )
{
	self endon( "disconnect" );
	self waittill( "spawned_player" );
	self setorigin( origin );
	angles = ( 0, yaw, 0 );
	self setplayerangles( angles );
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x8FD8
post_all_players_connected()
{
	level thread end_game();
	flag_wait( "start_zombie_round_logic" );
	println( "sessions: mapname=", level.script, " gametype zom isserver 1 player_count=", get_players().size );
	level thread clear_mature_blood();
	level thread round_end_monitor();
	if ( !(level.zombie_anim_intro) )
	{
		if ( IsDefined( level._round_start_func ) )
		{
				/* Error: unknown opcode (0x35) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0x9078
init_custom_ai_type()
{
	if ( IsDefined( level.custom_ai_type ) )
	{
		i = 0;
		if ( i < level.custom_ai_type.size )
		{
				/* Error: unknown opcode (0x2F) */
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x90AC
zombiemode_melee_miss()
{
	if ( IsDefined( level.curr_pay_turret ) )
	{
		level.enemy dodamage( GetDvarInt( #"0x423A0415" ), level.origin, self, self, "none", "melee" );
	}
} // SP = 0x0 - check OK

// 0x90E4
player_track_ammo_count()
{
	self endon( "disconnect" );
	self endon( "stop_ammo_tracking" );
	ammolowcount = 0;
	ammooutcount = 0;
	if ( 1 )
	{
		weap = self getcurrentweapon();
		if ( !(!(IsDefined( weap ))) )
		{
		}
		if ( !(weap == "none") )
		{
		}
		if ( !(can_track_ammo( weap )) )
		{
		}
		if ( !(self getammocount( weap ) > 5) )
		{
		}
		if ( self player_is_in_laststand() )
		{
			ammooutcount = 0;
			ammolowcount = 0;
		}
		if ( self getammocount( weap ) > 0 )
		{
			if ( ammolowcount < 1 )
			{
				self create_and_play_dialog( "general", "ammo_low" );
			}
		}
		if ( ammooutcount < 1 )
		{
			self create_and_play_dialog( "general", "ammo_out" );
		}
	}
} // SP = 0x1C - check failed (function may have been decompiled incorrectly)

// 0x91D0
can_track_ammo( weap )
{
	if ( !(IsDefined( weap )) )
	{
		return 0;
	}
		/* Error: unknown opcode (0x59) */
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x92E4
spawn_vo()
{
	players = get_players();
	if ( players.size > 1 )
	{
		player = random( players );
		index = get_player_index( player );
		player thread spawn_vo_player( index, players.size );
	}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x9338
spawn_vo_player( index, num )
{
	sound = "plr_" + index + "_vox_" + num + "play";
	self playsoundwithnotify( sound, "sound_done" );
	self waittill( "sound_done" );
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x9370
precache_shaders()
{
	precacheshader( "hud_chalk_1" );
	precacheshader( "hud_chalk_2" );
	precacheshader( "hud_chalk_3" );
	precacheshader( "hud_chalk_4" );
	precacheshader( "hud_chalk_5" );
	precacheshader( "zom_icon_community_pot" );
	precacheshader( "zom_icon_community_pot_strip" );
	precacheshader( "zom_icon_player_life" );
	precacheshader( "waypoint_revive" );
} // SP = 0x0 - check OK

// 0x93E4
precache_models()
{
	precachemodel( "p_zom_win_bars_01_vert04_bend_180" );
	precachemodel( "p_zom_win_bars_01_vert01_bend_180" );
	precachemodel( "p_zom_win_bars_01_vert04_bend" );
	precachemodel( "p_zom_win_bars_01_vert01_bend" );
	precachemodel( "p_zom_win_cell_bars_01_vert04_bent" );
	precachemodel( "p_zom_win_cell_bars_01_vert01_bent" );
	precachemodel( "tag_origin" );
	precachemodel( "zombie_z_money_icon" );
	if ( IsDefined( level.precachecustomcharacters ) )
	{
			/* Error: unknown opcode (0x31) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0x945C
init_shellshocks()
{
	level.player_killed_shellshock = "zombie_death";
	precacheshellshock( level.player_killed_shellshock );
	precacheshellshock( "pain" );
	precacheshellshock( "explosion" );
} // SP = 0x0 - check OK

// 0x9490
init_strings()
{
	precachestring( &"ZOMBIE_WEAPONCOSTAMMO" );
	precachestring( &"ZOMBIE_ROUND" );
	precachestring( &"SCRIPT_PLUS" );
	precachestring( &"ZOMBIE_GAME_OVER" );
	precachestring( &"ZOMBIE_SURVIVED_ROUND" );
	precachestring( &"ZOMBIE_SURVIVED_ROUNDS" );
	precachestring( &"ZOMBIE_SURVIVED_NOMANS" );
	precachestring( &"ZOMBIE_EXTRA_LIFE" );
	add_zombie_hint( "undefined", &"ZOMBIE_UNDEFINED" );
	add_zombie_hint( "default_treasure_chest_950", &"ZOMBIE_RANDOM_WEAPON_950" );
	add_zombie_hint( "default_buy_barrier_piece_10", &"ZOMBIE_BUTTON_BUY_BACK_BARRIER_10" );
	add_zombie_hint( "default_buy_barrier_piece_20", &"ZOMBIE_BUTTON_BUY_BACK_BARRIER_20" );
	add_zombie_hint( "default_buy_barrier_piece_50", &"ZOMBIE_BUTTON_BUY_BACK_BARRIER_50" );
	add_zombie_hint( "default_buy_barrier_piece_100", &"ZOMBIE_BUTTON_BUY_BACK_BARRIER_100" );
	add_zombie_hint( "default_reward_barrier_piece", &"ZOMBIE_BUTTON_REWARD_BARRIER" );
	add_zombie_hint( "default_reward_barrier_piece_10", &"ZOMBIE_BUTTON_REWARD_BARRIER_10" );
	add_zombie_hint( "default_reward_barrier_piece_20", &"ZOMBIE_BUTTON_REWARD_BARRIER_20" );
	add_zombie_hint( "default_reward_barrier_piece_30", &"ZOMBIE_BUTTON_REWARD_BARRIER_30" );
	add_zombie_hint( "default_reward_barrier_piece_40", &"ZOMBIE_BUTTON_REWARD_BARRIER_40" );
	add_zombie_hint( "default_reward_barrier_piece_50", &"ZOMBIE_BUTTON_REWARD_BARRIER_50" );
	add_zombie_hint( "default_buy_debris_100", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_100" );
	add_zombie_hint( "default_buy_debris_200", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_200" );
	add_zombie_hint( "default_buy_debris_250", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_250" );
	add_zombie_hint( "default_buy_debris_500", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_500" );
	add_zombie_hint( "default_buy_debris_750", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_750" );
	add_zombie_hint( "default_buy_debris_1000", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_1000" );
	add_zombie_hint( "default_buy_debris_1250", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_1250" );
	add_zombie_hint( "default_buy_debris_1500", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_1500" );
	add_zombie_hint( "default_buy_debris_1750", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_1750" );
	add_zombie_hint( "default_buy_debris_2000", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_2000" );
	add_zombie_hint( "default_buy_debris_3000", &"ZOMBIE_BUTTON_BUY_CLEAR_DEBRIS_3000" );
	add_zombie_hint( "default_buy_door_close", &"ZOMBIE_BUTTON_BUY_CLOSE_DOOR" );
	add_zombie_hint( "default_buy_door_100", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_100" );
	add_zombie_hint( "default_buy_door_200", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_200" );
	add_zombie_hint( "default_buy_door_250", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_250" );
	add_zombie_hint( "default_buy_door_500", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_500" );
	add_zombie_hint( "default_buy_door_750", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_750" );
	add_zombie_hint( "default_buy_door_1000", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_1000" );
	add_zombie_hint( "default_buy_door_1250", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_1250" );
	add_zombie_hint( "default_buy_door_1500", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_1500" );
	add_zombie_hint( "default_buy_door_1750", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_1750" );
	add_zombie_hint( "default_buy_door_2000", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_2000" );
	add_zombie_hint( "default_buy_door_2500", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_2500" );
	add_zombie_hint( "default_buy_door_3000", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_3000" );
	add_zombie_hint( "default_buy_door_4000", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_4000" );
	add_zombie_hint( "default_buy_door_8000", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_8000" );
	add_zombie_hint( "default_buy_door_16000", &"ZOMBIE_BUTTON_BUY_OPEN_DOOR_16000" );
	add_zombie_hint( "default_buy_area_100", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_100" );
	add_zombie_hint( "default_buy_area_200", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_200" );
	add_zombie_hint( "default_buy_area_250", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_250" );
	add_zombie_hint( "default_buy_area_500", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_500" );
	add_zombie_hint( "default_buy_area_750", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_750" );
	add_zombie_hint( "default_buy_area_1000", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_1000" );
	add_zombie_hint( "default_buy_area_1250", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_1250" );
	add_zombie_hint( "default_buy_area_1500", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_1500" );
	add_zombie_hint( "default_buy_area_1750", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_1750" );
	add_zombie_hint( "default_buy_area_2000", &"ZOMBIE_BUTTON_BUY_OPEN_AREA_2000" );
	add_zombie_hint( "powerup_fire_sale_cost", &"ZOMBIE_FIRE_SALE_COST" );
} // SP = 0x0 - check OK

// 0x9818
init_sounds()
{
	add_sound( "end_of_round", "mus_zmb_round_over" );
	add_sound( "end_of_game", "mus_zmb_game_over" );
	add_sound( "chalk_one_up", "mus_zmb_chalk" );
	add_sound( "purchase", "zmb_cha_ching" );
	add_sound( "no_purchase", "zmb_no_cha_ching" );
	add_sound( "playerzombie_usebutton_sound", "zmb_zombie_vocals_attack" );
	add_sound( "playerzombie_attackbutton_sound", "zmb_zombie_vocals_attack" );
	add_sound( "playerzombie_adsbutton_sound", "zmb_zombie_vocals_attack" );
	add_sound( "zombie_head_gib", "zmb_zombie_head_gib" );
	add_sound( "rebuild_barrier_piece", "zmb_repair_boards" );
	add_sound( "rebuild_barrier_metal_piece", "zmb_metal_repair" );
	add_sound( "rebuild_barrier_hover", "zmb_boards_float" );
	add_sound( "debris_hover_loop", "zmb_couch_loop" );
	add_sound( "break_barrier_piece", "zmb_break_boards" );
	add_sound( "grab_metal_bar", "zmb_bar_pull" );
	add_sound( "break_metal_bar", "zmb_bar_break" );
	add_sound( "drop_metal_bar", "zmb_bar_drop" );
	add_sound( "blocker_end_move", "zmb_board_slam" );
	add_sound( "barrier_rebuild_slam", "zmb_board_slam" );
	add_sound( "bar_rebuild_slam", "zmb_bar_repair" );
	add_sound( "zmb_rock_fix", "zmb_break_rock_barrier_fix" );
	add_sound( "zmb_vent_fix", "evt_vent_slat_repair" );
	add_sound( "door_slide_open", "zmb_door_slide_open" );
	add_sound( "door_rotate_open", "zmb_door_slide_open" );
	add_sound( "debris_move", "zmb_weap_wall" );
	add_sound( "open_chest", "zmb_lid_open" );
	add_sound( "music_chest", "zmb_music_box" );
	add_sound( "close_chest", "zmb_lid_close" );
	add_sound( "weapon_show", "zmb_weap_wall" );
	add_sound( "break_stone", "break_stone" );
} // SP = 0x0 - check OK

// 0x9A00
init_levelvars()
{
	level.is_zombie_level = 1;
	level.laststandpistol = "m1911_zm";
	level.default_laststandpistol = "m1911_zm";
	level.default_solo_laststandpistol = "m1911_upgraded_zm";
	level.start_weapon = "m1911_zm";
	level.first_round = 1;
	level.round_number = getgametypesetting( "startRound" );
	level.enable_magic = getgametypesetting( "magic" );
	level.headshots_only = getgametypesetting( "headshotsonly" );
	level.player_starting_points = level.round_number * 500;
	level.round_start_time = 0;
	level.pro_tips_start_time = 0;
	level.intermission = 0;
	level.dog_intermission = 0;
	level.zombie_total = 0;
	level.total_zombies_killed = 0;
	level.hudelem_count = 0;
	level.zombie_spawn_locations = [];
	level.zombie_rise_spawners = [];
	level.current_zombie_array = [];
	level.current_zombie_count = 0;
	level.zombie_total_subtract = 0;
	level.destructible_callbacks = [];
	level.zombie_vars = [];
	_a1023 = level.teams;
	_k1023 = GetFirstArrayKey( _a1023 );
	if ( IsDefined( _k1023 ) )
	{
		team = _a1023[_k1023];
		level.zombie_vars[team] = [];
		_k1023 = GetNextArrayKey( _a1023, _k1023 );
	}
	difficulty = 1;
	column = int( difficulty ) + 1;
	set_zombie_var( "zombie_health_increase", 100, 0, column );
	set_zombie_var( "zombie_health_increase_multiplier", 0.1, 1, column );
	set_zombie_var( "zombie_health_start", 150, 0, column );
	set_zombie_var( "zombie_spawn_delay", 2, 1, column );
	set_zombie_var( "zombie_new_runner_interval", 10, 0, column );
	set_zombie_var( "zombie_move_speed_multiplier", 8, 0, column );
	set_zombie_var( "zombie_move_speed_multiplier_easy", 2, 0, column );
	set_zombie_var( "zombie_max_ai", 24, 0, column );
	set_zombie_var( "zombie_ai_per_player", 6, 0, column );
	set_zombie_var( "below_world_check", -1000 );
	set_zombie_var( "spectators_respawn", 1 );
	set_zombie_var( "zombie_use_failsafe", 1 );
	set_zombie_var( "zombie_between_round_time", 10 );
	set_zombie_var( "zombie_intermission_time", 15 );
	set_zombie_var( "game_start_delay", 0, 0, column );
	set_zombie_var( "penalty_no_revive", 0.1, 1, column );
	set_zombie_var( "penalty_died", 0, 1, column );
	set_zombie_var( "penalty_downed", 0.05, 1, column );
	set_zombie_var( "starting_lives", 1, 0, column );
	set_zombie_var( "zombie_score_kill_4player", 50 );
	set_zombie_var( "zombie_score_kill_3player", 50 );
	set_zombie_var( "zombie_score_kill_2player", 50 );
	set_zombie_var( "zombie_score_kill_1player", 50 );
	set_zombie_var( "zombie_score_kill_4p_team", 30 );
	set_zombie_var( "zombie_score_kill_3p_team", 35 );
	set_zombie_var( "zombie_score_kill_2p_team", 45 );
	set_zombie_var( "zombie_score_kill_1p_team", 0 );
	set_zombie_var( "zombie_score_damage_normal", 10 );
	set_zombie_var( "zombie_score_damage_light", 10 );
	set_zombie_var( "zombie_score_bonus_melee", 80 );
	set_zombie_var( "zombie_score_bonus_head", 50 );
	set_zombie_var( "zombie_score_bonus_neck", 20 );
	set_zombie_var( "zombie_score_bonus_torso", 10 );
	set_zombie_var( "zombie_score_bonus_burn", 10 );
	set_zombie_var( "zombie_flame_dmg_point_delay", 500 );
	set_zombie_var( "zombify_player", 0 );
	if ( issplitscreen() )
	{
		set_zombie_var( "zombie_timer_offset", 280 );
	}
	level thread init_player_levelvars();
	level.gamedifficulty = getgametypesetting( "zmDifficulty" );
	if ( level.gamedifficulty == 0 )
	{
		level.zombie_move_speed = level.round_number * level.zombie_vars["zombie_move_speed_multiplier_easy"];
	}
	level.zombie_move_speed = level.round_number * level.zombie_vars["zombie_move_speed_multiplier"];
	if ( level.round_number == 1 )
	{
		level.zombie_move_speed = 1;
	}
	i = 1;
	if ( i <= level.round_number )
	{
		timer = level.zombie_vars["zombie_spawn_delay"];
		if ( timer > 0.08 )
		{
			level.zombie_vars["zombie_spawn_delay"] = timer * 0.95;
		}
		if ( timer < 0.08 )
		{
			level.zombie_vars["zombie_spawn_delay"] = 0.08;
		}
	}
	level.speed_change_max = 0;
	level.speed_change_num = 0;
} // SP = 0x0 - check OK

// 0x9E7C
init_player_levelvars()
{
	flag_wait( "start_zombie_round_logic" );
	difficulty = 1;
	column = int( difficulty ) + 1;
	i = 0;
	if ( i < 8 )
	{
		points = 500;
		if ( i > 3 )
		{
			points = 3000;
		}
		points = set_zombie_var( "zombie_score_start_" + (i + 1) + "p", points, 0, column );
	}
} // SP = 0x0 - check OK

// 0x9EF4
init_dvars()
{
	if ( GetDvar( #"0xFA91EA91" ) == "" )
	{
		setdvar( "zombie_debug", "0" );
	}
	if ( GetDvar( #"0xB0FB65D0" ) == "" )
	{
		setdvar( "scr_zm_enable_bots", "0" );
	}
	if ( GetDvar( #"0xFA81816F" ) == "" )
	{
		setdvar( "zombie_cheat", "0" );
	}
	if ( level.script != "zombie_cod5_prototype" )
	{
		setdvar( "magic_chest_movable", "1" );
	}
	setdvar( "revive_trigger_radius", "75" );
	setdvar( "player_lastStandBleedoutTime", "45" );
	setdvar( "scr_deleteexplosivesonspawn", "0" );
} // SP = 0x0 - check OK

// 0x9FA8
init_function_overrides()
{
	level.callbackplayerdamage = ::callback_playerdamage;
	level.overrideplayerdamage = ::player_damage_override;
	level.callbackplayerkilled = ::player_killed_override;
	level.playerlaststand_func = ::player_laststand;
	level.callbackplayerlaststand = ::callback_playerlaststand;
	level.prevent_player_damage = ::player_prevent_damage;
	level.callbackactorkilled = ::actor_killed_override;
	level.callbackactordamage = ::actor_damage_override_wrapper;
	level.custom_introscreen = ::zombie_intro_screen;
	level.custom_intermission = ::player_intermission;
	level.global_damage_func = ::zombie_damage;
	level.global_damage_func_ads = ::zombie_damage_ads;
	level.reset_clientdvars = ::onplayerconnect_clientdvars;
	level.zombie_last_stand = ::last_stand_pistol_swap;
	level.zombie_last_stand_pistol_memory = ::last_stand_save_pistol_ammo;
	level.zombie_last_stand_ammo_return = ::last_stand_restore_pistol_ammo;
	level.player_becomes_zombie = ::zombify_player;
	level.validate_enemy_path_length = ::default_validate_enemy_path_length;
} // SP = 0x0 - check OK

// 0xA088
callback_playerlaststand( einflictor, eattacker, idamage, smeansofdeath, sweapon, vdir, shitloc, psoffsettime, deathanimduration )
{
	self endon( "disconnect" );
		/* Error: unknown opcode (0x2F) */
} // SP = 0x30 - check failed (function may have been decompiled incorrectly)

// 0xA0C4
codecallback_destructibleevent( event, param1, param2, param3 )
{
	if ( event == "broken" )
	{
		notify_type = param1;
		attacker = param2;
		weapon = param3;
		if ( IsDefined( level.destructible_callbacks[notify_type] ) )
		{
				/* Error: unknown opcode (0x35) */
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0xA14C
breakafter( time, damage, piece )
{
	self endon( "breakafter" );
	self dodamage( damage, level.origin, undefined, undefined );
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0xA17C
callback_playerdamage( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime, boneindex )
{
	println( "ZM Callback_PlayerDamage" + idamage + "\n" );
	if ( IsDefined( eattacker ) )
	{
	}
	if ( isplayer( eattacker ) )
	{
	}
	if ( level.sessionteam == level.sessionteam )
	{
	}
	if ( !(eattacker hasperk( "specialty_noname" )) )
	{
		if ( IsDefined( level.is_zombie ) )
		{
		}
	}
	if ( !(level.is_zombie) )
	{
		if ( self != eattacker )
		{
			println( "Exiting - players can't hurt each other." );
			return;
		}
		if ( smeansofdeath != "MOD_GRENADE_SPLASH" )
		{
		}
		if ( smeansofdeath != "MOD_GRENADE" )
		{
		}
		if ( smeansofdeath != "MOD_EXPLOSIVE" )
		{
		}
		if ( smeansofdeath != "MOD_PROJECTILE" )
		{
		}
		if ( smeansofdeath != "MOD_PROJECTILE_SPLASH" )
		{
		}
		if ( smeansofdeath != "MOD_BURNED" )
		{
		}
		if ( smeansofdeath != "MOD_SUICIDE" )
		{
			println( "Exiting - damage type verbotten." );
			return;
		}
		if ( is_classic() )
		{
			if ( IsDefined( level.pers_upgrade_insta_kill ) )
			{
			}
			if ( level.pers_upgrade_insta_kill )
			{
				if ( IsDefined( smeansofdeath ) )
				{
				}
				if ( smeansofdeath == "MOD_MELEE" )
				{
					if ( isplayer( self ) )
					{
					}
					if ( is_insta_kill_upgraded_and_active() )
					{
						level.pers_melee_swipe_zombie_swiper = eattacker;
					}
					if ( IsDefined( level.overrideplayerdamage ) )
					{
							/* Error: unknown opcode (0x31) */
} // SP = 0x40 - check failed (function may have been decompiled incorrectly)

// 0xA434
finishplayerdamagewrapper( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime, boneindex )
{
	self finishplayerdamage( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime, boneindex );
} // SP = 0x0 - check OK

// 0xA474
init_flags()
{
	flag_init( "solo_game" );
	flag_init( "start_zombie_round_logic" );
	flag_init( "start_encounters_match_logic" );
	flag_init( "spawn_point_override" );
	flag_init( "power_on" );
	flag_init( "crawler_round" );
	flag_init( "spawn_zombies", 1 );
	flag_init( "dog_round" );
	flag_init( "begin_spawning" );
	flag_init( "end_round_wait" );
	flag_init( "wait_and_revive" );
	flag_init( "instant_revive" );
	flag_init( "initial_blackscreen_passed" );
	flag_init( "initial_players_connected" );
} // SP = 0x0 - check OK

// 0xA528
init_client_flags()
{
	if ( IsDefined( level.use_clientside_board_fx ) )
	{
	}
	if ( level.use_clientside_board_fx )
	{
		level._zombie_scriptmover_flag_board_horizontal_fx = 14;
		level._zombie_scriptmover_flag_board_vertical_fx = 13;
	}
	if ( IsDefined( level.use_clientside_rock_tearin_fx ) )
	{
	}
	if ( level.use_clientside_rock_tearin_fx )
	{
		level._zombie_scriptmover_flag_rock_fx = 12;
	}
	level._zombie_player_flag_cloak_weapon = 14;
	registerclientfield( "toplayer", "deadshot_perk", 1, 1, "int" );
	registerclientfield( "actor", "zombie_riser_fx", 1, 1, "int" );
	if ( IsDefined( level._no_water_risers ) )
	{
	}
	if ( !(level._no_water_risers) )
	{
		registerclientfield( "actor", "zombie_riser_fx_water", 1, 1, "int" );
	}
	if ( IsDefined( level.risers_use_low_gravity_fx ) )
	{
	}
	if ( level.risers_use_low_gravity_fx )
	{
		registerclientfield( "actor", "zombie_riser_fx_lowg", 1, 1, "int" );
	}
} // SP = 0x0 - check OK

// 0xA5F0
init_fx()
{
	level.createfx_callback_thread = ::delete_in_createfx;
	level._effect["wood_chunk_destory"] = loadfx( "impacts/fx_large_woodhit" );
	level._effect["fx_zombie_bar_break"] = loadfx( "maps/zombie/fx_zombie_bar_break" );
	level._effect["fx_zombie_bar_break_lite"] = loadfx( "maps/zombie/fx_zombie_bar_break_lite" );
	level._effect["edge_fog"] = loadfx( "maps/zombie/fx_fog_zombie_amb" );
	level._effect["chest_light"] = loadfx( "maps/zombie/fx_zmb_tranzit_marker_glow" );
	level._effect["eye_glow"] = loadfx( "misc/fx_zombie_eye_single" );
	level._effect["headshot"] = loadfx( "impacts/fx_flesh_hit" );
	level._effect["headshot_nochunks"] = loadfx( "misc/fx_zombie_bloodsplat" );
	level._effect["bloodspurt"] = loadfx( "misc/fx_zombie_bloodspurt" );
	level._effect["tesla_head_light"] = loadfx( "maps/zombie/fx_zombie_tesla_neck_spurt" );
	level._effect["zombie_guts_explosion"] = loadfx( "maps/zombie/fx_zmb_tranzit_torso_explo" );
	level._effect["rise_burst_water"] = loadfx( "maps/zombie/fx_mp_zombie_hand_dirt_burst" );
	level._effect["rise_billow_water"] = loadfx( "maps/zombie/fx_mp_zombie_body_dirt_billowing" );
	level._effect["rise_dust_water"] = loadfx( "maps/zombie/fx_mp_zombie_body_dust_falling" );
	level._effect["rise_burst"] = loadfx( "maps/zombie/fx_mp_zombie_hand_dirt_burst" );
	level._effect["rise_billow"] = loadfx( "maps/zombie/fx_mp_zombie_body_dirt_billowing" );
	level._effect["rise_dust"] = loadfx( "maps/zombie/fx_mp_zombie_body_dust_falling" );
	level._effect["fall_burst"] = loadfx( "maps/zombie/fx_mp_zombie_hand_dirt_burst" );
	level._effect["fall_billow"] = loadfx( "maps/zombie/fx_mp_zombie_body_dirt_billowing" );
	level._effect["fall_dust"] = loadfx( "maps/zombie/fx_mp_zombie_body_dust_falling" );
	level._effect["character_fire_death_sm"] = loadfx( "env/fire/fx_fire_zombie_md" );
	level._effect["character_fire_death_torso"] = loadfx( "env/fire/fx_fire_zombie_torso" );
	level._effect["def_explosion"] = loadfx( "explosions/fx_default_explosion" );
	if ( IsDefined( level._uses_default_wallbuy_fx ) )
	{
	}
	if ( !(!(level._uses_default_wallbuy_fx)) )
	{
		level._effect["870mcs_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_870mcs" );
		level._effect["ak74u_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_ak74u" );
		level._effect["beretta93r_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_berreta93r" );
		level._effect["bowie_knife_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_bowie" );
		level._effect["claymore_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_claymore" );
		level._effect["m14_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_m14" );
		level._effect["m16_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_m16" );
		level._effect["mp5k_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_mp5k" );
		level._effect["rottweil72_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_olympia" );
	}
	if ( IsDefined( level._uses_sticky_grenades ) )
	{
	}
	if ( !(!(level._uses_sticky_grenades)) )
	{
		level._effect["sticky_grenade_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_semtex" );
	}
	if ( IsDefined( level._uses_taser_knuckles ) )
	{
	}
	if ( !(!(level._uses_taser_knuckles)) )
	{
		level._effect["tazer_knuckles_zm_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_taseknuck" );
	}
	if ( IsDefined( level.buildable_wallbuy_weapons ) )
	{
		level._effect["dynamic_wallbuy_fx"] = loadfx( "maps/zombie/fx_zmb_wall_buy_question" );
	}
	level._effect["upgrade_aquired"] = loadfx( "maps/zombie/fx_zmb_tanzit_upgrade" );
} // SP = 0x0 - check OK

// 0xA90C
zombie_intro_screen( string1, string2, string3, string4, string5 )
{
	flag_wait( "start_zombie_round_logic" );
} // SP = 0x0 - check OK

// 0xA92C
players_playing()
{
	players = get_players();
	level.players_playing = players.size;
	players = get_players();
	level.players_playing = players.size;
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xA960
onplayerconnect_clientdvars()
{
	self setclientcompass( 0 );
	self setclientthirdperson( 0 );
	self resetfov();
	self setclientthirdpersonangle( 0 );
	self setclientammocounterhide( 1 );
	self setclientminiscoreboardhide( 1 );
	self setclienthudhardcore( 0 );
	self setclientplayerpushamount( 1 );
	self setdepthoffield( 0, 0, 512, 4000, 4, 0 );
	self setclientaimlockonpitchstrength( 0 );
	self player_getup_setup();
} // SP = 0x0 - check OK

// 0xA9FC
checkforalldead( excluded_player )
{
	players = get_players();
	count = 0;
	i = 0;
	if ( i < players.size )
	{
		if ( IsDefined( excluded_player ) )
		{
		}
		if ( excluded_player == players[i] )
		{
		}
		if ( !(players[i] player_is_in_laststand()) )
		{
		}
		if ( !(level.sessionstate == "spectator") )
		{
		}
	}
	if ( count == 0 )
	{
		if ( IsDefined( level.no_end_game_check ) )
		{
		}
	}
	if ( !(level.no_end_game_check) )
	{
	}
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0xAA90
onplayerspawned()
{
	self endon( "disconnect" );
	self endon( "stop_onPlayerSpawned" );
	self waittill( "spawned_player" );
	if ( IsDefined( level.host_ended_game ) )
	{
	}
	if ( !(level.host_ended_game) )
	{
		self freezecontrols( 0 );
		println( " Unfreeze controls 7" );
	}
	level.hits = 0;
	self init_player_offhand_weapons();
	lethal_grenade = self get_player_lethal_grenade();
	if ( !(self hasweapon( lethal_grenade )) )
	{
		self giveweapon( lethal_grenade );
		self setweaponammoclip( lethal_grenade, 0 );
	}
	self recordplayerrevivezombies( self );
	if ( GetDvarInt( #"0xFA81816F" ) >= 1 )
	{
	}
	if ( GetDvarInt( #"0xFA81816F" ) <= 3 )
	{
		self enableinvulnerability();
	}
	self setactionslot( 3, "altMode" );
	self playerknockback( 0 );
	self setclientthirdperson( 0 );
	self resetfov();
	self setclientthirdpersonangle( 0 );
	self setdepthoffield( 0, 0, 512, 4000, 4, 0 );
	self cameraactivate( 0 );
	level.num_perks = 0;
	self setblur( 0, 0.1 );
	level.zmbdialogqueue = [];
	level.zmbdialogactive = 0;
	level.zmbdialoggroups = [];
	level.zmbdialoggroup = "";
	if ( IsDefined( level.player_out_of_playable_area_monitor ) )
	{
	}
	if ( level.player_out_of_playable_area_monitor )
	{
		self thread player_out_of_playable_area_monitor();
	}
	if ( IsDefined( level.player_too_many_weapons_monitor ) )
	{
	}
	if ( level.player_too_many_weapons_monitor )
	{
			/* Error: unknown opcode (0x35) */
} // SP = 0x24 - check failed (function may have been decompiled incorrectly)

// 0xAD30
player_spawn_protection()
{
	self endon( "disconnect" );
	x = 0;
	if ( x < 60 )
	{
		level.ignoreme = 1;
	}
	level.ignoreme = 0;
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xAD68
spawn_life_brush( origin, radius, height )
{
	life_brush = spawn( "trigger_radius", origin, 0, radius, height );
	level.script_noteworthy = "life_brush";
	return life_brush;
} // SP = 0x0 - check OK

// 0xAD98
in_life_brush()
{
	life_brushes = getentarray( "life_brush", "script_noteworthy" );
	if ( !(IsDefined( life_brushes )) )
	{
		return 0;
	}
	i = 0;
	if ( i < life_brushes.size )
	{
		if ( self istouching( life_brushes[i] ) )
		{
			return 1;
		}
	}
	return 0;
} // SP = 0x0 - check OK

// 0xADE8
spawn_kill_brush( origin, radius, height )
{
	kill_brush = spawn( "trigger_radius", origin, 0, radius, height );
	level.script_noteworthy = "kill_brush";
	return kill_brush;
} // SP = 0x0 - check OK

// 0xAE18
in_kill_brush()
{
	kill_brushes = getentarray( "kill_brush", "script_noteworthy" );
	if ( !(IsDefined( kill_brushes )) )
	{
		return 0;
	}
	i = 0;
	if ( i < kill_brushes.size )
	{
		if ( self istouching( kill_brushes[i] ) )
		{
			return 1;
		}
	}
	return 0;
} // SP = 0x0 - check OK

// 0xAE68
in_enabled_playable_area()
{
	playable_area = getentarray( "player_volume", "script_noteworthy" );
	if ( !(IsDefined( playable_area )) )
	{
		return 0;
	}
	i = 0;
	if ( i < playable_area.size )
	{
		if ( zone_is_enabled( level.targetname ) )
		{
		}
		if ( self istouching( playable_area[i] ) )
		{
			return 1;
		}
	}
	return 0;
} // SP = 0x0 - check OK

// 0xAED0
get_player_out_of_playable_area_monitor_wait_time()
{
	if ( IsDefined( level.check_kill_thread_every_frame ) )
	{
	}
	if ( level.check_kill_thread_every_frame )
	{
		return 0.05;
	}
	return 3;
} // SP = 0x0 - check OK

// 0xAEF4
player_out_of_playable_area_monitor()
{
	self endon( "stop_player_out_of_playable_area_monitor" );
	self endon( "disconnect" );
	level endon( "end_game" );
	if ( !(IsDefined( level.characterindex )) )
	{
	}
	if ( 1 )
	{
		if ( level.sessionstate == "spectator" )
		{
		}
		if ( is_true( level.hostmigration_occured ) )
		{
		}
		if ( !(self in_life_brush()) )
		{
			if ( !(self in_kill_brush()) )
			{
			}
			if ( !(self in_enabled_playable_area()) )
			{
				if ( !(!(IsDefined( level.player_out_of_playable_area_monitor_callback ))) )
				{
						/* Error: unknown opcode (0x31) */
} // SP = 0x34 - check failed (function may have been decompiled incorrectly)

// 0xB0C4
get_player_too_many_weapons_monitor_wait_time()
{
	return 3;
} // SP = 0x0 - check OK

// 0xB0CC
player_too_many_weapons_monitor_takeaway_simultaneous( primary_weapons_to_take )
{
	self endon( "player_too_many_weapons_monitor_takeaway_sequence_done" );
	self waittill_any( "player_downed", "replace_weapon_powerup" );
	i = 0;
	if ( i < primary_weapons_to_take.size )
	{
		self takeweapon( primary_weapons_to_take[i] );
	}
	self minus_to_player_score( level.score );
	self give_start_weapon( 0 );
	if ( !(self player_is_in_laststand()) )
	{
		self decrement_is_drinking();
	}
	if ( flag( "solo_game" ) )
	{
		level.score_lost_when_downed = 0;
	}
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xB168
player_too_many_weapons_monitor_takeaway_sequence( primary_weapons_to_take )
{
	self thread player_too_many_weapons_monitor_takeaway_simultaneous( primary_weapons_to_take );
	self endon( "player_downed" );
	self endon( "replace_weapon_powerup" );
	self increment_is_drinking();
	score_decrement = round_up_to_ten( int( level.score / (primary_weapons_to_take.size + 1) ) );
	i = 0;
	if ( i < primary_weapons_to_take.size )
	{
		self playlocalsound( level.zmb_laugh_alias );
		self switchtoweapon( primary_weapons_to_take[i] );
		self minus_to_player_score( score_decrement );
		self takeweapon( primary_weapons_to_take[i] );
	}
	self playlocalsound( level.zmb_laugh_alias );
	self minus_to_player_score( level.score );
	self give_start_weapon( 1 );
	self decrement_is_drinking();
} // SP = 0x1C - check failed (function may have been decompiled incorrectly)

// 0xB254
player_too_many_weapons_monitor()
{
	self endon( "stop_player_too_many_weapons_monitor" );
	self endon( "disconnect" );
	level endon( "end_game" );
	scalar = level.characterindex;
	if ( !(IsDefined( scalar )) )
	{
		scalar = self getentitynumber();
	}
	if ( 1 )
	{
		if ( !(self has_powerup_weapon()) )
		{
		}
		if ( !(self player_is_in_laststand()) )
		{
		}
		if ( level.sessionstate == "spectator" )
		{
		}
		if ( GetDvarInt( #"0xFA81816F" ) > 0 )
		{
		}
		weapon_limit = 2;
		if ( self hasperk( "specialty_additionalprimaryweapon" ) )
		{
			weapon_limit = 3;
		}
		primaryweapons = self getweaponslistprimaries();
		if ( primaryweapons.size > weapon_limit )
		{
			self take_fallback_weapon();
			primaryweapons = self getweaponslistprimaries();
		}
		primary_weapons_to_take = [];
		i = 0;
		if ( i < primaryweapons.size )
		{
			if ( !(is_weapon_included( primaryweapons[i] )) )
			{
			}
			if ( is_weapon_upgraded( primaryweapons[i] ) )
			{
				primary_weapons_to_take[primary_weapons_to_take.size] = primaryweapons[i];
			}
		}
		if ( primary_weapons_to_take.size > weapon_limit )
		{
			if ( !(!(IsDefined( level.player_too_many_weapons_monitor_callback ))) )
			{
					/* Error: unknown opcode (0x31) */
} // SP = 0x34 - check failed (function may have been decompiled incorrectly)

// 0xB3F8
player_monitor_travel_dist()
{
	self endon( "disconnect" );
	self endon( "stop_player_monitor_travel_dist" );
	prevpos = level.origin;
	if ( 1 )
	{
		level.pers["distance_traveled"] = level.pers["distance_traveled"] + distance( level.origin, prevpos );
		prevpos = level.origin;
	}
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0xB454
player_monitor_time_played()
{
	self endon( "disconnect" );
	self endon( "stop_player_monitor_time_played" );
	flag_wait( "start_zombie_round_logic" );
	increment_client_stat( "time_played_total" );
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0xB490
reset_rampage_bookmark_kill_times()
{
	if ( !(IsDefined( level.rampage_bookmark_kill_times )) )
	{
		level.rampage_bookmark_kill_times = [];
		level.ignore_rampage_kill_times = 0;
	}
	i = 0;
	if ( i < level.rampage_bookmark_kill_times_count )
	{
		level.rampage_bookmark_kill_times[i] = 0;
	}
} // SP = 0x0 - check OK

// 0xB4D0
add_rampage_bookmark_kill_time()
{
	now = GetTime();
	if ( now <= level.ignore_rampage_kill_times )
	{
		return;
	}
	oldest_index = 0;
	oldest_time = now + 1;
	i = 0;
	if ( i < level.rampage_bookmark_kill_times_count )
	{
		if ( !(level.rampage_bookmark_kill_times[i]) )
		{
			oldest_index = i;
		}
		if ( oldest_time > level.rampage_bookmark_kill_times[i] )
		{
			oldest_index = i;
			oldest_time = level.rampage_bookmark_kill_times[i];
		}
	}
	level.rampage_bookmark_kill_times[oldest_index] = now;
} // SP = 0x0 - check OK

// 0xB550
watch_rampage_bookmark()
{
	if ( 1 )
	{
		now = GetTime();
		oldest_allowed = now - level.rampage_bookmark_kill_times_msec;
		players = get_players();
		player_index = 0;
		if ( player_index < players.size )
		{
			player = players[player_index];
			if ( IsDefined( level.pers["isBot"] ) )
			{
			}
			if ( level.pers["isBot"] )
			{
			}
			time_index = 0;
			if ( time_index < level.rampage_bookmark_kill_times_count )
			{
				if ( !(level.rampage_bookmark_kill_times[time_index]) )
				{
				}
				if ( oldest_allowed > level.rampage_bookmark_kill_times[time_index] )
				{
					level.rampage_bookmark_kill_times[time_index] = 0;
				}
			}
			if ( time_index >= level.rampage_bookmark_kill_times_count )
			{
				bookmark( "zm_player_rampage", GetTime(), player );
				player reset_rampage_bookmark_kill_times();
				level.ignore_rampage_kill_times = now + level.rampage_bookmark_kill_times_delay;
			}
		}
	}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xB654
player_grenade_multiattack_bookmark_watcher( grenade )
{
	self endon( "disconnect" );
	if ( !(IsDefined( grenade )) )
	{
		return;
	}
	inflictorentnum = grenade getentitynumber();
	inflictorenttype = grenade getentitytype();
	inflictorbirthtime = 0;
	if ( IsDefined( level.birthtime ) )
	{
		inflictorbirthtime = level.birthtime;
	}
	ret_val = grenade waittill_any_timeout( 15, "explode" );
	if ( !(!(IsDefined( self ))) )
	{
		if ( IsDefined( ret_val ) )
		{
		}
		if ( "timeout" == ret_val )
		{
			return;
		}
		level.grenade_multiattack_count = 0;
		if ( !(IsDefined( self )) )
		{
			return;
		}
		count = level.grenade_multiattack_bookmark_count;
		if ( IsDefined( level.grenade_multiattack_bookmark_count ) )
		{
		}
		if ( level.grenade_multiattack_bookmark_count )
		{
			count = level.grenade_multiattack_bookmark_count;
		}
		bookmark_string = "zm_player_grenade_multiattack";
		if ( IsDefined( level.use_grenade_special_long_bookmark ) )
		{
		}
		if ( level.use_grenade_special_long_bookmark )
		{
			bookmark_string = "zm_player_grenade_special_long";
		}
		if ( IsDefined( level.use_grenade_special_bookmark ) )
		{
		}
		if ( level.use_grenade_special_bookmark )
		{
			bookmark_string = "zm_player_grenade_special";
		}
		if ( count <= level.grenade_multiattack_count )
		{
		}
		if ( IsDefined( level.grenade_multiattack_ent ) )
		{
			adddemobookmark( level.bookmark[bookmark_string], GetTime(), self getentitynumber(), 255, 0, inflictorentnum, inflictorenttype, inflictorbirthtime, 0, level.grenade_multiattack_ent getentitynumber() );
		}
		level.grenade_multiattack_count = 0;
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xB798
player_grenade_watcher()
{
	self endon( "disconnect" );
	self endon( "stop_player_grenade_watcher" );
	level.grenade_multiattack_count = 0;
	if ( 1 )
	{
		self waittill( "grenade_fire" );
		if ( IsDefined( grenade ) )
		{
		}
		if ( isalive( grenade ) )
		{
			level.team = level.team;
		}
		self thread player_grenade_multiattack_bookmark_watcher( grenade );
		if ( IsDefined( level.grenade_watcher ) )
		{
				/* Error: unknown opcode (0x31) */
} // SP = 0x2C - check failed (function may have been decompiled incorrectly)

// 0xB814
player_prevent_damage( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime )
{
	if ( !(!(IsDefined( einflictor ))) )
	{
	}
	if ( !(IsDefined( eattacker )) )
	{
		return 0;
	}
	if ( !(einflictor == self) )
	{
	}
	if ( eattacker == self )
	{
		return 0;
	}
	if ( IsDefined( einflictor ) )
	{
	}
	if ( IsDefined( level.team ) )
	{
		if ( IsDefined( level.damage_own_team ) )
		{
		}
		if ( !(level.damage_own_team) )
		{
			if ( level.team == level.team )
			{
				return 1;
			}
			return 0;
} // SP = 0x0 - check OK

// 0xB88C
player_revive_monitor()
{
	self endon( "disconnect" );
	self endon( "stop_player_revive_monitor" );
	if ( 1 )
	{
		self waittill( "player_revived" );
		self playsoundtoplayer( "zmb_character_revived", self );
		if ( IsDefined( level.isresetting_grief ) )
		{
		}
		if ( level.isresetting_grief )
		{
		}
		bbprint( "zombie_playerdeaths", "round %d playername %s deathtype %s x %f y %f z %f", level.round_number, level.name, "revived", level.origin );
		if ( IsDefined( reviver ) )
		{
			self create_and_play_dialog( "general", "revive_up" );
			points = level.score_lost_when_downed;
			println( "ZM >> LAST STAND - points = " + points );
			reviver player_add_points( "reviver", points );
			level.score_lost_when_downed = 0;
		}
	}
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0xB94C
laststand_giveback_player_perks()
{
	if ( IsDefined( level.laststand_perks ) )
	{
		lost_perk_index = int( -1 );
		if ( level.laststand_perks.size > 1 )
		{
			lost_perk_index = randomint( level.laststand_perks.size - 1 );
		}
		i = 0;
		if ( i < level.laststand_perks.size )
		{
			if ( self hasperk( level.laststand_perks[i] ) )
			{
			}
			if ( i == lost_perk_index )
			{
			}
			give_perk( level.laststand_perks[i] );
		}
} // SP = 0x0 - check OK

// 0xB9DC
remote_revive_watch()
{
	self endon( "death" );
	self endon( "player_revived" );
	keep_checking = 1;
	if ( keep_checking )
	{
		self waittill( "remote_revive" );
		if ( level.team == level.team )
		{
			keep_checking = 0;
		}
	}
	self remote_revive( reviver );
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0xBA2C
remove_deadshot_bottle()
{
	if ( IsDefined( level.lastactiveweapon ) )
	{
	}
	if ( level.lastactiveweapon == "zombie_perk_bottle_deadshot" )
	{
		level.lastactiveweapon = "none";
	}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xBA58
take_additionalprimaryweapon()
{
	weapon_to_take = undefined;
	if ( IsDefined( level._retain_perks ) )
	{
	}
	if ( level._retain_perks )
	{
		return weapon_to_take;
	}
	primary_weapons_that_can_be_taken = [];
	primaryweapons = self getweaponslistprimaries();
	i = 0;
	if ( i < primaryweapons.size )
	{
		if ( !(is_weapon_included( primaryweapons[i] )) )
		{
		}
		if ( is_weapon_upgraded( primaryweapons[i] ) )
		{
			primary_weapons_that_can_be_taken[primary_weapons_that_can_be_taken.size] = primaryweapons[i];
		}
	}
	if ( primary_weapons_that_can_be_taken.size >= 3 )
	{
		weapon_to_take = primary_weapons_that_can_be_taken[primary_weapons_that_can_be_taken.size - 1];
		if ( weapon_to_take == self getcurrentweapon() )
		{
			self switchtoweapon( primary_weapons_that_can_be_taken[0] );
		}
		self takeweapon( weapon_to_take );
	}
	return weapon_to_take;
} // SP = 0x0 - check OK

// 0xBB10
player_laststand( einflictor, attacker, idamage, smeansofdeath, sweapon, vdir, shitloc, psoffsettime, deathanimduration )
{
	println( "ZM >> LAST STAND - player_laststand called" );
	b_alt_visionset = 0;
	self allowjump( 0 );
	currweapon = self getcurrentweapon();
	statweapon = currweapon;
	if ( is_alt_weapon( statweapon ) )
	{
		statweapon = weaponaltweaponname( statweapon );
	}
	self addweaponstat( statweapon, "deathsDuringUse", 1 );
	if ( IsDefined( level.hasperkspecialtytombstone ) )
	{
	}
	if ( level.hasperkspecialtytombstone )
	{
		level.laststand_perks = tombstone_save_perks( self );
	}
	if ( IsDefined( level.pers_upgrades_awarded["perk_lose"] ) )
	{
	}
	if ( level.pers_upgrades_awarded["perk_lose"] )
	{
		self pers_upgrade_perk_lose_save_perks();
	}
	players = get_players();
	if ( players.size == 1 )
	{
	}
	if ( flag( "solo_game" ) )
	{
		if ( level.lives > 0 )
		{
		}
		if ( self hasperk( "specialty_quickrevive" ) )
		{
			self thread wait_and_revive();
		}
		if ( self hasperk( "specialty_additionalprimaryweapon" ) )
		{
			primary_weapons_that_can_be_taken = [];
			primaryweapons = self getweaponslistprimaries();
			i = 0;
			if ( i < primaryweapons.size )
			{
				if ( !(is_weapon_included( primaryweapons[i] )) )
				{
				}
				if ( is_weapon_upgraded( primaryweapons[i] ) )
				{
					primary_weapons_that_can_be_taken[primary_weapons_that_can_be_taken.size] = primaryweapons[i];
				}
			}
			if ( primary_weapons_that_can_be_taken.size >= 3 )
			{
				weapon_to_take = primary_weapons_that_can_be_taken[primary_weapons_that_can_be_taken.size - 1];
				self takeweapon( weapon_to_take );
				level.weapon_taken_by_losing_specialty_additionalprimaryweapon = weapon_to_take;
			}
			if ( IsDefined( level.hasperkspecialtytombstone ) )
			{
			}
			if ( level.hasperkspecialtytombstone )
			{
					/* Error: unknown opcode (0x31) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0xBE54
failsafe_revive_give_back_weapons( excluded_player )
{
	i = 0;
	if ( i < 10 )
	{
		players = get_players();
		_a2551 = players;
		_k2551 = GetFirstArrayKey( _a2551 );
		if ( IsDefined( _k2551 ) )
		{
			player = _a2551[_k2551];
			if ( !(player == excluded_player) )
			{
			}
			if ( !(!(IsDefined( level.reviveprogressbar ))) )
			{
			}
			if ( player is_reviving_any() )
			{
			}
			iprintlnbold( "FAILSAFE CLEANING UP REVIVE HUD AND GUN" );
			player revive_give_back_weapons( "none" );
			if ( IsDefined( level.reviveprogressbar ) )
			{
				level.reviveprogressbar destroyelem();
			}
			if ( IsDefined( level.revivetexthud ) )
			{
				level.revivetexthud destroy();
			}
			_k2551 = GetNextArrayKey( _a2551, _k2551 );
		}
	}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xBF30
spawnspectator()
{
	self endon( "disconnect" );
	self endon( "spawned_spectator" );
	if ( level.intermission )
	{
		return;
	}
	if ( IsDefined( level.no_spectator ) )
	{
	}
	if ( level.no_spectator )
	{
		exitlevel();
	}
	level.is_zombie = 1;
	level thread failsafe_revive_give_back_weapons( self );
	if ( IsDefined( level.revivetrigger ) )
	{
		level.revivetrigger delete();
	}
	level.zombification_time = GetTime();
	resettimeout();
	self stopshellshock();
	self stoprumble( "damage_heavy" );
	level.sessionstate = "spectator";
	level.spectatorclient = -1;
	level.maxhealth = level.health;
	level.shellshocked = 0;
	level.inwater = 0;
	level.hasspawned = 1;
	level.spawntime = GetTime();
	level.afk = 0;
	println( "*************************Zombie Spectator***" );
	self detachall();
	if ( IsDefined( level.custom_spectate_permissions ) )
	{
			/* Error: unknown opcode (0x31) */
} // SP = 0x3C - check failed (function may have been decompiled incorrectly)

// 0xC074
setspectatepermissions( ison )
{
	if ( ison )
	{
	}
	self allowspectateteam( "allies", level.team == "allies" );
	if ( ison )
	{
	}
	self allowspectateteam( "axis", level.team == "axis" );
	self allowspectateteam( "freelook", 0 );
	self allowspectateteam( "none", 0 );
} // SP = 0x0 - check OK

// 0xC0D8
spectator_thread()
{
	self endon( "disconnect" );
	self endon( "spawned_player" );
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xC0EC
spectator_toggle_3rd_person()
{
	self endon( "disconnect" );
	self endon( "spawned_player" );
	third_person = 1;
	self set_third_person( 1 );
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xC114
set_third_person( value )
{
	if ( value )
	{
		self setclientthirdperson( 1 );
		self setclientthirdpersonangle( 354 );
		self setdepthoffield( 0, 128, 512, 4000, 6, 1.8 );
	}
	self setclientthirdperson( 0 );
	self setclientthirdpersonangle( 0 );
	self setdepthoffield( 0, 0, 512, 4000, 4, 0 );
	self resetfov();
} // SP = 0x0 - check OK

// 0xC19C
last_stand_revive()
{
	level endon( "between_round_over" );
	players = get_players();
	laststand_count = 0;
	_a2741 = players;
	_k2741 = GetFirstArrayKey( _a2741 );
	if ( IsDefined( _k2741 ) )
	{
		player = _a2741[_k2741];
		if ( !(is_player_valid( player )) )
		{
		}
		_k2741 = GetNextArrayKey( _a2741, _k2741 );
	}
	if ( laststand_count == players.size )
	{
		i = 0;
		if ( i < players.size )
		{
			if ( players[i] player_is_in_laststand() )
			{
			}
			if ( level.beingrevived == 0 )
			{
				players[i] auto_revive( players[i] );
			}
		}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xC258
last_stand_pistol_rank_init()
{
	level.pistol_values = [];
	level.pistol_values[level.pistol_values.size] = "m1911_zm";
	level.pistol_values[level.pistol_values.size] = "cz75_zm";
	level.pistol_values[level.pistol_values.size] = "cz75dw_zm";
	level.pistol_values[level.pistol_values.size] = "python_zm";
	level.pistol_values[level.pistol_values.size] = "python_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "judge_zm";
	level.pistol_values[level.pistol_values.size] = "judge_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "kard_zm";
	level.pistol_values[level.pistol_values.size] = "kard_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "fiveseven_zm";
	level.pistol_values[level.pistol_values.size] = "fiveseven_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "beretta93r_zm";
	level.pistol_values[level.pistol_values.size] = "beretta93r_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "fivesevendw_zm";
	level.pistol_values[level.pistol_values.size] = "fivesevendw_upgraded_zm";
	level.pistol_value_solo_replace_below = level.pistol_values.size - 1;
	level.pistol_values[level.pistol_values.size] = "cz75_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "cz75dw_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "m1911_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "ray_gun_zm";
	level.pistol_values[level.pistol_values.size] = "freezegun_zm";
	level.pistol_values[level.pistol_values.size] = "ray_gun_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "freezegun_upgraded_zm";
	level.pistol_values[level.pistol_values.size] = "microwavegundw_zm";
	level.pistol_values[level.pistol_values.size] = "microwavegundw_upgraded_zm";
} // SP = 0x0 - check OK

// 0xC3F0
last_stand_pistol_swap()
{
	if ( self has_powerup_weapon() )
	{
		level.lastactiveweapon = "none";
	}
	if ( !(self hasweapon( level.laststandpistol )) )
	{
		self giveweapon( level.laststandpistol );
	}
	ammoclip = weaponclipsize( level.laststandpistol );
	doubleclip = ammoclip * 2;
	if ( IsDefined( level._special_solo_pistol_swap ) )
	{
	}
	if ( !(level._special_solo_pistol_swap) )
	{
		if ( level.laststandpistol == "m1911_upgraded_zm" )
		{
		}
		if ( !(level.hadpistol) )
		{
			level._special_solo_pistol_swap = 0;
			level.hadpistol = 0;
			self setweaponammostock( level.laststandpistol, doubleclip );
		}
		if ( flag( "solo_game" ) )
		{
		}
		if ( level.laststandpistol == "m1911_upgraded_zm" )
		{
			self setweaponammostock( level.laststandpistol, doubleclip );
		}
		if ( level.laststandpistol == "m1911_zm" )
		{
			self setweaponammostock( level.laststandpistol, doubleclip );
		}
		if ( !(level.laststandpistol == "ray_gun_zm") )
		{
		}
		if ( level.laststandpistol == "ray_gun_upgraded_zm" )
		{
			if ( level.total_amt >= ammoclip )
			{
				self setweaponammoclip( level.laststandpistol, ammoclip );
				level.given_amt = ammoclip;
			}
			self setweaponammoclip( level.laststandpistol, level.total_amt );
			level.given_amt = level.total_amt;
			self setweaponammostock( level.laststandpistol, 0 );
		}
		if ( level.stock_amt >= doubleclip )
		{
			self setweaponammostock( level.laststandpistol, doubleclip );
			level.given_amt = doubleclip + level.clip_amt + level.left_clip_amt;
		}
		self setweaponammostock( level.laststandpistol, level.stock_amt );
		level.given_amt = level.total_amt;
		self switchtoweapon( level.laststandpistol );
} // SP = 0x0 - check OK

// 0xC62C
last_stand_best_pistol()
{
	pistol_array = [];
	current_weapons = self getweaponslistprimaries();
	i = 0;
	if ( i < current_weapons.size )
	{
		class = weaponclass( current_weapons[i] );
		if ( issubstr( current_weapons[i], "knife_ballistic_" ) )
		{
			class = "knife";
		}
		if ( !(class == "pistol") )
		{
		}
		if ( !(class == "pistolspread") )
		{
		}
		if ( class == "pistol spread" )
		{
			if ( current_weapons[i] != "m1911_zm" )
			{
			}
			if ( !(!(flag( "solo_game" ))) )
			{
				if ( !(flag( "solo_game" )) )
				{
				}
				if ( current_weapons[i] != "m1911_upgraded_zm" )
				{
					if ( self getammocount( current_weapons[i] ) <= 0 )
					{
					}
					pistol_array_index = pistol_array.size;
					pistol_array[pistol_array_index] = spawnstruct();
					level.gun = current_weapons[i];
					level.value = 0;
					j = 0;
					if ( j < level.pistol_values.size )
					{
						if ( level.pistol_values[j] == current_weapons[i] )
						{
							level.value = j;
						}
					}
					level.laststandpistol = last_stand_compare_pistols( pistol_array );
} // SP = 0x0 - check OK

// 0xC774
last_stand_compare_pistols( struct_array )
{
	if ( !(!(isarray( struct_array ))) )
	{
	}
	if ( struct_array.size <= 0 )
	{
		level.hadpistol = 0;
		if ( IsDefined( level.stored_weapon_info ) )
		{
			stored_weapon_info = getarraykeys( level.stored_weapon_info );
			j = 0;
			if ( j < stored_weapon_info.size )
			{
				if ( stored_weapon_info[j] == level.laststandpistol )
				{
					level.hadpistol = 1;
				}
			}
			return level.laststandpistol;
			highest_score_pistol = struct_array[0];
			i = 1;
			if ( i < struct_array.size )
			{
				if ( level.value > level.value )
				{
					highest_score_pistol = struct_array[i];
				}
			}
			if ( flag( "solo_game" ) )
			{
				level._special_solo_pistol_swap = 0;
				if ( level.value <= level.pistol_value_solo_replace_below )
				{
					level.hadpistol = 0;
					level._special_solo_pistol_swap = 1;
					if ( IsDefined( level.force_solo_quick_revive ) )
					{
					}
					if ( level.force_solo_quick_revive )
					{
						if ( !(self hasperk( "specialty_quickrevive" )) )
						{
						}
						if ( !(self hasperk( "specialty_quickrevive" )) )
						{
							return level.gun;
						}
						return level.laststandpistol;
						return level.gun;
						return level.gun;
} // SP = 0x0 - check OK

// 0xC8AC
last_stand_save_pistol_ammo()
{
	weapon_inventory = self getweaponslist( 1 );
	level.stored_weapon_info = [];
	i = 0;
	if ( i < weapon_inventory.size )
	{
		weapon = weapon_inventory[i];
		class = weaponclass( weapon );
		if ( issubstr( weapon, "knife_ballistic_" ) )
		{
			class = "knife";
		}
		if ( !(class == "pistol") )
		{
		}
		if ( !(class == "pistolspread") )
		{
		}
		if ( class == "pistol spread" )
		{
			level.stored_weapon_info[weapon] = spawnstruct();
			level.clip_amt = self getweaponammoclip( weapon );
			level.left_clip_amt = 0;
			dual_wield_name = weapondualwieldweaponname( weapon );
			if ( "none" != dual_wield_name )
			{
				level.left_clip_amt = self getweaponammoclip( dual_wield_name );
			}
			level.stock_amt = self getweaponammostock( weapon );
			level.total_amt = level.clip_amt + level.left_clip_amt + level.stock_amt;
			level.given_amt = 0;
		}
	}
	self last_stand_best_pistol();
} // SP = 0x0 - check OK

// 0xC9FC
last_stand_restore_pistol_ammo()
{
	if ( !(IsDefined( level.stored_weapon_info )) )
	{
		return;
	}
	weapon_inventory = self getweaponslist( 1 );
	weapon_to_restore = getarraykeys( level.stored_weapon_info );
	i = 0;
	if ( i < weapon_inventory.size )
	{
		weapon = weapon_inventory[i];
		if ( weapon != level.laststandpistol )
		{
		}
		j = 0;
		if ( j < weapon_to_restore.size )
		{
			check_weapon = weapon_to_restore[j];
			if ( weapon == check_weapon )
			{
				dual_wield_name = weapondualwieldweaponname( weapon_to_restore[j] );
				if ( weapon != "m1911_zm" )
				{
					last_clip = self getweaponammoclip( weapon );
					last_left_clip = 0;
					if ( "none" != dual_wield_name )
					{
						last_left_clip = self getweaponammoclip( dual_wield_name );
					}
					last_stock = self getweaponammostock( weapon );
					last_total = last_clip + last_left_clip + last_stock;
					used_amt = level.given_amt - last_total;
					if ( used_amt >= level.stock_amt )
					{
						used_amt = used_amt - level.stock_amt;
						level.stock_amt = 0;
						level.clip_amt = level.clip_amt - used_amt;
						if ( level.clip_amt < 0 )
						{
							level.clip_amt = 0;
						}
					}
					new_stock_amt = level.stock_amt - used_amt;
					if ( new_stock_amt < level.stock_amt )
					{
						level.stock_amt = new_stock_amt;
					}
					self setweaponammoclip( weapon_to_restore[j], level.clip_amt );
					if ( "none" != dual_wield_name )
					{
						self setweaponammoclip( dual_wield_name, level.left_clip_amt );
					}
					self setweaponammostock( weapon_to_restore[j], level.stock_amt );
} // SP = 0x0 - check OK

// 0xCC18
zombiemode_solo_last_stand_pistol()
{
	level.laststandpistol = "m1911_upgraded_zm";
} // SP = 0x0 - check OK

// 0xCC28
last_stand_take_thrown_grenade()
{
	self endon( "disconnect" );
	self endon( "bled_out" );
	self endon( "player_revived" );
	self waittill( "grenade_fire" );
	if ( IsDefined( level.lsgsar_lethal ) )
	{
	}
	if ( weaponname == level.lsgsar_lethal )
	{
	}
	if ( IsDefined( level.lsgsar_tactical ) )
	{
	}
	if ( weaponname == level.lsgsar_tactical )
	{
	}
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xCC80
last_stand_grenade_save_and_return()
{
	if ( IsDefined( level.isresetting_grief ) )
	{
	}
	if ( level.isresetting_grief )
	{
		return;
	}
	self endon( "disconnect" );
	self endon( "bled_out" );
	level endon( "between_round_over" );
	level.lsgsar_lethal_nade_amt = 0;
	level.lsgsar_has_lethal_nade = 0;
	level.lsgsar_tactical_nade_amt = 0;
	level.lsgsar_has_tactical_nade = 0;
	if ( self isthrowinggrenade() )
	{
		self thread last_stand_take_thrown_grenade();
	}
	weapons_on_player = self getweaponslist( 1 );
	i = 0;
	if ( i < weapons_on_player.size )
	{
		if ( self is_player_lethal_grenade( weapons_on_player[i] ) )
		{
			level.lsgsar_has_lethal_nade = 1;
			level.lsgsar_lethal = self get_player_lethal_grenade();
			level.lsgsar_lethal_nade_amt = self getweaponammoclip( self get_player_lethal_grenade() );
			self setweaponammoclip( self get_player_lethal_grenade(), 0 );
			self takeweapon( self get_player_lethal_grenade() );
		}
		if ( self is_player_tactical_grenade( weapons_on_player[i] ) )
		{
			level.lsgsar_has_tactical_nade = 1;
			level.lsgsar_tactical = self get_player_tactical_grenade();
			level.lsgsar_tactical_nade_amt = self getweaponammoclip( self get_player_tactical_grenade() );
			self setweaponammoclip( self get_player_tactical_grenade(), 0 );
			self takeweapon( self get_player_tactical_grenade() );
		}
	}
	self waittill( "player_revived" );
	if ( level.lsgsar_has_lethal_nade )
	{
		self set_player_lethal_grenade( level.lsgsar_lethal );
		self giveweapon( level.lsgsar_lethal );
		self setweaponammoclip( level.lsgsar_lethal, level.lsgsar_lethal_nade_amt );
	}
	if ( level.lsgsar_has_tactical_nade )
	{
		self set_player_tactical_grenade( level.lsgsar_tactical );
		self giveweapon( level.lsgsar_tactical );
		self setweaponammoclip( level.lsgsar_tactical, level.lsgsar_tactical_nade_amt );
	}
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xCE78
spectators_respawn()
{
	level endon( "between_round_over" );
	if ( !(!(IsDefined( level.zombie_vars["spectators_respawn"] ))) )
	{
	}
	if ( !(level.zombie_vars["spectators_respawn"]) )
	{
		return;
	}
	if ( !(IsDefined( level.custom_spawnplayer )) )
	{
		level.custom_spawnplayer = ::spectator_respawn;
	}
	if ( 1 )
	{
		players = get_players();
		i = 0;
		if ( i < players.size )
		{
			if ( level.sessionstate == "spectator" )
			{
			}
			if ( IsDefined( level.spectator_respawn ) )
			{
					/* Error: unknown opcode (0x31) */
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xCF7C
spectator_respawn()
{
	println( "*************************Respawn Spectator***" );
	assert( IsDefined( level.spectator_respawn ) );
	origin = level.origin;
	angles = level.angles;
	self setspectatepermissions( 0 );
	new_origin = undefined;
	if ( IsDefined( level.check_valid_spawn_override ) )
	{
			/* Error: unknown opcode (0x2F) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0xD13C
check_for_valid_spawn_near_team( revivee, return_struct )
{
	if ( IsDefined( level.check_for_valid_spawn_near_team_callback ) )
	{
			/* Error: unknown opcode (0x2F) */
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xD2B8
get_valid_spawn_location( revivee, spawn_points, closest_group, return_struct )
{
	spawn_array = getstructarray( level.target, "targetname" );
	spawn_array = array_randomize( spawn_array );
	k = 0;
	if ( k < spawn_array.size )
	{
		if ( IsDefined( level.plyr ) )
		{
		}
		if ( level.plyr == revivee getentitynumber() )
		{
			if ( positionwouldtelefrag( level.origin ) )
			{
			}
			if ( IsDefined( return_struct ) )
			{
			}
			if ( return_struct )
			{
				return spawn_array[k];
			}
			return level.origin;
		}
	}
	k = 0;
	if ( k < spawn_array.size )
	{
		if ( positionwouldtelefrag( level.origin ) )
		{
		}
		if ( !(!(IsDefined( level.plyr ))) )
		{
		}
		if ( level.plyr == revivee getentitynumber() )
		{
			level.plyr = revivee getentitynumber();
			if ( IsDefined( return_struct ) )
			{
			}
			if ( return_struct )
			{
				return spawn_array[k];
			}
			return level.origin;
		}
	}
	if ( IsDefined( return_struct ) )
	{
	}
	if ( return_struct )
	{
		return spawn_array[0];
	}
	return level.origin;
} // SP = 0x0 - check OK

// 0xD41C
check_for_valid_spawn_near_position( revivee, v_position, return_struct )
{
	spawn_points = get_player_spawns_for_gametype();
	if ( spawn_points.size == 0 )
	{
		return undefined;
	}
	closest_group = undefined;
	closest_distance = 100000000;
	backup_group = undefined;
	backup_distance = 100000000;
	i = 0;
	if ( i < spawn_points.size )
	{
		if ( IsDefined( level.script_int ) )
		{
			ideal_distance = level.script_int;
		}
		ideal_distance = 1000;
		if ( level.locked == 0 )
		{
			dist = distancesquared( v_position, level.origin );
			if ( dist < ideal_distance * ideal_distance )
			{
				if ( dist < closest_distance )
				{
					closest_distance = dist;
					closest_group = i;
				}
			}
			if ( dist < backup_distance )
			{
				backup_group = i;
				backup_distance = dist;
			}
			if ( !(IsDefined( closest_group )) )
			{
				closest_group = backup_group;
			}
			if ( IsDefined( closest_group ) )
			{
				spawn_location = get_valid_spawn_location( revivee, spawn_points, closest_group, return_struct );
				if ( IsDefined( spawn_location ) )
				{
					return spawn_location;
				}
				return undefined;
} // SP = 0x0 - check OK

// 0xD530
check_for_valid_spawn_within_range( revivee, v_position, return_struct, min_distance, max_distance )
{
	spawn_points = get_player_spawns_for_gametype();
	if ( spawn_points.size == 0 )
	{
		return undefined;
	}
	closest_group = undefined;
	closest_distance = 100000000;
	i = 0;
	if ( i < spawn_points.size )
	{
		if ( level.locked == 0 )
		{
			dist = distance( v_position, level.origin );
			if ( dist >= min_distance )
			{
			}
			if ( dist <= max_distance )
			{
				if ( dist < closest_distance )
				{
					closest_distance = dist;
					closest_group = i;
				}
				if ( IsDefined( closest_group ) )
				{
					spawn_location = get_valid_spawn_location( revivee, spawn_points, closest_group, return_struct );
					if ( IsDefined( spawn_location ) )
					{
						return spawn_location;
					}
					return undefined;
} // SP = 0x0 - check OK

// 0xD5F4
get_players_on_team( exclude )
{
	teammates = [];
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		if ( level.spawn_side == level.spawn_side )
		{
		}
		if ( !(IsDefined( level.revivetrigger )) )
		{
		}
		if ( players[i] != exclude )
		{
			teammates[teammates.size] = players[i];
		}
	}
	return teammates;
} // SP = 0x0 - check OK

// 0xD664
get_safe_breadcrumb_pos( player )
{
	players = get_players();
	valid_players = [];
	min_dist = 22500;
	i = 0;
	if ( i < players.size )
	{
		if ( !(is_player_valid( players[i] )) )
		{
		}
		valid_players[valid_players.size] = players[i];
	}
	i = 0;
	if ( i < valid_players.size )
	{
		count = 0;
		q = 1;
		if ( q < level.zombie_breadcrumbs.size )
		{
			if ( distancesquared( level.zombie_breadcrumbs[q], level.origin ) < min_dist )
			{
			}
			if ( count == valid_players.size )
			{
				return level.zombie_breadcrumbs[q];
			}
		}
	}
	return undefined;
} // SP = 0x0 - check OK

// 0xD734
default_max_zombie_func( max_num )
{
	max = max_num;
	if ( level.round_number < 2 )
	{
		max = int( max_num * 0.25 );
	}
	if ( level.round_number < 3 )
	{
		max = int( max_num * 0.3 );
	}
	if ( level.round_number < 4 )
	{
		max = int( max_num * 0.5 );
	}
	if ( level.round_number < 5 )
	{
		max = int( max_num * 0.7 );
	}
	if ( level.round_number < 6 )
	{
		max = int( max_num * 0.9 );
	}
	return max;
} // SP = 0x0 - check OK

// 0xD7E8
round_spawning()
{
	level endon( "intermission" );
	level endon( "end_of_round" );
	level endon( "restart_round" );
	level endon( "kill_round" );
	if ( level.intermission )
	{
		return;
	}
	if ( !(GetDvarInt( #"0xFA81816F" ) == 2) )
	{
	}
	if ( GetDvarInt( #"0xFA81816F" ) >= 4 )
	{
		return;
	}
	if ( level.zombie_spawn_locations.size < 1 )
	{
		assertmsg( "No active spawners in the map.  Check to see if the zone is active and if it's pointing to spawners." );
		return;
	}
	ai_calculate_health( level.round_number );
	count = 0;
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		level.zombification_time = 0;
	}
	max = level.zombie_vars["zombie_max_ai"];
	multiplier = level.round_number / 5;
	if ( multiplier < 1 )
	{
		multiplier = 1;
	}
	if ( level.round_number >= 10 )
	{
		multiplier = multiplier * (level.round_number * 0.15);
	}
	player_num = get_players().size;
	if ( player_num == 1 )
	{
		max = max + int( 0.5 * level.zombie_vars["zombie_ai_per_player"] * multiplier );
	}
	max = max + int( (player_num - 1) * level.zombie_vars["zombie_ai_per_player"] * multiplier );
	if ( !(IsDefined( level.max_zombie_func )) )
	{
		level.max_zombie_func = ::default_max_zombie_func;
	}
	if ( IsDefined( level.kill_counter_hud ) )
	{
	}
	if ( !(level.zombie_total > 0) )
	{
			/* Error: unknown opcode (0x2F) */
} // SP = 0x1C - check failed (function may have been decompiled incorrectly)

// 0xDD6C
run_custom_ai_spawn_checks()
{
	_a3869 = level.custom_ai_spawn_check_funcs;
	str_id = GetFirstArrayKey( _a3869 );
	if ( IsDefined( str_id ) )
	{
		s = _a3869[str_id];
			/* Error: unknown opcode (0x2F) */
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xDFA4
register_custom_ai_spawn_check( str_id, func_check, func_get_spawners, func_get_locations )
{
	if ( !(IsDefined( level.custom_ai_spawn_check_funcs[str_id] )) )
	{
		level.custom_ai_spawn_check_funcs[str_id] = spawnstruct();
	}
	level.func_check = func_check;
	level.func_get_spawners = func_get_spawners;
	level.func_get_locations = func_get_locations;
} // SP = 0x0 - check OK

// 0xE000
zombie_speed_up()
{
	if ( level.round_number <= 3 )
	{
		return;
	}
	level endon( "intermission" );
	level endon( "end_of_round" );
	level endon( "restart_round" );
	level endon( "kill_round" );
	if ( level.zombie_total > 4 )
	{
	}
	num_zombies = get_current_zombie_count();
	if ( num_zombies > 3 )
	{
		num_zombies = get_current_zombie_count();
	}
	zombies = get_round_enemy_array();
	if ( zombies.size > 0 )
	{
		if ( zombies.size == 1 )
		{
			if ( IsDefined( level.has_legs ) )
			{
			}
			if ( level.has_legs )
			{
				if ( IsDefined( level.zombie_speed_up ) )
				{
						/* Error: unknown opcode (0x35) */
} // SP = 0x24 - check failed (function may have been decompiled incorrectly)

// 0xE104
round_spawning_test()
{
	if ( 1 )
	{
		spawn_point = level.zombie_spawn_locations[randomint( level.zombie_spawn_locations.size )];
		spawner = random( level.zombie_spawners );
		ai = spawn_zombie( spawner, level.targetname, spawn_point );
		ai waittill( "death" );
	}
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xE160
round_pause( delay )
{
	if ( !(IsDefined( delay )) )
	{
		delay = 30;
	}
	level.countdown_hud = create_counter_hud();
	level.countdown_hud setvalue( delay );
		/* Error: unknown opcode (0x5E) */
} // SP = 0x0 - check OK

// 0xE284
round_start()
{
	println( "ZM >> round_start start" );
	if ( IsDefined( level.round_prestart_func ) )
	{
			/* Error: unknown opcode (0x2F) */
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xE384
play_door_dialog()
{
	level endon( "power_on" );
	self endon( "warning_dialog" );
	timer = 0;
	if ( 1 )
	{
		players = get_players();
		i = 0;
		if ( i < players.size )
		{
			dist = distancesquared( level.origin, level.origin );
			if ( dist > 4900 )
			{
				timer = 0;
			}
			if ( dist < 4900 )
			{
			}
			if ( timer < 3 )
			{
			}
			if ( dist > 4900 )
			{
			}
			if ( timer >= 3 )
			{
				self playsound( "door_deny" );
				players[i] create_and_play_dialog( "general", "door_deny" );
			}
		}
	}
} // SP = 0x20 - check failed (function may have been decompiled incorrectly)

// 0xE464
wait_until_first_player()
{
	players = get_players();
	if ( !(IsDefined( players[0] )) )
	{
		level waittill( "first_player_ready" );
	}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xE488
round_one_up()
{
	level endon( "end_game" );
	if ( IsDefined( level.noroundnumber ) )
	{
	}
	if ( level.noroundnumber == 1 )
	{
		return;
	}
	if ( !(IsDefined( level.doground_nomusic )) )
	{
		level.doground_nomusic = 0;
	}
	if ( level.first_round )
	{
		intro = 1;
		if ( IsDefined( level._custom_intro_vox ) )
		{
				/* Error: unknown opcode (0x35) */
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xE598
round_over()
{
	if ( IsDefined( level.noroundnumber ) )
	{
	}
	if ( level.noroundnumber == 1 )
	{
		return;
	}
	time = level.zombie_vars["zombie_between_round_time"];
	players = getplayers();
	player_index = 0;
	if ( player_index < players.size )
	{
		if ( !(IsDefined( level.pers["previous_distance_traveled"] )) )
		{
			level.pers["previous_distance_traveled"] = 0;
		}
		distancethisround = int( level.pers["distance_traveled"] - level.pers["previous_distance_traveled"] );
		level.pers["previous_distance_traveled"] = level.pers["distance_traveled"];
		players[player_index] incrementplayerstat( "distance_traveled", distancethisround );
		if ( level.pers["team"] != "spectator" )
		{
			zonename = players[player_index] get_current_zone();
			if ( IsDefined( zonename ) )
			{
				players[player_index] recordzombiezone( "endingZone", zonename );
			}
			recordzombieroundend();
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xE6B4
round_think( restart )
{
	println( "ZM >> round_think start" );
	level endon( "end_round_think" );
	if ( IsDefined( restart ) )
	{
	}
	if ( !(restart) )
	{
		if ( IsDefined( level.initial_round_wait_func ) )
		{
				/* Error: unknown opcode (0x2F) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0xEAE0
award_grenades_for_survivors()
{
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		if ( !(level.is_zombie) )
		{
			lethal_grenade = players[i] get_player_lethal_grenade();
			if ( !(players[i] hasweapon( lethal_grenade )) )
			{
				players[i] giveweapon( lethal_grenade );
				players[i] setweaponammoclip( lethal_grenade, 0 );
			}
			if ( players[i] getfractionmaxammo( lethal_grenade ) < 0.25 )
			{
				players[i] setweaponammoclip( lethal_grenade, 2 );
			}
			if ( players[i] getfractionmaxammo( lethal_grenade ) < 0.5 )
			{
				players[i] setweaponammoclip( lethal_grenade, 3 );
			}
			players[i] setweaponammoclip( lethal_grenade, 4 );
		}
	}
} // SP = 0x0 - check OK

// 0xEBD4
ai_calculate_health( round_number )
{
	level.zombie_health = level.zombie_vars["zombie_health_start"];
	i = 2;
	if ( i <= round_number )
	{
		if ( i >= 10 )
		{
			old_health = level.zombie_health;
			level.zombie_health = level.zombie_health + int( level.zombie_health * level.zombie_vars["zombie_health_increase_multiplier"] );
			if ( level.zombie_health < old_health )
			{
				level.zombie_health = old_health;
				return;
			}
		}
		level.zombie_health = int( level.zombie_health + level.zombie_vars["zombie_health_increase"] );
	}
} // SP = 0x0 - check OK

// 0xEC6C
ai_zombie_health( round_number )
{
	zombie_health = level.zombie_vars["zombie_health_start"];
	i = 2;
	if ( i <= round_number )
	{
		if ( i >= 10 )
		{
			old_health = zombie_health;
			zombie_health = zombie_health + int( zombie_health * level.zombie_vars["zombie_health_increase_multiplier"] );
			if ( zombie_health < old_health )
			{
				return old_health;
			}
		}
		zombie_health = int( zombie_health + level.zombie_vars["zombie_health_increase"] );
	}
	return zombie_health;
} // SP = 0x0 - check OK

// 0xECEC
round_spawn_failsafe_debug()
{
	level endon( "failsafe_debug_stop" );
	start = GetTime();
	level.chunk_time = 0;
	if ( 1 )
	{
		level.failsafe_time = GetTime() - start;
		if ( IsDefined( level.lastchunk_destroy_time ) )
		{
			level.chunk_time = GetTime() - level.lastchunk_destroy_time;
		}
		wait_network_frame();
	}
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xED44
round_spawn_failsafe()
{
	self endon( "death" );
	prevorigin = level.origin;
	if ( 1 )
	{
		if ( !(level.zombie_vars["zombie_use_failsafe"]) )
		{
			return;
		}
		if ( IsDefined( level.ignore_round_spawn_failsafe ) )
		{
		}
		if ( level.ignore_round_spawn_failsafe )
		{
			return;
		}
		if ( !(level.has_legs) )
		{
		}
		if ( IsDefined( level.is_inert ) )
		{
		}
		if ( level.is_inert )
		{
		}
		if ( IsDefined( level.lastchunk_destroy_time ) )
		{
			if ( GetTime() - level.lastchunk_destroy_time < 8000 )
			{
			}
			if ( level.origin[2] < level.zombie_vars["below_world_check"] )
			{
				if ( IsDefined( level.put_timed_out_zombies_back_in_queue ) )
				{
				}
				if ( level.put_timed_out_zombies_back_in_queue )
				{
				}
				if ( !(flag( "dog_round" )) )
				{
					if ( IsDefined( level.isscreecher ) )
					{
					}
				}
				if ( !(level.isscreecher) )
				{
				}
					/* Error: unknown opcode (0x5E) */
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xEEE0
round_wait()
{
	level endon( "restart_round" );
	if ( GetDvarInt( #"0x7688603C" ) )
	{
		level waittill( "forever" );
	}
	if ( !(GetDvarInt( #"0xFA81816F" ) == 2) )
	{
	}
	if ( GetDvarInt( #"0xFA81816F" ) >= 4 )
	{
		level waittill( "forever" );
	}
	if ( flag( "dog_round" ) )
	{
		if ( level.dog_intermission )
		{
		}
		increment_dog_round_stat( "finished" );
	}
	if ( 1 )
	{
		should_wait = 0;
		if ( IsDefined( level.is_ghost_round_started ) )
		{
				/* Error: unknown opcode (0x2F) */
} // SP = 0x20 - check failed (function may have been decompiled incorrectly)

// 0xEFD0
zombify_player()
{
	self player_died_penalty();
	bbprint( "zombie_playerdeaths", "round %d playername %s deathtype %s x %f y %f z %f", level.round_number, level.name, "died", level.origin );
	self recordplayerdeathzombies();
	if ( IsDefined( level.deathcard_spawn_func ) )
	{
			/* Error: unknown opcode (0x31) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0xF11C
playerzombie_player_damage()
{
	self endon( "death" );
	self endon( "disconnect" );
	self thread playerzombie_infinite_health();
	level.zombiehealth = level.zombie_health;
	if ( 1 )
	{
		self waittill( "damage" );
		if ( !(!(IsDefined( attacker ))) )
		{
		}
		if ( !(isplayer( attacker )) )
		{
		}
		level.zombiehealth = level.zombiehealth - amount;
		if ( level.zombiehealth <= 0 )
		{
			self thread playerzombie_downed_state();
			self waittill( "playerzombie_downed_state_done" );
			level.zombiehealth = level.zombie_health;
		}
	}
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0xF1BC
playerzombie_downed_state()
{
	self endon( "death" );
	self endon( "disconnect" );
	downtime = 15;
	starttime = GetTime();
	endtime = starttime + downtime * 1000;
	self thread playerzombie_downed_hud();
	level.playerzombie_soundboard_disable = 1;
	self thread zombie_eye_glow_stop();
	self disableweapons();
	self allowstand( 0 );
	self allowcrouch( 0 );
	self allowprone( 1 );
	if ( GetTime() < endtime )
	{
	}
	level.playerzombie_soundboard_disable = 0;
	self thread zombie_eye_glow();
	self enableweapons();
	self allowstand( 1 );
	self allowcrouch( 0 );
	self allowprone( 0 );
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0xF298
playerzombie_downed_hud()
{
	self endon( "death" );
	self endon( "disconnect" );
	text = newclienthudelem( self );
	level.alignx = "center";
	level.aligny = "middle";
	level.horzalign = "user_center";
	level.vertalign = "user_bottom";
	level.foreground = 1;
	level.font = "default";
	level.fontscale = 1.8;
	level.alpha = 0;
		/* Error: unknown opcode (0x5E) */
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0xF37C
playerzombie_infinite_health()
{
	self endon( "death" );
	self endon( "disconnect" );
	bighealth = 100000;
	if ( 1 )
	{
		if ( level.health < bighealth )
		{
			level.health = bighealth;
		}
	}
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0xF3C0
playerzombie_soundboard()
{
	self endon( "death" );
	self endon( "disconnect" );
	level.playerzombie_soundboard_disable = 0;
	level.buttonpressed_use = 0;
	level.buttonpressed_attack = 0;
	level.buttonpressed_ads = 0;
	level.usesound_waittime = 3000;
	level.usesound_nexttime = GetTime();
	usesound = "playerzombie_usebutton_sound";
	level.attacksound_waittime = 3000;
	level.attacksound_nexttime = GetTime();
	attacksound = "playerzombie_attackbutton_sound";
	level.adssound_waittime = 3000;
	level.adssound_nexttime = GetTime();
	adssound = "playerzombie_adsbutton_sound";
	level.inputsound_nexttime = GetTime();
	if ( 1 )
	{
		if ( level.playerzombie_soundboard_disable )
		{
		}
		if ( self usebuttonpressed() )
		{
			if ( self can_do_input( "use" ) )
			{
				self thread playerzombie_play_sound( usesound );
				self thread playerzombie_waitfor_buttonrelease( "use" );
				level.usesound_nexttime = GetTime() + level.usesound_waittime;
			}
		}
		if ( self attackbuttonpressed() )
		{
			if ( self can_do_input( "attack" ) )
			{
				self thread playerzombie_play_sound( attacksound );
				self thread playerzombie_waitfor_buttonrelease( "attack" );
				level.attacksound_nexttime = GetTime() + level.attacksound_waittime;
			}
		}
		if ( self adsbuttonpressed() )
		{
			if ( self can_do_input( "ads" ) )
			{
				self thread playerzombie_play_sound( adssound );
				self thread playerzombie_waitfor_buttonrelease( "ads" );
				level.adssound_nexttime = GetTime() + level.adssound_waittime;
			}
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0xF530
can_do_input( inputtype )
{
	if ( GetTime() < level.inputsound_nexttime )
	{
		return 0;
	}
	cando = 0;
		/* Error: unknown opcode (0x59) */
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xF5E4
playerzombie_play_sound( alias )
{
	self play_sound_on_ent( alias );
} // SP = 0x0 - check OK

// 0xF5FC
playerzombie_waitfor_buttonrelease( inputtype )
{
	if ( inputtype != "use" )
	{
	}
	if ( inputtype != "attack" )
	{
	}
	if ( inputtype != "ads" )
	{
		assertmsg( "playerzombie_waitfor_buttonrelease(): inputType of " + inputtype + " is not recognized." );
		return;
	}
	notifystring = "waitfor_buttonrelease_" + inputtype;
	self endon( notifystring );
	if ( inputtype == "use" )
	{
		level.buttonpressed_use = 1;
		if ( self usebuttonpressed() )
		{
		}
		level.buttonpressed_use = 0;
	}
	if ( inputtype == "attack" )
	{
		level.buttonpressed_attack = 1;
		if ( self attackbuttonpressed() )
		{
		}
		level.buttonpressed_attack = 0;
	}
	if ( inputtype == "ads" )
	{
		level.buttonpressed_ads = 1;
		if ( self adsbuttonpressed() )
		{
		}
		level.buttonpressed_ads = 0;
	}
} // SP = 0x1C - check failed (function may have been decompiled incorrectly)

// 0xF6EC
remove_ignore_attacker()
{
	self endon( "new_ignore_attacker" );
	self endon( "disconnect" );
	if ( !(IsDefined( level.ignore_enemy_timer )) )
	{
		level.ignore_enemy_timer = 0.4;
	}
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0xF728
player_shield_facing_attacker( vdir, limit )
{
	orientation = self getplayerangles();
		/* Error: unknown opcode (0x63) */
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0xF7AC
player_damage_override_cheat( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime )
{
	player_damage_override( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime );
	return 0;
} // SP = 0x0 - check OK

// 0xF7E8
player_damage_override( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime )
{
	if ( IsDefined( level._game_module_player_damage_callback ) )
	{
			/* Error: unknown opcode (0x31) */
} // SP = 0x34 - check failed (function may have been decompiled incorrectly)

// 0x10018
clear_path_timers()
{
	zombies = getaiarray( level.zombie_team );
	_a5483 = zombies;
	_k5483 = GetFirstArrayKey( _a5483 );
	if ( IsDefined( _k5483 ) )
	{
		zombie = _a5483[_k5483];
		if ( IsDefined( level.favoriteenemy ) )
		{
		}
		if ( level.favoriteenemy == self )
		{
			level.zombie_path_timer = 0;
		}
		_k5483 = GetNextArrayKey( _a5483, _k5483 );
	}
} // SP = 0x0 - check OK

// 0x1007C
check_player_damage_callbacks( einflictor, eattacker, idamage, idflags, smeansofdeath, sweapon, vpoint, vdir, shitloc, psoffsettime )
{
	if ( !(IsDefined( level.player_damage_callbacks )) )
	{
		return idamage;
	}
	i = 0;
	if ( i < level.player_damage_callbacks.size )
	{
			/* Error: unknown opcode (0x31) */
} // SP = 0x34 - check failed (function may have been decompiled incorrectly)

// 0x100F0
register_player_damage_callback( func )
{
	if ( !(IsDefined( level.player_damage_callbacks )) )
	{
		level.player_damage_callbacks = [];
	}
	level.player_damage_callbacks[level.player_damage_callbacks.size] = func;
} // SP = 0x0 - check OK

// 0x10118
wait_and_revive()
{
	flag_set( "wait_and_revive" );
	if ( IsDefined( level.waiting_to_revive ) )
	{
	}
	if ( level.waiting_to_revive == 1 )
	{
		return;
	}
	level.waiting_to_revive = 1;
	if ( IsDefined( level.exit_level_func ) )
	{
			/* Error: unknown opcode (0x35) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0x101F8
actor_damage_override( inflictor, attacker, damage, flags, meansofdeath, weapon, vpoint, vdir, shitloc, psoffsettime )
{
	if ( !(!(IsDefined( self ))) )
	{
	}
	if ( !(IsDefined( attacker )) )
	{
		return damage;
	}
	if ( !(weapon == "tazer_knuckles_zm") )
	{
	}
	if ( weapon == "jetgun_zm" )
	{
		level.knuckles_extinguish_flames = 1;
	}
	if ( weapon != "none" )
	{
	}
	if ( IsDefined( level.animname ) )
	{
	}
	if ( level.animname == "quad_zombie" )
	{
		if ( IsDefined( level.animname ) )
		{
		}
		if ( level.animname == "quad_zombie" )
		{
			return 0;
		}
		if ( !(isplayer( attacker )) )
		{
		}
		if ( IsDefined( level.non_attacker_func ) )
		{
				/* Error: unknown opcode (0x31) */
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0x10464
actor_damage_override_wrapper( inflictor, attacker, damage, flags, meansofdeath, weapon, vpoint, vdir, shitloc, psoffsettime, boneindex )
{
	damage_override = self actor_damage_override( inflictor, attacker, damage, flags, meansofdeath, weapon, vpoint, vdir, shitloc, psoffsettime );
	if ( !(damage < level.health) )
	{
		if ( IsDefined( level.dont_die_on_me ) )
		{
		}
	}
	if ( !(level.dont_die_on_me) )
	{
		self finishactordamage( inflictor, attacker, damage_override, flags, meansofdeath, weapon, vpoint, vdir, shitloc, psoffsettime, boneindex );
	}
} // SP = 0x0 - check OK

// 0x104E0
actor_killed_override( einflictor, attacker, idamage, smeansofdeath, sweapon, vdir, shitloc, psoffsettime )
{
	if ( game["state"] == "postgame" )
	{
		return;
	}
	if ( isai( attacker ) )
	{
	}
	if ( IsDefined( level.script_owner ) )
	{
		if ( level.team != level.aiteam )
		{
			attacker = level.script_owner;
		}
		if ( level.classname == "script_vehicle" )
		{
		}
		if ( IsDefined( level.owner ) )
		{
			attacker = level.owner;
		}
		if ( IsDefined( attacker ) )
		{
		}
		if ( isplayer( attacker ) )
		{
			multiplier = 1;
			if ( is_headshot( sweapon, shitloc, smeansofdeath ) )
			{
				multiplier = 1.5;
			}
			type = undefined;
			if ( IsDefined( level.animname ) )
			{
					/* Error: unknown opcode (0x59) */
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x10638
round_end_monitor()
{
	if ( 1 )
	{
		level waittill( "end_of_round" );
		bookmark( "zm_round_end", GetTime(), undefined, undefined, 1 );
		bbpostdemostreamstatsforround( level.round_number );
	}
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x10674
end_game()
{
	level waittill( "end_game" );
	check_end_game_intermission_delay();
	println( "end_game TRIGGERED " );
	clientnotify( "zesn" );
	if ( IsDefined( level.sndgameovermusicoverride ) )
	{
		level thread change_zombie_music( level.sndgameovermusicoverride );
	}
	level thread change_zombie_music( "game_over" );
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		setclientsysstate( "lsm", "0", players[i] );
	}
	i = 0;
	if ( i < players.size )
	{
		if ( players[i] player_is_in_laststand() )
		{
			players[i] recordplayerdeathzombies();
			players[i] increment_player_stat( "deaths" );
			players[i] increment_client_stat( "deaths" );
			players[i] pers_upgrade_jugg_player_death_stat();
		}
		if ( IsDefined( level.revivetexthud ) )
		{
			level.revivetexthud destroy();
		}
	}
	stopallrumbles();
	level.intermission = 1;
	level.zombie_vars["zombie_powerup_insta_kill_time"] = 0;
	level.zombie_vars["zombie_powerup_fire_sale_time"] = 0;
	level.zombie_vars["zombie_powerup_point_doubler_time"] = 0;
	game_over = [];
	survived = [];
	players = get_players();
	setmatchflag( "disableIngameMenu", 1 );
	_a5802 = players;
	_k5802 = GetFirstArrayKey( _a5802 );
	if ( IsDefined( _k5802 ) )
	{
		player = _a5802[_k5802];
		player closemenu();
		player closeingamemenu();
		_k5802 = GetNextArrayKey( _a5802, _k5802 );
	}
	if ( !(IsDefined( level._supress_survived_screen )) )
	{
		i = 0;
		if ( i < players.size )
		{
			if ( IsDefined( level.custom_game_over_hud_elem ) )
			{
					/* Error: unknown opcode (0x2F) */
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0x10D08
disable_end_game_intermission( delay )
{
	level.disable_intermission = 1;
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x10D20
check_end_game_intermission_delay()
{
	if ( IsDefined( level.disable_intermission ) )
	{
		if ( 1 )
		{
			if ( !(IsDefined( level.disable_intermission )) )
			{
			}
		}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x10D50
upload_leaderboards()
{
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		players[i] uploadleaderboards();
	}
} // SP = 0x0 - check OK

// 0x10D8C
initializestattracking()
{
	level.global_zombies_killed = 0;
	level.zombies_timeout_spawn = 0;
	level.zombies_timeout_playspace = 0;
	level.zombies_timeout_undamaged = 0;
	level.zombie_player_killed_count = 0;
	level.zombie_trap_killed_count = 0;
	level.zombie_pathing_failed = 0;
	level.zombie_breadcrumb_failed = 0;
} // SP = 0x0 - check OK

// 0x10DC4
uploadglobalstatcounters()
{
	incrementcounter( "global_zombies_killed", level.global_zombies_killed );
	incrementcounter( "global_zombies_killed_by_players", level.zombie_player_killed_count );
	incrementcounter( "global_zombies_killed_by_traps", level.zombie_trap_killed_count );
} // SP = 0x0 - check OK

// 0x10DFC
player_fake_death()
{
	self takeallweapons();
	self allowstand( 0 );
	self allowcrouch( 0 );
	self allowprone( 1 );
	level.ignoreme = 1;
	self enableinvulnerability();
	self freezecontrols( 1 );
} // SP = 0x1C - check failed (function may have been decompiled incorrectly)

// 0x10E64
player_exit_level()
{
	self allowstand( 1 );
	self allowcrouch( 0 );
	self allowprone( 0 );
	if ( IsDefined( level.game_over_bg ) )
	{
		level.foreground = 1;
		level.sort = 100;
		level.game_over_bg fadeovertime( 1 );
		level.alpha = 1;
	}
} // SP = 0x0 - check OK

// 0x10ECC
player_killed_override( einflictor, attacker, idamage, smeansofdeath, sweapon, vdir, shitloc, psoffsettime, deathanimduration )
{
	level waittill( "forever" );
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x10EEC
player_zombie_breadcrumb()
{
	self endon( "stop_player_zombie_breadcrumb" );
	self endon( "disconnect" );
	self endon( "spawned_spectator" );
	level endon( "intermission" );
	level.zombie_breadcrumbs = [];
	level.zombie_breadcrumb_distance = 576;
	level.zombie_breadcrumb_area_num = 3;
	level.zombie_breadcrumb_area_distance = 16;
	self store_crumb( level.origin );
	last_crumb = level.origin;
	self thread debug_breadcrumbs();
	if ( 1 )
	{
		wait_time = 0.1;
		if ( level.ignoreme )
		{
		}
		store_crumb = 1;
		airborne = 0;
		crumb = level.origin;
		if ( !(self isonground()) )
		{
		}
		if ( self isinvehicle() )
		{
				/* Error: unknown opcode (0x5E) */
} // SP = 0x38 - check failed (function may have been decompiled incorrectly)

// 0x1107C
store_crumb( origin )
{
	offsets = [];
	height_offset = 32;
	index = 0;
	j = 1;
	if ( j <= level.zombie_breadcrumb_area_num )
	{
		offset = j * level.zombie_breadcrumb_area_distance;
		offsets[0] = ( origin[0] - offset, origin[1], origin[2] );
		offsets[1] = ( origin[0] + offset, origin[1], origin[2] );
		offsets[2] = ( origin[0], origin[1] - offset, origin[2] );
		offsets[3] = ( origin[0], origin[1] + offset, origin[2] );
		offsets[4] = ( origin[0] - offset, origin[1], origin[2] + height_offset );
		offsets[5] = ( origin[0] + offset, origin[1], origin[2] + height_offset );
		offsets[6] = ( origin[0], origin[1] - offset, origin[2] + height_offset );
		offsets[7] = ( origin[0], origin[1] + offset, origin[2] + height_offset );
		i = 0;
		if ( i < offsets.size )
		{
			level.zombie_breadcrumbs[index] = offsets[i];
		}
	}
} // SP = 0x0 - check OK

// 0x111B0
to_mins( seconds )
{
	hours = 0;
	minutes = 0;
	if ( seconds > 59 )
	{
		minutes = int( seconds / 60 );
		seconds = int( seconds * 1000 ) %% 60000;
		seconds = seconds * 0.001;
		if ( minutes > 59 )
		{
			hours = int( minutes / 60 );
			minutes = int( minutes * 1000 ) %% 60000;
			minutes = minutes * 0.001;
		}
		if ( hours < 10 )
		{
			hours = "0" + hours;
		}
		if ( minutes < 10 )
		{
			minutes = "0" + minutes;
		}
		seconds = int( seconds );
		if ( seconds < 10 )
		{
			seconds = "0" + seconds;
		}
		combined = "" + hours + ":" + minutes + ":" + seconds;
		return combined;
} // SP = 0x0 - check OK

// 0x112AC
intermission()
{
	level.intermission = 1;
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		setclientsysstate( "levelNotify", "zi", players[i] );
		players[i] setclientthirdperson( 0 );
		players[i] resetfov();
		level.health = 100;
			/* Error: unknown opcode (0x35) */
} // SP = 0x18 - check failed (function may have been decompiled incorrectly)

// 0x11388
zombie_game_over_death()
{
	zombies = getaiarray( level.zombie_team );
	i = 0;
	if ( i < zombies.size )
	{
		if ( !(isalive( zombies[i] )) )
		{
		}
		zombies[i] setgoalpos( level.origin );
	}
	i = 0;
	if ( i < zombies.size )
	{
		if ( !(isalive( zombies[i] )) )
		{
		}
		if ( IsDefined( level.ignore_game_over_death ) )
		{
		}
		if ( level.ignore_game_over_death )
		{
		}
		if ( IsDefined( zombies[i] ) )
		{
			zombies[i] zombie_head_gib();
			zombies[i] dodamage( level.health + 666, level.origin );
		}
	}
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x11488
player_intermission()
{
	self closemenu();
	self closeingamemenu();
	level endon( "stop_intermission" );
	self endon( "disconnect" );
	self endon( "death" );
	level.score = level.score_total;
	level.sessionstate = "intermission";
	level.spectatorclient = -1;
	level.killcamentity = -1;
	level.archivetime = 0;
	level.psoffsettime = 0;
	points = getstructarray( "intermission", "targetname" );
	if ( !(!(IsDefined( points ))) )
	{
	}
	if ( points.size == 0 )
	{
		points = getentarray( "info_intermission", "classname" );
		if ( points.size < 1 )
		{
			println( "NO info_intermission POINTS IN MAP" );
			return;
		}
		level.game_over_bg = newclienthudelem( self );
		level.horzalign = "fullscreen";
		level.vertalign = "fullscreen";
		level.game_over_bg setshader( "black", 640, 480 );
		level.alpha = 1;
		org = undefined;
		if ( 1 )
		{
			points = array_randomize( points );
			i = 0;
			if ( i < points.size )
			{
				point = points[i];
				if ( !(IsDefined( org )) )
				{
					self spawn( level.origin, level.angles );
				}
				if ( IsDefined( level.target ) )
				{
					if ( !(IsDefined( org )) )
					{
							/* Error: unknown opcode (0x5E) */
} // SP = 0x24 - check failed (function may have been decompiled incorrectly)

// 0x117B0
fade_up_over_time( t )
{
	self fadeovertime( t );
	level.alpha = 1;
} // SP = 0x0 - check OK

// 0x117D0
default_exit_level()
{
	zombies = getaiarray( level.zombie_team );
	i = 0;
	if ( i < zombies.size )
	{
		if ( IsDefined( level.ignore_solo_last_stand ) )
		{
		}
		if ( level.ignore_solo_last_stand )
		{
		}
		if ( IsDefined( level.find_exit_point ) )
		{
				/* Error: unknown opcode (0x35) */
} // SP = 0xC - check failed (function may have been decompiled incorrectly)

// 0x11874
default_delayed_exit()
{
	self endon( "death" );
	if ( 1 )
	{
		if ( !(flag( "wait_and_revive" )) )
		{
			return;
		}
		if ( !(level.ignoreme) )
		{
		}
	}
	self thread default_find_exit_point();
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x118B8
default_find_exit_point()
{
	self endon( "death" );
	player = get_players()[0];
	dist_zombie = 0;
	dist_player = 0;
	dest = 0;
	away = vectornormalize( level.origin - level.origin );
		/* Error: unknown opcode (0x60) */
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0x119F8
play_level_start_vox_delayed()
{
	players = get_players();
	num = randomintrange( 0, players.size );
	players[num] create_and_play_dialog( "general", "intro" );
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x11A38
register_sidequest( id, sidequest_stat )
{
	if ( !(IsDefined( level.zombie_sidequest_stat )) )
	{
		level.zombie_sidequest_previously_completed = [];
		level.zombie_sidequest_stat = [];
	}
	level.zombie_sidequest_stat[id] = sidequest_stat;
	flag_wait( "start_zombie_round_logic" );
	level.zombie_sidequest_previously_completed[id] = 0;
	if ( !(level.systemlink) )
	{
	}
	if ( GetDvarInt( #"0xE6FF85E6" ) == get_players().size )
	{
		return;
	}
	if ( IsDefined( level.zm_disable_recording_stats ) )
	{
	}
	if ( level.zm_disable_recording_stats )
	{
		return;
	}
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		if ( players[i] get_global_stat( level.zombie_sidequest_stat[id] ) )
		{
			level.zombie_sidequest_previously_completed[id] = 1;
			return;
		}
	}
} // SP = 0x0 - check OK

// 0x11AEC
is_sidequest_previously_completed( id )
{
	if ( IsDefined( level.zombie_sidequest_previously_completed[id] ) )
	{
	}
	return level.zombie_sidequest_previously_completed[id];
} // SP = 0x0 - check OK

// 0x11B08
set_sidequest_completed( id )
{
	level.zombie_sidequest_previously_completed[id] = 1;
	if ( level.systemlink )
	{
		return;
	}
	if ( GetDvarInt( #"0xE6FF85E6" ) == get_players().size )
	{
		return;
	}
	if ( IsDefined( level.zm_disable_recording_stats ) )
	{
	}
	if ( level.zm_disable_recording_stats )
	{
		return;
	}
	players = get_players();
	i = 0;
	if ( i < players.size )
	{
		if ( IsDefined( level.zombie_sidequest_stat[id] ) )
		{
			players[i] add_global_stat( level.zombie_sidequest_stat[id], 1 );
		}
	}
} // SP = 0x10 - check failed (function may have been decompiled incorrectly)

// 0x11B9C
playswipesound( mod, attacker )
{
	if ( IsDefined( level.is_zombie ) )
	{
	}
	if ( level.is_zombie )
	{
		self playsoundtoplayer( "evt_player_swiped", self );
		return;
	}
} // SP = 0x0 - check OK

// 0x11BCC
precache_zombie_leaderboards()
{
	if ( sessionmodeissystemlink() )
	{
		return;
	}
	globalleaderboards = "LB_ZM_GB_BULLETS_FIRED_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_BULLETS_HIT_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_DEATHS_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_DISTANCE_TRAVELED_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_DOORS_PURCHASED_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_DOWNS_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_GIBS_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_GRENADE_KILLS_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_HEADSHOTS_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_KILLS_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_PERKS_DRANK_AT ";
	globalleaderboards = globalleaderboards + "LB_ZM_GB_REVIVES_AT ";
	if ( sessionmodeisprivateonlinegame() )
	{
		precacheleaderboards( globalleaderboards );
		return;
	}
	maplocationname = level.scr_zm_map_start_location;
	if ( !(maplocationname == "default") )
	{
	}
	if ( maplocationname == "" )
	{
	}
	if ( IsDefined( level.default_start_location ) )
	{
		maplocationname = level.default_start_location;
	}
	if ( !(level.scr_zm_ui_gametype_group == "zclassic") )
	{
	}
	if ( level.scr_zm_ui_gametype_group == "zsurvival" )
	{
	}
	if ( level.scr_zm_ui_gametype != "zcleansed" )
	{
		expectedplayernum = getnumexpectedplayers();
		if ( expectedplayernum == 1 )
		{
			gamemodeleaderboard = "LB_ZM_GM_" + level.scr_zm_ui_gametype + "_" + maplocationname + "_" + expectedplayernum + "PLAYER";
		}
		gamemodeleaderboard = "LB_ZM_GM_" + level.scr_zm_ui_gametype + "_" + maplocationname + "_" + expectedplayernum + "PLAYERS";
	}
	gamemodeleaderboard = "LB_ZM_GM_" + level.scr_zm_ui_gametype + "_" + maplocationname;
	precacheleaderboards( globalleaderboards + gamemodeleaderboard );
} // SP = 0x0 - check OK

// 0x11D44
zm_on_player_connect()
{
	if ( level.passed_introscreen )
	{
		self setclientuivisibilityflag( "hud_visible", 1 );
	}
	self thread watchdisconnect();
} // SP = 0x0 - check OK

// 0x11D78
zm_on_player_disconnect()
{
} // SP = 0x0 - check OK

// 0x11D88
watchdisconnect()
{
	self endon( "watchDisconnect" );
	self waittill( "disconnect" );
	zm_on_player_disconnect();
} // SP = 0x14 - check failed (function may have been decompiled incorrectly)

// 0x11DAC
increment_dog_round_stat( stat )
{
	players = get_players();
	_a6721 = players;
	_k6721 = GetFirstArrayKey( _a6721 );
	if ( IsDefined( _k6721 ) )
	{
		player = _a6721[_k6721];
		player increment_client_stat( "zdog_rounds_" + stat );
		_k6721 = GetNextArrayKey( _a6721, _k6721 );
	}
} // SP = 0x0 - check OK

// 0x11E00
setup_player_navcard_hud()
{
	flag_wait( "start_zombie_round_logic" );
} // SP = 0x0 - check OK

// 0x11E1C
refresh_player_navcard_hud()
{
	if ( !(IsDefined( level.navcards )) )
	{
		return;
	}
	players = get_players();
	_a6739 = players;
	_k6739 = GetFirstArrayKey( _a6739 );
	if ( IsDefined( _k6739 ) )
	{
		player = _a6739[_k6739];
		navcard_bits = 0;
		i = 0;
		if ( i < level.navcards.size )
		{
			hasit = player get_global_stat( level.navcards[i] );
			if ( IsDefined( level.navcard_grabbed ) )
			{
			}
			if ( level.navcard_grabbed == level.navcards[i] )
			{
				hasit = 1;
			}
			if ( hasit )
			{
				navcard_bits = navcard_bits + (1 << i);
			}
		}
		wait_network_frame();
		player setclientfield( "navcard_held", 0 );
		if ( navcard_bits > 0 )
		{
			wait_network_frame();
			player setclientfield( "navcard_held", navcard_bits );
		}
		_k6739 = GetNextArrayKey( _a6739, _k6739 );
	}
} // SP = 0x0 - check OK

// 0x11F08
check_quickrevive_for_hotjoin( disconnecting_player )
{
	solo_mode = 0;
	subtract_num = 0;
	should_update = 0;
	if ( IsDefined( disconnecting_player ) )
	{
		subtract_num = 1;
	}
	players = get_players();
	if ( !(players.size - subtract_num == 1) )
	{
		if ( IsDefined( level.force_solo_quick_revive ) )
		{
		}
		if ( level.force_solo_quick_revive )
		{
			solo_mode = 1;
			if ( !(flag( "solo_game" )) )
			{
				should_update = 1;
			}
			flag_set( "solo_game" );
		}
		if ( flag( "solo_game" ) )
		{
			should_update = 1;
		}
		flag_clear( "solo_game" );
		level.using_solo_revive = solo_mode;
		level.revive_machine_is_solo = solo_mode;
		set_default_laststand_pistol( solo_mode );
		if ( should_update )
		{
		}
		if ( IsDefined( level.quick_revive_machine ) )
		{
			update_quick_revive( solo_mode );
		}
} // SP = 0x0 - check OK

// 0x11FD8
set_default_laststand_pistol( solo_mode )
{
	if ( !(solo_mode) )
	{
		level.laststandpistol = level.default_laststandpistol;
	}
	level.laststandpistol = level.default_solo_laststandpistol;
} // SP = 0x0 - check OK

// 0x11FFC
update_quick_revive( solo_mode )
{
	if ( !(IsDefined( solo_mode )) )
	{
		solo_mode = 0;
	}
	clip = undefined;
	if ( IsDefined( level.quick_revive_machine_clip ) )
	{
		clip = level.quick_revive_machine_clip;
	}
	level.quick_revive_machine thread reenable_quickrevive( clip, solo_mode );
} // SP = 0x0 - check OK

// 0x12038
player_too_many_players_check()
{
	max_players = 4;
	if ( !(level.scr_zm_ui_gametype == "zgrief") )
	{
	}
	if ( level.scr_zm_ui_gametype == "zmeat" )
	{
		max_players = 8;
	}
	if ( get_players().size > max_players )
	{
		freeze_players( 1 );
	}
} // SP = 0xC - check failed (function may have been decompiled incorrectly)