/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* This file contains all SCTP related functionality */

#ifndef __DU_SCTP_H__
#define __DU_SCTP_H__

#define DU_SCTP_DOWN 0
#define DU_SCTP_UP 1
#define DU_SCTP_CONNECTING 2
#define MAX_RETRY 5
/*-----*/
/*For du is socket server */
#define MAX_IPV6_LEN 16
#define MAX_L1_SUPPORTED 2
#define MAX_REMOTE_DU_SUPPORTED 1
#define MAX_ASSOC_SUPPORTED (MAX_L1_SUPPORTED + MAX_REMOTE_DU_SUPPORTED)
#define MAX_DU_SUPPORTED 1

/* Global variable declaration */
uint8_t   socket_type;      /* Socket type */
bool nonblocking;      /* Blocking/Non-blocking socket */
bool connUp;           /* Is connection up */
bool pollingState; 
CmInetNetAddrLst localAddrLst;
CmInetNetAddrLst remoteAddrLst;

/*------*/
/*For du is socket server */
/*typedef enum
{
   F1_INTERFACE,
   XN_INTERFACE,
   p5_INTERFACE
}InterfaceType;

typedef enum
{
   SERVER,
   CLIENT
}NodeType;
*/
/*------*/

typedef struct
{
   uint8_t     numFd;            /* Total count number of receivers socket Fd */
   uint16_t    port;              /* Filled by the InetSctpRecvMsg during polling */              
   uint32_t    flag;              /* Refers to the notifyHandler Flag during successful InetSctpRecvMsg*/
   Buffer      *mBuf;             /* Buffer filled during Socket polling*/
   MsgLen      bufLen;            /* Determines the length of the Buffer filled during socket polling */
   CmInetNetAddr addr;              /* Determines the destination port filled during socket polling */
   CmInetFdSet      readFd;         /* Refers to the Read Fd socket */
   CmInetSctpSndRcvInfo   info;     /* Refers to Send Receivers Info*/
   CmInetSctpNotification ntfy;     /* Refers to the sctp Notification Message */
}sctpSockPollParams;

typedef struct
{
   uint8_t               itfState;         /* determines the interface State*/
   uint16_t              destPort;         /* Refers to the destination port of F1/E2 */
   uint16_t              srcPort;          /* Refers to respective src port of DU */
   uint32_t              assocId;          
   bool             recvMsgSet;
   F1IpAddr         destIpAddr;       /* Refers to te destination Ip Address */
   CmInetFd         sockFd;           /* Socket file descriptor */
   CmInetNetAddrLst destAddrLst;      /* Refers to the destinaiton Addr Lst in CmInetNetAddrLst format */
   CmInetNetAddr    destIpNetAddr;    /* Refers to the destination Addr in CmInetNet Addr format */
}DuSctpDestCb;

typedef struct
{
   uint8_t               itfState;         /* determines the interface State*/
   uint16_t              destPort;         /* Refers to the destination port of F1/E2 */
   uint16_t              srcPort;          /* Refers to respective src port of DU */
   uint32_t              assocId;          
   bool             recvMsgSet;
   CmInetFd         sockFd;           /* Socket file descriptor */
}DuSctpLocalCb;


/* Global variable declaration */
DuSctpDestCb f1Params;     /* SCTP configurations at DU */ 
DuSctpDestCb ricParams;    /* SCTP configurations at DU */ 
DuSctpLocalCb p5Params;    /* SCTP configurations at DU */ 

/*-------------------------------------------*/
/*Add sctpcb for du is socket server*/
typedef struct
{
   P5SctpParams     sctpCfg;
   CmInetNetAddrLst localAddrLst;
   CmInetFd         f1LstnSockFd;       /* Listening Socket file descriptor for F1 association */
   CmInetFd         xnLstnSockFd;       /* Listening Socket file descriptor for Xn association */
   CmInetFd         p5LstnSockFd;       /* Listening Socket file descriptor for p5 association */
   //NodeType       localXnNodeType;    /* Local node acts as Server or client while establishing SCTP assoc at Xn interface */
   uint8_t          numAssoc;
   p5SctpAssocCb    assocCb[MAX_ASSOC_SUPPORTED];
}SctpGlobalCb;

SctpGlobalCb sctpCb;


uint8_t sctpActvInit(Ent entity, Inst inst, Region region, Reason reason);
uint8_t sctpActvTsk(Pst *pst, Buffer *mBuf);
void sctpAssocReq();

/*For du is socket server*/
//uint8_t sctpStartReq();
uint8_t sctpservertest(DuSctpLocalCb *paramPtr);

void sendToDuApp(Buffer *mBuf, Event event);
uint8_t sctpSend(Buffer *mBuf, uint8_t itfType);
uint8_t duSctpCfgReq(SctpParams sctpCfg);
uint8_t fillAddrLst(CmInetNetAddrLst *addrLstPtr, F1IpAddr *ipAddr);
uint8_t fillDestNetAddr(CmInetNetAddr *destAddrPtr, F1IpAddr *dstIpPtr);
uint8_t establishReq(DuSctpDestCb *paramPtr);
uint8_t duSctpAssocReq(uint8_t itfType);
uint8_t duFillSctpPst(Pst *pst, Event event);
/*Establishes P5 SCTP connection with peer*/
uint8_t sctpAccept(p5SctpAssocCb *assocCb);
uint8_t sctpSetSockOpts(CmInetFd *sock_Fd);
uint8_t processPolling(sctpSockPollParams *pollParams, CmInetFd *sockFd, uint32_t *timeoutPtr, CmInetMemInfo *memInfo, bool recvMsgSet);
uint8_t sctpSockPoll();
/*For du is socket server*/
uint8_t duP5SctpCfgReq();
/*P5 checks for valid readFd and process the InetSctpRecvMsg during polling*/
uint8_t processP5Polling(sctpSockPollParams *pollParams, p5SctpAssocCb *assocCb, uint32_t *timeoutPtr, CmInetMemInfo *memInfo);
#endif

/**********************************************************************
         End of file
**********************************************************************/
