package thrift.test;

import org.apache.thrift.TApplicationException;
import org.apache.thrift.TException;
import org.apache.thrift.protocol.TField;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.protocol.TProtocolUtil;
import org.apache.thrift.protocol.TType;

/**
 * .
 * <p/>
 *
 * @author <a href="mailto:oxsean@gmail.com">sean yang</a>
 * @version V1.0, 14-3-9
 */
public class STest {

  public interface Iface {

    public ATest test(String arg0, String arg1) throws Xception, Xception2, TException;
  }

  public static TField[][] _META = {{new TField(TType.STRING, 1), new TField(TType.BYTE, 4)}};

  public static class Client extends org.apache.thrift.TServiceClient implements Iface {

    public Client(TProtocol prot) {
      super(prot, prot);
    }

    @Override
    public ATest test(String arg0, String arg1) throws Xception, Xception2, TException {
      sendBegin("test");
      if (arg0 != null) {
        oprot_.writeFieldBegin(_META[1][1]);
        oprot_.writeString(arg0);
      }
      if (arg1 != null) {
        oprot_.writeFieldBegin(_META[1][2]);
        oprot_.writeString(arg1);
      }
      oprot_.writeFieldStop();
      sendEnd();

      receiveBegin();
      TField schemeField;
      while (true) {
        schemeField = iprot_.readFieldBegin();
        if (schemeField.type == TType.STOP) {
          break;
        }
        switch (schemeField.id) {
          case 0: // SUCCESS
            if (schemeField.type == TType.STRUCT) {
              ATest ret = new ATest();
              ret.read(iprot_);
              return ret;
            } else {
              TProtocolUtil.skip(iprot_, schemeField.type);
            }
            break;
          case 1: // ERR1
            if (schemeField.type == TType.STRUCT) {
              Xception ex1 = new Xception();
              ex1.read(iprot_);
              throw ex1;
            } else {
              TProtocolUtil.skip(iprot_, schemeField.type);
            }
            break;
          default:
            TProtocolUtil.skip(iprot_, schemeField.type);
        }
        iprot_.readFieldEnd();
      }
      receiveEnd();
      throw new TApplicationException(TApplicationException.MISSING_RESULT, "unknown result");
    }
  }
}
