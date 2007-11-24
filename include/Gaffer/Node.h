#ifndef GAFFER_NODE_H
#define GAFFER_NODE_H

#include "Gaffer/TypeIds.h"

#include "IECore/RunTimeTyped.h"

#include "boost/signals.hpp"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( Plug )
IE_CORE_FORWARDDECLARE( Node )

/// GraphComponent base class?? with userData?? and trackability - maybe not this (Plugs
/// aren't trackable?)
/// and Plug derives off it too?
/// maybe not - do Plugs need userData()? do we want them to be that heavy??
///
/// Hierarchical base class for plugs and nodes??
///
///		- benefits
///			- maintain a single namespace that maps well to python binding
///			- reduce amount of logic in destructors
///			- acceptsChild(), acceptsOrphaning()? - different for dynamic/static plugs
///			- probably useful elsewhere too
///		- downside
///			- no grouping of plugs into input/output? or in and out CompoundPlugs instead?
///			- increased memory usage (lots of signals for everything including plugs)
///				- can we offset this by only constructing signals when they are used?
///					- and can we detect when signals aren't used again and destroy 'em?
///						- do we need to?
///
/// Threading
///
///		- can we allow multiple computes() at once?
///		- or do we have to resort to computes() being threaded internally?
///
/// memory usage :
/// 
///		sizeof( map ) = 24 bytes
///		sizeof( signal ) = 20 bytes
///		100000 nodes * 20 plugs * 10 signals = 381 mbytes
///			-	before we even include refcounts and child maps etc
///
/// 	but :
///
///			baseline python = 123M
///			2,000,000 maps = 194M = 38 bytes a map
///			2,000,000 signals = 567M = 232 bytes a signal
///
///		ie. both maps and signals use much more than sizeof( map|signal ) in any case
///
///		what is a reasonable target number of nodes? and plugs? and signals?
///		what is a reasonable memory overhead for this target?
///		implement then optimise? or worry about it now?
///
/// difference between dynamic and static plugs and children?
///		- flag in plug?
///		- implement as Node::acceptsRemoval()
///			- don't thing plugs need to know their dynamic/static status
///			- but nodes do for serialisation i think
///				- or they do clever addition of plugs during parsing
///
/// difference between input and output plugs?
///		- either in and out CompoundPlug parents
///		- or flags in plug
///			- plugs do need to know their own direction - to disallow dirtying of inputs with no input connection
class Node : public IECore::RunTimeTyped, public boost::signals::trackable
{

	public :

		Node();
		virtual ~Node();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Node, NodeTypeId, IECore::RunTimeTyped );
		IE_CORE_DECLAREMEMBERPTR( Node );

		IECore::CompoundDataPtr userData();
		IECore::ConstCompoundDataPtr userData() const;

		/// ??
		const std::vector<ConstPlugPtr> &inputs() const;
		const std::vector<ConstPlugPtr> &outputs() const;
		
		template<typename T>
		typename T::ConstPtr plug( const std::string &name ) const;
		template<typename T>
		typename T::Ptr plug( const std::string &name );
		const std::vector<ConstNodePtr> &children() const;
		template<typename T>
		typename T::Ptr child( const std::string &name );
		
		typedef boost::signal<void ( NodePtr, PlugPtr )> NodePlugSignal;

		NodePlugSignal plugAddedSignal;
		NodePlugSignal plugRemovedSignal;
		/// if we can make their use cheap do these belong on the Plug?
		/// or do we have them on the node so you can get all signals at a time?
		/// or do we have them both on the node and the plug?
		NodePlugSignal plugSetSignal;
		NodePlugSignal plugDirtiedSignal;
		NodePlugSignal plugConnectedSignal;

		typedef boost::signal<void ( NodePtr, NodePtr )> NodeNodeSignal;

		NodeNodeSignal childAddedSignal;
		NodeNodeSignal childRemovedSignal;
		
	protected :
	
		// ??
		void addPlug( PlugPtr plug );
		void addInput( PlugPtr plug );
		void addOutput( PlugPtr plug );
	
		virtual void dirty( ConstPlugPtr dirty ) const = 0;
		virtual ObjectPtr compute( ConstPlugPtr output ) const = 0;

	private :
	
		friend class Plug;
	
		IECore::CompoundDataPtr m_userData;
	
		Node *m_parent;
		std::vector<NodePtr> m_children;

};

} // namespace Gaffer

#endif // GAFFER_NODE_H
