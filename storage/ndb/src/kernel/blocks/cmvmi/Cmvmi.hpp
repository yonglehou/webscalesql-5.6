/*
   Copyright (C) 2003-2007 MySQL AB, 2009 Sun Microsystems, Inc.
    All rights reserved. Use is subject to license terms.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef Cmvmi_H_
#define Cmvmi_H_

#include <pc.hpp>
#include <SimulatedBlock.hpp>
#include <LogLevel.hpp>

#include <DLList.hpp>

/**
 * Cmvmi class
 */
class Cmvmi : public SimulatedBlock {
public:
  Cmvmi(Block_context&);
  virtual ~Cmvmi();
  
private:
  /**
   * These methods used to be reportXXX
   *
   * But they in a nasty way intefere with the execution model
   * they been turned in to exec-Method used via prio A signals
   */
  void execDISCONNECT_REP(Signal*);
  void execCONNECT_REP(Signal*);
  
private:
  BLOCK_DEFINES(Cmvmi);

  // The signal processing functions
  void execNDB_TAMPER(Signal* signal);
  void execSET_LOGLEVELORD(Signal* signal);
  void execEVENT_REP(Signal* signal);
  void execREAD_CONFIG_REQ(Signal* signal);
  void execSTTOR(Signal* signal);
  void execCLOSE_COMREQ(Signal* signal);
  void execENABLE_COMORD(Signal* signal);
  void execOPEN_COMREQ(Signal* signal);
  void execSIZEALT_ACK(Signal* signal);
  void execTEST_ORD(Signal* signal);

  void execSTOP_ORD(Signal* signal);
  void execSTART_ORD(Signal* signal);
  void execTAMPER_ORD(Signal* signal);

  void execDUMP_STATE_ORD(Signal* signal);

  void execEVENT_SUBSCRIBE_REQ(Signal *);
  void cancelSubscription(NodeId nodeId);

  void execTESTSIG(Signal* signal);
  void execNODE_START_REP(Signal* signal);

  void execALLOC_MEM_REF(Signal* signal);
  void execALLOC_MEM_CONF(Signal* signal);
  
  char theErrorMessage[256];
  void sendSTTORRY(Signal* signal);

  LogLevel clogLevel;
  NdbNodeBitmask c_dbNodes;

  /**
   * This struct defines the data needed for a EVENT_REP subscriber
   */
  struct EventRepSubscriber {
    /**
     * What log level is the subscriber using
     */
    LogLevel       logLevel;

    /**
     * What block reference does he use
     *   (Where should the EVENT_REP's be forwarded)
     */
    BlockReference blockRef;

    /**
     * Next ptr (used in pool/list)
     */
    union { Uint32 nextPool; Uint32 nextList; };
    Uint32 prevList;
  };
  typedef Ptr<EventRepSubscriber> SubscriberPtr;
  
  /**
   * Pool of EventRepSubscriber record
   */
  ArrayPool<EventRepSubscriber> subscriberPool;
  
  /**
   * List of current subscribers
   */
  DLList<EventRepSubscriber> subscribers;

private:
  // Declared but not defined
  Cmvmi(const Cmvmi &obj);
  void operator = (const Cmvmi &);

  void startFragmentedSend(Signal* signal, Uint32 variant, Uint32 numSigs, NodeReceiverGroup rg);
  void testNodeFailureCleanupCallback(Signal* signal, Uint32 variant, Uint32 elementsCleaned);
  void testFragmentedCleanup(Signal* signal, SectionHandle* handle, Uint32 testType, Uint32 variant);
  void sendFragmentedComplete(Signal* signal, Uint32 data, Uint32 returnCode);
};

#endif
