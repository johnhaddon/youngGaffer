#include "GafferBindings/ConnectionBinding.h"

using namespace boost::python;
using namespace GafferBindings;

Connection::Connection()
{
}

Connection::~Connection()
{
	disconnect();
}

void Connection::disconnect() const
{
	m_connection.disconnect();
}

bool Connection::connected() const
{
	return m_connection.connected();
}

void Connection::block()
{
	m_connection.block();
}

void Connection::unblock()
{
	m_connection.unblock();
}

bool Connection::blocked() const
{
	return m_connection.blocked();
}

boost::python::object Connection::slot()
{
	object o( handle<>( borrowed( m_pyObject ) ) );
	dict d = extract<dict>( o.attr( "__dict__" ) );
	return d["slot"];
}

void GafferBindings::bindConnection()
{
	class_<Connection, boost::noncopyable>( "Connection", no_init )
		.def( "disconnect", &Connection::disconnect )
		.def( "connected", &Connection::connected )
		.def( "block", &Connection::block )
		.def( "unblock", &Connection::unblock )
		.def( "blocked", &Connection::blocked )
	;
}
