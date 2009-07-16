#ifndef GAFFERBINDINGS_SERIALISER_H
#define GAFFERBINDINGS_SERIALISER_H

#include "boost/python.hpp"

#include "Gaffer/Node.h"
#include "Gaffer/Set.h"

#include "boost/function.hpp"

namespace GafferBindings
{

// must be able to
//
//	- serialise a script and reload it
//	- also serialise script nodes embedded in that script

// should this be paired with an ExecutionContext type thing?
//
//		ExecutionContext could be like the executionGlobals in
//		ScriptNodeWrapper
//
//		but also act like a context manager for "with" by pushing
//		it's methods into the current global table in __enter__ 
//		and then popping them in __exit__

// it could actually serialise the target of the context maybe?
class Serialiser
{

	public :
	
		/// Serialises all the children of context, yielding a string which should
		/// be executed in an equivalent context to reconstruct it. The filter can be
		/// used to restrict the set of children which are serialised.
		static std::string serialise( Gaffer::ConstNodePtr context, Gaffer::ConstNodeSetPtr filter=0 );
		
		//! @name Serialiser methods
		/// These functions are for use within serialisation functions registered
		/// with registerSerialiser. As a Serialiser cannot be instantiated directly
		/// they're of no use to anything else.
		/// \todo merge add and serialiseC together? or rename them nicely? make serialiseC
		/// specific to plugs only?
		/////////////////////////////////////////////////////////////////////////////////
		//@{
		/// Makes sure the module needed for object o is imported, and returns a string
		/// which can be used to refer to it.
		std::string modulePath( Gaffer::ConstGraphComponentPtr o );
		/// As above but returns the empty string if the object has no module (is a built in type).
		std::string modulePath( boost::python::object &o );
		/// Adds the specified object to the serialisation and returns the name of
		/// a local variable which can be used to reference it in subsequent parts of the
		/// serialisation. If component is not in filter then nothing is done and the empty
		/// string is returned.
		std::string add( Gaffer::ConstNodePtr o );
		/// Returns a serialisation for component - this will not yet have been added
		/// to the result.
		std::string serialiseC( Gaffer::ConstGraphComponentPtr o );
		//@}
		
		typedef boost::function<std::string ( Serialiser &s, Gaffer::ConstGraphComponentPtr g )> SerialisationFunction;
		static void registerSerialiser( IECore::TypeId type, SerialisationFunction serialiser );
		
	private :

		Serialiser( Gaffer::ConstNodePtr context, Gaffer::ConstNodeSetPtr filter );
		
		std::string m_result;
		
		Gaffer::ConstNodeSetPtr m_filter;
		
		std::set<std::string> m_modules;
		
		typedef std::set<Gaffer::ConstNodePtr> VisitedSet;
		VisitedSet m_visited;		
		
		typedef std::map<IECore::TypeId, SerialisationFunction> FunctionMap;
		static FunctionMap g_serialisers;

};

} // namespace GafferBindings

#endif // GAFFERBINDINGS_SERIALISER_H
