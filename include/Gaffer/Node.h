#ifndef GAFFER_NODE_H
#define GAFFER_NODE_H

#include "Gaffer/GraphComponent.h"
#include "Gaffer/PlugIterator.h"
#include "Gaffer/FilteredChildIterator.h"

#include "IECore/Object.h"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( Plug )
IE_CORE_FORWARDDECLARE( ValuePlug )
IE_CORE_FORWARDDECLARE( Node )
IE_CORE_FORWARDDECLARE( ScriptNode )

/// Threading
///
///		- can we allow multiple computes() at once?
///		- or do we have to resort to computes() being threaded internally?
///		- perhaps we could have a method which takes a bunch of input plugs, and guarantees
///		  that they'll have been computed upon return? that method could deal with the threading.
class Node : public GraphComponent
{

	public :

		Node( const std::string &name=staticTypeName() );
		virtual ~Node();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Node, NodeTypeId, GraphComponent );

		typedef boost::signal<void (PlugPtr)> UnaryPlugSignal;
		typedef boost::signal<void (PlugPtr, PlugPtr)> BinaryPlugSignal;
		
		/// @name Plug iterators
		//////////////////////////////////////////////////////////////
		//@{
		PlugIterator plugsBegin() const;
		PlugIterator plugsEnd() const;
		InputPlugIterator inputPlugsBegin() const;
		InputPlugIterator inputPlugsEnd() const;
		OutputPlugIterator outputPlugsBegin() const;
		OutputPlugIterator outputPlugsEnd() const;
		//@}
		
		/// @name Plug signals
		/// These signals are emitted on events relating to child Plugs
		/// of this Node. They are implemented on the Node rather than
		/// on individual Plugs to limit the proliferation of huge numbers
		/// of signals.
		//////////////////////////////////////////////////////////////
		//@{
		/// Called when the value on a plug of this node is set.
		UnaryPlugSignal &plugSetSignal();
		/// Called when a plug of this node is dirtied.
		UnaryPlugSignal &plugDirtiedSignal();
		/// Called when the input changes on a plug of this node.
		UnaryPlugSignal &plugInputChangedSignal();
		//@}
		
		/// Convenience function which simply returns ancestor<ScriptNode>()
		ScriptNodePtr scriptNode();
		/// Convenience function which simply returns ancestor<ScriptNode>()
		ConstScriptNodePtr scriptNode() const;
		
		/// Accepts only Nodes and Plugs.
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
		/// Accepts only Nodes.
		virtual bool acceptsParent( const GraphComponent *potentialParent ) const;
		
	protected :
		
		/// Called when an input plug becomes dirty. Must be implemented to dirty any
		/// output plugs which depend on the input.
		virtual void dirty( ConstPlugPtr dirty ) const = 0;
		/// Called when getValue() is called on an output plug which is dirty. Must
		/// be implemented to calculate and set the value for this Plug.
		virtual void compute( PlugPtr output ) const = 0;
		
	private :
		
		friend class Plug;
		friend class ValuePlug;
	
		UnaryPlugSignal m_plugSetSignal;
		UnaryPlugSignal m_plugDirtiedSignal;
		UnaryPlugSignal m_plugInputChangedSignal;
		
};

typedef FilteredChildIterator<TypePredicate<Node> > ChildNodeIterator;

} // namespace Gaffer

#endif // GAFFER_NODE_H
