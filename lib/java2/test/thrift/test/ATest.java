package thrift.test;

import org.apache.thrift.TBase;
import org.apache.thrift.TException;
import org.apache.thrift.protocol.*;
import org.apache.thrift.transport.TIOStreamTransport;

import java.io.IOException;

/**
 * .
 * <p/>
 *
 * @author <a href="mailto:oznyang@163.com">oznyang</a>
 * @version V1.0, 14-3-7
 */
public class ATest implements TBase {

    public static TField[] _META = {new TField("string_thing", TType.STRING, (short) 1), new TField("byte_thing", TType.BYTE, (short) 4)};

    private String string_thing;
    private Byte byte_thing;// required

    public String getString_thing() {
        return string_thing;
    }

    public void setString_thing(String string_thing) {
        this.string_thing = string_thing;
    }

    public Byte getByte_thing() {
        return byte_thing;
    }

    public void setByte_thing(Byte byte_thing) {
        this.byte_thing = byte_thing;
    }

    @Override
    public void read(TProtocol iprot) throws TException {
        TField schemeField;
        iprot.readStructBegin();
        while (true) {
            schemeField = iprot.readFieldBegin();
            if (schemeField.type == TType.STOP) {
                break;
            }
            switch (schemeField.id) {
                case 1: // STRING_THING
                    if (schemeField.type == TType.STRING) {
                        string_thing = iprot.readString();
                    } else {
                        TProtocolUtil.skip(iprot, schemeField.type);
                    }
                    break;
                case 4: // BYTE_THING
                    if (schemeField.type == TType.BYTE) {
                        byte_thing = iprot.readByte();
                    } else {
                        TProtocolUtil.skip(iprot, schemeField.type);
                    }
                    break;
                default:
                    TProtocolUtil.skip(iprot, schemeField.type);
            }
            iprot.readFieldEnd();
        }
        iprot.readStructEnd();
        validate();
    }

    @Override
    public void write(TProtocol oprot) throws TException {
        validate();
        if (string_thing != null) {
            oprot.writeFieldBegin(_META[0]);
            oprot.writeString(string_thing);
        }
        if (byte_thing != null) {
            oprot.writeFieldBegin(_META[1]);
            oprot.writeByte(byte_thing);
        }
    }

    public void validate() throws TException {
        if (byte_thing == null) {
            throw new TProtocolException("Required field 'list_field' is unset! Struct:" + toString());
        }
    }

    private void writeObject(java.io.ObjectOutputStream out) throws java.io.IOException {
        try {
            write(new TBinaryProtocol(new TIOStreamTransport(out)));
        } catch (TException te) {
            throw new IOException(te.getMessage());
        }
    }

    private void readObject(java.io.ObjectInputStream in) throws java.io.IOException, ClassNotFoundException {
        try {
            read(new TBinaryProtocol(new TIOStreamTransport(in)));
        } catch (TException te) {
            throw new IOException(te.getMessage());
        }
    }
}
