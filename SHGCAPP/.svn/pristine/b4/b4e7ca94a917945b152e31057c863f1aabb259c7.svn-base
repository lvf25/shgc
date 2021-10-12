

#pragma once

//#include "lib/mavlink/QGC_Mavlink.h"

//#define MAVLINK_USE_MESSAGE_INFO
//#define MAVLINK_EXTERNAL_RX_STATUS  // Single m_mavlink_status instance is in QGCApplication.cc
#include <stddef.h>                 // Hack workaround for Mav 2.0 header problem with respect to offsetof usage
#include <mavlink_types.h>
extern mavlink_status_t m_mavlink_status[MAVLINK_COMM_NUM_BUFFERS];
#include <mavlink.h>


class QGCMAVLink
{
public:
    static bool isFixedWing(MAV_TYPE mavType);
    static bool isRover(MAV_TYPE mavType);
    static bool isSub(MAV_TYPE mavType);
    static bool isMultiRotor(MAV_TYPE mavType);
    static bool isVTOL(MAV_TYPE mavType);
};
