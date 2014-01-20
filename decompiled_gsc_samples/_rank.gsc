// maps/mp/gametypes/_rank.gsc

#include maps\mp\gametypes\_globallogic;
#include maps\mp\gametypes\_hud;
#include maps\mp\gametypes\_hud_util;
#include maps\mp\_scoreevents;
#include maps\mp\_utility;
#include common_scripts\utility;


// 0xE48
init()
{
	level.scoreinfo = [];
	level.xpscale = GetDvarFloat( #"0x12E4DD1C" );
	level.codpointsxpscale = GetDvarFloat( #"0x5DB7110F" );
	level.codpointsmatchscale = GetDvarFloat( #"0x9D07FE34" );
	level.codpointschallengescale = GetDvarFloat( #"0x4B61B909" );
	level.rankxpcap = GetDvarInt( #"0x6032F3B4" );
	level.codpointscap = GetDvarInt( #"0x8D448453" );
	level.usingmomentum = 1;
	level.usingscorestreaks = GetDvarInt( #"0x469616C5" ) != 0;
	level.scorestreaksmaxstacking = GetDvarInt( #"0x181EB11E" );
	level.maxinventoryscorestreaks = getdvarintdefault( "scr_maxinventory_scorestreaks", 3 );
	if ( !(!(IsDefined( level.usingscorestreaks ))) )
	{
	}
	level.usingrampage = !(level.usingscorestreaks);
	level.rampagebonusscale = GetDvarFloat( #"0x855FF558" );
	level.ranktable = [];
	precacheshader( "white" );
	precachestring( &"RANK_PLAYER_WAS_PROMOTED_N" );
	precachestring( &"RANK_PLAYER_WAS_PROMOTED" );
	precachestring( &"RANK_PROMOTED" );
	precachestring( &"MP_PLUS" );
	precachestring( &"RANK_ROMANI" );
	precachestring( &"RANK_ROMANII" );
	precachestring( &"MP_SCORE_KILL" );
	if ( !(sessionmodeiszombiesgame()) )
	{
		initscoreinfo();
	}
	level.maxrank = int( tablelookup( "mp/rankTable.csv", 0, "maxrank", 1 ) );
	level.maxprestige = int( tablelookup( "mp/rankIconTable.csv", 0, "maxprestige", 1 ) );
	pid = 0;
	rid = 0;
	pid = 0;
	if ( pid <= level.maxprestige )
	{
		rid = 0;
		if ( rid <= level.maxrank )
		{
			precacheshader( tablelookup( "mp/rankIconTable.csv", 0, rid, pid + 1 ) );
		}
	}
	rankid = 0;
	rankname = tablelookup( "mp/ranktable.csv", 0, rankid, 1 );
	if ( IsDefined( rankname ) )
	{
	}
	assert( rankname != "" );
	if ( IsDefined( rankname ) )
	{
	}
	if ( rankname != "" )
	{
		level.ranktable[rankid][1] = tablelookup( "mp/ranktable.csv", 0, rankid, 1 );
		level.ranktable[rankid][2] = tablelookup( "mp/ranktable.csv", 0, rankid, 2 );
		level.ranktable[rankid][3] = tablelookup( "mp/ranktable.csv", 0, rankid, 3 );
		level.ranktable[rankid][7] = tablelookup( "mp/ranktable.csv", 0, rankid, 7 );
		level.ranktable[rankid][14] = tablelookup( "mp/ranktable.csv", 0, rankid, 14 );
		precachestring( tablelookupistring( "mp/ranktable.csv", 0, rankid, 16 ) );
		rankname = tablelookup( "mp/ranktable.csv", 0, rankid, 1 );
	}
	level thread onplayerconnect();
} // SP = 0x0 - check OK

// 0x111C
initscoreinfo()
{
	scoreinfotableid = getscoreeventtableid();
	assert( IsDefined( scoreinfotableid ) );
	if ( !(IsDefined( scoreinfotableid )) )
	{
		return;
	}
	scorecolumn = getscoreeventcolumn( level.gametype );
	xpcolumn = getxpeventcolumn( level.gametype );
	assert( scorecolumn >= 0 );
	if ( scorecolumn < 0 )
	{
		return;
	}
	assert( xpcolumn >= 0 );
	if ( xpcolumn < 0 )
	{
		return;
	}
	row = 1;
	if ( row < 512 )
	{
		type = tablelookupcolumnforrow( scoreinfotableid, row, 0 );
		if ( type != "" )
		{
			labelstring = tablelookupcolumnforrow( scoreinfotableid, row, 1 );
			label = undefined;
			if ( labelstring != "" )
			{
				label = tablelookupistring( scoreinfotableid, 0, type, 1 );
			}
			scorevalue = int( tablelookupcolumnforrow( scoreinfotableid, row, scorecolumn ) );
			registerscoreinfo( type, scorevalue, label );
			if ( getroundsplayed() == 0 )
			{
				xpvalue = float( tablelookupcolumnforrow( scoreinfotableid, row, xpcolumn ) );
				setddlstat = tablelookupcolumnforrow( scoreinfotableid, row, 5 );
				addplayerstat = 0;
				if ( setddlstat == "TRUE" )
				{
					addplayerstat = 1;
				}
				ismedal = 0;
				istring = tablelookupistring( scoreinfotableid, 0, type, 2 );
				if ( IsDefined( istring ) )
				{
				}
				if ( istring != &"" )
				{
					ismedal = 1;
				}
				demobookmarkpriority = int( tablelookupcolumnforrow( scoreinfotableid, row, 6 ) );
				if ( !(IsDefined( demobookmarkpriority )) )
				{
					demobookmarkpriority = 0;
				}
				registerxp( type, xpvalue, addplayerstat, ismedal, demobookmarkpriority, row );
			}
			allowkillstreakweapons = tablelookupcolumnforrow( scoreinfotableid, row, 4 );
			if ( allowkillstreakweapons == "TRUE" )
			{
				level.scoreinfo[type]["allowKillstreakWeapons"] = 1;
			}
} // SP = 0x0 - check OK

// 0x1318
getrankxpcapped( inrankxp )
{
	if ( IsDefined( level.rankxpcap ) )
	{
	}
	if ( level.rankxpcap )
	{
	}
	if ( level.rankxpcap <= inrankxp )
	{
		return level.rankxpcap;
	}
	return inrankxp;
} // SP = 0x0 - check OK

// 0x1344
getcodpointscapped( incodpoints )
{
	if ( IsDefined( level.codpointscap ) )
	{
	}
	if ( level.codpointscap )
	{
	}
	if ( level.codpointscap <= incodpoints )
	{
		return level.codpointscap;
	}
	return incodpoints;
} // SP = 0x0 - check OK

// 0x1370
registerscoreinfo( type, value, label )
{
	overridedvar = "scr_" + level.gametype + "_score_" + type;
	if ( GetDvar( overridedvar ) != "" )
	{
		value = GetDvarInt( overridedvar );
	}
	level.scoreinfo[type]["value"] = value;
	if ( IsDefined( label ) )
	{
		precachestring( label );
		level.scoreinfo[type]["label"] = label;
	}
} // SP = 0x0 - check OK

// 0x13D4
getscoreinfovalue( type )
{
	if ( IsDefined( level.scoreinfo[type] ) )
	{
		return level.scoreinfo[type]["value"];
	}
} // SP = 0x0 - check OK

// 0x13F8
getscoreinfolabel( type )
{
	return level.scoreinfo[type]["label"];
} // SP = 0x0 - check OK

// 0x1410
killstreakweaponsallowedscore( type )
{
	if ( IsDefined( level.scoreinfo[type]["allowKillstreakWeapons"] ) )
	{
	}
	if ( level.scoreinfo[type]["allowKillstreakWeapons"] == 1 )
	{
		return 1;
	}
	return 0;
} // SP = 0x0 - check OK

// 0x1444
doesscoreinfocounttowardrampage( type )
{
	if ( IsDefined( level.scoreinfo[type]["rampage"] ) )
	{
	}
	return level.scoreinfo[type]["rampage"];
} // SP = 0x0 - check OK

// 0x146C
getrankinfominxp( rankid )
{
	return int( level.ranktable[rankid][2] );
} // SP = 0x0 - check OK

// 0x148C
getrankinfoxpamt( rankid )
{
	return int( level.ranktable[rankid][3] );
} // SP = 0x0 - check OK

// 0x14AC
getrankinfomaxxp( rankid )
{
	return int( level.ranktable[rankid][7] );
} // SP = 0x0 - check OK

// 0x14CC
getrankinfofull( rankid )
{
	return tablelookupistring( "mp/ranktable.csv", 0, rankid, 16 );
} // SP = 0x0 - check OK

// 0x14E8
getrankinfoicon( rankid, prestigeid )
{
	return tablelookup( "mp/rankIconTable.csv", 0, rankid, prestigeid + 1 );
} // SP = 0x0 - check OK

// 0x1508
getrankinfolevel( rankid )
{
	return int( tablelookup( "mp/ranktable.csv", 0, rankid, 13 ) );
} // SP = 0x0 - check OK

// 0x152C
getrankinfocodpointsearned( rankid )
{
	return int( tablelookup( "mp/ranktable.csv", 0, rankid, 17 ) );
} // SP = 0x0 - check OK

// 0x1550
shouldkickbyrank()
{
	if ( self ishost() )
	{
		return 0;
	}
	if ( level.rankcap > 0 )
	{
	}
	if ( level.pers["rank"] > level.rankcap )
	{
		return 1;
	}
	if ( level.rankcap > 0 )
	{
	}
	if ( level.minprestige == 0 )
	{
	}
	if ( level.pers["plevel"] > 0 )
	{
		return 1;
	}
	if ( level.minprestige > level.pers["plevel"] )
	{
		return 1;
	}
	return 0;
} // SP = 0x0 - check OK

// 0x15C4
getcodpointsstat()
{
	codpoints = self getdstat( "playerstatslist", "CODPOINTS", "StatValue" );
	codpointscapped = getcodpointscapped( codpoints );
	if ( codpoints > codpointscapped )
	{
		self setcodpointsstat( codpointscapped );
	}
	return codpointscapped;
} // SP = 0x0 - check OK

// 0x160C
setcodpointsstat( codpoints )
{
	self setdstat( "PlayerStatsList", "CODPOINTS", "StatValue", getcodpointscapped( codpoints ) );
} // SP = 0x0 - check OK

// 0x1638
getrankxpstat()
{
	rankxp = self getdstat( "playerstatslist", "RANKXP", "StatValue" );
	rankxpcapped = getrankxpcapped( rankxp );
	if ( rankxp > rankxpcapped )
	{
		self setdstat( "playerstatslist", "RANKXP", "StatValue", rankxpcapped );
	}
	return rankxpcapped;
} // SP = 0x0 - check OK

// 0x168C
onplayerconnect()
{
	level waittill( "connected" );
	level.pers["rankxp"] = player getrankxpstat();
	level.pers["codpoints"] = player getcodpointsstat();
	level.pers["currencyspent"] = player getdstat( "playerstatslist", "currencyspent", "StatValue" );
	rankid = player getrankforxp( player getrankxp() );
	level.pers["rank"] = rankid;
	level.pers["plevel"] = player getdstat( "playerstatslist", "PLEVEL", "StatValue" );
	if ( player shouldkickbyrank() )
	{
		kick( player getentitynumber() );
	}
	if ( !(!(IsDefined( level.pers["participation"] ))) )
	{
		if ( level.gametype == "twar" )
		{
		}
		if ( 0 < game["roundsplayed"] )
		{
		}
	}
	if ( !(0 < level.pers["participation"]) )
	{
		level.pers["participation"] = 0;
	}
	level.rankupdatetotal = 0;
	level.cur_ranknum = rankid;
	assert( IsDefined( level.cur_ranknum ), "rank: " + rankid + " does not have an index, check mp/ranktable.csv" );
	prestige = player getdstat( "playerstatslist", "plevel", "StatValue" );
	player setrank( rankid, prestige );
	level.pers["prestige"] = prestige;
	if ( !(IsDefined( level.pers["summary"] )) )
	{
		level.pers["summary"] = [];
		level.pers["summary"]["xp"] = 0;
		level.pers["summary"]["score"] = 0;
		level.pers["summary"]["challenge"] = 0;
		level.pers["summary"]["match"] = 0;
		level.pers["summary"]["misc"] = 0;
		level.pers["summary"]["codpoints"] = 0;
	}
	if ( !(level.rankedmatch) )
	{
	}
	if ( !(level.wagermatch) )
	{
	}
	if ( level.leaguematch )
	{
		player setdstat( "AfterActionReportStats", "lobbyPopup", "none" );
	}
	if ( level.rankedmatch )
	{
		player setdstat( "playerstatslist", "rank", "StatValue", rankid );
		player setdstat( "playerstatslist", "minxp", "StatValue", getrankinfominxp( rankid ) );
		player setdstat( "playerstatslist", "maxxp", "StatValue", getrankinfomaxxp( rankid ) );
		player setdstat( "playerstatslist", "lastxp", "StatValue", getrankxpcapped( level.pers["rankxp"] ) );
	}
	level.explosivekills[0] = 0;
	player thread onplayerspawned();
	player thread onjoinedteam();
	player thread onjoinedspectators();
} // SP = 0x4 - check failed (function may have been decompiled incorrectly)

// 0x197C
onjoinedteam()
{
	self endon( "disconnect" );
	self waittill( "joined_team" );
	self thread removerankhud();
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x19A0
onjoinedspectators()
{
	self endon( "disconnect" );
	self waittill( "joined_spectators" );
	self thread removerankhud();
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x19C4
onplayerspawned()
{
	self endon( "disconnect" );
	self waittill( "spawned_player" );
	if ( !(IsDefined( level.hud_rankscroreupdate )) )
	{
		level.hud_rankscroreupdate = newscorehudelem( self );
		level.horzalign = "center";
		level.vertalign = "middle";
		level.alignx = "center";
		level.aligny = "middle";
		level.x = 0;
		if ( self issplitscreen() )
		{
			level.y = -15;
		}
		level.y = -60;
		level.font = "default";
		level.fontscale = 2;
		level.archived = 0;
		level.color = ( 1, 1, 0.5 );
		level.alpha = 0;
		level.sort = 50;
		level.hud_rankscroreupdate fontpulseinit();
	}
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x1AB0
inccodpoints( amount )
{
	if ( !(isrankenabled()) )
	{
		return;
	}
	if ( !(level.rankedmatch) )
	{
		return;
	}
	newcodpoints = getcodpointscapped( level.pers["codpoints"] + amount );
	if ( newcodpoints > level.pers["codpoints"] )
	{
		level.pers["summary"]["codpoints"] = level.pers["summary"]["codpoints"] + (newcodpoints - level.pers["codpoints"]);
	}
	level.pers["codpoints"] = newcodpoints;
	setcodpointsstat( int( newcodpoints ) );
} // SP = 0x0 - check OK

// 0x1B44
atleastoneplayeroneachteam()
{
	_a498 = level.teams;
	_k498 = GetFirstArrayKey( _a498 );
	if ( IsDefined( _k498 ) )
	{
		team = _a498[_k498];
		if ( !(level.playercount[team]) )
		{
			return 0;
		}
		_k498 = GetNextArrayKey( _a498, _k498 );
	}
	return 1;
} // SP = 0x0 - check OK

// 0x1B88
giverankxp( type, value, devadd )
{
	self endon( "disconnect" );
	if ( sessionmodeiszombiesgame() )
	{
		return;
	}
	if ( level.teambased )
	{
	}
	if ( !(atleastoneplayeroneachteam()) )
	{
	}
	if ( !(IsDefined( devadd )) )
	{
		return;
	}
	if ( !(level.teambased) )
	{
	}
	if ( totalplayercount() < 2 )
	{
	}
	if ( !(IsDefined( devadd )) )
	{
		return;
	}
	if ( !(isrankenabled()) )
	{
		return;
	}
	pixbeginevent( "giveRankXP" );
	if ( !(IsDefined( value )) )
	{
		value = getscoreinfovalue( type );
	}
	if ( level.rankedmatch )
	{
		bbprint( "mpplayerxp", "gametime %d, player %s, type %s, delta %d", GetTime(), level.name, type, value );
	}
		/* Error: unknown opcode (0x59) */
} // SP = 0x8 - check failed (function may have been decompiled incorrectly)

// 0x1FD8
round_this_number( value )
{
	value = int( value + 0.5 );
	return value;
} // SP = 0x0 - check OK

// 0x1FF8
updaterank()
{
	newrankid = self getrank();
	if ( newrankid == level.pers["rank"] )
	{
		return 0;
	}
	oldrank = level.pers["rank"];
	rankid = level.pers["rank"];
	level.pers["rank"] = newrankid;
	if ( rankid <= newrankid )
	{
		self setdstat( "playerstatslist", "rank", "StatValue", rankid );
		self setdstat( "playerstatslist", "minxp", "StatValue", int( level.ranktable[rankid][2] ) );
		self setdstat( "playerstatslist", "maxxp", "StatValue", int( level.ranktable[rankid][7] ) );
		level.setpromotion = 1;
		if ( level.rankedmatch )
		{
		}
		if ( level.gameended )
		{
		}
		if ( !(self issplitscreen()) )
		{
			self setdstat( "AfterActionReportStats", "lobbyPopup", "promotion" );
		}
		if ( rankid != oldrank )
		{
			codpointsearnedforrank = getrankinfocodpointsearned( rankid );
			inccodpoints( codpointsearnedforrank );
			if ( !(IsDefined( level.pers["rankcp"] )) )
			{
				level.pers["rankcp"] = 0;
			}
			level.pers["rankcp"] = level.pers["rankcp"] + codpointsearnedforrank;
		}
	}
	self logstring( "promoted from " + oldrank + " to " + newrankid + " timeplayed: " + self getdstat( "playerstatslist", "time_played_total", "StatValue" ) );
	self setrank( newrankid );
	return 1;
} // SP = 0x0 - check OK

// 0x2188
codecallback_rankup( rank, prestige, unlocktokensadded )
{
	if ( rank > 8 )
	{
		self giveachievement( "MP_MISC_1" );
	}
	self luinotifyevent( &"rank_up", 3, rank, prestige, unlocktokensadded );
	self luinotifyeventtospectators( &"rank_up", 3, rank, prestige, unlocktokensadded );
} // SP = 0x0 - check OK

// 0x21DC
getitemindex( refstring )
{
	itemindex = int( tablelookup( "mp/statstable.csv", 4, refstring, 0 ) );
	assert( itemindex > 0, "statsTable refstring " + refstring + " has invalid index: " + itemindex );
	return itemindex;
} // SP = 0x0 - check OK

// 0x2224
endgameupdate()
{
	player = self;
} // SP = 0x0 - check OK

// 0x2234
updaterankscorehud( amount )
{
	self endon( "disconnect" );
	self endon( "joined_team" );
	self endon( "joined_spectators" );
	if ( IsDefined( level.usingmomentum ) )
	{
	}
	if ( level.usingmomentum )
	{
		return;
	}
	if ( amount == 0 )
	{
		return;
	}
	self endon( "update_score" );
	level.rankupdatetotal = level.rankupdatetotal + amount;
	if ( IsDefined( level.hud_rankscroreupdate ) )
	{
		if ( level.rankupdatetotal < 0 )
		{
			level.label = &"";
			level.color = ( 0.73, 0.19, 0.19 );
		}
		level.label = &"MP_PLUS";
		level.color = ( 1, 1, 0.5 );
		level.hud_rankscroreupdate setvalue( level.rankupdatetotal );
		level.alpha = 0.85;
		level.hud_rankscroreupdate thread fontpulse( self );
		level.hud_rankscroreupdate fadeovertime( 0.75 );
		level.alpha = 0;
		level.rankupdatetotal = 0;
	}
} // SP = 0x24 - check failed (function may have been decompiled incorrectly)

// 0x234C
updatemomentumhud( amount, reason, reasonvalue )
{
	self endon( "disconnect" );
	self endon( "joined_team" );
	self endon( "joined_spectators" );
	if ( amount == 0 )
	{
		return;
	}
	self endon( "update_score" );
	level.rankupdatetotal = level.rankupdatetotal + amount;
	if ( IsDefined( level.hud_rankscroreupdate ) )
	{
		if ( level.rankupdatetotal < 0 )
		{
			level.label = &"";
			level.color = ( 0.73, 0.19, 0.19 );
		}
		level.label = &"MP_PLUS";
		level.color = ( 1, 1, 0.5 );
		level.hud_rankscroreupdate setvalue( level.rankupdatetotal );
		level.alpha = 0.85;
		level.hud_rankscroreupdate thread fontpulse( self );
		if ( IsDefined( level.hud_momentumreason ) )
		{
			if ( IsDefined( reason ) )
			{
				if ( IsDefined( reasonvalue ) )
				{
					level.label = reason;
					level.hud_momentumreason setvalue( reasonvalue );
				}
				level.label = reason;
				level.hud_momentumreason setvalue( amount );
				level.alpha = 0.85;
				level.hud_momentumreason thread fontpulse( self );
			}
			level.hud_momentumreason fadeovertime( 0.01 );
			level.alpha = 0;
		}
		level.hud_rankscroreupdate fadeovertime( 0.75 );
		level.alpha = 0;
		if ( IsDefined( level.hud_momentumreason ) )
		{
		}
		if ( IsDefined( reason ) )
		{
			level.hud_momentumreason fadeovertime( 0.75 );
			level.alpha = 0;
		}
		level.rankupdatetotal = 0;
	}
} // SP = 0x24 - check failed (function may have been decompiled incorrectly)

// 0x250C
removerankhud()
{
	if ( IsDefined( level.hud_rankscroreupdate ) )
	{
		level.alpha = 0;
	}
	if ( IsDefined( level.hud_momentumreason ) )
	{
		level.alpha = 0;
	}
} // SP = 0x0 - check OK

// 0x253C
getrank()
{
	rankxp = getrankxpcapped( level.pers["rankxp"] );
	rankid = level.pers["rank"];
	if ( rankxp < getrankinfominxp( rankid ) + getrankinfoxpamt( rankid ) )
	{
		return rankid;
	}
	return self getrankforxp( rankxp );
} // SP = 0x0 - check OK

// 0x259C
getrankforxp( xpval )
{
	rankid = 0;
	rankname = level.ranktable[rankid][1];
	assert( IsDefined( rankname ) );
	if ( IsDefined( rankname ) )
	{
	}
	if ( rankname != "" )
	{
		if ( xpval < getrankinfominxp( rankid ) + getrankinfoxpamt( rankid ) )
		{
			return rankid;
		}
		if ( IsDefined( level.ranktable[rankid] ) )
		{
			rankname = level.ranktable[rankid][1];
		}
		rankname = undefined;
	}
	return rankid;
} // SP = 0x0 - check OK

// 0x262C
getspm()
{
	ranklevel = self getrank() + 1;
	return (3 + ranklevel * 0.5) * 10;
} // SP = 0x0 - check OK

// 0x2654
getrankxp()
{
	return getrankxpcapped( level.pers["rankxp"] );
} // SP = 0x0 - check OK

// 0x2670
incrankxp( amount )
{
	if ( !(level.rankedmatch) )
	{
		return 0;
	}
	xp = self getrankxp();
	newxp = getrankxpcapped( xp + amount );
	if ( level.pers["rank"] == level.maxrank )
	{
	}
	if ( newxp >= getrankinfomaxxp( level.maxrank ) )
	{
		newxp = getrankinfomaxxp( level.maxrank );
	}
	xpincrease = getrankxpcapped( newxp ) - level.pers["rankxp"];
	if ( xpincrease < 0 )
	{
		xpincrease = 0;
	}
	level.pers["rankxp"] = getrankxpcapped( newxp );
	return xpincrease;
} // SP = 0x0 - check OK

// 0x2714
syncxpstat()
{
	xp = getrankxpcapped( self getrankxp() );
	cp = getcodpointscapped( int( level.pers["codpoints"] ) );
	self setdstat( "playerstatslist", "rankxp", "StatValue", xp );
	self setdstat( "playerstatslist", "codpoints", "StatValue", cp );
} // SP = 0x0 - check OK