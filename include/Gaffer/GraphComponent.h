#ifndef GAFFER_GRAPHCOMPONENT_H
#define GAFFER_GRAPHCOMPONENT_H

#include "Gaffer/TypeIds.h"

#include "IECore/RunTimeTyped.h"

#include "boost/signals.hpp"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( GraphComponent )

class GraphComponent : public IECore::RunTimeTyped, public boost::signals::trackable
{

	public :

		GraphComponent( const std::string &name=staticTypeName() );
		virtual ~GraphComponent();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( GraphComponent, GraphComponentTypeId, IECore::RunTimeTyped );

		typedef boost::signal<void (GraphComponent *)> UnarySignal;
		typedef boost::signal<void (GraphComponent *, GraphComponent *)> BinarySignal;
		
		/// @name Naming
		/// All GraphComponents have a name, which must be unique among
		/// siblings.
		/// \todo Enforce name constraints.
		////////////////////////////////////////////////////////////////////
		//@{
		/// Sets the name for this component. Note that the requested name
		/// may have a numeric suffix appended to keep the name unique within
		/// parent(), and illegal characters may be replaced - for this reason
		/// the new name is returned.
		const std::string &setName( const std::string &name );
		/// Returns the name for this component.
		const std::string &getName() const;
		/// Returns the full path name from the topmost parent to this component.
		std::string fullName() const;
		/// A signal which is emitted whenever a name is changed.
		UnarySignal &nameChangedSignal();
		//@}
		
		/// @name Parent-child relationships
		////////////////////////////////////////////////////////////////////
		//@{
		/// The datatype used internally to store children.
		typedef std::list<GraphComponentPtr> ChildContainer;
		/// Components can accept or reject potential children by implementing this
		/// call.
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
		/// Components can accept or reject potential parents by implementing this
		/// call.
		virtual bool acceptsParent( const GraphComponent *potentialParent ) const;
		/// Adds a child to this component. If the child already has a parent it
		/// will first be removed from it. Note that the child may be renamed to
		/// preserve uniqueness, and an exception is thrown if the child or
		/// parent doesn't accept the new relationship.
		void addChild( GraphComponentPtr child );
		/// Removes a child. Throws an Exception if the passed component is
		/// not a child of this component.
		/// \todo Do we need acceptsRemoval()?
		void removeChild( GraphComponentPtr child );
		/// Get a child by name, performing a runTimeCast to T.
		template<typename T>
		typename T::Ptr getChild( const std::string &name );
		/// Get a child by name, performing a runTimeCast to T.
		template<typename T>
		typename T::ConstPtr getChild( const std::string &name ) const;
		/// Read only access to the internal container of children. This
		/// is useful for iteration over children.
		const ChildContainer &children() const;
		/// Returns the parent for this component, performing a runTimeCast to T.
		template<typename T>
		typename T::Ptr parent();
		/// Returns the parent for this component, performing a runTimeCast to T.
		/// \todo Create an equivalent function in the python binding, but have it
		/// be passed a typeid for the required return type.
		template<typename T>
		typename T::ConstPtr parent() const;
		/// Returns the first ancestor of type T.
		template<typename T>
		typename T::Ptr ancestor();
		/// Returns the first ancestor of type T.
		template<typename T>
		typename T::ConstPtr ancestor() const;
		/// A signal emitted when a child is added to this component. Slots should
		/// be of the form void ( parent, child ).
		BinarySignal &childAddedSignal();
		/// A signal emitted when a child is removed from this component. Slot format
		/// is as above.
		BinarySignal &childRemovedSignal();
		/// A signal emitted when the parent of this component changes.
		UnarySignal &parentChangedSignal();
		//@}
		
	private :

		/// \todo The memory overhead of all these signals may become too great.
		/// At this point we need to reimplement the signal returning functions to
		/// create the signals on the fly (and possibly to delete signals when they have
		/// no connections). One method might be to store a static map from this to signal *.
		/// Or alternatively we could make all the signals static. Both these trade off
		/// reduced memory usage for slower execution.
		UnarySignal m_nameChangedSignal;
		BinarySignal m_childAddedSignal;
		BinarySignal m_childRemovedSignal;
		UnarySignal m_parentChangedSignal;
		
		std::string m_name;
		GraphComponent *m_parent;
		ChildContainer m_children;

};

} // namespace Gaffer

#include "Gaffer/GraphComponent.inl"

#endif // GAFFER_GRAPHCOMPONENT_H
