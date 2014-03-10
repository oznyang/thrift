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

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>

#include <sys/stat.h>
#include <stdexcept>

#include "platform.h"
#include "t_oop_generator.h"

using namespace std;


/**
 * java2 code generator
 * @author <a href="mailto:oxsean@gmail.com">sean yang</a>
 * @version V1.0, 14-3-8
 */
class t_java2_generator : public t_oop_generator {
public:
  t_java2_generator(
    t_program* program, const std::map<std::string, std::string> &parsed_options, const std::string &option_string)
  : t_oop_generator(program) {
    (void) parsed_options;
    (void) option_string;
    std::map<std::string, std::string>::const_iterator iter;
    out_dir_base_ = "gen-java2";
  }

  /**
   * Init and close methods
   */
  void init_generator();

  void close_generator();

  void generate_consts(std::vector<t_const*> consts);

  /**
   * Program-level generation functions
   */
  void generate_typedef(t_typedef* ttypedef);

  void generate_enum(t_enum* tenum);

  void generate_struct(t_struct* tstruct);

  void generate_xception(t_struct* txception);

  void generate_service(t_service* tservice);

  void print_const_value(std::ofstream &out, std::string name, t_type* type, t_const_value* value, bool in_static, bool defval = false);

  std::string render_const_value(std::ofstream &out, t_type* type, t_const_value* value);

  /**
   * Service-level generation functions
   */
  void generate_java_struct(t_struct* tstruct, bool is_exception);

  void generate_java_struct_definition(std::ofstream &out, t_struct* tstruct, bool is_xception = false, bool in_class = false, bool is_result = false);

  void generate_java_struct_reader(std::ofstream &out, t_struct* tstruct);

  void generate_java_validator(std::ofstream &out, t_struct* tstruct);

  void generate_java_struct_writer(std::ofstream &out, t_struct* tstruct);

  void generate_java_struct_write_object(std::ofstream &out, t_struct* tstruct);

  void generate_java_struct_read_object(std::ofstream &out, t_struct* tstruct);

  std::string get_java_type_string(t_type* type);

  void generate_java_bean_boilerplate(std::ofstream &out, t_struct* tstruct);

  std::string get_cap_name(std::string name);

  void generate_service_interface(t_service* tservice);

  void generate_service_async_interface(t_service* tservice);

  void generate_service_meta(t_service* tservice);

  void generate_service_client(t_service* tservice);

  /**
   * Serialization constructs
   */
  void generate_deserialize_field(std::ofstream &out, t_field* tfield, std::string prefix = "");

  void generate_deserialize_struct(std::ofstream &out, t_struct* tstruct, std::string prefix = "");

  void generate_deserialize_container(std::ofstream &out, t_type* ttype, std::string prefix = "");

  void generate_deserialize_set_element(std::ofstream &out, t_set* tset, std::string prefix = "");

  void generate_deserialize_map_element(std::ofstream &out, t_map* tmap, std::string prefix = "");

  void generate_deserialize_list_element(std::ofstream &out, t_list* tlist, std::string prefix = "");

  void generate_serialize_field(std::ofstream &out, t_field* tfield, std::string prefix = "");

  void generate_serialize_struct(std::ofstream &out, t_struct* tstruct, std::string prefix = "");

  void generate_serialize_container(std::ofstream &out, t_type* ttype, std::string prefix = "");

  void generate_serialize_map_element(std::ofstream &out, t_map* tmap, std::string iter, std::string map);

  void generate_serialize_set_element(std::ofstream &out, t_set* tmap, std::string iter);

  void generate_serialize_list_element(std::ofstream &out, t_list* tlist, std::string iter);

  void generate_java_doc(std::ofstream &out, t_field* field);

  void generate_java_doc(std::ofstream &out, t_doc* tdoc);

  void generate_java_doc(std::ofstream &out, t_function* tdoc);

  void generate_java_docstring_comment(std::ofstream &out, string contents);

  /**
   * Helper rendering functions
   */
  std::string java_package();

  std::string java_type_imports();

  std::string java_thrift_imports();

  std::string type_name(t_type* ttype, bool in_container = false, bool in_init = false, bool skip_generic = false);

  std::string base_type_name(t_base_type* tbase, bool in_container = false);

  std::string declare_field(t_field* tfield, bool init = false);

  std::string function_signature(t_function* tfunction, std::string prefix = "");

  std::string function_signature_async(t_function* tfunction, bool use_base_method = false, std::string prefix = "");

  std::string argument_list(t_struct* tstruct, bool include_types = true);

  std::string async_argument_list(t_function* tfunc, bool use_base_method = true, bool include_types = true);

  std::string type_to_enum(t_type* ttype);

  std::string get_enum_class_name(t_type* type);

  void generate_field_descs(ofstream &out, t_struct* tstruct);

  bool type_can_be_null(t_type* ttype) {
    ttype = get_true_type(ttype);

    return
      ttype->is_container() ||
        ttype->is_struct() ||
        ttype->is_xception() ||
        ttype->is_string() ||
        ttype->is_enum();
  }

  std::string constant_name(std::string name);

private:

  /**
   * File streams
   */
  std::string package_name_;
  std::ofstream f_service_;
  std::string package_dir_;

};


/**
 * Prepares for file generation by opening up the necessary file output
 * streams.
 *
 * @param tprogram The program to generate
 */
void t_java2_generator::init_generator() {
  // Make output directory
  MKDIR(get_out_dir().c_str());
  package_name_ = program_->get_namespace("java");

  string dir = package_name_;
  string subdir = get_out_dir();
  string::size_type loc;
  while ((loc = dir.find(".")) != string::npos) {
    subdir = subdir + "/" + dir.substr(0, loc);
    MKDIR(subdir.c_str());
    dir = dir.substr(loc + 1);
  }
  if (dir.size() > 0) {
    subdir = subdir + "/" + dir;
    MKDIR(subdir.c_str());
  }

  package_dir_ = subdir;
}

/**
 * Packages the generated file
 *
 * @return String of the package, i.e. "package org.apache.thriftdemo;"
 */
string t_java2_generator::java_package() {
  if (!package_name_.empty()) {
    return string("package ") + package_name_ + ";\n\n";
  }
  return "";
}

/**
 * Prints standard java imports
 *
 * @return List of imports for Java types that are used in here
 */
string t_java2_generator::java_type_imports() {
  return
    string() +
      "import java.util.*;\n" +
      "import java.nio.ByteBuffer;\n\n";
}

/**
 * Prints standard java imports
 *
 * @return List of imports necessary for thrift
 */
string t_java2_generator::java_thrift_imports() {
  return
    string() +
      "import org.apache.thrift.*;\n" +
      "import org.apache.thrift.transport.*;\n" +
      "import org.apache.thrift.protocol.*;\n\n";
}

/**
 * Nothing in Java
 */
void t_java2_generator::close_generator() {
}

/**
 * Generates a typedef. This is not done in Java, since it does
 * not support arbitrary name replacements, and it'd be a wacky waste
 * of overhead to make wrapper classes.
 *
 * @param ttypedef The type definition
 */
void t_java2_generator::generate_typedef(t_typedef* ttypedef) {
  (void) ttypedef;
}

/**
 * Enums are a class with a set of static constants.
 *
 * @param tenum The enumeration
 */
void t_java2_generator::generate_enum(t_enum* tenum) {
  // Make output file
  string f_enum_name = package_dir_ + "/" + (tenum->get_name()) + ".java";
  ofstream f_enum;
  f_enum.open(f_enum_name.c_str());

  // Comment and package it
  f_enum <<
    autogen_comment() <<
    java_package() << endl;

  // Add java imports
  f_enum << string() +
    "import java.util.Map;\n" +
    "import java.util.HashMap;\n" +
    "import org.apache.thrift.TEnum;" << endl << endl;

  generate_java_doc(f_enum, tenum);
  indent(f_enum) <<
    "public enum " << tenum->get_name() << " implements TEnum ";
  scope_up(f_enum);

  vector<t_enum_value*> constants = tenum->get_constants();
  vector<t_enum_value*>::iterator c_iter;
  bool first = true;
  for (c_iter = constants.begin(); c_iter != constants.end(); ++c_iter) {
    int value = (*c_iter)->get_value();

    if (first) {
      first = false;
    } else {
      f_enum << "," << endl;
    }

    generate_java_doc(f_enum, *c_iter);
    indent(f_enum) << (*c_iter)->get_name() << "(" << value << ")";
  }
  f_enum << ";" << endl << endl;

  // Field for thriftCode
  indent(f_enum) << "private final int value;" << endl << endl;

  indent(f_enum) << "private " << tenum->get_name() << "(int value) {" << endl;
  indent(f_enum) << "  this.value = value;" << endl;
  indent(f_enum) << "}" << endl << endl;

  indent(f_enum) << "public int getValue() {" << endl;
  indent(f_enum) << "  return value;" << endl;
  indent(f_enum) << "}" << endl << endl;

  indent(f_enum) << "public static " + tenum->get_name() + " findByValue(int value) { " << endl;

  indent_up();

  indent(f_enum) << "switch (value) {" << endl;
  indent_up();

  for (c_iter = constants.begin(); c_iter != constants.end(); ++c_iter) {
    int value = (*c_iter)->get_value();
    indent(f_enum) << "case " << value << ":" << endl;
    indent(f_enum) << "  return " << (*c_iter)->get_name() << ";" << endl;
  }

  indent(f_enum) << "default:" << endl;
  indent(f_enum) << "  return null;" << endl;

  indent_down();

  indent(f_enum) << "}" << endl;

  indent_down();

  indent(f_enum) << "}" << endl;

  scope_down(f_enum);

  f_enum.close();
}

/**
 * Generates a class that holds all the constants.
 */
void t_java2_generator::generate_consts(std::vector<t_const*> consts) {
  if (consts.empty()) {
    return;
  }

  string f_consts_name = package_dir_ + '/' + program_name_ + "Constants.java";
  ofstream f_consts;
  f_consts.open(f_consts_name.c_str());

  // Print header
  f_consts <<
    autogen_comment() <<
    java_package() <<
    java_type_imports();

  f_consts <<
    "public class " << program_name_ << "Constants {" << endl <<
    endl;
  indent_up();
  vector<t_const*>::iterator c_iter;
  for (c_iter = consts.begin(); c_iter != consts.end(); ++c_iter) {
    generate_java_doc(f_consts, (*c_iter));
    print_const_value(f_consts, (*c_iter)->get_name(), (*c_iter)->get_type(), (*c_iter)->get_value(), false);
  }
  indent_down();
  indent(f_consts) << "}" << endl;
  f_consts.close();
}


/**
 * Prints the value of a constant with the given type. Note that type checking
 * is NOT performed in this function as it is always run beforehand using the
 * validate_types method in main.cc
 */
void t_java2_generator::print_const_value(std::ofstream &out, string name, t_type* type, t_const_value* value, bool in_static, bool defval) {
  type = get_true_type(type);

  indent(out);
  if (!defval) {
    out <<
      (in_static ? "" : "public static final ") <<
      type_name(type) << " ";
  }
  if (type->is_base_type()) {
    string v2 = render_const_value(out, type, value);
    out << name << " = " << v2 << ";" << endl << endl;
  } else if (type->is_enum()) {
    out << name << " = " << render_const_value(out, type, value) << ";" << endl << endl;
  } else if (type->is_struct() || type->is_xception()) {
    const vector<t_field*> &fields = ((t_struct*) type)->get_members();
    vector<t_field*>::const_iterator f_iter;
    const map<t_const_value*, t_const_value*> &val = value->get_map();
    map<t_const_value*, t_const_value*>::const_iterator v_iter;
    out << name << " = new " << type_name(type, false, true) << "();" << endl;
    if (!in_static) {
      indent(out) << "static {" << endl;
      indent_up();
    }
    for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
      t_type* field_type = NULL;
      for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        if ((*f_iter)->get_name() == v_iter->first->get_string()) {
          field_type = (*f_iter)->get_type();
        }
      }
      if (field_type == NULL) {
        throw "type error: " + type->get_name() + " has no field " + v_iter->first->get_string();
      }
      string val = render_const_value(out, field_type, v_iter->second);
      indent(out) << name << ".";
      std::string cap_name = get_cap_name(v_iter->first->get_string());
      out << "set" << cap_name << "(" << val << ");" << endl;
    }
    if (!in_static) {
      indent_down();
      indent(out) << "}" << endl;
    }
    out << endl;
  } else if (type->is_map()) {
    out << name << " = new " << type_name(type, false, true) << "();" << endl;
    if (!in_static) {
      indent(out) << "static {" << endl;
      indent_up();
    }
    t_type* ktype = ((t_map*) type)->get_key_type();
    t_type* vtype = ((t_map*) type)->get_val_type();
    const map<t_const_value*, t_const_value*> &val = value->get_map();
    map<t_const_value*, t_const_value*>::const_iterator v_iter;
    for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
      string key = render_const_value(out, ktype, v_iter->first);
      string val = render_const_value(out, vtype, v_iter->second);
      indent(out) << name << ".put(" << key << ", " << val << ");" << endl;
    }
    if (!in_static) {
      indent_down();
      indent(out) << "}" << endl;
    }
    out << endl;
  } else if (type->is_list() || type->is_set()) {
    out << name << " = new " << type_name(type, false, true) << "();" << endl;
    if (!in_static) {
      indent(out) << "static {" << endl;
      indent_up();
    }
    t_type* etype;
    if (type->is_list()) {
      etype = ((t_list*) type)->get_elem_type();
    } else {
      etype = ((t_set*) type)->get_elem_type();
    }
    const vector<t_const_value*> &val = value->get_list();
    vector<t_const_value*>::const_iterator v_iter;
    for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
      string val = render_const_value(out, etype, *v_iter);
      indent(out) << name << ".add(" << val << ");" << endl;
    }
    if (!in_static) {
      indent_down();
      indent(out) << "}" << endl;
    }
    out << endl;
  } else {
    throw "compiler error: no const of type " + type->get_name();
  }
}

string t_java2_generator::render_const_value(ofstream &out, t_type* type, t_const_value* value) {
  type = get_true_type(type);
  std::ostringstream render;

  if (type->is_base_type()) {
    t_base_type::t_base tbase = ((t_base_type*) type)->get_base();
    switch (tbase) {
      case t_base_type::TYPE_STRING:
        render << '"' << get_escaped_string(value) << '"';
        break;
      case t_base_type::TYPE_BOOL:
        render << ((value->get_integer() > 0) ? "true" : "false");
        break;
      case t_base_type::TYPE_BYTE:
        render << "(byte)" << value->get_integer();
        break;
      case t_base_type::TYPE_I16:
        render << "(short)" << value->get_integer();
        break;
      case t_base_type::TYPE_I32:
        render << value->get_integer();
        break;
      case t_base_type::TYPE_I64:
        render << value->get_integer() << "L";
        break;
      case t_base_type::TYPE_DOUBLE:
        if (value->get_type() == t_const_value::CV_INTEGER) {
          render << "(double)" << value->get_integer();
        } else {
          render << value->get_double();
        }
        break;
      default:
        throw "compiler error: no const of base type " + t_base_type::t_base_name(tbase);
    }
  } else if (type->is_enum()) {
    render << type->get_program()->get_namespace("java") << "." << value->get_identifier_with_parent();
  } else {
    string t = tmp("tmp");
    print_const_value(out, t, type, value, true);
    render << t;
  }

  return render.str();
}

/**
 * Generates a struct definition for a thrift data type. This will be a TBase 
 * implementor.
 *
 * @param tstruct The struct definition
 */
void t_java2_generator::generate_struct(t_struct* tstruct) {
  generate_java_struct(tstruct, false);
}

/**
 * Exceptions are structs, but they inherit from Exception
 *
 * @param tstruct The struct definition
 */
void t_java2_generator::generate_xception(t_struct* txception) {
  generate_java_struct(txception, true);
}


/**
 * Java struct definition.
 *
 * @param tstruct The struct definition
 */
void t_java2_generator::generate_java_struct(t_struct* tstruct, bool is_exception) {
  // Make output file
  string f_struct_name = package_dir_ + "/" + (tstruct->get_name()) + ".java";
  ofstream f_struct;
  f_struct.open(f_struct_name.c_str());

  f_struct <<
    autogen_comment() <<
    java_package() <<
    java_type_imports() <<
    java_thrift_imports();

  generate_java_struct_definition(f_struct, tstruct, is_exception);
  f_struct.close();
}

/**
 * Java struct definition. This has various parameters, as it could be
 * generated standalone or inside another class as a helper. If it
 * is a helper than it is a static class.
 *
 * @param tstruct      The struct definition
 * @param is_exception Is this an exception?
 * @param in_class     If inside a class, needs to be static class
 * @param is_result    If this is a result it needs a different writer
 */
void t_java2_generator::generate_java_struct_definition(ofstream &out, t_struct* tstruct, bool is_exception, bool in_class, bool is_result) {
  generate_java_doc(out, tstruct);

  bool is_final = (tstruct->annotations_.find("final") != tstruct->annotations_.end());

  indent(out) <<
    "public " << (is_final ? "final " : "") <<
    (in_class ? "static " : "") << "class " << tstruct->get_name() << " ";

  if (is_exception) {
    out << "extends TException ";
  } else {
    out << "implements TBase ";
  }

  scope_up(out);

  // Members are public for -java, private for -javabean
  const vector<t_field*> &members = tstruct->get_members();
  vector<t_field*>::const_iterator m_iter;

  out << endl;

  generate_field_descs(out, tstruct);

  out << endl;

  for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
    indent(out) << "private ";
    out << declare_field(*m_iter, false) << endl;
  }

  out << endl;

  generate_java_bean_boilerplate(out, tstruct);

  generate_java_validator(out, tstruct);

  generate_java_struct_reader(out, tstruct);

  generate_java_struct_writer(out, tstruct);

  generate_java_struct_read_object(out, tstruct);
  generate_java_struct_write_object(out, tstruct);

  scope_down(out);
  out << endl;
}

/**
 * Generates a function to read all the fields of the struct.
 *
 * @param tstruct The struct definition
 */
void t_java2_generator::generate_java_struct_reader(ofstream &out, t_struct* tstruct) {
  out <<
    indent() << "public void read(TProtocol iprot) throws TException {" << endl;
  indent_up();

  const vector<t_field*> &fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;

  // Declare stack tmp variables and read struct header
  out <<
    indent() << "TField field;" << endl;

  // Loop over reading in fields
  indent(out) <<
    "while (true) {" << endl;
  indent_up();

  // Read beginning field marker
  indent(out) <<
    "field = iprot.readFieldBegin();" << endl;

  // Check for field STOP marker and break
  indent(out) <<
    "if (field.type == TType.STOP) { " << endl;
  indent_up();
  indent(out) <<
    "break;" << endl;
  indent_down();
  indent(out) <<
    "}" << endl;

  // Switch statement on the field we are reading
  indent(out) << "switch (field.id) {" << endl;

  indent_up();

  // Generate deserialization code for known cases
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    indent(out) <<
      "case " << (*f_iter)->get_key() << ": // " << constant_name((*f_iter)->get_name()) << endl;
    indent_up();
    indent(out) <<
      "if (field.type == " << type_to_enum((*f_iter)->get_type()) << ") {" << endl;
    indent_up();

    generate_deserialize_field(out, *f_iter, "this.");
    indent_down();
    out <<
      indent() << "} else { " << endl <<
      indent() << "  TProtocolUtil.skip(iprot, field.type);" << endl <<
      indent() << "}" << endl <<
      indent() << "break;" << endl;
    indent_down();
  }

  indent(out) << "default:" << endl;
  indent(out) << "  TProtocolUtil.skip(iprot, field.type);" << endl;

  indent_down();
  indent(out) << "}" << endl;

  // Read field end marker
  indent(out) <<
    "iprot.readFieldEnd();" << endl;

  indent_down();
  indent(out) << "}" << endl;

  // performs various checks (e.g. check that all required fields are set)
  indent(out) << "validate();" << endl;

  indent_down();
  out <<
    indent() << "}" << endl <<
    endl;
}

// generates java method to perform various checks
// (e.g. check that all required fields are set)
void t_java2_generator::generate_java_validator(ofstream &out, t_struct* tstruct) {
  indent(out) << "public void validate() throws TException {" << endl;
  indent_up();

  const vector<t_field*> &fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;

  indent();
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    if ((*f_iter)->get_req() == t_field::T_REQUIRED) {
      out <<
        indent() << "if (this." << (*f_iter)->get_name() << " == null) {" << endl <<
        indent() << "    TProtocolException.requiredField(\"" << (*f_iter)->get_name() << "\");" << endl <<
        indent() << "}" << endl;
    }
  }

  indent_down();
  indent(out) << "}" << endl << endl;
}

/**
 * Generates a function to write all the fields of the struct
 *
 * @param tstruct The struct definition
 */
void t_java2_generator::generate_java_struct_writer(ofstream &out, t_struct* tstruct) {
  out <<
    indent() << "public void write(TProtocol oprot) throws TException {" << endl;
  indent_up();

  string name = tstruct->get_name();
  const vector<t_field*> &fields = tstruct->get_sorted_members();
  vector<t_field*>::const_iterator f_iter;

  // performs various checks (e.g. check that all required fields are set)
  indent(out) << "validate();" << endl << endl;

  int i = 0;
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    bool optional = (*f_iter)->get_req() == t_field::T_OPTIONAL;
    if (optional) {
      indent(out) << "if (this." << (*f_iter)->get_name() << " != null) {" << endl;
      indent_up();
    }

    indent(out) << "oprot.writeFieldBegin(_META[" << i++ << "]);" << endl;

    // Write field contents
    generate_serialize_field(out, *f_iter, "this.");

    // Write field closer
    indent(out) <<
      "oprot.writeFieldEnd();" << endl;

    if (optional) {
      indent_down();
      indent(out) << "}" << endl;
    }
  }
  // Write the struct map
  out <<
    indent() << "oprot.writeFieldStop();" << endl;

  indent_down();
  out <<
    indent() << "}" << endl <<
    endl;
}

/**
 * Generates a set of Java Bean boilerplate functions (setters, getters, etc.)
 * for the given struct.
 *
 * @param tstruct The struct definition
 */
void t_java2_generator::generate_java_bean_boilerplate(ofstream &out, t_struct* tstruct) {
  const vector<t_field*> &fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    t_field* field = *f_iter;
    t_type* type = get_true_type(field->get_type());
    std::string field_name = field->get_name();
    std::string cap_name = get_cap_name(field_name);

    // Simple getter
    generate_java_doc(out, field);

    indent(out) << "public " << type_name(type);
    if (type->is_base_type() &&
      ((t_base_type*) type)->get_base() == t_base_type::TYPE_BOOL) {
      out << " is";
    } else {
      out << " get";
    }
    out << cap_name << "() {" << endl;
    indent_up();
    indent(out) << "return this." << field_name << ";" << endl;
    indent_down();
    indent(out) << "}" << endl << endl;

    // Simple setter
    generate_java_doc(out, field);
    if (type->is_base_type() && ((t_base_type*) type)->is_binary()) {
      indent(out) << "public ";
      out << "void";
      out << " set" << cap_name << "(byte[] " << field_name << ") {" << endl;
      indent(out) << "  set" << cap_name << "(" << field_name << " == null ? (ByteBuffer)null : ByteBuffer.wrap(" << field_name << "));" << endl;
      indent(out) << "}" << endl << endl;
    }
    indent(out) << "public ";
    out << "void";
    out << " set" << cap_name << "(" << type_name(type) << " " << field_name << ") {" << endl;
    indent_up();
    indent(out) << "this." << field_name << " = " << field_name << ";" << endl;

    indent_down();
    indent(out) << "}" << endl << endl;
  }
}

/** 
 * Returns a string with the java representation of the given thrift type
 * (e.g. for the type struct it returns "TType.STRUCT")
 */
std::string t_java2_generator::get_java_type_string(t_type* type) {
  if (type->is_list()) {
    return "TType.LIST";
  } else if (type->is_map()) {
    return "TType.MAP";
  } else if (type->is_set()) {
    return "TType.SET";
  } else if (type->is_struct() || type->is_xception()) {
    return "TType.STRUCT";
  } else if (type->is_enum()) {
    return "TType.ENUM";
  } else if (type->is_typedef()) {
    return get_java_type_string(((t_typedef*) type)->get_type());
  } else if (type->is_base_type()) {
    switch (((t_base_type*) type)->get_base()) {
      case t_base_type::TYPE_VOID   :
        return "TType.VOID";
        break;
      case t_base_type::TYPE_STRING :
        return "TType.STRING";
        break;
      case t_base_type::TYPE_BOOL   :
        return "TType.BOOL";
        break;
      case t_base_type::TYPE_BYTE   :
        return "TType.BYTE";
        break;
      case t_base_type::TYPE_I16    :
        return "TType.I16";
        break;
      case t_base_type::TYPE_I32    :
        return "TType.I32";
        break;
      case t_base_type::TYPE_I64    :
        return "TType.I64";
        break;
      case t_base_type::TYPE_DOUBLE :
        return "TType.DOUBLE";
        break;
      default :
        throw std::runtime_error("Unknown thrift type \"" + type->get_name() + "\" passed to t_java2_generator::get_java_type_string!");
        break; // This should never happen!
    }
  } else {
    throw std::runtime_error("Unknown thrift type \"" + type->get_name() + "\" passed to t_java2_generator::get_java_type_string!"); // This should never happen!
  }
}

/**
 * Generates a thrift service. In C++, this comprises an entirely separate
 * header and source file. The header file defines the methods and includes
 * the data types defined in the main header file, and the implementation
 * file contains implementations of the basic printer and default interfaces.
 *
 * @param tservice The service definition
 */
void t_java2_generator::generate_service(t_service* tservice) {
  // Make output file
  string f_service_name = package_dir_ + "/" + service_name_ + ".java";
  f_service_.open(f_service_name.c_str());

  f_service_ <<
    autogen_comment() <<
    java_package() <<
    java_type_imports() <<
    java_thrift_imports();

  f_service_ <<
    "public class " << service_name_ << " {" << endl << endl;
  indent_up();

  // Generate the three main parts of the service
  generate_service_interface(tservice);
  generate_service_async_interface(tservice);
  generate_service_meta(tservice);
  generate_service_client(tservice);

  indent_down();
  f_service_ << "}" << endl;
  f_service_.close();
}

/**
 * Generates a service interface definition.
 *
 * @param tservice The service to generate a header definition for
 */
void t_java2_generator::generate_service_interface(t_service* tservice) {
  string extends = "";
  string extends_iface = "";
  if (tservice->get_extends() != NULL) {
    extends = type_name(tservice->get_extends());
    extends_iface = " extends " + extends + ".Iface";
  }

  generate_java_doc(f_service_, tservice);
  f_service_ << indent() << "public interface Iface" << extends_iface <<
    " {" << endl << endl;
  indent_up();
  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::iterator f_iter;
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    generate_java_doc(f_service_, *f_iter);
    indent(f_service_) << function_signature(*f_iter) << ";" << endl << endl;
  }
  indent_down();
  f_service_ << indent() << "}" << endl << endl;
}

void t_java2_generator::generate_service_async_interface(t_service* tservice) {
  string extends = "";
  string extends_iface = "";
  if (tservice->get_extends() != NULL) {
    extends = type_name(tservice->get_extends());
    extends_iface = " extends " + extends + " .AsyncIface";
  }

  f_service_ << indent() << "public interface AsyncIface" << extends_iface << " {" << endl << endl;
  indent_up();
  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::iterator f_iter;
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    indent(f_service_) << function_signature_async(*f_iter, true) << " throws TException;" << endl << endl;
  }
  indent_down();
  f_service_ << indent() << "}" << endl << endl;
}

void t_java2_generator::generate_service_meta(t_service* tservice) {
  indent(f_service_) << "public static TField[][] _META = {";
  f_service_ << endl;
  indent_up();

  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::const_iterator f_iter;

  size_t i = functions.size();
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    indent(f_service_) << "{";
    const vector<t_field*> &fields = (*f_iter)->get_arglist()->get_members();
    vector<t_field*>::const_iterator fi_iter;
    int j = 0;
    for (fi_iter = fields.begin(); fi_iter != fields.end(); ++fi_iter) {
      if (++j > 1) {
        f_service_ << ", ";
      }
      f_service_ << "new TField(" << type_to_enum((*fi_iter)->get_type()) << ", " << (*fi_iter)->get_key() << ")";
    }
    f_service_ << "}";
    if (--i > 0) {
      f_service_ << ", ";
    }
    f_service_ << endl;
  }
  indent_down();
  indent(f_service_) << "};" << endl;
}

/**
 * Generates a service client definition.
 *
 * @param tservice The service to generate a server for.
 */
void t_java2_generator::generate_service_client(t_service* tservice) {
  string extends = "";
  string extends_client = "";
  if (tservice->get_extends() == NULL) {
    extends_client = "org.apache.thrift.TServiceClient";
  } else {
    extends = type_name(tservice->get_extends());
    extends_client = extends + ".Client";
  }

  indent(f_service_) <<
    "public static class Client extends " << extends_client << " implements Iface {" << endl << endl;
  indent_up();

  indent(f_service_) << "public Client(TProtocol prot) {" << endl;
  indent(f_service_) << "    super(prot, prot);" << endl;
  indent(f_service_) << "}" << endl << endl;

  // Generate client method implementations
  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::const_iterator f_iter;
  int i = 0;
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    // Open function
    indent(f_service_) << "public " << function_signature(*f_iter) << " {" << endl;
    indent_up();

    // Send
    indent(f_service_) << "sendBegin(\"" << (*f_iter)->get_name() << "\");" << endl;
    const vector<t_field*> &fields = (*f_iter)->get_arglist()->get_members();
    vector<t_field*>::const_iterator fi_iter;
    int j = 0;
    for (fi_iter = fields.begin(); fi_iter != fields.end(); ++fi_iter) {
      indent(f_service_) << "if (" << (*fi_iter)->get_name() << " != null) {" << endl;
      indent_up();
      indent(f_service_) << "oprot.writeFieldBegin(_META[" << i++ << "][" << j++ << "]);" << endl;
      // Write field contents
      generate_serialize_field(f_service_, *fi_iter, "");
      // Write field closer
      indent(f_service_) << "oprot.writeFieldEnd();" << endl;
      indent_down();
      indent(f_service_) << "}" << endl;
    }
    // Write the struct map
    indent(f_service_) << "oprot.writeFieldStop();" << endl;
    indent(f_service_) << "sendEnd();" << endl;

    //Receive
    if (!(*f_iter)->is_oneway()) {
      indent(f_service_) << "receiveBegin();" << endl;
      indent(f_service_) << "TField $_field;" << endl;
      // Loop over reading in fields
      indent(f_service_) << "while (true) {" << endl;
      indent_up();
      // Read beginning field marker
      indent(f_service_) << "$_field = iprot.readFieldBegin();" << endl;

      // Check for field STOP marker and break
      indent(f_service_) << "if ($_field.type == TType.STOP) { " << endl;
      indent_up();
      indent(f_service_) << "break;" << endl;
      indent_down();
      indent(f_service_) << "}" << endl;

      // Switch statement on the field we are reading
      indent(f_service_) << "switch (field.id) {" << endl;
      indent_up();

      // Generate deserialization code for known cases
      for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        indent(f_service_) << "case " << (*f_iter)->get_key() << ": // " << constant_name((*f_iter)->get_name()) << endl;
        indent_up();
        indent(f_service_) << "if (field.type == " << type_to_enum((*f_iter)->get_type()) << ") {" << endl;
        indent_up();

        generate_deserialize_field(out, *f_iter, "this.");
        indent_down();
        out <<
          indent() << "} else { " << endl <<
          indent() << "  TProtocolUtil.skip(iprot, field.type);" << endl <<
          indent() << "}" << endl <<
          indent() << "break;" << endl;
        indent_down();
      }

      indent(f_service_) << "default:" << endl;
      indent(f_service_) << "  TProtocolUtil.skip(iprot, field.type);" << endl;

      indent_down();
      indent(f_service_) << "}" << endl;

      // Read field end marker
      indent(f_service_) << "iprot.readFieldEnd();" << endl;

      indent_down();
      indent(f_service_) << "}" << endl;

      indent(f_service_) << "receiveEnd();" << endl;
    }

    indent_down();
    indent(f_service_) << "}" << endl << endl;
  }

  indent_down();
  indent(f_service_) << "}" << endl;
}

/**
 * Deserializes a field of any type.
 *
 * @param tfield The field
 * @param prefix The variable name or container for this field
 */
void t_java2_generator::generate_deserialize_field(ofstream &out, t_field* tfield, string prefix) {
  t_type* type = get_true_type(tfield->get_type());

  if (type->is_void()) {
    throw "CANNOT GENERATE DESERIALIZE CODE FOR void TYPE: " + prefix + tfield->get_name();
  }

  string name = prefix + tfield->get_name();

  if (type->is_struct() || type->is_xception()) {
    generate_deserialize_struct(out,
      (t_struct*) type,
      name);
  } else if (type->is_container()) {
    generate_deserialize_container(out, type, name);
  } else if (type->is_base_type()) {
    indent(out) << name << " = iprot.";


    t_base_type::t_base tbase = ((t_base_type*) type)->get_base();
    switch (tbase) {
      case t_base_type::TYPE_VOID:
        throw "compiler error: cannot serialize void field in a struct: " + name;
        break;
      case t_base_type::TYPE_STRING:
        if (((t_base_type*) type)->is_binary()) {
          out << "readBinary();";
        } else {
          out << "readString();";
        }
        break;
      case t_base_type::TYPE_BOOL:
        out << "readBool();";
        break;
      case t_base_type::TYPE_BYTE:
        out << "readByte();";
        break;
      case t_base_type::TYPE_I16:
        out << "readI16();";
        break;
      case t_base_type::TYPE_I32:
        out << "readI32();";
        break;
      case t_base_type::TYPE_I64:
        out << "readI64();";
        break;
      case t_base_type::TYPE_DOUBLE:
        out << "readDouble();";
        break;
      default:
        throw "compiler error: no Java name for base type " + t_base_type::t_base_name(tbase);
    }
    out << endl;
  } else if (type->is_enum()) {
    indent(out) << name << " = " << type_name(tfield->get_type(), true, false) + ".findByValue(iprot.readI32());" << endl;
  } else {
    printf("DO NOT KNOW HOW TO DESERIALIZE FIELD '%s' TYPE '%s'\n",
      tfield->get_name().c_str(), type_name(type).c_str());
  }
}

/**
 * Generates an unserializer for a struct, invokes read()
 */
void t_java2_generator::generate_deserialize_struct(ofstream &out, t_struct* tstruct, string prefix) {
  out <<
    indent() << prefix << " = new " << type_name(tstruct) << "();" << endl <<
    indent() << prefix << ".read(iprot);" << endl;
}

/**
 * Deserializes a container by reading its size and then iterating
 */
void t_java2_generator::generate_deserialize_container(ofstream &out, t_type* ttype, string prefix) {

  scope_up(out);

  string obj;

  if (ttype->is_map()) {
    obj = tmp("_map");
  } else if (ttype->is_set()) {
    obj = tmp("_set");
  } else if (ttype->is_list()) {
    obj = tmp("_list");
  }

  if (ttype->is_map()) {
    indent(out) << "TMap " << obj << " = iprot.readMapBegin();" << endl;
  } else if (ttype->is_set()) {
    indent(out) << "TSet " << obj << " = iprot.readSetBegin();" << endl;
  } else if (ttype->is_list()) {
    indent(out) << "TList " << obj << " = iprot.readListBegin();" << endl;
  }

  indent(out) << prefix << " = new " << type_name(ttype, false, true);
  // size the collection correctly
  out << "("
    << (ttype->is_list() ? "" : "2*")
    << obj << ".size"
    << ");" << endl;

  // For loop iterates over elements
  string i = tmp("_i");
  indent(out) <<
    "for (int " << i << " = 0; " <<
    i << " < " << obj << ".size" << "; " <<
    "++" << i << ")" << endl;

  scope_up(out);

  if (ttype->is_map()) {
    generate_deserialize_map_element(out, (t_map*) ttype, prefix);
  } else if (ttype->is_set()) {
    generate_deserialize_set_element(out, (t_set*) ttype, prefix);
  } else if (ttype->is_list()) {
    generate_deserialize_list_element(out, (t_list*) ttype, prefix);
  }

  scope_down(out);

  if (ttype->is_map()) {
    indent(out) << "iprot.readMapEnd();" << endl;
  } else if (ttype->is_set()) {
    indent(out) << "iprot.readSetEnd();" << endl;
  } else if (ttype->is_list()) {
    indent(out) << "iprot.readListEnd();" << endl;
  }
  scope_down(out);
}


/**
 * Generates code to deserialize a map
 */
void t_java2_generator::generate_deserialize_map_element(ofstream &out, t_map* tmap, string prefix) {
  string key = tmp("_key");
  string val = tmp("_val");
  t_field fkey(tmap->get_key_type(), key);
  t_field fval(tmap->get_val_type(), val);

  indent(out) << declare_field(&fkey) << endl;
  indent(out) << declare_field(&fval) << endl;

  generate_deserialize_field(out, &fkey, "");
  generate_deserialize_field(out, &fval, "");

  indent(out) << prefix << ".put(" << key << ", " << val << ");" << endl;
}

/**
 * Deserializes a set element
 */
void t_java2_generator::generate_deserialize_set_element(ofstream &out, t_set* tset, string prefix) {
  string elem = tmp("_elem");
  t_field felem(tset->get_elem_type(), elem);

  indent(out) << declare_field(&felem) << endl;

  generate_deserialize_field(out, &felem, "");

  indent(out) << prefix << ".add(" << elem << ");" << endl;
}

/**
 * Deserializes a list element
 */
void t_java2_generator::generate_deserialize_list_element(ofstream &out, t_list* tlist, string prefix) {
  string elem = tmp("_elem");
  t_field felem(tlist->get_elem_type(), elem);

  indent(out) << declare_field(&felem) << endl;

  generate_deserialize_field(out, &felem, "");

  indent(out) << prefix << ".add(" << elem << ");" << endl;
}

/**
 * Serializes a field of any type.
 *
 * @param tfield The field to serialize
 * @param prefix Name to prepend to field name
 */
void t_java2_generator::generate_serialize_field(ofstream &out, t_field* tfield, string prefix) {
  t_type* type = get_true_type(tfield->get_type());

  // Do nothing for void types
  if (type->is_void()) {
    throw "CANNOT GENERATE SERIALIZE CODE FOR void TYPE: " + prefix + tfield->get_name();
  }

  if (type->is_struct() || type->is_xception()) {
    generate_serialize_struct(out, (t_struct*) type, prefix + tfield->get_name());
  } else if (type->is_container()) {
    generate_serialize_container(out, type, prefix + tfield->get_name());
  } else if (type->is_enum()) {
    indent(out) << "oprot.writeI32(" << prefix + tfield->get_name() << ".getValue());" << endl;
  } else if (type->is_base_type()) {
    string name = prefix + tfield->get_name();
    indent(out) << "oprot.";

    if (type->is_base_type()) {
      t_base_type::t_base tbase = ((t_base_type*) type)->get_base();
      switch (tbase) {
        case t_base_type::TYPE_VOID:
          throw "compiler error: cannot serialize void field in a struct: " + name;
          break;
        case t_base_type::TYPE_STRING:
          if (((t_base_type*) type)->is_binary()) {
            out << "writeBinary(" << name << ");";
          } else {
            out << "writeString(" << name << ");";
          }
          break;
        case t_base_type::TYPE_BOOL:
          out << "writeBool(" << name << ");";
          break;
        case t_base_type::TYPE_BYTE:
          out << "writeByte(" << name << ");";
          break;
        case t_base_type::TYPE_I16:
          out << "writeI16(" << name << ");";
          break;
        case t_base_type::TYPE_I32:
          out << "writeI32(" << name << ");";
          break;
        case t_base_type::TYPE_I64:
          out << "writeI64(" << name << ");";
          break;
        case t_base_type::TYPE_DOUBLE:
          out << "writeDouble(" << name << ");";
          break;
        default:
          throw "compiler error: no Java name for base type " + t_base_type::t_base_name(tbase);
      }
    } else if (type->is_enum()) {
      out << "writeI32(struct." << name << ");";
    }
    out << endl;
  } else {
    printf("DO NOT KNOW HOW TO SERIALIZE FIELD '%s%s' TYPE '%s'\n",
      prefix.c_str(),
      tfield->get_name().c_str(),
      type_name(type).c_str());
  }
}

/**
 * Serializes all the members of a struct.
 *
 * @param tstruct The struct to serialize
 * @param prefix  String prefix to attach to all fields
 */
void t_java2_generator::generate_serialize_struct(ofstream &out, t_struct* tstruct, string prefix) {
  (void) tstruct;
  out << indent() << prefix << ".write(oprot);" << endl;
}

/**
 * Serializes a container by writing its size then the elements.
 *
 * @param ttype  The type of container
 * @param prefix String prefix for fields
 */
void t_java2_generator::generate_serialize_container(ofstream &out, t_type* ttype, string prefix) {
  scope_up(out);

  if (ttype->is_map()) {
    indent(out) <<
      "oprot.writeMapBegin(new TMap(" <<
      type_to_enum(((t_map*) ttype)->get_key_type()) << ", " <<
      type_to_enum(((t_map*) ttype)->get_val_type()) << ", " <<
      prefix << ".size()));" << endl;
  } else if (ttype->is_set()) {
    indent(out) <<
      "oprot.writeSetBegin(new TSet(" <<
      type_to_enum(((t_set*) ttype)->get_elem_type()) << ", " <<
      prefix << ".size()));" << endl;
  } else if (ttype->is_list()) {
    indent(out) <<
      "oprot.writeListBegin(new TList(" <<
      type_to_enum(((t_list*) ttype)->get_elem_type()) << ", " <<
      prefix << ".size()));" << endl;
  }

  string iter = tmp("_iter");
  if (ttype->is_map()) {
    indent(out) <<
      "for (Map.Entry<" <<
      type_name(((t_map*) ttype)->get_key_type(), true, false) << ", " <<
      type_name(((t_map*) ttype)->get_val_type(), true, false) << "> " << iter <<
      " : " <<
      prefix << ".entrySet())";
  } else if (ttype->is_set()) {
    indent(out) <<
      "for (" <<
      type_name(((t_set*) ttype)->get_elem_type()) << " " << iter <<
      " : " <<
      prefix << ")";
  } else if (ttype->is_list()) {
    indent(out) <<
      "for (" <<
      type_name(((t_list*) ttype)->get_elem_type()) << " " << iter <<
      " : " <<
      prefix << ")";
  }

  out << endl;
  scope_up(out);
  if (ttype->is_map()) {
    generate_serialize_map_element(out, (t_map*) ttype, iter, prefix);
  } else if (ttype->is_set()) {
    generate_serialize_set_element(out, (t_set*) ttype, iter);
  } else if (ttype->is_list()) {
    generate_serialize_list_element(out, (t_list*) ttype, iter);
  }
  scope_down(out);

  if (ttype->is_map()) {
    indent(out) << "oprot.writeMapEnd();" << endl;
  } else if (ttype->is_set()) {
    indent(out) << "oprot.writeSetEnd();" << endl;
  } else if (ttype->is_list()) {
    indent(out) << "oprot.writeListEnd();" << endl;
  }

  scope_down(out);
}

/**
 * Serializes the members of a map.
 */
void t_java2_generator::generate_serialize_map_element(ofstream &out, t_map* tmap, string iter, string map) {
  (void) map;
  t_field kfield(tmap->get_key_type(), iter + ".getKey()");
  generate_serialize_field(out, &kfield, "");
  t_field vfield(tmap->get_val_type(), iter + ".getValue()");
  generate_serialize_field(out, &vfield, "");
}

/**
 * Serializes the members of a set.
 */
void t_java2_generator::generate_serialize_set_element(ofstream &out, t_set* tset, string iter) {
  t_field efield(tset->get_elem_type(), iter);
  generate_serialize_field(out, &efield, "");
}

/**
 * Serializes the members of a list.
 */
void t_java2_generator::generate_serialize_list_element(ofstream &out, t_list* tlist, string iter) {
  t_field efield(tlist->get_elem_type(), iter);
  generate_serialize_field(out, &efield, "");
}

/**
 * Returns a Java type name
 *
 * @param ttype The type
 * @param container Is the type going inside a container?
 * @return Java type name, i.e. Vector
 */
string t_java2_generator::type_name(t_type* ttype, bool in_container, bool in_init, bool skip_generic) {
  // In Java typedefs are just resolved to their real type
  ttype = get_true_type(ttype);
  string prefix;

  if (ttype->is_base_type()) {
    return base_type_name((t_base_type*) ttype, in_container);
  } else if (ttype->is_map()) {
    t_map* tmap = (t_map*) ttype;
    if (in_init) {
      prefix = "LinkedHashMap";
    } else {
      prefix = "Map";
    }
    return prefix + (skip_generic ? "" : "<" +
      type_name(tmap->get_key_type(), true) + "," +
      type_name(tmap->get_val_type(), true) + ">");
  } else if (ttype->is_set()) {
    t_set* tset = (t_set*) ttype;
    if (in_init) {
      prefix = "HashSet";
    } else {
      prefix = "Set";
    }
    return prefix + (skip_generic ? "" : "<" + type_name(tset->get_elem_type(), true) + ">");
  } else if (ttype->is_list()) {
    t_list* tlist = (t_list*) ttype;
    if (in_init) {
      prefix = "ArrayList";
    } else {
      prefix = "List";
    }
    return prefix + (skip_generic ? "" : "<" + type_name(tlist->get_elem_type(), true) + ">");
  }

  // Check for namespacing
  t_program* program = ttype->get_program();
  if (program != NULL && program != program_) {
    string package = program->get_namespace("java");
    if (!package.empty()) {
      return package + "." + ttype->get_name();
    }
  }

  return ttype->get_name();
}

/**
 * Returns the C++ type that corresponds to the thrift type.
 *
 * @param tbase The base type
 * @param container Is it going in a Java container?
 */
string t_java2_generator::base_type_name(t_base_type* type, bool in_container) {
  t_base_type::t_base tbase = type->get_base();

  switch (tbase) {
    case t_base_type::TYPE_VOID:
      return "void";
    case t_base_type::TYPE_STRING:
      if (!type->is_binary()) {
        return "String";
      } else {
        return "byte[]";
      }
    case t_base_type::TYPE_BOOL:
      return (in_container ? "Boolean" : "boolean");
    case t_base_type::TYPE_BYTE:
      return (in_container ? "Byte" : "byte");
    case t_base_type::TYPE_I16:
      return (in_container ? "Short" : "short");
    case t_base_type::TYPE_I32:
      return (in_container ? "Integer" : "int");
    case t_base_type::TYPE_I64:
      return (in_container ? "Long" : "long");
    case t_base_type::TYPE_DOUBLE:
      return (in_container ? "Double" : "double");
    default:
      throw "compiler error: no C++ name for base type " + t_base_type::t_base_name(tbase);
  }
}

/**
 * Declares a field, which may include initialization as necessary.
 *
 * @param ttype The type
 */
string t_java2_generator::declare_field(t_field* tfield, bool init) {
  string result = type_name(tfield->get_type(), true) + " " + tfield->get_name();
  if (init) {
    t_type* ttype = get_true_type(tfield->get_type());
    if (ttype->is_base_type() && tfield->get_value() != NULL) {
      ofstream dummy;
      result += " = " + render_const_value(dummy, ttype, tfield->get_value());
    } else if (ttype->is_base_type()) {
      t_base_type::t_base tbase = ((t_base_type*) ttype)->get_base();
      switch (tbase) {
        case t_base_type::TYPE_VOID:
          throw "NO T_VOID CONSTRUCT";
        case t_base_type::TYPE_STRING:
          result += " = null";
          break;
        case t_base_type::TYPE_BOOL:
          result += " = false";
          break;
        case t_base_type::TYPE_BYTE:
        case t_base_type::TYPE_I16:
        case t_base_type::TYPE_I32:
        case t_base_type::TYPE_I64:
          result += " = 0";
          break;
        case t_base_type::TYPE_DOUBLE:
          result += " = (double)0";
          break;
      }
    } else if (ttype->is_enum()) {
      result += " = 0";
    } else if (ttype->is_container()) {
      result += " = new " + type_name(ttype, false, true) + "()";
    } else {
      result += " = new " + type_name(ttype, false, true) + "()";;
    }
  }
  result += "; // ";
  if (tfield->get_req() == t_field::T_OPTIONAL) {
    result += "optional";
  } else {
    result += "required";
  }
  return result;
}

/**
 * Renders a function signature of the form 'type name(args)'
 *
 * @param tfunction Function definition
 * @return String of rendered function definition
 */
string t_java2_generator::function_signature(t_function* tfunction, string prefix) {
  t_type* ttype = tfunction->get_returntype();
  std::string result =
    type_name(ttype) + " " + prefix + tfunction->get_name() + "(" + argument_list(tfunction->get_arglist()) + ") throws ";
  t_struct* xs = tfunction->get_xceptions();
  const std::vector<t_field*> &xceptions = xs->get_members();
  vector<t_field*>::const_iterator x_iter;
  for (x_iter = xceptions.begin(); x_iter != xceptions.end(); ++x_iter) {
    result += type_name((*x_iter)->get_type(), false, false) + ", ";
  }
  result += "TException";
  return result;
}

/**
 * Renders a function signature of the form 'void name(args, resultHandler)'
 *
 * @params tfunction Function definition
 * @return String of rendered function definition
 */
string t_java2_generator::function_signature_async(t_function* tfunction, bool use_base_method, string prefix) {
  std::string arglist = async_argument_list(tfunction, use_base_method, true);

  std::string ret_type = type_name(tfunction->get_returntype(), true);

  if (ret_type == "void") {
    ret_type = "Void";
  }

  std::string result = prefix + "Future<" + ret_type + "> " + tfunction->get_name() + "(" + arglist + ")";
  return result;
}

/**
 * Renders a comma separated field list, with type names
 */
string t_java2_generator::argument_list(t_struct* tstruct, bool include_types) {
  string result = "";

  const vector<t_field*> &fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;
  bool first = true;
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    if (first) {
      first = false;
    } else {
      result += ", ";
    }
    if (include_types) {
      result += type_name((*f_iter)->get_type()) + " ";
    }
    result += (*f_iter)->get_name();
  }
  return result;
}

string t_java2_generator::async_argument_list(t_function* tfunc, bool use_base_method, bool include_types) {
  std::string arglist = "";
  if (tfunc->get_arglist()->get_members().size() > 0) {
    arglist = argument_list(tfunc->get_arglist(), include_types) + ", ";
  }

  std::string ret_type = type_name(tfunc->get_returntype(), true);

  if (ret_type == "void") {
    ret_type = "Void";
  }

  if (include_types) {
    arglist += "AsyncMethodCallback<" + ret_type + "> ";
  }
  arglist += "callback";

  return arglist;
}

/**
 * Converts the parse type to a C++ enum string for the given type.
 */
string t_java2_generator::type_to_enum(t_type* type) {
  type = get_true_type(type);

  if (type->is_base_type()) {
    t_base_type::t_base tbase = ((t_base_type*) type)->get_base();
    switch (tbase) {
      case t_base_type::TYPE_VOID:
        throw "NO T_VOID CONSTRUCT";
      case t_base_type::TYPE_STRING:
        return "TType.STRING";
      case t_base_type::TYPE_BOOL:
        return "TType.BOOL";
      case t_base_type::TYPE_BYTE:
        return "TType.BYTE";
      case t_base_type::TYPE_I16:
        return "TType.I16";
      case t_base_type::TYPE_I32:
        return "TType.I32";
      case t_base_type::TYPE_I64:
        return "TType.I64";
      case t_base_type::TYPE_DOUBLE:
        return "TType.DOUBLE";
    }
  } else if (type->is_enum()) {
    return "TType.I32";
  } else if (type->is_struct() || type->is_xception()) {
    return "TType.STRUCT";
  } else if (type->is_map()) {
    return "TType.MAP";
  } else if (type->is_set()) {
    return "TType.SET";
  } else if (type->is_list()) {
    return "TType.LIST";
  }

  throw "INVALID TYPE IN type_to_enum: " + type->get_name();
}

/**
 * Applies the correct style to a string based on the value of nocamel_style_
 */
std::string t_java2_generator::get_cap_name(std::string name) {
  name[0] = toupper(name[0]);
  return name;
}

string t_java2_generator::constant_name(string name) {
  string constant_name;

  bool is_first = true;
  bool was_previous_char_upper = false;
  for (string::iterator iter = name.begin(); iter != name.end(); ++iter) {
    string::value_type character = (*iter);

    bool is_upper = isupper(character);

    if (is_upper && !is_first && !was_previous_char_upper) {
      constant_name += '_';
    }
    constant_name += toupper(character);

    is_first = false;
    was_previous_char_upper = is_upper;
  }

  return constant_name;
}

void t_java2_generator::generate_java_docstring_comment(ofstream &out, string contents) {
  generate_docstring_comment(out, "/**\n", " * ", contents, " */\n");
}

void t_java2_generator::generate_java_doc(ofstream &out, t_field* field) {
  if (field->get_type()->is_enum()) {
    string combined_message = field->get_doc() + "\n@see " + get_enum_class_name(field->get_type());
    generate_java_docstring_comment(out, combined_message);
  } else {
    generate_java_doc(out, (t_doc*) field);
  }
}

/**
 * Emits a JavaDoc comment if the provided object has a doc in Thrift
 */
void t_java2_generator::generate_java_doc(ofstream &out, t_doc* tdoc) {
  if (tdoc->has_doc()) {
    generate_java_docstring_comment(out, tdoc->get_doc());
  }
}

/**
 * Emits a JavaDoc comment if the provided function object has a doc in Thrift
 */
void t_java2_generator::generate_java_doc(ofstream &out, t_function* tfunction) {
  if (tfunction->has_doc()) {
    stringstream ss;
    ss << tfunction->get_doc();
    const vector<t_field*> &fields = tfunction->get_arglist()->get_members();
    vector<t_field*>::const_iterator p_iter;
    for (p_iter = fields.begin(); p_iter != fields.end(); ++p_iter) {
      t_field* p = *p_iter;
      ss << "\n@param " << p->get_name();
      if (p->has_doc()) {
        ss << " " << p->get_doc();
      }
    }
    generate_docstring_comment(out, "/**\n", " * ", ss.str(), " */\n");
  }
}

std::string t_java2_generator::get_enum_class_name(t_type* type) {
  string package = "";
  t_program* program = type->get_program();
  if (program != NULL && program != program_) {
    package = program->get_namespace("java") + ".";
  }
  return package + type->get_name();
}

void t_java2_generator::generate_field_descs(ofstream &out, t_struct* tstruct) {
  const vector<t_field*> &members = tstruct->get_members();
  vector<t_field*>::const_iterator m_iter;

  indent(out) << "public static TField[] _META = {";

  int i = 0;
  for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
    if (++i > 1) {
      out << ", ";
    }
    out << "new TField(" << type_to_enum((*m_iter)->get_type()) << ", " << (*m_iter)->get_key() << ")";
  }

  out << "};" << endl;
}

// generates java method to serialize (in the Java sense) the object
void t_java2_generator::generate_java_struct_write_object(ofstream &out, t_struct* tstruct) {
  (void) tstruct;
  indent(out) << "private void writeObject(java.io.ObjectOutputStream out) throws java.io.IOException {" << endl;
  indent(out) << "    try {" << endl;
  indent(out) << "        write(new TBinaryProtocol(new TIOStreamTransport(out)));" << endl;
  indent(out) << "    } catch (org.apache.thrift.TException te) {" << endl;
  indent(out) << "        throw new java.io.IOException(te.getMessage());" << endl;
  indent(out) << "    }" << endl;
  indent(out) << "}" << endl << endl;
}

// generates java method to serialize (in the Java sense) the object
void t_java2_generator::generate_java_struct_read_object(ofstream &out, t_struct* tstruct) {
  indent(out) << "private void readObject(java.io.ObjectInputStream in) throws java.io.IOException, ClassNotFoundException {" << endl;
  indent(out) << "    try {" << endl;
  indent(out) << "        read(new TBinaryProtocol(new TIOStreamTransport(in)));" << endl;
  indent(out) << "    } catch (org.apache.thrift.TException te) {" << endl;
  indent(out) << "        throw new java.io.IOException(te.getMessage());" << endl;
  indent(out) << "    }" << endl;
  indent(out) << "}" << endl << endl;
}

THRIFT_REGISTER_GENERATOR(java2, "Java2", "")

