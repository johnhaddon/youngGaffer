#include "boost/tokenizer.hpp"
#include "boost/format.hpp"

#include "IECore/MessageHandler.h"

#include "GafferBindings/Serialiser.h"

using namespace GafferBindings;
using namespace boost::python;

Serialiser::FunctionMap Serialiser::g_serialisers;

Serialiser::Serialiser( Gaffer::ConstNodePtr context, Gaffer::ConstSetPtr filter )
	:	m_filter( filter )
{
}

std::string Serialiser::serialise( Gaffer::ConstNodePtr context, Gaffer::ConstSetPtr filter )
{
	Serialiser s( context, filter );
	Gaffer::ChildNodeIterator nIt;
	for( nIt=context->childrenBegin<Gaffer::Node>(); nIt!=context->childrenEnd<Gaffer::Node>(); nIt++ )
	{
		s.add( *nIt );
	}
	
	std::string importStatements;
	for( std::set<std::string>::const_iterator it=s.m_modules.begin(); it!=s.m_modules.end(); it++ )
	{
		importStatements += "import " + *it + "\n";
	}
	
	return importStatements + "\n__nodes = {}\n\n" + s.m_result + "\n\ndel __nodes\n";
}

std::string Serialiser::modulePath( Gaffer::ConstGraphComponentPtr o )
{
	object po( boost::const_pointer_cast<Gaffer::GraphComponent>( o ) ); // we can only push non-const objects to python so we need the cast
	return modulePath( po );
}

std::string Serialiser::modulePath( boost::python::object &o )
{
	if( !PyObject_HasAttrString( o.ptr(), "__module__" ) )
	{
		return "";
	}
	std::string modulePath = extract<std::string>( o.attr( "__module__" ) );
	std::string className = extract<std::string>( o.attr( "__class__" ).attr( "__name__" ) );

	typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
	std::string sanitisedModulePath;
	Tokenizer tokens( modulePath, boost::char_separator<char>( "." ) );
	
	for( Tokenizer::iterator tIt=tokens.begin(); tIt!=tokens.end(); tIt++ )
	{
		if( tIt->compare( 0, 1, "_" )==0 )
		{
			// assume that module path components starting with _ are bogus, and are used only to bring
			// binary components into a namespace.
			continue;
		}
		Tokenizer::iterator next = tIt; next++;
		if( next==tokens.end() && *tIt == className )
		{
			// if the last module name is the same as the class name then assume this is just the file the
			// class has been implemented in.
			continue;
		}
		if( sanitisedModulePath.size() )
		{
			sanitisedModulePath += ".";
		}
		sanitisedModulePath += *tIt;
	}
	
	m_modules.insert( sanitisedModulePath );
	return sanitisedModulePath;
}

std::string Serialiser::add( Gaffer::ConstNodePtr o )
{
	if( m_filter && !m_filter->contains( o ) )
	{
		return "";
	}
	if( m_visited.find( o )!=m_visited.end() )
	{
		return "__nodes[\"" + o->getName() + "\"]";
	}
	
	std::string s = serialiseC( o );
	if( s=="" )
	{
		return s;
	}

	m_visited.insert( o );
	std::string name = "__nodes[\"" + o->getName() + "\"]";
	m_result += name + " = " + s + "\n";
	m_result += "addChild( " + name + " )\n\n";
	return o->getName();
}

std::string Serialiser::serialiseC( Gaffer::ConstGraphComponentPtr o )
{
	FunctionMap::const_iterator it = g_serialisers.find( o->typeId() );
	if( it==g_serialisers.end() )
	{
		const std::vector<IECore::TypeId> &baseTypes = IECore::RunTimeTyped::baseTypeIds( o->typeId() );
		for( std::vector<IECore::TypeId>::const_iterator tIt=baseTypes.begin(); tIt!=baseTypes.end(); tIt++ )
		{
			if( ( it = g_serialisers.find( *tIt ) )!=g_serialisers.end() )
			{
				break;
			}
		}
	}
	
	if( it==g_serialisers.end() )
	{
		IECore::msg( IECore::Msg::Warning, "Serialiser", boost::format( "Cannot serialise object \"%s\" of type \"%s\"." ) % o->getName() % o->typeName() );
		return "";
	}
	
	std::string result = it->second( *this, o );
	return it->second( *this, o );
}
	
void Serialiser::registerSerialiser( IECore::TypeId type, SerialisationFunction serialiser )
{
	g_serialisers[type] = serialiser;
}

