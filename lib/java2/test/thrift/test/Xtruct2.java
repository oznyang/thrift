/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package thrift.test;

import org.apache.thrift.EncodingUtils;
import org.apache.thrift.TException;
import org.apache.thrift.scheme.IScheme;
import org.apache.thrift.scheme.SchemeFactory;
import org.apache.thrift.scheme.StandardScheme;

import java.util.HashMap;
import java.util.Map;

public class Xtruct2 implements org.apache.thrift.TBase<Xtruct2, Xtruct2._Fields>, java.io.Serializable, Cloneable {

    private static final org.apache.thrift.protocol.TStruct STRUCT_DESC = new org.apache.thrift.protocol.TStruct("Xtruct2");

    private static final org.apache.thrift.protocol.TField BYTE_THING_FIELD_DESC = new org.apache.thrift.protocol.TField("byte_thing", org.apache.thrift.protocol.TType.BYTE, (short) 1);
    private static final org.apache.thrift.protocol.TField STRUCT_THING_FIELD_DESC = new org.apache.thrift.protocol.TField("struct_thing", org.apache.thrift.protocol.TType.STRUCT, (short) 2);
    private static final org.apache.thrift.protocol.TField I32_THING_FIELD_DESC = new org.apache.thrift.protocol.TField("i32_thing", org.apache.thrift.protocol.TType.I32, (short) 3);

    private static final Map<Class<? extends IScheme>, SchemeFactory> schemes = new HashMap<Class<? extends IScheme>, SchemeFactory>();

    static {
        schemes.put(StandardScheme.class, new Xtruct2StandardSchemeFactory());
    }

    private byte byte_thing; // required
    private Xtruct struct_thing; // required
    private int i32_thing; // required

    public enum _Fields implements org.apache.thrift.TFieldIdEnum {
        ;

        @Override
        public short getThriftFieldId() {
            return 0;
        }

        @Override
        public String getFieldName() {
            return null;
        }
    }

    // isset id assignments
    private static final int __BYTE_THING_ISSET_ID = 0;
    private static final int __I32_THING_ISSET_ID = 1;
    private byte __isset_bitfield = 0;

    public Xtruct2() {
    }

    /**
     * Performs a deep copy on <i>other</i>.
     */
    public Xtruct2(Xtruct2 other) {
        __isset_bitfield = other.__isset_bitfield;
        this.byte_thing = other.byte_thing;
        if (other.isSetStruct_thing()) {
            this.struct_thing = new Xtruct(other.struct_thing);
        }
        this.i32_thing = other.i32_thing;
    }

    public Xtruct2 deepCopy() {
        return new Xtruct2(this);
    }

    public void clear() {
        setByte_thingIsSet(false);
        this.byte_thing = 0;
        this.struct_thing = null;
        setI32_thingIsSet(false);
        this.i32_thing = 0;
    }

    public byte getByte_thing() {
        return this.byte_thing;
    }

    public void setByte_thing(byte byte_thing) {
        this.byte_thing = byte_thing;
        setByte_thingIsSet(true);
    }

    public void unsetByte_thing() {
        __isset_bitfield = EncodingUtils.clearBit(__isset_bitfield, __BYTE_THING_ISSET_ID);
    }

    /**
     * Returns true if field byte_thing is set (has been assigned a value) and false otherwise
     */
    public boolean isSetByte_thing() {
        return EncodingUtils.testBit(__isset_bitfield, __BYTE_THING_ISSET_ID);
    }

    public void setByte_thingIsSet(boolean value) {
        __isset_bitfield = EncodingUtils.setBit(__isset_bitfield, __BYTE_THING_ISSET_ID, value);
    }

    public Xtruct getStruct_thing() {
        return this.struct_thing;
    }

    public void setStruct_thing(Xtruct struct_thing) {
        this.struct_thing = struct_thing;
    }

    public void unsetStruct_thing() {
        this.struct_thing = null;
    }

    /**
     * Returns true if field struct_thing is set (has been assigned a value) and false otherwise
     */
    public boolean isSetStruct_thing() {
        return this.struct_thing != null;
    }

    public void setStruct_thingIsSet(boolean value) {
        if (!value) {
            this.struct_thing = null;
        }
    }

    public int getI32_thing() {
        return this.i32_thing;
    }

    public void setI32_thing(int i32_thing) {
        this.i32_thing = i32_thing;
        setI32_thingIsSet(true);
    }

    public void unsetI32_thing() {
        __isset_bitfield = EncodingUtils.clearBit(__isset_bitfield, __I32_THING_ISSET_ID);
    }

    /**
     * Returns true if field i32_thing is set (has been assigned a value) and false otherwise
     */
    public boolean isSetI32_thing() {
        return EncodingUtils.testBit(__isset_bitfield, __I32_THING_ISSET_ID);
    }

    public void setI32_thingIsSet(boolean value) {
        __isset_bitfield = EncodingUtils.setBit(__isset_bitfield, __I32_THING_ISSET_ID, value);
    }

    @Override
    public boolean isSet(_Fields field) {
        return false;
    }

    @Override
    public Object getFieldValue(_Fields field) {
        return null;
    }

    @Override
    public void setFieldValue(_Fields field, Object value) {
    }

    @Override
    public _Fields fieldForId(int fieldId) {
        return null;
    }

    public int compareTo(Xtruct2 other) {
        return 0;
    }

    public void read(org.apache.thrift.protocol.TProtocol iprot) throws TException {
        schemes.get(iprot.getScheme()).getScheme().read(iprot, this);
    }

    public void write(org.apache.thrift.protocol.TProtocol oprot) throws TException {
        schemes.get(oprot.getScheme()).getScheme().write(oprot, this);
    }

    public void validate() throws TException {
        // check for required fields
        // check for sub-struct validity
        if (struct_thing != null) {
            struct_thing.validate();
        }
    }

    private void writeObject(java.io.ObjectOutputStream out) throws java.io.IOException {
        try {
            write(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(out)));
        } catch (TException te) {
            throw new java.io.IOException(te.getMessage());
        }
    }

    private void readObject(java.io.ObjectInputStream in) throws java.io.IOException, ClassNotFoundException {
        try {
            // it doesn't seem like you should have to do this, but java serialization is wacky, and doesn't call the default constructor.
            __isset_bitfield = 0;
            read(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(in)));
        } catch (TException te) {
            throw new java.io.IOException(te.getMessage());
        }
    }

    private static class Xtruct2StandardSchemeFactory implements SchemeFactory {

        public Xtruct2StandardScheme getScheme() {
            return new Xtruct2StandardScheme();
        }
    }

    private static class Xtruct2StandardScheme extends StandardScheme<Xtruct2> {

        public void read(org.apache.thrift.protocol.TProtocol iprot, Xtruct2 struct) throws TException {
            org.apache.thrift.protocol.TField schemeField;
            iprot.readStructBegin();
            while (true) {
                schemeField = iprot.readFieldBegin();
                if (schemeField.type == org.apache.thrift.protocol.TType.STOP) {
                    break;
                }
                switch (schemeField.id) {
                    case 1: // BYTE_THING
                        if (schemeField.type == org.apache.thrift.protocol.TType.BYTE) {
                            struct.byte_thing = iprot.readByte();
                            struct.setByte_thingIsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 2: // STRUCT_THING
                        if (schemeField.type == org.apache.thrift.protocol.TType.STRUCT) {
                            struct.struct_thing = new Xtruct();
                            struct.struct_thing.read(iprot);
                            struct.setStruct_thingIsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 3: // I32_THING
                        if (schemeField.type == org.apache.thrift.protocol.TType.I32) {
                            struct.i32_thing = iprot.readI32();
                            struct.setI32_thingIsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    default:
                        org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                }
                iprot.readFieldEnd();
            }
            iprot.readStructEnd();
            struct.validate();
        }

        public void write(org.apache.thrift.protocol.TProtocol oprot, Xtruct2 struct) throws TException {
            struct.validate();

            oprot.writeStructBegin(STRUCT_DESC);
            oprot.writeFieldBegin(BYTE_THING_FIELD_DESC);
            oprot.writeByte(struct.byte_thing);
            oprot.writeFieldEnd();
            if (struct.struct_thing != null) {
                oprot.writeFieldBegin(STRUCT_THING_FIELD_DESC);
                struct.struct_thing.write(oprot);
                oprot.writeFieldEnd();
            }
            oprot.writeFieldBegin(I32_THING_FIELD_DESC);
            oprot.writeI32(struct.i32_thing);
            oprot.writeFieldEnd();
            oprot.writeFieldStop();
            oprot.writeStructEnd();
        }

    }

}

