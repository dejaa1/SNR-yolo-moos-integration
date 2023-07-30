#!/bin/bash
TIME_WARP=1

SHORE_IP="192.168.1.200"
SHORE_LISTEN="9300"

TRAIL_RANGE="3"
TRAIL_ANGLE="330"
HELP="no"
JUST_BUILD="no"
VTEAM=""
VNAME=""
RNAME=""
VMODEL="M1_8"

START_POS="140,45,240"
RETURN_POS="5,0"
LOITER_POS="x=100,y=-180"
GRAB_POS=""
GRABR_POS=""
GRABL_POS=""
UNTAG_POS=""

HERON_TEAMMATE=""
HERON_TEAMMATE_VTEAM=""

CID=000 #COMP ID
LOGPATH=./

source ../aquaticus_params.txt

function help(){
    echo ""
    echo "USAGE: $0 <heron_vehicle_name> <vehicle_role> <heron_teammate_vehicle_role> [SWITCHES]"
    
    echo ""
    echo "POSSIBLE SURVEYOR VEHICLE NAMES:"
    echo "  scott,         s   : Scott surveyor."
    echo "  thomas,        t   : Thomas surveyor."
    echo "  ursula,        u   : Ursula surveyor."
    echo "  valhalla,      v   : Valhalla surveyor."
    echo "  walter,        w   : Walter surveyor."
    echo "  xavier,        x   : Xavier surveyor."
    echo "  yolanda,       y   : Yolanda surveyor."
    echo "  zach,          z   : Zach surveyor."

    echo ""
    echo "POSSIBLE ROLES (and heron teammate_roles):"
    echo "  blue_one,     b1  : Vehicle one on blue team."
    echo "  blue_two,     b2  : Vehicle two on blue team."
    echo "  red_one,      r1  : Vehicle one on red team."
    echo "  red_two,      r2  : Vehicle two on red team."

    echo ""
    echo "POSSIBLE SWITCHES:"
    echo "  --role,           : Autonomy startup roles: ATTACK/DEFEND _E/_MED."
    echo "  --sim,        -s  : Simulation mode."
    echo "  --start-x=        : Start from x position (requires x y a)."
    echo "  --start-y=        : Start from y position (requires x y a)."
    echo "  --start-a=        : Start from angle (requires x y a)."
    echo "  --cid=            : Competition ID (for log file)"
    echo "  --logpath=        : Log path"
    echo "  --just_build, -J  : Just build targ files."
    echo "  --help,       -H  : Display this message."
    echo "  #                 : Moos Time Warp."
    exit 0
}

#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
case "$1" in
    s|scott)
        SURVEYOR_IP=192.168.1.11
        VNAME="SCOTT"
        echo "SCOTT surveyor selected."
        ;;
    t|thomas)
        SURVEYOR_IP=192.168.1.21
        VNAME="THOMAS"
        echo "THOMAS surveyor selected."
        ;;
    u|ursula)
        SURVEYOR_IP=192.168.1.31
        VNAME="URSULA"
        echo "URSULA surveyor selected."
        ;;
    v|valhalla)
        SURVEYOR_IP=192.168.1.41
        VNAME="VALHALLA"
        echo "VALHALLA surveyor selected."
        ;;
    w|walter)
        SURVEYOR_IP=192.168.1.51
        VNAME="WALTER"
        echo "WALTER surveyor selected."
        ;;
    x|xavier)
        SURVEYOR_IP=192.168.1.61
        VNAME="XAVIER"
        echo "XAVIER surveyor selected."
        ;;
    y|yolanda)
        SURVEYOR_IP=192.168.1.71
        VNAME="YOLANDA"
        echo "YOLANDA surveyor selected."
        ;;
    z|zach)
	SURVEYOR_IP=192.168.1.81
        VNAME="ZACH"
	    echo "ZACH surveyor selected."
	    ;;
    *)
        echo "!!! Error invalid positional argument $1 !!!"
        ;;
esac

RNAME=$VNAME

VPORT="9011"
VR_PORT="9811"
SHARE_LISTEN="9311"

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files.
#-------------------------------------------------------

echo "Assembling MOOS file targ_${RNAME}.moos"

nsplug meta_surveyor.moos targ_${RNAME}.moos -f \
    VNAME=$VNAME                 \
    RNAME=$RNAME                 \
    VPORT=$VPORT                 \
    VR_PORT=$VR_PORT             \
    WARP=$TIME_WARP              \
    SHARE_LISTEN=$SHARE_LISTEN   \
    SHORE_LISTEN=$SHORE_LISTEN   \
    SHORE_IP=$SHORE_IP           \
    SURVEYOR_IP=$SURVEYOR_IP     \
    HOSTIP_FORCE="localhost"     \
    VARIATION=$VARIATION         \
    VMODEL=$VMODEL               \
    VTYPE="kayak"                \
    VTEAM=$VTEAM                 \
    CID=$CID                     \
    LOGPATH=$LOGPATH             \
    OPFOR=$OPFOR                 \
    OPFOR_ZONE=$OPFOR_ZONE

echo "Assembling BHV file targ_${RNAME}.bhv"
nsplug meta_surveyor.bhv targ_${RNAME}.bhv -f  \
    CORNER_1=$CORNER_1             \
    CORNER_2=$CORNER_2             \
    CORNER_3=$CORNER_3             \
    CORNER_4=$CORNER_4


if [ ${JUST_BUILD} = "yes" ] ; then
    echo "Files assembled; vehicle not launched; exiting per request."
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------

echo "Launching $RNAME MOOS Community "
pAntler targ_${RNAME}.moos >& /dev/null &

uMAC targ_${RNAME}.moos

echo "Killing all processes ..."
kill -- -$$
echo "Done killing processes."
