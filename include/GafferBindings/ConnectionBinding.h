#ifndef GAFFERBINDINGS_CONNECTIONBINDING_H
#define GAFFERBINDINGS_CONNECTIONBINDING_H

#include "boost/python.hpp"
#include "boost/signals.hpp"

namespace GafferBindings
{

void bindConnection();

class Connection : public boost::noncopyable
{
	public :
		
		~Connection();
		
		template<typename Signal, typename SlotCaller>
		static PyObject *create( Signal &s, boost::python::object &slot );
		
		void disconnect() const;
		bool connected() const;
		void block();
		void unblock();
		bool blocked() const;
		
		boost::python::object slot();
		
	private :
		
		Connection();
	
		PyObject *m_pyObject;
		boost::signals::connection m_connection;

};

} // namespace GafferBindings

#include "GafferBindings/ConnectionBinding.inl"

#endif // GAFFERBINDINGS_CONNECTIONBINDING_H
