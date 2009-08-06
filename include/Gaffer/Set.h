#ifndef GAFFER_SET_H
#define GAFFER_SET_H

#include "Gaffer/Node.h"

#include "IECore/RunTimeTyped.h"

#include "boost/multi_index_container.hpp"
#include "boost/multi_index/sequenced_index.hpp"
#include "boost/multi_index/ordered_index.hpp"

namespace Gaffer
{

namespace Detail
{

struct MemberAcceptanceCombiner
{
	typedef bool result_type;
	
	template<typename InputIterator>
	bool operator()( InputIterator first, InputIterator last ) const
	{
		if( first==last )
		{
			return true;
		}
		while( first!=last )
		{
			if( !(*first) )
			{
				return false;
			}
			++first;
		}
		return true;
	}
};

} // namespace Detail

class Set : public IECore::RunTimeTyped, public boost::signals::trackable
{

	public :

		Set();
		virtual ~Set();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Set, SetTypeId, IECore::RunTimeTyped );
	
		typedef IECore::RunTimeTyped Member;
		typedef Member::Ptr MemberPtr;
		typedef Member::ConstPtr ConstMemberPtr;
		
		typedef boost::multi_index::multi_index_container<
			MemberPtr,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<boost::multi_index::identity<MemberPtr> >,
				boost::multi_index::sequenced<>
			>
		> MemberContainer;
		
		typedef const MemberContainer::nth_index<0>::type OrderedIndex;
		typedef const MemberContainer::nth_index<1>::type SequencedIndex;

		typedef boost::signal<bool ( ConstPtr, ConstMemberPtr ), Detail::MemberAcceptanceCombiner> MemberAcceptanceSignal;
		/// This signal is emitted to determine whether or not a member is eligible
		/// to be in the Set. Members are only added if all slots of the signal
		/// return true, or if no slots have been connected - otherwise an exception is thrown.
		/// You may call the signal yourself at any time to determine if a member is eligible.
		MemberAcceptanceSignal &memberAcceptanceSignal();
		/// A function suitable for use as a memberAcceptanceSignal slot. This rejects all
		/// members not derived from T.
		template<typename T>
		static bool typedMemberAcceptor( Ptr set, ConstMemberPtr potentialMember );

		/// Adds a member to the set. Returns true if the member
		/// was not already present and passes the acceptance tests,
		/// and false otherwise.
		bool add( MemberPtr member );
		/// Adds all the objects in the specified range into this set, returning
		/// the number of new members added.
		template<typename I>
		size_t add( I first, I last );
		/// Removes a member from the set. Returns true if the member
		/// is removed and false if it wasn't there in the first place.
		bool remove( MemberPtr member );
		/// Removes all the in the specified range from this set, returning the
		/// number of members removed.
		template<typename I>
		size_t remove( I first, I last );
		/// Removes all members from the set.
		void clear();
		/// Returns true if the object is a member of the set.
		bool contains( ConstMemberPtr object ) const;
		/// Returns the number of members of the set.
		size_t size() const;
		/// Returns the last object added to the Set, or 0 if the set is
		/// empty.
		MemberPtr lastAdded();
		ConstMemberPtr lastAdded() const;
		
		/// Const access to the internal container indices to allow iteration etc.
		const OrderedIndex &members() const;
		const SequencedIndex &sequencedMembers() const;

		typedef boost::signal<void ( Ptr, MemberPtr )> MemberSignal;
		
		MemberSignal &memberAddedSignal();
		MemberSignal &memberRemovedSignal();

	private :

		MemberAcceptanceSignal m_memberAcceptanceSignal;

		MemberContainer m_members;

		MemberSignal m_memberAddedSignal;
		MemberSignal m_memberRemovedSignal;
		
};

IE_CORE_DECLAREPTR( Set );
	
} // namespace Gaffer

#include "Gaffer/Set.inl"

#endif // GAFFER_SET_H
