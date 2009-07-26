#ifndef GAFFER_SET_H
#define GAFFER_SET_H

#include "Gaffer/Node.h"

#include "IECore/RunTimeTyped.h"

#include "boost/multi_index_container.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index/ordered_index.hpp"

namespace Gaffer
{

template<typename T>
class Set : public IECore::RunTimeTyped, public boost::signals::trackable
{

	public :

		Set();
		virtual ~Set();

		IE_CORE_DECLAREMEMBERPTR( Set );
	
		typedef T ValueType;
		typedef typename T::Ptr ValuePtr;
		
		typedef boost::multi_index::multi_index_container<
			ValuePtr,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<boost::multi_index::identity<ValuePtr> >,
				boost::multi_index::sequenced<>
			>
		> MemberContainer;
		
		typedef const typename MemberContainer::template nth_index<0>::type OrderedIndex;
		typedef const typename MemberContainer::template nth_index<1>::type SequencedIndex;

		//! @name RunTimeTyped interface
		////////////////////////////////////////////////////////////
		//@{
		virtual IECore::TypeId typeId() const;
		virtual const char *typeName() const;
		virtual bool isInstanceOf( IECore::TypeId typeId ) const;
		virtual bool isInstanceOf( const char *typeName ) const;
		static IECore::TypeId staticTypeId();
		static const char *staticTypeName();
		static bool inheritsFrom( IECore::TypeId typeId );
		static bool inheritsFrom( const char *typeName );
		typedef IECore::RunTimeTyped BaseClass;
		//@}

		/// Adds a member to the set. Returns true if the member
		/// was not already present, and false otherwise.
		bool add( typename T::Ptr member );
		/// Adds all the objects in the specified range into this set, returning
		/// the number of new members added. Uses IECore::runTimeCast
		/// to weed out unsuitable members.
		template<typename I>
		size_t add( I first, I last );
		/// Removes a member from the set. Returns true if the member
		/// is removed and false if it wasn't there in the first place.
		bool remove( typename T::Ptr member );
		/// Removes all the in the specified range from this set, returning the
		/// number of members removed.
		template<typename I>
		size_t remove( I first, I last );
		/// Removes all members from the set.
		void clear();
		/// Returns true if the object is a member of the set.
		bool contains( typename T::ConstPtr object ) const;
		/// Returns the number of members of the set.
		size_t size() const;
		/// Returns the last object added to the Set, or 0 if the set is
		/// empty.
		typename T::Ptr lastAdded();
		typename T::ConstPtr lastAdded() const;
		
		/// Const access to the internal container indices to allow iteration etc.
		const OrderedIndex &members() const;
		const SequencedIndex &sequencedMembers() const;

		typedef boost::signal<void ( Ptr, typename T::Ptr )> MemberSignal;
		
		MemberSignal &memberAddedSignal();
		MemberSignal &memberRemovedSignal();
		

	private :

		IE_CORE_DECLARERUNTIMETYPEDDESCRIPTION( Set<T> );

		MemberContainer m_members;

		MemberSignal m_memberAddedSignal;
		MemberSignal m_memberRemovedSignal;
		
};

#define GAFFER_DECLARESETSPECIALISATIONS( TYPENAME, TYPEID  )				\
																			\
	template<>																\
	IECore::TypeId TYPENAME::staticTypeId()									\
	{																		\
		return (IECore::TypeId)TYPEID;										\
	}																		\
	template<>																\
	const char *TYPENAME::staticTypeName()									\
	{																		\
		return #TYPENAME;													\
	}																		\
	template<> 																\
	const IECore::RunTimeTyped::TypeDescription<TYPENAME>  TYPENAME::g_typeDescription; \


typedef Set<GraphComponent> GraphComponentSet;
IE_CORE_DECLAREPTR( GraphComponentSet );

typedef Set<Node> NodeSet;
IE_CORE_DECLAREPTR( NodeSet );

typedef Set<Plug> PlugSet;
IE_CORE_DECLAREPTR( PlugSet );
	
} // namespace Gaffer

#include "Gaffer/Set.inl"

#endif // GAFFER_SET_H
