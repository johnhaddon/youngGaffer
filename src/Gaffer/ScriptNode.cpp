#include "IECore/Exception.h"
#include "IECore/SimpleTypedData.h"

#include "Gaffer/ScriptNode.h"
#include "Gaffer/TypedPlug.h"
#include "Gaffer/Action.h"
#include "Gaffer/ApplicationRoot.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

using namespace Gaffer;

namespace Gaffer
{

GAFFER_DECLARECONTAINERSPECIALISATIONS( ScriptContainer, ScriptContainerTypeId )

}

IE_CORE_DEFINERUNTIMETYPED( ScriptNode );

ScriptNode::ScriptNode( const std::string &name )
	:	Node( name ), m_selection( new Set ), m_undoIterator( m_undoList.end() )
{
	m_fileNamePlug = new StringPlug( "fileName", Plug::In, "" );
	addChild( m_fileNamePlug );
	
	m_selection->memberAcceptanceSignal().connect( boost::bind( &ScriptNode::selectionSetAcceptor, this, ::_1, ::_2 ) );

	childRemovedSignal().connect( boost::bind( &ScriptNode::childRemoved, this, ::_1, ::_2 ) );
}

ScriptNode::~ScriptNode()
{
}

bool ScriptNode::acceptsParent( const GraphComponent *potentialParent ) const
{
	return potentialParent->isInstanceOf( ScriptContainer::staticTypeId() );
}

ApplicationRootPtr ScriptNode::application()
{
	return ancestor<ApplicationRoot>();
}

ConstApplicationRootPtr ScriptNode::application() const
{
	return ancestor<ApplicationRoot>();
}

bool ScriptNode::selectionSetAcceptor( Set::ConstPtr s, Set::ConstMemberPtr m )
{
	ConstNodePtr n = IECore::runTimeCast<const Node>( m );
	if( !n )
	{
		return false;
	}
	return n->parent<ScriptNode>()==this;
}

SetPtr ScriptNode::selection()
{
	return m_selection;
}

ConstSetPtr ScriptNode::selection() const
{
	return m_selection;
}

void ScriptNode::undo()
{
	if( m_undoIterator==m_undoList.begin() )
	{
		throw IECore::Exception( "Nothing to undo" );
	}
	m_undoIterator--;
	for( ActionVector::reverse_iterator it=(*m_undoIterator)->rbegin(); it!=(*m_undoIterator)->rend(); it++ )
	{
		(*it)->undoAction();
	}
}

void ScriptNode::redo()
{
	if( m_undoIterator==m_undoList.end() )
	{
		throw IECore::Exception( "Nothing to redo" );
	}
	for( ActionVector::iterator it=(*m_undoIterator)->begin(); it!=(*m_undoIterator)->end(); it++ )
	{
		(*it)->doAction();
	}
	m_undoIterator++;
}

void ScriptNode::copy( ConstSetPtr filter )
{
	ApplicationRootPtr app = application();
	if( !app )
	{
		throw( "ScriptNode has no ApplicationRoot" );
	}
	
	std::string s = serialise( filter );
	app->setClipboardContents( new IECore::StringData( s ) );
}

void ScriptNode::cut( ConstSetPtr filter )
{
	copy( filter );
	deleteNodes( filter );
}

void ScriptNode::paste()
{
	ApplicationRootPtr app = application();
	if( !app )
	{
		throw( "ScriptNode has no ApplicationRoot" );
	}
	
	IECore::ConstStringDataPtr s = IECore::runTimeCast<const IECore::StringData>( app->getClipboardContents() );
	if( s )
	{
		// set up something catch all the newly created nodes
		SetPtr newNodes = new Set;
		childAddedSignal().connect( boost::bind( (bool (Set::*)( IECore::RunTimeTypedPtr ) )&Set::add, newNodes.get(), ::_2 ) );
			
			// do the paste
			execute( s->readable() );

		// transfer the newly created nodes into the selection
		selection()->clear();
		selection()->add( newNodes->sequencedMembers().begin(), newNodes->sequencedMembers().end() );
	}
}

void ScriptNode::deleteNodes( ConstSetPtr filter )
{
	ChildNodeIterator nIt;
	for( nIt=childrenBegin<Node>(); nIt!=childrenEnd<Node>(); )
	{
	
		ChildNodeIterator next = nIt; next++;
		
		if( !filter || filter->contains( *nIt ) )
		{
			
			for( InputPlugIterator it=(*nIt)->inputPlugsBegin(); it!=(*nIt)->inputPlugsEnd(); it++ )
			{
				(*it)->setInput( 0 );
			}

			for( OutputPlugIterator it=(*nIt)->outputPlugsBegin(); it!=(*nIt)->outputPlugsEnd(); it++ )
			{
				(*it)->removeOutputs();
			}

			(*nIt)->parent<GraphComponent>()->removeChild( (*nIt) );
		}
		
		nIt = next;
		
	}

}

StringPlugPtr ScriptNode::fileNamePlug()
{
	return m_fileNamePlug;
}

ConstStringPlugPtr ScriptNode::fileNamePlug() const
{
	return m_fileNamePlug;
}
	
void ScriptNode::dirty( ConstPlugPtr dirty ) const
{
}

void ScriptNode::compute( PlugPtr output ) const
{
	assert( 0 );
}

void ScriptNode::execute( const std::string &pythonScript )
{
	throw IECore::Exception( "Cannot execute scripts on a ScriptNode not created in Python." );
}

ScriptNode::ScriptExecutedSignal &ScriptNode::scriptExecutedSignal()
{
	return m_scriptExecutedSignal;
}

PyObject *ScriptNode::evaluate( const std::string &pythonExpression )
{
	throw IECore::Exception( "Cannot execute scripts on a ScriptNode not created in Python." );
}

ScriptNode::ScriptEvaluatedSignal &ScriptNode::scriptEvaluatedSignal()
{
	return m_scriptEvaluatedSignal;
}

std::string ScriptNode::serialise( ConstSetPtr filter ) const
{
	throw IECore::Exception( "Cannot serialise scripts on a ScriptNode not created in Python." );
}

void ScriptNode::load()
{
	throw IECore::Exception( "Cannot load scripts on a ScriptNode not created in Python." );
}

void ScriptNode::save() const
{
	throw IECore::Exception( "Cannot save scripts on a ScriptNode not created in Python." );
}

void ScriptNode::childRemoved( GraphComponent *parent, GraphComponent *child )
{
	m_selection->remove( child );
}
