#ifndef GAFFER_PLUG_INL
#define GAFFER_PLUG_INL

namespace Gaffer
{

template<typename T>
typename T::Ptr Plug::getInput()
{
	return IECore::runTimeCast<T>( m_input );
}
		
template<typename T>
typename T::ConstPtr Plug::getInput() const
{
	return IECore::runTimeCast<const T>( m_input );
}

} // namespace Gaffer

#endif // GAFFER_PLUG_INL
