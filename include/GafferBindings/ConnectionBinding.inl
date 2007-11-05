#ifndef GAFFERBINDINGS_CONNECTIONBINDING_INL
#define GAFFERBINDINGS_CONNECTIONBINDING_INL

namespace GafferBindings
{

struct SlotCaller
{
	SlotCaller( Connection *connection )
		:	m_connection( connection )
	{
	}
	
	int operator()( float f )
	{
		return boost::python::extract<int>( m_connection->slot()( f ) );
	}
	Connection *m_connection;
};

template<typename Signal>
PyObject *Connection::create( Signal &s, boost::python::object &slot )
{

	Connection *connection = new Connection;
	
	typedef boost::python::manage_new_object::apply<Connection *>::type ResultConverter;
	connection->m_pyObject = ResultConverter()( connection );

	// now we need to stuff the slot into the dictionary for the
	// new python object. it has to go in there so it can be seen
	// by the garbage collector (the garbage collector can't see
	// inside the c++ Connection instance). this is an annoying
	// complication, and the only reason we have this weird create()
	// function instead of just having a normal constructor.
	boost::python::object connectionObj( boost::python::handle<>( boost::python::borrowed( connection->m_pyObject ) ) );
	boost::python::dict d = boost::python::extract<boost::python::dict>( connectionObj.attr( "__dict__" ) );
	d["slot"] = slot;
	connection->m_connection = s.connect( SlotCaller( connection ) );
	return connection->m_pyObject;
}

}; // namespace GafferBindings

#endif // GAFFERBINDINGS_CONNECTIONBINDING_INL
