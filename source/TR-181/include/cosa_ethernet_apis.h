/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2018 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

/**************************************************************************

    module: cosa_ethernet_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_ETHERNET_API_H
#define  _COSA_ETHERNET_API_H

#include "../middle_layer_src/cosa_apis.h"
#include "../middle_layer_src/plugin_main_apis.h"
#include "ccsp_hal_ethsw.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
#if defined (FEATURE_RDKB_WAN_MANAGER)
#define ETHERNET_IF_PATH                  "Device.Ethernet.X_RDKCENTRAL-COM_Interface."

//VLAN Agent - ETHERNET.LINK
#define VLAN_DBUS_PATH                    "/com/cisco/spvtg/ccsp/vlanagent"
#define VLAN_COMPONENT_NAME               "eRT.com.cisco.spvtg.ccsp.vlanagent"

#define VLAN_ETH_LINK_TABLE_NAME          "Device.X_RDKCENTRAL-COM_Ethernet.Link."
#define VLAN_ETH_NOE_PARAM_NAME           "Device.X_RDKCENTRAL-COM_Ethernet.LinkNumberOfEntries"
#define VLAN_ETH_LINK_PARAM_ALIAS         "Device.X_RDKCENTRAL-COM_Ethernet.Link.%d.Alias"
#define VLAN_ETH_LINK_PARAM_NAME          "Device.X_RDKCENTRAL-COM_Ethernet.Link.%d.Name"
#define VLAN_ETH_LINK_PARAM_LOWERLAYERS   "Device.X_RDKCENTRAL-COM_Ethernet.Link.%d.LowerLayers"
#define VLAN_ETH_LINK_PARAM_ENABLE        "Device.X_RDKCENTRAL-COM_Ethernet.Link.%d.Enable"

//WAN Agent
#define WAN_DBUS_PATH                     "/com/cisco/spvtg/ccsp/wanagent"
#define WAN_COMPONENT_NAME                "eRT.com.cisco.spvtg.ccsp.wanagent"
#define WAN_NOE_PARAM_NAME                "Device.X_RDK-Central_COM_WanAgent.InterfaceNumberOfEntries"
#define WAN_PHY_STATUS_PARAM_NAME         "Device.X_RDK-Central_COM_WanAgent.Interface.%d.Phy.Status"
#define WAN_PHY_PATH_PARAM_NAME           "Device.X_RDK-Central_COM_WanAgent.Interface.%d.Phy.Path"
#define WAN_STATUS_PARAM_NAME             "Device.X_RDK-Central_COM_WanAgent.Interface.%d.Wan.Status"
#define WAN_IF_NAME_PARAM_NAME            "Device.X_RDK-Central_COM_WanAgent.Interface.%d.Name"

#define ETH_IF_PHY_PATH                   "Device.Ethernet.X_RDKCENTRAL-COM_Interface.%d"
#endif //#if defined (FEATURE_RDKB_WAN_MANAGER)

typedef struct _Eth_host_t
{
    UCHAR eth_macAddr[6];
    INT eth_port;
    BOOL eth_Active;
    struct _Eth_host_t* next;
}Eth_host_t;

typedef  struct
_COSA_DATAMODEL_ETH_WAN_AGENT_CONTENT
{
    BOOLEAN                Enable;
    ULONG             	    Port;
}
COSA_DATAMODEL_ETH_WAN_AGENT, *PCOSA_DATAMODEL_ETH_WAN_AGENT;

struct _COSA_DML_ETH_LOG_STATUS
{
    ULONG       Log_Period;
    BOOLEAN     Log_Enable;	
}_struct_pack_;

typedef struct _COSA_DML_ETH_LOG_STATUS COSA_DML_ETH_LOG_STATUS, *PCOSA_DML_ETH_LOG_STATUS;

/* Enum to store link status (up/down) of ethernet ports. */
typedef enum
_COSA_DML_ETH_LINK_STATUS
{
    ETH_LINK_STATUS_UP = 1,
    ETH_LINK_STATUS_DOWN
} COSA_DML_ETH_LINK_STATUS;

/** Enum to store wan status of port .( up/down) */
typedef enum
_COSA_DML_ETH_WAN_STATUS
{
    ETH_WAN_UP = 1,
    ETH_WAN_DOWN
} COSA_DML_ETH_WAN_STATUS;

/* Structure to hold port configuration data. */
typedef struct
_COSA_DML_ETH_PORT_CONFIG
{
    ULONG ulInstanceNumber; /* Instance number. */
    CHAR Name[64]; /* Interface name. eth0, eth1 etc */
    COSA_DML_ETH_LINK_STATUS LinkStatus; /* Link status - up/down */
    COSA_DML_ETH_WAN_STATUS WanStatus; /* Wan link status - Up/down */
    BOOL Upstream; /* Indicates interface is used for WAN. */
    BOOL WanValidated; /* Is it is a valid WAN interface */
    CHAR Path[128]; /* contains current table path */
} COSA_DML_ETH_PORT_CONFIG, *PCOSA_DML_ETH_PORT_CONFIG;

/* Structure to hold global data used for wan connection management. */
typedef struct
_COSA_DML_ETH_PORT_GLOBAL_CONFIG
{
    BOOL Upstream;
    CHAR Name[64];                       /* Name of interface. */
    COSA_DML_ETH_LINK_STATUS LinkStatus; /* Link status of interface. */
    COSA_DML_ETH_WAN_STATUS WanStatus; /* Wan link status. */
    BOOL WanValidated;
    CHAR Path[128]; /* contains current table path */
} COSA_DML_ETH_PORT_GLOBAL_CONFIG, *PCOSA_DML_ETH_PORT_GLOBAL_CONFIG;

/* Enum wan status. */
typedef enum _COSA_ETH_NOTIFY_ENUM
{
    NOTIFY_TO_WAN_AGENT = 1,
    NOTIFY_TO_VLAN_AGENT
} COSA_ETH_NOTIFY_ENUM;

typedef enum _COSA_ETH_VLAN_OBJ_TYPE
{
    VLAN_ETH_LINK_OBJ = 1,
    VLAN_TERMINATION_OBJ
} COSA_ETH_VLAN_OBJ_TYPE;

/*
    Standard function declaration
*/
ANSC_STATUS
CosaDmlEthInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlEthPortInit
    (
        PANSC_HANDLE                phContext
    );

ANSC_STATUS CosaDmlEthGetPortCfg( INT nIndex, PCOSA_DML_ETH_PORT_CONFIG pEthLink);
ANSC_STATUS CosaDmlEthPortSetUpstream( INT IfIndex, BOOL Upstream );
ANSC_STATUS CosaDmlEthPortSetWanValidated(INT IfIndex, BOOL WanValidated);
ANSC_STATUS CosaDmlEthPortGetWanStatus( INT IfIndex, COSA_DML_ETH_WAN_STATUS *wan_status );
ANSC_STATUS CosaDmlEthPortSetWanStatus( INT IfIndex, COSA_DML_ETH_WAN_STATUS wan_status);
ANSC_STATUS CosaDmlEthPortGetLinkStatus( INT IfIndex, COSA_DML_ETH_LINK_STATUS *LinkStatus );

#if defined (FEATURE_RDKB_WAN_MANAGER)
INT CosaDmlEthPortLinkStatusCallback( CHAR *ifname, CHAR* state );
ANSC_STATUS CosaDmlEthPortGetCopyOfGlobalInfoForGivenIfName(char* ifName, PCOSA_DML_ETH_PORT_GLOBAL_CONFIG pGlobalInfo );
ANSC_STATUS CosaDmlEthSetWanStatusForWanAgent(char *ifname, char *WanStatus);
ANSC_STATUS CosaDmlEthCreateEthLink(char *l2ifName, char *Path);
ANSC_STATUS CosaDmlEthDeleteEthLink(char *ifName, char *Path);
ANSC_STATUS CosaDmlEthGetPhyStatusForWanAgent( char *ifname, char *PhyStatus );
ANSC_STATUS CosaDmlEthSetPhyStatusForWanAgent( char *ifname, char *PhyStatus );
#endif //#if defined (FEATURE_RDKB_WAN_MANAGER)

void
Ethernet_Hosts_Sync(void);
INT CosaDmlEth_AssociatedDevice_callback(eth_device_t *eth_dev);
#endif
