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

    module: cosa_ethernet_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaEthernetCreate
        *  CosaEthernetInitialize
        *  CosaEthernetRemove
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "cosa_ethernet_internal.h"
#include "cosa_ethernet_apis.h"
#include "ccsp_trace.h"

extern void * g_pDslhDmlAgent;
extern ANSC_HANDLE g_EthObject;

#define LENGTH_MAC_ADDRESS              (18)
#define LENGTH_DELIMITER                (2)

static void CosaEthernetLogger(void);

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaEthernetCreate
            (
            );

    description:

        This function constructs cosa Ethernet object and return handle.

    argument:  

    return:     newly created Ethernet object.

**********************************************************************/

ANSC_HANDLE
CosaEthernetCreate
    (
        VOID
    )
{
    ANSC_STATUS                 returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ETHERNET    pMyObject    = (PCOSA_DATAMODEL_ETHERNET)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_ETHERNET)AnscAllocateMemory(sizeof(COSA_DATAMODEL_ETHERNET));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_ETHERNET_OID;
    pMyObject->Create            = CosaEthernetCreate;
    pMyObject->Remove            = CosaEthernetRemove;
    pMyObject->Initialize        = CosaEthernetInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaEthernetInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Ethernet object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaEthernetInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ETHERNET        pMyObject           = (PCOSA_DATAMODEL_ETHERNET)hThisObject;
    syscfg_init();

    CosaDmlEthGetLogStatus(&pMyObject->LogStatus);
    CosaEthernetLogger();
    CcspHalExtSw_ethAssociatedDevice_callback_register(CosaDmlEth_AssociatedDevice_callback);
    CosaDmlEthWanGetCfg(&pMyObject->EthWanCfg);
    
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaEthernetRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa Ethernet object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaEthernetRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_ETHERNET        pMyObject               = (PCOSA_DATAMODEL_ETHERNET)hThisObject;

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

static int ethGetClientsCount
    (
        CCSP_HAL_ETHSW_PORT PortId,
        LONG num_eth_device,
        eth_device_t *eth_device
        )
{
    int idx;
    int count_client = 0;

    if (!eth_device)
    {
        CcspTraceWarning(("ethGetClientsCount Invalid input Param\n"));
        return 0;
    }

    for (idx = 0; idx < num_eth_device; idx++)
    {
        if (PortId == eth_device[idx].eth_port)
        {
            count_client++;
        }
    }

    return count_client;
}

static void ethGetClientMacDetails
    (
        CCSP_HAL_ETHSW_PORT PortId,
        LONG num_eth_device,
        eth_device_t *eth_device,
        int total_client,
        char *mac
    )
{
    int idx;
    char mac_addr[20];
    char isFirst = TRUE;

    if (!eth_device || !mac)
    {
        CcspTraceWarning(("ethGetClientMacDetails Invalid input Param\n"));
        return;
    }

    for (idx = 0; idx < num_eth_device; idx++)
    {
        if (PortId == eth_device[idx].eth_port)
        {
            _ansc_memset(mac_addr, 0, 20);
            if (isFirst)
            {
                sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                        eth_device[idx].eth_devMacAddress[0],
                        eth_device[idx].eth_devMacAddress[1],
                        eth_device[idx].eth_devMacAddress[2],
                        eth_device[idx].eth_devMacAddress[3],
                        eth_device[idx].eth_devMacAddress[4],
                        eth_device[idx].eth_devMacAddress[5]);
                isFirst = FALSE;
            }
            else
            {
                sprintf(mac_addr, ", %02X:%02X:%02X:%02X:%02X:%02X",
                        eth_device[idx].eth_devMacAddress[0],
                        eth_device[idx].eth_devMacAddress[1],
                        eth_device[idx].eth_devMacAddress[2],
                        eth_device[idx].eth_devMacAddress[3],
                        eth_device[idx].eth_devMacAddress[4],
                        eth_device[idx].eth_devMacAddress[5]);
            }
            strcat(mac, mac_addr);
        }
    }
}

static int ethGetPHYRate
    (
        CCSP_HAL_ETHSW_PORT PortId
    )
{
    INT status                              = RETURN_ERR;
    CCSP_HAL_ETHSW_LINK_RATE LinkRate       = CCSP_HAL_ETHSW_LINK_NULL;
    CCSP_HAL_ETHSW_DUPLEX_MODE DuplexMode   = CCSP_HAL_ETHSW_DUPLEX_Auto;
    INT PHYRate                             = 0;

    status = CcspHalEthSwGetPortCfg(PortId, &LinkRate, &DuplexMode);
    if (RETURN_OK == status)
    {
        switch (LinkRate)
        {
            case CCSP_HAL_ETHSW_LINK_10Mbps:
            {
                PHYRate = 10;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_100Mbps:
            {
                PHYRate = 100;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_1Gbps:
            {
                PHYRate = 1000;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_10Gbps:
            {
                PHYRate = 10000;
                break;
            }
            case CCSP_HAL_ETHSW_LINK_Auto:
            {
                PHYRate = 10000;
                break;
            }
            default:
            {
                PHYRate = 0;
                break;
            }
        }
    }
    return PHYRate;
}

void Ethernet_Log(void)
{
    ULONG total_eth_device = 0;
    eth_device_t *output_struct = NULL;
    int i;
    int mem_size = 0;
    int total_port = 0;
    int count_client = 0;
    char *mac_address = NULL;
    int ret = ANSC_STATUS_FAILURE;

#if defined(_CBR_PRODUCT_REQ_)
    total_port = 8;
#elif defined(_XB6_PRODUCT_REQ_)
    total_port = 2;
#else
    total_port = 4;
#endif

    ret = CcspHalExtSw_getAssociatedDevice(&total_eth_device, &output_struct);
    if (ANSC_STATUS_SUCCESS != ret)
    {
        return;
    }

    //Port number start from 1
    for (i = 1; i <= total_port; i++)
    {
        count_client = ethGetClientsCount(i, total_eth_device, output_struct);
        CcspTraceWarning(("ETH_MAC_%d_TOTAL_COUNT:%d\n", i, count_client));
        if (count_client)
        {
            mem_size = (LENGTH_MAC_ADDRESS + LENGTH_DELIMITER) * count_client;
            mac_address = (char *)AnscAllocateMemory(mem_size);
            if (mac_address)
            {
                _ansc_memset(mac_address, 0, mem_size);
                ethGetClientMacDetails(
                        i,
                        total_eth_device,
                        output_struct,
                        count_client,
                        mac_address);
                CcspTraceWarning(("ETH_MAC_%d:%s\n", i, mac_address));
                CcspTraceWarning(("ETH_PHYRATE_%d:%d\n", i, ethGetPHYRate(i)));

                AnscFreeMemory(mac_address);
                mac_address= NULL;
            }
            else
            {
                CcspTraceWarning(("Alloc Failed\n"));
            }
        }
    }

    if (output_struct)
    {
        AnscFreeMemory(output_struct);
        output_struct= NULL;
    }
}

void * Ethernet_Logger_Thread(void *data)
{
    COSA_DML_ETH_LOG_STATUS log_status = {3600, FALSE};
    LONG timeleft;
    ULONG Log_Period_old;

    pthread_detach(pthread_self());
    sleep(60);

    while(!g_EthObject)
        sleep(10);

    volatile PCOSA_DATAMODEL_ETHERNET pMyObject = (PCOSA_DATAMODEL_ETHERNET)g_EthObject;

    while (1)
    {
        if (pMyObject->LogStatus.Log_Enable)
            Ethernet_Log();

        timeleft = pMyObject->LogStatus.Log_Period;
        while (timeleft > 0)
        {
			Log_Period_old = pMyObject->LogStatus.Log_Period;
			sleep(60);
			timeleft = timeleft - 60 + pMyObject->LogStatus.Log_Period - Log_Period_old;
        }
    }
}

static void CosaEthernetLogger(void)
{
    pthread_t logger_tid;
    int res;

    res = pthread_create(&logger_tid, NULL, Ethernet_Logger_Thread, NULL);
    if (res != 0) 
    {
        AnscTraceWarning(("CosaEthernetInitialize Create Ethernet_Logger_Thread error %d\n", res));
    }
    else
    {
        AnscTraceWarning(("CosaEthernetInitialize Ethernet_Logger_Thread Created\n"));
    }
}

