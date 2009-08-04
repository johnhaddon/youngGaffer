#include "boost/python.hpp"
#include "boost/python/suite/indexing/container_utils.hpp"

#include "GafferBindings/SignalBinding.h"
#include "GafferBindings/CatchingSlotCaller.h"

#include "Gaffer/Set.h"

#include "IECore/bindings/RunTimeTypedBinding.h"

using namespace Gaffer;

namespace GafferBindings
{

namespace Detail
{

boost::python::object setMembers( Set &s )
{
	boost::python::object result( boost::python::handle<>( PySet_New( 0 ) ) );
	for( Set::MemberContainer::const_iterator it=s.members().begin(); it!=s.members().end(); it++ )
	{
		result.attr( "add" )( *it );
	}
	return result;
}

boost::python::object sequencedSetMembers( Set &s )
{
	Set::SequencedIndex &i = s.sequencedMembers();
	boost::python::list l;
	for( Set::SequencedIndex::const_iterator it = i.begin(); it!=i.end(); it++ )
	{
		l.append( *it );
	}
	return boost::python::tuple( l );
}

SetPtr setConstructor( boost::python::object o )
{
	SetPtr result = new Set;
	std::vector<Set::MemberPtr> members;
	boost::python::container_utils::extend_container( members, o );
	result->add( members.begin(), members.end() );
	return result;
}

static size_t addFromSequence( Set &s, boost::python::object o )
{
	std::vector<Set::MemberPtr> members;
	boost::python::container_utils::extend_container( members, o );
	return s.add( members.begin(), members.end() );
}

static size_t removeFromSequence( Set &s, boost::python::object o )
{
	std::vector<Set::MemberPtr> members;
	boost::python::container_utils::extend_container( members, o );
	return s.remove( members.begin(), members.end() );
}

struct MemberAcceptanceSlotCaller
{
	bool operator()( boost::python::object slot, ConstSetPtr s, IECore::ConstRunTimeTypedPtr m )
	{
		try
		{
			return slot( boost::const_pointer_cast<Set>( s ), boost::const_pointer_cast<IECore::RunTimeTyped>( m ) );
		}
		catch( const boost::python::error_already_set &e )
		{
			PyErr_PrintEx( 0 ); // clears the error status
		}
		return false;
	}
};

} // namespace Detail

void bindSet()
{
	
	boost::python::scope s = IECore::RunTimeTypedClass<Set>()
		.def( boost::python::init<>() )
		.def( "__init__", boost::python::make_constructor( Detail::setConstructor ) )
		.def( "add", &Detail::addFromSequence )
		.def( "add", (bool (Set::*)( Set::MemberPtr ) )&Set::add )
		.def( "remove", &Detail::removeFromSequence )
		.def( "remove", (bool (Set::*)( Set::MemberPtr ) )&Set::remove )
		.def( "clear", &Set::clear )
		.def( "contains", &Set::contains )
		.def( "size", &Set::size )
		.def( "lastAdded", (Set::MemberPtr (Set::*)())&Set::lastAdded )
		.def( "__contains__", &Set::contains )
		.def( "__len__", &Set::size )
		.def( "members", &Detail::setMembers )
		.def( "sequencedMembers", &Detail::sequencedSetMembers )
		.def( "memberAddedSignal", &Set::memberAddedSignal, boost::python::return_internal_reference<1>() )
		.def( "memberRemovedSignal", &Set::memberRemovedSignal, boost::python::return_internal_reference<1>() )
		.def( "memberAcceptanceSignal", &Set::memberAcceptanceSignal, boost::python::return_internal_reference<1>() )
	;	

	SignalBinder<Set::MemberSignal, DefaultSignalCaller<Set::MemberSignal>, CatchingSlotCaller<Set::MemberSignal> >::bind( "MemberSignal" );
	SignalBinder<Set::MemberAcceptanceSignal, DefaultSignalCaller<Set::MemberAcceptanceSignal>, Detail::MemberAcceptanceSlotCaller>::bind( "MemberAcceptanceSignal" );
	
}

} // namespace GafferBindings
