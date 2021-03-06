//  Libiqxmlrpc - an object-oriented XML-RPC solution.
//  Copyright (C) 2004-2006 Anton Dedov
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
//
//  $Id: request.h,v 1.7 2006-09-07 04:45:21 adedov Exp $

#ifndef _iqxmlrpc_request_h_
#define _iqxmlrpc_request_h_

#include <string>
#include <vector>
#include "api_export.h"
#include "value.h"
#include "xml_utils.h"

namespace xmlpp {
  class Node;
}

namespace iqxmlrpc {

class Request;
class Value;
typedef std::vector<Value> Param_list;

//! Build request object from XML-formed string.
LIBIQXMLRPC_API  Request* parse_request( const std::string& );

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

//! Incoming RPC request.
class LIBIQXMLRPC_API Request: public Serializable_to_xml {
public:
  typedef Param_list::const_iterator const_iterator;

private:
  std::string name;
  Param_list  params;

public:
  Request( const xmlpp::Document* );
  Request( const xmlpp::Node* );
  Request( const std::string& name, const Param_list& params );
  virtual ~Request();

  const std::string& get_name()   const { return name; }
  const Param_list&  get_params() const { return params; }

private:
  void parse( const xmlpp::Node* );
  void parse_name( const xmlpp::Node* );
  void parse_params( const xmlpp::Node* );
  virtual xmlpp::Document* to_xml() const;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

} // namespace iqxmlrpc

#endif
