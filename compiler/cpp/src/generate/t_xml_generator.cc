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

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include <stdlib.h>
#include <sys/stat.h>
#include <sstream>
#include "t_generator.h"
#include "platform.h"
using namespace std;


/**
 * XML model generator
 * @author <a href="mailto:oxsean@gmail.com">sean yang</a>
 * @version V1.0, 12-9-4
 */
class t_xml_generator : public t_generator {
public:
  t_xml_generator(
    t_program* program,
    const std::map<std::string, std::string>& parsed_options,
    const std::string& option_string)
    : t_generator(program)
  {
    (void) parsed_options;
    (void) option_string;  
    out_dir_base_ = "gen-xml";
    escape_.clear();
    escape_['&']  = "&amp;";
    escape_['<']  = "&lt;";
    escape_['>']  = "&gt;";
    escape_['"']  = "&quot;";
    escape_['\''] = "&apos;";
  }
  void generate_program();

  /**
  * Program-level generation functions
  */
  void generate_typedef (t_typedef*  ttypedef);
  void generate_enum    (t_enum*     tenum);
  void generate_const   (t_const*    tconst);
  void generate_struct  (t_struct*   tstruct);
  void generate_service (t_service*  tservice);
  void generate_xception(t_struct*   txception);

  void print_doc_value(string value);
  int  print_type       (t_type* ttype);
  void print_const_value(t_const_value* tvalue);

  std::ofstream f_out_;
};

/**
* Prepares for file generation by opening up the necessary file output
* stream.
*/
void t_xml_generator::generate_program() {
  // Make output directory
  MKDIR(get_out_dir().c_str());
  string dir = program_->get_namespace("java");
  string subdir = get_out_dir();
  string::size_type loc;
  while ((loc = dir.find(".")) != string::npos) {
    subdir = subdir + "/" + dir.substr(0, loc);
    MKDIR(subdir.c_str());
    dir = dir.substr(loc+1);
  }
  if (dir.size() > 0) {
    subdir = subdir + "/" + dir;
    MKDIR(subdir.c_str());
  }
  string fname = subdir + "/metadata.xml";
  f_out_.open(fname.c_str());
  f_out_ << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
  f_out_ << "<package name=\"" << program_->get_name();
  f_out_ << "\" javans=\"" << program_->get_namespace("java");
  f_out_ << "\" csharpns=\"" << program_->get_namespace("csharp");
  f_out_ << "\" jsns=\"" << program_->get_namespace("js");
  f_out_ << "\" phpns=\"" << program_->get_namespace("php");
  f_out_ << "\">" << endl; 
  print_doc_value(program_->get_doc());

  if (!program_->get_consts().empty()) {
    f_out_ << "<consts>" << endl;
    vector<t_const*> consts=program_->get_consts();
    vector<t_const*>::iterator c_iter;
    for (c_iter = consts.begin(); c_iter != consts.end(); ++c_iter) {
      generate_const(*c_iter);
    }
    f_out_ << "</consts>" << endl;
  }

  if (!program_->get_enums().empty()) {
    f_out_ << "<enums>" << endl;
    vector<t_enum*> enums = program_->get_enums();
    vector<t_enum*>::iterator en_iter;
    for (en_iter = enums.begin(); en_iter != enums.end(); ++en_iter) {
      generate_enum(*en_iter);
    }
    f_out_ << "</enums>" << endl;
  }

  if (!program_->get_objects().empty()) {
    f_out_ << "<objects>" << endl;
    vector<t_struct*> objects = program_->get_objects();
    vector<t_struct*>::iterator o_iter;
    for (o_iter = objects.begin(); o_iter != objects.end(); ++o_iter) {
      if ((*o_iter)->is_xception()) {
        generate_xception(*o_iter);
      } else {
        generate_struct(*o_iter);
      }
    }
    f_out_ << "</objects>" << endl;
  }

  if (!program_->get_services().empty()) {
    f_out_ << "<services>" << endl;
    // Generate services
    vector<t_service*> services = program_->get_services();
    vector<t_service*>::iterator sv_iter;
    for (sv_iter = services.begin(); sv_iter != services.end(); ++sv_iter) {
      service_name_ = get_service_name(*sv_iter);
      generate_service(*sv_iter);
    }
    f_out_ << "</services>" << endl;
  }

  f_out_ << "</package>" << endl;
  f_out_.close();
}

/**
* Prints out the provided type in XML
*/
int t_xml_generator::print_type(t_type* ttype) {
  int len = 0;
  if (ttype->is_container()) {
    if (ttype->is_list()) {
      f_out_ << "list&lt;";
      len = 6 + print_type(((t_list*)ttype)->get_elem_type());
      f_out_ << "&gt;";
    } else if (ttype->is_set()) {
      f_out_ << "set&lt;";
      len = 5 + print_type(((t_set*)ttype)->get_elem_type());
      f_out_ << "&gt;";
    } else if (ttype->is_map()) {
      f_out_ << "map&lt;";
      len = 5 + print_type(((t_map*)ttype)->get_key_type());
      f_out_ << ", ";
      len += print_type(((t_map*)ttype)->get_val_type());
      f_out_ << "&gt;";
    }
  } else if (ttype->is_base_type()) {
    f_out_ << (((t_base_type*)ttype)->is_binary() ? "binary" : ttype->get_name());
    len = ttype->get_name().size();
  } else {
    string prog_name = ttype->get_program()->get_name();
    string type_name = ttype->get_name();
    len = type_name.size();
    if (ttype->get_program() != program_) {
      f_out_ << prog_name << ".";
      len += prog_name.size() + 1;
    }
    f_out_ << type_name;
  }
  return len;
}

void t_xml_generator::print_doc_value(string value) {
  if(!value.empty()){
    f_out_ << endl << "<doc><![CDATA[" << value << "]]></doc>" << endl;
  }
}

/**
* Prints out an XML representation of the provided constant value
*/
void t_xml_generator::print_const_value(t_const_value* tvalue) {
  bool first = true;
  switch (tvalue->get_type()) {
  case t_const_value::CV_INTEGER:
    f_out_ << tvalue->get_integer();
    break;
  case t_const_value::CV_DOUBLE:
    f_out_ << tvalue->get_double();
    break;
  case t_const_value::CV_STRING:
    f_out_ << '"' << get_escaped_string(tvalue) << '"';
    break;
  case t_const_value::CV_MAP:
    {
      f_out_ << "{ ";
      map<t_const_value*, t_const_value*> map_elems = tvalue->get_map();
      map<t_const_value*, t_const_value*>::iterator map_iter;
      for (map_iter = map_elems.begin(); map_iter != map_elems.end(); map_iter++) {
        if (!first) {
          f_out_ << ", ";
        }
        first = false;
        print_const_value(map_iter->first);
        f_out_ << " = ";
        print_const_value(map_iter->second);
      }
      f_out_ << " }";
    }
    break;
  case t_const_value::CV_LIST:
    {
      f_out_ << "{ ";
      vector<t_const_value*> list_elems = tvalue->get_list();;
      vector<t_const_value*>::iterator list_iter;
      for (list_iter = list_elems.begin(); list_iter != list_elems.end(); list_iter++) {
        if (!first) {
          f_out_ << ", ";
        }
        first = false;
        print_const_value(*list_iter);
      }
      f_out_ << " }";
    }
    break;
  default:
    f_out_ << "UNKNOWN";
    break;
  }
}

/**
* Generates a typedef. This is not done in Java, since it does
* not support arbitrary name replacements, and it'd be a wacky waste
* of overhead to make wrapper classes.
*
* @param ttypedef The type definition
*/
void t_xml_generator::generate_typedef(t_typedef* ttypedef) {
  (void) ttypedef;
}

/**
* Generates code for an enumerated type.
*
* @param tenum The enumeration
*/
void t_xml_generator::generate_enum(t_enum* tenum) {
  string name = tenum->get_name();
  f_out_ << "<enum name=\"" << name << "\">";
  print_doc_value(tenum->get_doc());
  vector<t_enum_value*> values = tenum->get_constants();
  vector<t_enum_value*>::iterator val_iter;
  for (val_iter = values.begin(); val_iter != values.end(); ++val_iter) {
    f_out_ << "<item name=\"" << (*val_iter)->get_name() << "\" value=\"" << (*val_iter)->get_value() << "\">";
    print_doc_value((*val_iter)->get_doc());
    f_out_ << "</item>" << endl;
  }
  f_out_ << "</enum>" << endl;
}

/**
* Generates a constant value
*/
void t_xml_generator::generate_const(t_const* tconst) {
  string name = tconst->get_name();
  f_out_ << "<const name=\"" << name << "\" type=\"";
  print_type(tconst->get_type());
  f_out_  << "\">";
  f_out_ << "<value>";
  print_const_value(tconst->get_value());
  f_out_ << "</value>";
  print_doc_value(tconst->get_doc());
  f_out_ << "</const>";
}

/**
* Generates a struct definition for a thrift data type.
*
* @param tstruct The struct definition
*/
void t_xml_generator::generate_struct(t_struct* tstruct) {
  string name = tstruct->get_name();
  f_out_ << "<object name=\"" << name << "\" isEx=\"";
  if (tstruct->is_xception()) {
    f_out_ << "true";
  } else {
    f_out_ << "false";
  }
  f_out_ << "\">";
  print_doc_value(tstruct->get_doc());
  vector<t_field*> members = tstruct->get_members();
  vector<t_field*>::iterator mem_iter = members.begin();
  for ( ; mem_iter != members.end(); mem_iter++) {
    f_out_ << "<field index=\"" << (*mem_iter)->get_key() << "\" name=\"";
    f_out_ << (*mem_iter)->get_name() << "\" type=\"";
    print_type((*mem_iter)->get_type());
    f_out_ << "\" required=\"";
    if ((*mem_iter)->get_req() == t_field::T_OPTIONAL) {
      f_out_ << "optional";
    } else if ((*mem_iter)->get_req() == t_field::T_REQUIRED) {
      f_out_ << "required";
    } else {
      f_out_ << "default";
    }
    f_out_ << "\" default=\"";
    t_const_value* default_val = (*mem_iter)->get_value();
    if (default_val != NULL) {
      print_const_value(default_val);
    }
    f_out_ << "\">";
    print_doc_value((*mem_iter)->get_doc());
    f_out_ << "</field>" << endl;
  }
  f_out_ << "</object>" << endl;
}

/**
* Exceptions are special structs
*
* @param tstruct The struct definition
*/
void t_xml_generator::generate_xception(t_struct* txception) {
  generate_struct(txception);
}

/**
* Generates the XML block for a Thrift service.
*
* @param tservice The service definition
*/
void t_xml_generator::generate_service(t_service* tservice) {
  f_out_ << "<service name=\"" << service_name_ << "\">";
  print_doc_value(tservice->get_doc());
  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::iterator fn_iter = functions.begin();
  for ( ; fn_iter != functions.end(); fn_iter++) {
    string fn_name = (*fn_iter)->get_name();
    f_out_ << "<method name=\"" << fn_name << "\" returntype=\"";
    print_type((*fn_iter)->get_returntype());
    f_out_ << "\">";
    print_doc_value((*fn_iter)->get_doc());
    vector<t_field*> args = (*fn_iter)->get_arglist()->get_members();
    vector<t_field*>::iterator arg_iter = args.begin();
    for ( ; arg_iter != args.end(); arg_iter++) {
      f_out_ << "<arg index=\"" << (*arg_iter)->get_key() << "\" name=\"" << (*arg_iter)->get_name() << "\" type=\"";      
      print_type((*arg_iter)->get_type());
      f_out_ << "\"";
      if ((*arg_iter)->get_value() != NULL) {
        f_out_ << " default=\"";
        print_const_value((*arg_iter)->get_value());
        f_out_ << "\"";
      }
      f_out_ << ">";
      print_doc_value((*arg_iter)->get_doc());
      f_out_ << "</arg>" << endl;
    }
    vector<t_field*> excepts = (*fn_iter)->get_xceptions()->get_members();
    vector<t_field*>::iterator ex_iter = excepts.begin();
    for ( ; ex_iter != excepts.end(); ex_iter++) {
      f_out_ << "<ex index=\"" << (*ex_iter)->get_key() << "\" name=\"" << (*ex_iter)->get_name() << "\" type=\"";  
      print_type((*ex_iter)->get_type());
      f_out_ << "\"";
      if ((*ex_iter)->get_value() != NULL) {
        f_out_ << " default=\"";
        print_const_value((*ex_iter)->get_value());
        f_out_ << "\"";
      }
      f_out_ << ">";
      print_doc_value((*ex_iter)->get_doc());
      f_out_ << "</ex>" << endl;
    }
    f_out_ << "</method>" << endl;
  }
  f_out_ << "</service>" << endl;
}

THRIFT_REGISTER_GENERATOR(xml, "XML", "")

