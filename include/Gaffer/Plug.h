#ifndef GAFFER_PLUG_H
#define GAFFER_PLUG_H

#include "Gaffer/GraphComponent.h"

#include "IECore/Object.h"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( Plug )
IE_CORE_FORWARDDECLARE( Node )

/// CompoundPlug mapping onto CompoundParameter? or just ui groupings independent of plugs?
/// ParameterPlug<ParameterType> subclasses.
///
///		- but - if we want connections into the components of vector and color plugs then
///				we'll have to not wrap V3fParameter but instead use a Compound of FloatPlugs
///					-it'd be nice to have getValue() for a V3fPlug return V3fData and not CompoundData though.
///						-what to do?
///		- so - shall we abandon Parameters?
///			 - or have a mapping layer so we can have OpHolderNode?
///			 - or use them where appropriate?
///				- they do implement a lot of useful validation logic already
///
///		- either way we should complete the Plug interface before worrying
///
/// memory usage
///
///		- IntPlug storing IntData rather than int has some overhead.
///		- is this a problem?
///
/// let's not have DoublePlug or DoubleVectorPlug
///		- keep all real types as float
///		- and all integers as int? - except for maybe UIntVectorData?
///
/// CompoundPlug with dynamic addition/removal of plugs? that would be useful for lots of stuff?
/// Cannot dirty an input plug without a connection
///		- and input plugs revert to default (or current value) on disconnect?
/// Expressions
///		- Node? or intrinsic to Plug somehow? do they dirty automatically? or compute each time?
///		- it'd be nice not to have to call IntData::value() just to get values from Plugs in these
/// Memory management
///		- Is this where we flush less often used values into a disk cache? and load 'em again when needed?
///
/// Image plugs with partial computation - how would they work????
///
class Plug : public GraphComponent
{

	public :
	
		enum Direction
		{
			In = 0,
			Out = 1
		};
	
		Plug( Direction direction=In );
		virtual ~Plug();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Plug, PlugTypeId, GraphComponent );

		/// @name Parent-child relationships
		//////////////////////////////////////////////////////////////////////
		//@{
		/// Accepts no children.
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
		/// Accepts only Nodes or Plugs as a parent.
		virtual bool acceptsParent( ConstGraphComponentPtr potentialParent ) const;
		/// Just returns parent<Node>() as a syntactic convenience.
		NodePtr node();
		/// Just returns parent<Node>() as a syntactic convenience.
		ConstNodePtr node() const;
		//@}

		Direction direction() const;

		/// @name Connections
		///////////////////////////////////////////////////////////////////////
		//@{
		typedef std::list<Plug *> OutputContainer;
		/// Accepts any input, provided that direction()==In.
		virtual bool acceptsInput( ConstPlugPtr input ) const;
		/// Pass input as 0 to remove any existing inputs.
		void setInput( PlugPtr input );
		template<typename T>
		typename T::Ptr getInput();
		template<typename T>
		typename T::ConstPtr getInput() const;
		const OutputContainer &outputs() const;
		//@}
					
	private :

		void setInput( PlugPtr input, bool emit );

		Direction m_direction;
		Plug *m_input;
		OutputContainer m_outputs;
				
};

IE_CORE_DECLAREPTR( Plug );

#include "Gaffer/Plug.inl"

} // namespace Gaffer

#endif // GAFFER_PLUG_H
