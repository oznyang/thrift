/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.thrift;

import org.apache.thrift.protocol.TMessage;
import org.apache.thrift.protocol.TMessageType;
import org.apache.thrift.protocol.TProtocol;

/**
 * A TServiceClient is used to communicate with a TService implementation
 * across protocols and transports.
 */
public abstract class TServiceClient {
  public TServiceClient(TProtocol prot) {
    this(prot, prot);
  }

  public TServiceClient(TProtocol iprot, TProtocol oprot) {
    iprot_ = iprot;
    oprot_ = oprot;
  }

  protected TProtocol iprot_;
  protected TProtocol oprot_;

  protected int seqid_;

  /**
   * Get the TProtocol being used as the input (read) protocol.
   * @return the TProtocol being used as the input (read) protocol.
   */
  public TProtocol getInputProtocol() {
    return this.iprot_;
  }

  /**
   * Get the TProtocol being used as the output (write) protocol.
   * @return the TProtocol being used as the output (write) protocol.
   */
  public TProtocol getOutputProtocol() {
    return this.oprot_;
  }

  protected void sendBegin(String methodName) throws TException {
    oprot_.writeMessageBegin(new TMessage(methodName, TMessageType.CALL, ++seqid_));
  }

  protected void sendEnd() throws TException {
    oprot_.writeMessageEnd();
    oprot_.getTransport().flush();
  }

  protected void receiveBegin() throws TException {
    TMessage msg = iprot_.readMessageBegin();
    if (msg.type == TMessageType.EXCEPTION) {
      TApplicationException x = TApplicationException.read(iprot_);
      iprot_.readMessageEnd();
      throw x;
    }
    if (msg.seqid != seqid_) {
      throw new TApplicationException(TApplicationException.BAD_SEQUENCE_ID, "Invoke failed: out of sequence response");
    }
  }

  protected void receiveEnd() throws TException {
    iprot_.readMessageEnd();
  }
}
