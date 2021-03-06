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
//  $Id: except.h,v 1.9 2006-09-07 04:45:21 adedov Exp $

#ifndef _iqxmlrpc_except_h_
#define _iqxmlrpc_except_h_

#include <stdexcept>
#include "api_export.h"

namespace xmlpp
{
  class Node;
};

// Exceptions are conformant ot Fault Code Interoperability, version 20010516.
// http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
namespace iqxmlrpc
{

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4275)
#endif

//! Base class for iqxmlrpc exceptions.
class LIBIQXMLRPC_API Exception: public std::runtime_error {
  int ex_code;

public:
  Exception( const std::string& i, int c = -32000 /*undefined error*/ ):
    runtime_error( i ), ex_code(c) {}

  virtual int code() const { return ex_code; }
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//! XML Parser error.
class LIBIQXMLRPC_API Parse_error: public Exception {
public:
  Parse_error( const std::string& d ):
    Exception(std::string("Parser error. ") += d, -32700) {}
};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4275)
#endif

//! XML-RPC structures not conforming to spec.
class LIBIQXMLRPC_API XML_RPC_violation: public Exception {
public:
  static XML_RPC_violation at_node( const xmlpp::Node* );
  static XML_RPC_violation caused( const std::string&, const xmlpp::Node* = 0 );

  XML_RPC_violation():
    Exception("Server error. XML-RPC violation.", -32600) {}

private:
  XML_RPC_violation( const std::string& s ):
    Exception(std::string("Server error. XML-RPC violation: ") += s, -32600) {}
};


//! Exception is being thrown when user tries to create
//! Method object for unregistered name.
class LIBIQXMLRPC_API Unknown_method: public Exception {
public:
  Unknown_method( const std::string& name ):
    Exception((std::string("Server error. Method '") += name) += "' not found.", -32601) {}
};


//! Invalid method parameters exception.
class LIBIQXMLRPC_API Invalid_meth_params: public Exception {
public:
  Invalid_meth_params():
    Exception( "Server error. Invalid method parameters.", -32602 ) {}
};


//! Exception which user should throw from Method to
//! initiate fault response.
class LIBIQXMLRPC_API Fault: public Exception {
public:
  class LIBIQXMLRPC_API FCI_violation: public std::runtime_error {
  public:
    FCI_violation():
      runtime_error(
        "You should not specify application specific error codes "
        "in interval [-32768, -32000]."
      ) {};
  };

public:
  Fault( int c, const std::string& s );
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

} // namespace iqxmlrpc

#endif
