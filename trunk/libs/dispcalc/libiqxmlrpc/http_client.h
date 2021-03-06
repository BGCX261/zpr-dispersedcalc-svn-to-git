//  Libiqxmlrpc - an object-oriented XML-RPC solution.
//  Copyright (C) 2004-2007 Anton Dedov
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

#ifndef _libiqxmlrpc_http_client_h_
#define _libiqxmlrpc_http_client_h_

#include "api_export.h"
#include "reactor.h"
#include "connector.h"
#include "client_conn.h"

namespace iqxmlrpc
{

class Http_proxy_client_connection;

//! XML-RPC \b HTTP client's connection (works in blocking mode).
class LIBIQXMLRPC_API Http_client_connection:
  public iqxmlrpc::Client_connection,
  public iqnet::Connection
{
  std::auto_ptr<iqnet::Reactor_base> reactor;
  std::string out_str;
  http::Packet* resp_packet;

public:
  typedef Http_proxy_client_connection Proxy_connection;

  Http_client_connection( const iqnet::Socket&, bool non_block );

  void handle_input( bool& );
  void handle_output( bool& );

protected:
  http::Packet* do_process_session( const std::string& );
};

//! XML-RPC \b HTTP PROXY client connection.
//! DO NOT USE IT IN YOUR CODE.
class LIBIQXMLRPC_API Http_proxy_client_connection:
  public Http_client_connection
{
public:
  Http_proxy_client_connection( const iqnet::Socket& s, bool non_block ):
    Http_client_connection( s, non_block ) {}

private:
  virtual std::string decorate_uri() const;
};

} // namespace iqxmlrpc

#endif
// vim:ts=2:sw=2:et
