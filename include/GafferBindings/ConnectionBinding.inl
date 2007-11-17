#ifndef GAFFERBINDINGS_CONNECTIONBINDING_INL
#define GAFFERBINDINGS_CONNECTIONBINDING_INL

namespace GafferBindings
{

template<int Arity, typename Signal>
struct SlotCallerBase;

template<typename Signal>
struct SlotCallerBase<1, Signal>
{
	SlotCallerBase( Connection *connection )
		:	m_connection( connection )
	{
	}
	
	typename Signal::result_type operator()( typename Signal::arg2_type a2 )
	{
		return boost::python::extract<typename Signal::result_type>( m_connection->slot()( a2 ) );
	}
	Connection *m_connection;
};

template<typename Signal>
struct SlotCallerBase<2, Signal>
{
	SlotCallerBase( Connection *connection )
		:	m_connection( connection )
	{
	}
	
	typename Signal::result_type operator()( typename Signal::arg2_type a2, typename Signal::arg2_type a3 )
	{
		return boost::python::extract<typename Signal::result_type>( m_connection->slot()( a2, a3 ) );
	}
	Connection *m_connection;
};

template<typename Signal>
struct SlotCaller : public SlotCallerBase<Signal::slot_function_type::arity, Signal>
{
	SlotCaller( Connection *connection )
		:	SlotCallerBase<Signal::slot_function_type::arity, Signal>( connection )
	{
	}
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
	connection->m_connection = s.connect( SlotCaller<Signal>( connection ) );
	return connection->m_pyObject;
}

}; // namespace GafferBindings

#endif // GAFFERBINDINGS_CONNECTIONBINDING_INL
