#ifndef GAFFERBINDINGS_SETBINDING_INL
#define GAFFERBINDINGS_SETBINDING_INL

#include "boost/python.hpp"

#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/CatchingSlotCaller.h"

#include "Gaffer/Set.h"

#include "IECore/bindings/RunTimeTypedBinding.h"
#include "IECore/bindings/IntrusivePtrPatch.h"

namespace GafferBindings
{

namespace Detail
{

template<typename T>
boost::python::object setMembers( T &s )
{
	boost::python::object result( boost::python::handle<>( PySet_New( 0 ) ) );
	for( typename T::MemberContainer::const_iterator it=s.members().begin(); it!=s.members().end(); it++ )
	{
		result.attr( "add" )( *it );
	}
	return result;
}

template<typename T>
boost::python::object sequencedSetMembers( T &s )
{
	typename T::SequencedIndex &i = s.sequencedMembers();
	boost::python::list l;
	for( typename T::SequencedIndex::const_iterator it = i.begin(); it!=i.end(); it++ )
	{
		l.append( *it );
	}
	return boost::python::tuple( l );
}

}

template <typename T>
void bindSet( const char *className )
{

	typedef boost::python::class_<T, typename T::Ptr, boost::noncopyable, boost::python::bases<IECore::RunTimeTyped> > PyClass;
	
	boost::python::scope s = PyClass( className )
		.def( "add", &T::add )
		.def( "remove", &T::remove )
		.def( "clear", &T::clear )
		.def( "contains", &T::contains )
		.def( "size", &T::size )
		.def( "lastAdded", (typename T::ValuePtr (T::*)())&T::lastAdded )
		.def( "__contains__", &T::contains )
		.def( "__len__", &T::size )
		.def( "members", &Detail::setMembers<T> )
		.def( "sequencedMembers", &Detail::sequencedSetMembers<T> )
		.def( "memberAddedSignal", &T::memberAddedSignal, boost::python::return_internal_reference<1>() )
		.def( "memberRemovedSignal", &T::memberRemovedSignal, boost::python::return_internal_reference<1>() )
		.IE_COREPYTHON_DEFRUNTIMETYPEDSTATICMETHODS( T )
	;	

	SignalBinder<typename T::MemberSignal, DefaultSignalCaller<typename T::MemberSignal>, CatchingSlotCaller<typename T::MemberSignal> >::bind( "MemberSignal" );

	INTRUSIVE_PTR_PATCH( T, typename PyClass );
	
	boost::python::implicitly_convertible<typename T::Ptr, IECore::RunTimeTypedPtr>();
	boost::python::implicitly_convertible<typename T::Ptr, typename T::ConstPtr>();
	
}

} // namespace GafferBindings


#endif GAFFERBINDINGS_SETBINDING_INL
