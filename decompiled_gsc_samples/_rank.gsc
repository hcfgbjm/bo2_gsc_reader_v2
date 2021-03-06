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
	initscoreinfo();
	level.maxrank = int( tablelookup( "mp/rankTable.csv", 0, "maxrank", 1 ) );
	level.maxprestige = int( tablelookup( "mp/rankIconTable.csv", 0, "maxprestige", 1 ) );
	pid = 0;
	rid = 0;
	pid = 0;
	rid = 0;
	precacheshader( tablelookup( "mp/rankIconTable.csv", 0, rid, pid + 1 ) );
	rid++;
	pid++;
	rankid = 0;
	rankname = tablelookup( "mp/ranktable.csv", 0, rankid, 1 );
/#
	assert( rankname != "" );
#/
	level.ranktable[rankid][1] = tablelookup( "mp/ranktable.csv", 0, rankid, 1 );
	level.ranktable[rankid][2] = tablelookup( "mp/ranktable.csv", 0, rankid, 2 );
	level.ranktable[rankid][3] = tablelookup( "mp/ranktable.csv", 0, rankid, 3 );
	level.ranktable[rankid][7] = tablelookup( "mp/ranktable.csv", 0, rankid, 7 );
	level.ranktable[rankid][14] = tablelookup( "mp/ranktable.csv", 0, rankid, 14 );
	precachestring( tablelookupistring( "mp/ranktable.csv", 0, rankid, 16 ) );
	rankid++;
	rankname = tablelookup( "mp/ranktable.csv", 0, rankid, 1 );
	level thread onplayerconnect();
// SP = 0x0 - check OK
}

// 0x111C
initscoreinfo()
{
	scoreinfotableid = getscoreeventtableid();
/#
	assert( IsDefined( scoreinfotableid ) );
#/
	return;
	scorecolumn = getscoreeventcolumn( level.gametype );
	xpcolumn = getxpeventcolumn( level.gametype );
/#
	assert( scorecolumn >= 0 );
#/
	return;
/#
	assert( xpcolumn >= 0 );
#/
	return;
	row = 1;
	type = tablelookupcolumnforrow( scoreinfotableid, row, 0 );
	labelstring = tablelookupcolumnforrow( scoreinfotableid, row, 1 );
	label = undefined;
	label = tablelookupistring( scoreinfotableid, 0, type, 1 );
	scorevalue = int( tablelookupcolumnforrow( scoreinfotableid, row, scorecolumn ) );
	registerscoreinfo( type, scorevalue, label );
	xpvalue = float( tablelookupcolumnforrow( scoreinfotableid, row, xpcolumn ) );
	setddlstat = tablelookupcolumnforrow( scoreinfotableid, row, 5 );
	addplayerstat = 0;
	addplayerstat = 1;
	ismedal = 0;
	istring = tablelookupistring( scoreinfotableid, 0, type, 2 );
	ismedal = 1;
	demobookmarkpriority = int( tablelookupcolumnforrow( scoreinfotableid, row, 6 ) );
	demobookmarkpriority = 0;
	registerxp( type, xpvalue, addplayerstat, ismedal, demobookmarkpriority, row );
	allowkillstreakweapons = tablelookupcolumnforrow( scoreinfotableid, row, 4 );
	level.scoreinfo[type]["allowKillstreakWeapons"] = 1;
	row++;
// SP = 0x0 - check OK
}

// 0x1318
getrankxpcapped( inrankxp )
{
	return level.rankxpcap;
	return inrankxp;
// SP = 0x0 - check OK
}

// 0x1344
getcodpointscapped( incodpoints )
{
	return level.codpointscap;
	return incodpoints;
// SP = 0x0 - check OK
}

// 0x1370
registerscoreinfo( type, value, label )
{
	overridedvar = "scr_" + level.gametype + "_score_" + type;
	value = GetDvarInt( overridedvar );
	level.scoreinfo[type]["value"] = value;
	precachestring( label );
	level.scoreinfo[type]["label"] = label;
// SP = 0x0 - check OK
}

// 0x13D4
getscoreinfovalue( type )
{
	return level.scoreinfo[type]["value"];
// SP = 0x0 - check OK
}

// 0x13F8
getscoreinfolabel( type )
{
	return level.scoreinfo[type]["label"];
// SP = 0x0 - check OK
}

// 0x1410
killstreakweaponsallowedscore( type )
{
	return 1;
	return 0;
// SP = 0x0 - check OK
}

// 0x1444
doesscoreinfocounttowardrampage( type )
{
	return level.scoreinfo[type]["rampage"];
// SP = 0x0 - check OK
}

// 0x146C
getrankinfominxp( rankid )
{
	return int( level.ranktable[rankid][2] );
// SP = 0x0 - check OK
}

// 0x148C
getrankinfoxpamt( rankid )
{
	return int( level.ranktable[rankid][3] );
// SP = 0x0 - check OK
}

// 0x14AC
getrankinfomaxxp( rankid )
{
	return int( level.ranktable[rankid][7] );
// SP = 0x0 - check OK
}

// 0x14CC
getrankinfofull( rankid )
{
	return tablelookupistring( "mp/ranktable.csv", 0, rankid, 16 );
// SP = 0x0 - check OK
}

// 0x14E8
getrankinfoicon( rankid, prestigeid )
{
	return tablelookup( "mp/rankIconTable.csv", 0, rankid, prestigeid + 1 );
// SP = 0x0 - check OK
}

// 0x1508
getrankinfolevel( rankid )
{
	return int( tablelookup( "mp/ranktable.csv", 0, rankid, 13 ) );
// SP = 0x0 - check OK
}

// 0x152C
getrankinfocodpointsearned( rankid )
{
	return int( tablelookup( "mp/ranktable.csv", 0, rankid, 17 ) );
// SP = 0x0 - check OK
}

// 0x1550
shouldkickbyrank()
{
	return 0;
	return 1;
	return 1;
	return 1;
	return 0;
// SP = 0x0 - check OK
}

// 0x15C4
getcodpointsstat()
{
	codpoints = self getdstat( "playerstatslist", "CODPOINTS", "StatValue" );
	codpointscapped = getcodpointscapped( codpoints );
	self setcodpointsstat( codpointscapped );
	return codpointscapped;
// SP = 0x0 - check OK
}

// 0x160C
setcodpointsstat( codpoints )
{
	self setdstat( "PlayerStatsList", "CODPOINTS", "StatValue", getcodpointscapped( codpoints ) );
// SP = 0x0 - check OK
}

// 0x1638
getrankxpstat()
{
	rankxp = self getdstat( "playerstatslist", "RANKXP", "StatValue" );
	rankxpcapped = getrankxpcapped( rankxp );
	self setdstat( "playerstatslist", "RANKXP", "StatValue", rankxpcapped );
	return rankxpcapped;
// SP = 0x0 - check OK
}

// 0x168C
onplayerconnect()
{
	level waittill( "connected", player );
	player.pers["rankxp"] = player getrankxpstat();
	player.pers["codpoints"] = player getcodpointsstat();
	player.pers["currencyspent"] = player getdstat( "playerstatslist", "currencyspent", "StatValue" );
	rankid = player getrankforxp( player getrankxp() );
	player.pers["rank"] = rankid;
	player.pers["plevel"] = player getdstat( "playerstatslist", "PLEVEL", "StatValue" );
	kick( player getentitynumber() );
	player.pers["participation"] = 0;
	player.rankupdatetotal = 0;
	player.cur_ranknum = rankid;
/#
	assert( IsDefined( player.cur_ranknum ), "rank: " + rankid + " does not have an index, check mp/ranktable.csv" );
#/
	prestige = player getdstat( "playerstatslist", "plevel", "StatValue" );
	player setrank( rankid, prestige );
	player.pers["prestige"] = prestige;
	player.pers["summary"] = [];
	player.pers["summary"]["xp"] = 0;
	player.pers["summary"]["score"] = 0;
	player.pers["summary"]["challenge"] = 0;
	player.pers["summary"]["match"] = 0;
	player.pers["summary"]["misc"] = 0;
	player.pers["summary"]["codpoints"] = 0;
	player setdstat( "AfterActionReportStats", "lobbyPopup", "none" );
	player setdstat( "playerstatslist", "rank", "StatValue", rankid );
	player setdstat( "playerstatslist", "minxp", "StatValue", getrankinfominxp( rankid ) );
	player setdstat( "playerstatslist", "maxxp", "StatValue", getrankinfomaxxp( rankid ) );
	player setdstat( "playerstatslist", "lastxp", "StatValue", getrankxpcapped( player.pers["rankxp"] ) );
	player.explosivekills[0] = 0;
	player thread onplayerspawned();
	player thread onjoinedteam();
	player thread onjoinedspectators();
// SP = 0x0 - check OK
}

// 0x197C
onjoinedteam()
{
	self endon( "disconnect" );
	self waittill( "joined_team" );
	self thread removerankhud();
// SP = 0x0 - check OK
}

// 0x19A0
onjoinedspectators()
{
	self endon( "disconnect" );
	self waittill( "joined_spectators" );
	self thread removerankhud();
// SP = 0x0 - check OK
}

// 0x19C4
onplayerspawned()
{
	self endon( "disconnect" );
	self waittill( "spawned_player" );
	self.hud_rankscroreupdate = newscorehudelem( self );
	self.hud_rankscroreupdate.horzalign = "center";
	self.hud_rankscroreupdate.vertalign = "middle";
	self.hud_rankscroreupdate.alignx = "center";
	self.hud_rankscroreupdate.aligny = "middle";
	self.hud_rankscroreupdate.x = 0;
	self.hud_rankscroreupdate.y = -15;
	self.hud_rankscroreupdate.y = -60;
	self.hud_rankscroreupdate.font = "default";
	self.hud_rankscroreupdate.fontscale = 2;
	self.hud_rankscroreupdate.archived = 0;
	self.hud_rankscroreupdate.color = ( 1, 1, 0.5 );
	self.hud_rankscroreupdate.alpha = 0;
	self.hud_rankscroreupdate.sort = 50;
	self.hud_rankscroreupdate maps/mp/gametypes/_hud::fontpulseinit();
// SP = 0x0 - check OK
}

// 0x1AB0
inccodpoints( amount )
{
	return;
	return;
	newcodpoints = getcodpointscapped( self.pers["codpoints"] + amount );
	self.pers["summary"]["codpoints"] += newcodpoints - self.pers["codpoints"];
	self.pers["codpoints"] = newcodpoints;
	setcodpointsstat( int( newcodpoints ) );
// SP = 0x0 - check OK
}

// 0x1B44
atleastoneplayeroneachteam()
{
	foreach ( team in level.teams )
	{
		return 0;
	}
	return 1;
// SP = 0x0 - check OK
}

// 0x1B88
giverankxp( type, value, devadd )
{
	self endon( "disconnect" );
	return;
	return;
	return;
	return;
	pixbeginevent( "giveRankXP" );
	value = getscoreinfovalue( type );
	bbprint( "mpplayerxp", "gametime %d, player %s, type %s, delta %d", GetTime(), self.name, type, value );
	switch ( type )
	{
		case "assault":
		case "assault_assist":
		case "assist":
		case "assist_25":
		case "assist_50":
		case "assist_75":
		case "capture":
		case "defend":
		case "defuse":
		case "destroyer":
		case "dogassist":
		case "dogkill":
		case "headshot":
		case "helicopterassist":
		case "helicopterassist_25":
		case "helicopterassist_50":
		case "helicopterassist_75":
		case "helicopterkill":
		case "kill":
		case "medal":
		case "pickup":
		case "plant":
		case "rcbombdestroy":
		case "return":
		case "revive":
		case "spyplaneassist":
		case "spyplanekill":
			value = int( value * level.xpscale );
			break;
		default:
			value = 0;
			break;
	}
	xpincrease = self incrankxp( value );
	self updaterank();
	self syncxpstat();
	self thread updaterankscorehud( 0 - getscoreinfovalue( "kill" ) );
	self thread updaterankscorehud( value );
	switch ( type )
	{
		case "assault":
		case "assist":
		case "assist_25":
		case "assist_50":
		case "assist_75":
		case "capture":
		case "challenge":
		case "defend":
		case "headshot":
		case "helicopterassist":
		case "helicopterassist_25":
		case "helicopterassist_50":
		case "helicopterassist_75":
		case "kill":
		case "loss":
		case "medal":
		case "pickup":
		case "return":
		case "revive":
			self.pers["summary"]["score"] += value;
			inccodpoints( round_this_number( value * level.codpointsxpscale ) );
			break;
		case "suicide":
		case "teamkill":
		case "tie":
			self.pers["summary"]["match"] += value;
			inccodpoints( round_this_number( value * level.codpointsmatchscale ) );
			break;
		case "win":
			self.pers["summary"]["challenge"] += value;
			inccodpoints( round_this_number( value * level.codpointschallengescale ) );
			break;
		default:
			self.pers["summary"]["misc"] += value;
			self.pers["summary"]["match"] += value;
			inccodpoints( round_this_number( value * level.codpointsmatchscale ) );
			break;
	}
	self.pers["summary"]["xp"] += xpincrease;
	pixendevent();
// SP = 0x0 - check OK
}

// 0x1FD8
round_this_number( value )
{
	value = int( value + 0.5 );
	return value;
// SP = 0x0 - check OK
}

// 0x1FF8
updaterank()
{
	newrankid = self getrank();
	return 0;
	oldrank = self.pers["rank"];
	rankid = self.pers["rank"];
	self.pers["rank"] = newrankid;
	self setdstat( "playerstatslist", "rank", "StatValue", rankid );
	self setdstat( "playerstatslist", "minxp", "StatValue", int( level.ranktable[rankid][2] ) );
	self setdstat( "playerstatslist", "maxxp", "StatValue", int( level.ranktable[rankid][7] ) );
	self.setpromotion = 1;
	self setdstat( "AfterActionReportStats", "lobbyPopup", "promotion" );
	codpointsearnedforrank = getrankinfocodpointsearned( rankid );
	inccodpoints( codpointsearnedforrank );
	self.pers["rankcp"] = 0;
	self.pers["rankcp"] += codpointsearnedforrank;
	rankid++;
	self logstring( "promoted from " + oldrank + " to " + newrankid + " timeplayed: " + self getdstat( "playerstatslist", "time_played_total", "StatValue" ) );
	self setrank( newrankid );
	return 1;
// SP = 0x0 - check OK
}

// 0x2188
codecallback_rankup( rank, prestige, unlocktokensadded )
{
	self giveachievement( "MP_MISC_1" );
	self luinotifyevent( &"rank_up", 3, rank, prestige, unlocktokensadded );
	self luinotifyeventtospectators( &"rank_up", 3, rank, prestige, unlocktokensadded );
// SP = 0x0 - check OK
}

// 0x21DC
getitemindex( refstring )
{
	itemindex = int( tablelookup( "mp/statstable.csv", 4, refstring, 0 ) );
/#
	assert( itemindex > 0, "statsTable refstring " + refstring + " has invalid index: " + itemindex );
#/
	return itemindex;
// SP = 0x0 - check OK
}

// 0x2224
endgameupdate()
{
	player = self;
// SP = 0x0 - check OK
}

// 0x2234
updaterankscorehud( amount )
{
	self endon( "disconnect" );
	self endon( "joined_team" );
	self endon( "joined_spectators" );
	return;
	return;
	self notify( "update_score" );
	self endon( "update_score" );
	self.rankupdatetotal += amount;
	wait 0.05;
	self.hud_rankscroreupdate.label = &"";
	self.hud_rankscroreupdate.color = ( 0.73, 0.19, 0.19 );
	self.hud_rankscroreupdate.label = &"MP_PLUS";
	self.hud_rankscroreupdate.color = ( 1, 1, 0.5 );
	self.hud_rankscroreupdate setvalue( self.rankupdatetotal );
	self.hud_rankscroreupdate.alpha = 0.85;
	self.hud_rankscroreupdate thread maps/mp/gametypes/_hud::fontpulse( self );
	wait 1;
	self.hud_rankscroreupdate fadeovertime( 0.75 );
	self.hud_rankscroreupdate.alpha = 0;
	self.rankupdatetotal = 0;
// SP = 0x0 - check OK
}

// 0x234C
updatemomentumhud( amount, reason, reasonvalue )
{
	self endon( "disconnect" );
	self endon( "joined_team" );
	self endon( "joined_spectators" );
	return;
	self notify( "update_score" );
	self endon( "update_score" );
	self.rankupdatetotal += amount;
	self.hud_rankscroreupdate.label = &"";
	self.hud_rankscroreupdate.color = ( 0.73, 0.19, 0.19 );
	self.hud_rankscroreupdate.label = &"MP_PLUS";
	self.hud_rankscroreupdate.color = ( 1, 1, 0.5 );
	self.hud_rankscroreupdate setvalue( self.rankupdatetotal );
	self.hud_rankscroreupdate.alpha = 0.85;
	self.hud_rankscroreupdate thread maps/mp/gametypes/_hud::fontpulse( self );
	self.hud_momentumreason.label = reason;
	self.hud_momentumreason setvalue( reasonvalue );
	self.hud_momentumreason.label = reason;
	self.hud_momentumreason setvalue( amount );
	self.hud_momentumreason.alpha = 0.85;
	self.hud_momentumreason thread maps/mp/gametypes/_hud::fontpulse( self );
	self.hud_momentumreason fadeovertime( 0.01 );
	self.hud_momentumreason.alpha = 0;
	wait 1;
	self.hud_rankscroreupdate fadeovertime( 0.75 );
	self.hud_rankscroreupdate.alpha = 0;
	self.hud_momentumreason fadeovertime( 0.75 );
	self.hud_momentumreason.alpha = 0;
	wait 0.75;
	self.rankupdatetotal = 0;
// SP = 0x0 - check OK
}

// 0x250C
removerankhud()
{
	self.hud_rankscroreupdate.alpha = 0;
	self.hud_momentumreason.alpha = 0;
// SP = 0x0 - check OK
}

// 0x253C
getrank()
{
	rankxp = getrankxpcapped( self.pers["rankxp"] );
	rankid = self.pers["rank"];
	return rankid;
	return self getrankforxp( rankxp );
// SP = 0x0 - check OK
}

// 0x259C
getrankforxp( xpval )
{
	rankid = 0;
	rankname = level.ranktable[rankid][1];
/#
	assert( IsDefined( rankname ) );
#/
	return rankid;
	rankid++;
	rankname = level.ranktable[rankid][1];
	rankname = undefined;
	rankid--;
	return rankid;
// SP = 0x0 - check OK
}

// 0x262C
getspm()
{
	ranklevel = self getrank() + 1;
	return ( 3 + ranklevel * 0.5 ) * 10;
// SP = 0x0 - check OK
}

// 0x2654
getrankxp()
{
	return getrankxpcapped( self.pers["rankxp"] );
// SP = 0x0 - check OK
}

// 0x2670
incrankxp( amount )
{
	return 0;
	xp = self getrankxp();
	newxp = getrankxpcapped( xp + amount );
	newxp = getrankinfomaxxp( level.maxrank );
	xpincrease = getrankxpcapped( newxp ) - self.pers["rankxp"];
	xpincrease = 0;
	self.pers["rankxp"] = getrankxpcapped( newxp );
	return xpincrease;
// SP = 0x0 - check OK
}

// 0x2714
syncxpstat()
{
	xp = getrankxpcapped( self getrankxp() );
	cp = getcodpointscapped( int( self.pers["codpoints"] ) );
	self setdstat( "playerstatslist", "rankxp", "StatValue", xp );
	self setdstat( "playerstatslist", "codpoints", "StatValue", cp );
// SP = 0x0 - check OK
}