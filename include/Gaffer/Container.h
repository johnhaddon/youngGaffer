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
		virtual const char *typeName() const;
		virtual bool isInstanceOf( IECore::TypeId typeId ) const;
		virtual bool isInstanceOf( const char *typeName ) const;
		static IECore::TypeId staticTypeId();
		static const char *staticTypeName();
		static bool inheritsFrom( IECore::TypeId typeId );
		static bool inheritsFrom( const char *typeName );
		typedef Base BaseClass;
		//@}
				
		/// Accepts only type T.				
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
	
	private :
	
		static const IECore::RunTimeTyped::TypeDescription< Container<Base, T> > g_typeDescription;
};

#define GAFFER_DECLARECONTAINERSPECIALISATIONS( TYPENAME, TYPEID  )			\
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


} // namespace Gaffer

#include "Gaffer/Container.inl"

#endif // GAFFER_CONTAINER_H
