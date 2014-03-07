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

import java.util.*;

public class LargeDeltas implements org.apache.thrift.TBase<LargeDeltas, LargeDeltas._Fields>, java.io.Serializable, Cloneable {

    private static final org.apache.thrift.protocol.TStruct STRUCT_DESC = new org.apache.thrift.protocol.TStruct("LargeDeltas");

    private static final org.apache.thrift.protocol.TField B1_FIELD_DESC = new org.apache.thrift.protocol.TField("b1", org.apache.thrift.protocol.TType.STRUCT, (short) 1);
    private static final org.apache.thrift.protocol.TField B10_FIELD_DESC = new org.apache.thrift.protocol.TField("b10", org.apache.thrift.protocol.TType.STRUCT, (short) 10);
    private static final org.apache.thrift.protocol.TField B100_FIELD_DESC = new org.apache.thrift.protocol.TField("b100", org.apache.thrift.protocol.TType.STRUCT, (short) 100);
    private static final org.apache.thrift.protocol.TField CHECK_TRUE_FIELD_DESC = new org.apache.thrift.protocol.TField("check_true", org.apache.thrift.protocol.TType.BOOL, (short) 500);
    private static final org.apache.thrift.protocol.TField B1000_FIELD_DESC = new org.apache.thrift.protocol.TField("b1000", org.apache.thrift.protocol.TType.STRUCT, (short) 1000);
    private static final org.apache.thrift.protocol.TField CHECK_FALSE_FIELD_DESC = new org.apache.thrift.protocol.TField("check_false", org.apache.thrift.protocol.TType.BOOL, (short) 1500);
    private static final org.apache.thrift.protocol.TField VERTWO2000_FIELD_DESC = new org.apache.thrift.protocol.TField("vertwo2000", org.apache.thrift.protocol.TType.STRUCT, (short) 2000);
    private static final org.apache.thrift.protocol.TField A_SET2500_FIELD_DESC = new org.apache.thrift.protocol.TField("a_set2500", org.apache.thrift.protocol.TType.SET, (short) 2500);
    private static final org.apache.thrift.protocol.TField VERTWO3000_FIELD_DESC = new org.apache.thrift.protocol.TField("vertwo3000", org.apache.thrift.protocol.TType.STRUCT, (short) 3000);
    private static final org.apache.thrift.protocol.TField BIG_NUMBERS_FIELD_DESC = new org.apache.thrift.protocol.TField("big_numbers", org.apache.thrift.protocol.TType.LIST, (short) 4000);

    private static final Map<Class<? extends IScheme>, SchemeFactory> schemes = new HashMap<Class<? extends IScheme>, SchemeFactory>();

    static {
        schemes.put(StandardScheme.class, new LargeDeltasStandardSchemeFactory());
    }

    private Bools b1; // required
    private Bools b10; // required
    private Bools b100; // required
    private boolean check_true; // required
    private Bools b1000; // required
    private boolean check_false; // required
    private VersioningTestV2 vertwo2000; // required
    private Set<String> a_set2500; // required
    private VersioningTestV2 vertwo3000; // required
    private List<Integer> big_numbers; // required

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
    private static final int __CHECK_TRUE_ISSET_ID = 0;
    private static final int __CHECK_FALSE_ISSET_ID = 1;
    private byte __isset_bitfield = 0;

    public LargeDeltas() {
    }

    /**
     * Performs a deep copy on <i>other</i>.
     */
    public LargeDeltas(LargeDeltas other) {
        __isset_bitfield = other.__isset_bitfield;
        if (other.isSetB1()) {
            this.b1 = new Bools(other.b1);
        }
        if (other.isSetB10()) {
            this.b10 = new Bools(other.b10);
        }
        if (other.isSetB100()) {
            this.b100 = new Bools(other.b100);
        }
        this.check_true = other.check_true;
        if (other.isSetB1000()) {
            this.b1000 = new Bools(other.b1000);
        }
        this.check_false = other.check_false;
        if (other.isSetVertwo2000()) {
            this.vertwo2000 = new VersioningTestV2(other.vertwo2000);
        }
        if (other.isSetA_set2500()) {
            Set<String> __this__a_set2500 = new HashSet<String>();
            for (String other_element : other.a_set2500) {
                __this__a_set2500.add(other_element);
            }
            this.a_set2500 = __this__a_set2500;
        }
        if (other.isSetVertwo3000()) {
            this.vertwo3000 = new VersioningTestV2(other.vertwo3000);
        }
        if (other.isSetBig_numbers()) {
            List<Integer> __this__big_numbers = new ArrayList<Integer>();
            for (Integer other_element : other.big_numbers) {
                __this__big_numbers.add(other_element);
            }
            this.big_numbers = __this__big_numbers;
        }
    }

    public LargeDeltas deepCopy() {
        return new LargeDeltas(this);
    }

    public void clear() {
        this.b1 = null;
        this.b10 = null;
        this.b100 = null;
        setCheck_trueIsSet(false);
        this.check_true = false;
        this.b1000 = null;
        setCheck_falseIsSet(false);
        this.check_false = false;
        this.vertwo2000 = null;
        this.a_set2500 = null;
        this.vertwo3000 = null;
        this.big_numbers = null;
    }

    public Bools getB1() {
        return this.b1;
    }

    public void setB1(Bools b1) {
        this.b1 = b1;
    }

    public void unsetB1() {
        this.b1 = null;
    }

    /**
     * Returns true if field b1 is set (has been assigned a value) and false otherwise
     */
    public boolean isSetB1() {
        return this.b1 != null;
    }

    public void setB1IsSet(boolean value) {
        if (!value) {
            this.b1 = null;
        }
    }

    public Bools getB10() {
        return this.b10;
    }

    public void setB10(Bools b10) {
        this.b10 = b10;
    }

    public void unsetB10() {
        this.b10 = null;
    }

    /**
     * Returns true if field b10 is set (has been assigned a value) and false otherwise
     */
    public boolean isSetB10() {
        return this.b10 != null;
    }

    public void setB10IsSet(boolean value) {
        if (!value) {
            this.b10 = null;
        }
    }

    public Bools getB100() {
        return this.b100;
    }

    public void setB100(Bools b100) {
        this.b100 = b100;
    }

    public void unsetB100() {
        this.b100 = null;
    }

    /**
     * Returns true if field b100 is set (has been assigned a value) and false otherwise
     */
    public boolean isSetB100() {
        return this.b100 != null;
    }

    public void setB100IsSet(boolean value) {
        if (!value) {
            this.b100 = null;
        }
    }

    public boolean isCheck_true() {
        return this.check_true;
    }

    public void setCheck_true(boolean check_true) {
        this.check_true = check_true;
        setCheck_trueIsSet(true);
    }

    public void unsetCheck_true() {
        __isset_bitfield = EncodingUtils.clearBit(__isset_bitfield, __CHECK_TRUE_ISSET_ID);
    }

    /**
     * Returns true if field check_true is set (has been assigned a value) and false otherwise
     */
    public boolean isSetCheck_true() {
        return EncodingUtils.testBit(__isset_bitfield, __CHECK_TRUE_ISSET_ID);
    }

    public void setCheck_trueIsSet(boolean value) {
        __isset_bitfield = EncodingUtils.setBit(__isset_bitfield, __CHECK_TRUE_ISSET_ID, value);
    }

    public Bools getB1000() {
        return this.b1000;
    }

    public void setB1000(Bools b1000) {
        this.b1000 = b1000;
    }

    public void unsetB1000() {
        this.b1000 = null;
    }

    /**
     * Returns true if field b1000 is set (has been assigned a value) and false otherwise
     */
    public boolean isSetB1000() {
        return this.b1000 != null;
    }

    public void setB1000IsSet(boolean value) {
        if (!value) {
            this.b1000 = null;
        }
    }

    public boolean isCheck_false() {
        return this.check_false;
    }

    public void setCheck_false(boolean check_false) {
        this.check_false = check_false;
        setCheck_falseIsSet(true);
    }

    public void unsetCheck_false() {
        __isset_bitfield = EncodingUtils.clearBit(__isset_bitfield, __CHECK_FALSE_ISSET_ID);
    }

    /**
     * Returns true if field check_false is set (has been assigned a value) and false otherwise
     */
    public boolean isSetCheck_false() {
        return EncodingUtils.testBit(__isset_bitfield, __CHECK_FALSE_ISSET_ID);
    }

    public void setCheck_falseIsSet(boolean value) {
        __isset_bitfield = EncodingUtils.setBit(__isset_bitfield, __CHECK_FALSE_ISSET_ID, value);
    }

    public VersioningTestV2 getVertwo2000() {
        return this.vertwo2000;
    }

    public void setVertwo2000(VersioningTestV2 vertwo2000) {
        this.vertwo2000 = vertwo2000;
    }

    public void unsetVertwo2000() {
        this.vertwo2000 = null;
    }

    /**
     * Returns true if field vertwo2000 is set (has been assigned a value) and false otherwise
     */
    public boolean isSetVertwo2000() {
        return this.vertwo2000 != null;
    }

    public void setVertwo2000IsSet(boolean value) {
        if (!value) {
            this.vertwo2000 = null;
        }
    }

    public int getA_set2500Size() {
        return (this.a_set2500 == null) ? 0 : this.a_set2500.size();
    }

    public java.util.Iterator<String> getA_set2500Iterator() {
        return (this.a_set2500 == null) ? null : this.a_set2500.iterator();
    }

    public void addToA_set2500(String elem) {
        if (this.a_set2500 == null) {
            this.a_set2500 = new HashSet<String>();
        }
        this.a_set2500.add(elem);
    }

    public Set<String> getA_set2500() {
        return this.a_set2500;
    }

    public void setA_set2500(Set<String> a_set2500) {
        this.a_set2500 = a_set2500;
    }

    public void unsetA_set2500() {
        this.a_set2500 = null;
    }

    /**
     * Returns true if field a_set2500 is set (has been assigned a value) and false otherwise
     */
    public boolean isSetA_set2500() {
        return this.a_set2500 != null;
    }

    public void setA_set2500IsSet(boolean value) {
        if (!value) {
            this.a_set2500 = null;
        }
    }

    public VersioningTestV2 getVertwo3000() {
        return this.vertwo3000;
    }

    public void setVertwo3000(VersioningTestV2 vertwo3000) {
        this.vertwo3000 = vertwo3000;
    }

    public void unsetVertwo3000() {
        this.vertwo3000 = null;
    }

    /**
     * Returns true if field vertwo3000 is set (has been assigned a value) and false otherwise
     */
    public boolean isSetVertwo3000() {
        return this.vertwo3000 != null;
    }

    public void setVertwo3000IsSet(boolean value) {
        if (!value) {
            this.vertwo3000 = null;
        }
    }

    public int getBig_numbersSize() {
        return (this.big_numbers == null) ? 0 : this.big_numbers.size();
    }

    public java.util.Iterator<Integer> getBig_numbersIterator() {
        return (this.big_numbers == null) ? null : this.big_numbers.iterator();
    }

    public void addToBig_numbers(int elem) {
        if (this.big_numbers == null) {
            this.big_numbers = new ArrayList<Integer>();
        }
        this.big_numbers.add(elem);
    }

    public List<Integer> getBig_numbers() {
        return this.big_numbers;
    }

    public void setBig_numbers(List<Integer> big_numbers) {
        this.big_numbers = big_numbers;
    }

    public void unsetBig_numbers() {
        this.big_numbers = null;
    }

    /**
     * Returns true if field big_numbers is set (has been assigned a value) and false otherwise
     */
    public boolean isSetBig_numbers() {
        return this.big_numbers != null;
    }

    public void setBig_numbersIsSet(boolean value) {
        if (!value) {
            this.big_numbers = null;
        }
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

    public int compareTo(LargeDeltas other) {
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
        if (b1 != null) {
            b1.validate();
        }
        if (b10 != null) {
            b10.validate();
        }
        if (b100 != null) {
            b100.validate();
        }
        if (b1000 != null) {
            b1000.validate();
        }
        if (vertwo2000 != null) {
            vertwo2000.validate();
        }
        if (vertwo3000 != null) {
            vertwo3000.validate();
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

    private static class LargeDeltasStandardSchemeFactory implements SchemeFactory {

        public LargeDeltasStandardScheme getScheme() {
            return new LargeDeltasStandardScheme();
        }
    }

    private static class LargeDeltasStandardScheme extends StandardScheme<LargeDeltas> {

        public void read(org.apache.thrift.protocol.TProtocol iprot, LargeDeltas struct) throws TException {
            org.apache.thrift.protocol.TField schemeField;
            iprot.readStructBegin();
            while (true) {
                schemeField = iprot.readFieldBegin();
                if (schemeField.type == org.apache.thrift.protocol.TType.STOP) {
                    break;
                }
                switch (schemeField.id) {
                    case 1: // B1
                        if (schemeField.type == org.apache.thrift.protocol.TType.STRUCT) {
                            struct.b1 = new Bools();
                            struct.b1.read(iprot);
                            struct.setB1IsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 10: // B10
                        if (schemeField.type == org.apache.thrift.protocol.TType.STRUCT) {
                            struct.b10 = new Bools();
                            struct.b10.read(iprot);
                            struct.setB10IsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 100: // B100
                        if (schemeField.type == org.apache.thrift.protocol.TType.STRUCT) {
                            struct.b100 = new Bools();
                            struct.b100.read(iprot);
                            struct.setB100IsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 500: // CHECK_TRUE
                        if (schemeField.type == org.apache.thrift.protocol.TType.BOOL) {
                            struct.check_true = iprot.readBool();
                            struct.setCheck_trueIsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 1000: // B1000
                        if (schemeField.type == org.apache.thrift.protocol.TType.STRUCT) {
                            struct.b1000 = new Bools();
                            struct.b1000.read(iprot);
                            struct.setB1000IsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 1500: // CHECK_FALSE
                        if (schemeField.type == org.apache.thrift.protocol.TType.BOOL) {
                            struct.check_false = iprot.readBool();
                            struct.setCheck_falseIsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 2000: // VERTWO2000
                        if (schemeField.type == org.apache.thrift.protocol.TType.STRUCT) {
                            struct.vertwo2000 = new VersioningTestV2();
                            struct.vertwo2000.read(iprot);
                            struct.setVertwo2000IsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 2500: // A_SET2500
                        if (schemeField.type == org.apache.thrift.protocol.TType.SET) {
                            {
                                org.apache.thrift.protocol.TSet _set70 = iprot.readSetBegin();
                                struct.a_set2500 = new HashSet<String>(2 * _set70.size);
                                for (int _i71 = 0; _i71 < _set70.size; ++_i71) {
                                    String _elem72; // required
                                    _elem72 = iprot.readString();
                                    struct.a_set2500.add(_elem72);
                                }
                                iprot.readSetEnd();
                            }
                            struct.setA_set2500IsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 3000: // VERTWO3000
                        if (schemeField.type == org.apache.thrift.protocol.TType.STRUCT) {
                            struct.vertwo3000 = new VersioningTestV2();
                            struct.vertwo3000.read(iprot);
                            struct.setVertwo3000IsSet(true);
                        } else {
                            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
                        }
                        break;
                    case 4000: // BIG_NUMBERS
                        if (schemeField.type == org.apache.thrift.protocol.TType.LIST) {
                            {
                                org.apache.thrift.protocol.TList _list73 = iprot.readListBegin();
                                struct.big_numbers = new ArrayList<Integer>(_list73.size);
                                for (int _i74 = 0; _i74 < _list73.size; ++_i74) {
                                    int _elem75; // required
                                    _elem75 = iprot.readI32();
                                    struct.big_numbers.add(_elem75);
                                }
                                iprot.readListEnd();
                            }
                            struct.setBig_numbersIsSet(true);
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

        public void write(org.apache.thrift.protocol.TProtocol oprot, LargeDeltas struct) throws TException {
            struct.validate();

            oprot.writeStructBegin(STRUCT_DESC);
            if (struct.b1 != null) {
                oprot.writeFieldBegin(B1_FIELD_DESC);
                struct.b1.write(oprot);
                oprot.writeFieldEnd();
            }
            if (struct.b10 != null) {
                oprot.writeFieldBegin(B10_FIELD_DESC);
                struct.b10.write(oprot);
                oprot.writeFieldEnd();
            }
            if (struct.b100 != null) {
                oprot.writeFieldBegin(B100_FIELD_DESC);
                struct.b100.write(oprot);
                oprot.writeFieldEnd();
            }
            oprot.writeFieldBegin(CHECK_TRUE_FIELD_DESC);
            oprot.writeBool(struct.check_true);
            oprot.writeFieldEnd();
            if (struct.b1000 != null) {
                oprot.writeFieldBegin(B1000_FIELD_DESC);
                struct.b1000.write(oprot);
                oprot.writeFieldEnd();
            }
            oprot.writeFieldBegin(CHECK_FALSE_FIELD_DESC);
            oprot.writeBool(struct.check_false);
            oprot.writeFieldEnd();
            if (struct.vertwo2000 != null) {
                oprot.writeFieldBegin(VERTWO2000_FIELD_DESC);
                struct.vertwo2000.write(oprot);
                oprot.writeFieldEnd();
            }
            if (struct.a_set2500 != null) {
                oprot.writeFieldBegin(A_SET2500_FIELD_DESC);
                {
                    oprot.writeSetBegin(new org.apache.thrift.protocol.TSet(org.apache.thrift.protocol.TType.STRING, struct.a_set2500.size()));
                    for (String _iter76 : struct.a_set2500) {
                        oprot.writeString(_iter76);
                    }
                    oprot.writeSetEnd();
                }
                oprot.writeFieldEnd();
            }
            if (struct.vertwo3000 != null) {
                oprot.writeFieldBegin(VERTWO3000_FIELD_DESC);
                struct.vertwo3000.write(oprot);
                oprot.writeFieldEnd();
            }
            if (struct.big_numbers != null) {
                oprot.writeFieldBegin(BIG_NUMBERS_FIELD_DESC);
                {
                    oprot.writeListBegin(new org.apache.thrift.protocol.TList(org.apache.thrift.protocol.TType.I32, struct.big_numbers.size()));
                    for (int _iter77 : struct.big_numbers) {
                        oprot.writeI32(_iter77);
                    }
                    oprot.writeListEnd();
                }
                oprot.writeFieldEnd();
            }
            oprot.writeFieldStop();
            oprot.writeStructEnd();
        }

    }

}

