#ifndef GAFFER_CONTAINER_H
#define GAFFER_CONTAINER_H

namespace Gaffer
{

template<typename Base, typename T>
class Container : public Base
{

	public :
	
		IE_CORE_DECLAREMEMBERPTR( Container );

		Container();
		virtual ~Container();

		//! @name RunTimeTyped interface
		////////////////////////////////////////////////////////////
		//@{
		virtual IECore::TypeId typeId() const;
		virtual std::string typeName() const;
		virtual bool isInstanceOf( IECore::TypeId typeId ) const;
		virtual bool isInstanceOf( const std::string &typeName ) const;
		static IECore::TypeId staticTypeId();
		static std::string staticTypeName();
		static bool inheritsFrom( IECore::TypeId typeId );
		static bool inheritsFrom( const std::string &typeName );
		//@}
				
		/// Accepts only type T.				
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
	
};

#define GAFFER_DECLARECONTAINERSPECIALISATIONS( TYPENAME, TYPEID  )			\
																			\
	template<>																\
	IECore::TypeId TYPENAME::staticTypeId()									\
	{																		\
		return (IECore::TypeId)TYPEID;										\
	}																		\
	template<>																\
	std::string TYPENAME::staticTypeName()									\
	{																		\
		return #TYPENAME;													\
	}																		\


} // namespace Gaffer

#include "Gaffer/Container.inl"

#endif // GAFFER_CONTAINER_H
