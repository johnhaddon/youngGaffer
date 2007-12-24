#include "Gaffer/ScriptNode.h"
#include "Gaffer/TypedPlug.h"

using namespace Gaffer;

ScriptNode::ScriptNode( const std::string &name )
	:	Node( name )
{
	m_fileNamePlug = new StringPlug( "fileName", Plug::In, "" );
	addChild( m_fileNamePlug );
}

ScriptNode::~ScriptNode()
{
}

bool ScriptNode::acceptsParent( const GraphComponent *potentialParent ) const
{
	return false;
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
