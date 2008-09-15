#include "GafferUI/LinearContainer.h"

#include "IECore/Exception.h"

#include "boost/bind.hpp"

using namespace GafferUI;
using namespace Imath;
using namespace std;

LinearContainer::LinearContainer( const std::string &name, Orientation orientation,
	Alignment alignment, float spacing )
	:	ContainerGadget( name ), m_orientation( orientation ), m_alignment( alignment ), m_spacing( spacing ), m_clean( true )
{
	// we already initialised these values above, but that didn't perform any range checking,
	// so we set them here as well. the reason we initialize them at all is so that the set
	// methods will determine that the values are not changing, and therefore not trigger
	// renderRequestSignal() unecessarily.
	setOrientation( orientation );
	setAlignment( alignment );
	setSpacing( spacing );

	renderRequestSignal().connect( boost::bind( &LinearContainer::renderRequested, this, ::_1 ) );
}

LinearContainer::~LinearContainer()
{
}

void LinearContainer::setOrientation( Orientation orientation )
{
	if( orientation < X || orientation > Y )
	{
		throw IECore::InvalidArgumentException( "Invalid orientation" );
	}
	if( orientation != m_orientation )
	{
		m_orientation = orientation;
		renderRequestSignal()( this );
		m_clean = false;
	}
}

LinearContainer::Orientation LinearContainer::getOrientation() const
{
	return m_orientation;
}

void LinearContainer::setAlignment( Alignment alignment )
{
	if( alignment < Min || alignment > Max )
	{
		throw IECore::InvalidArgumentException( "Invalid alignment" );
	}
	if( alignment != m_alignment )
	{
		m_alignment = alignment;
		renderRequestSignal()( this );
		m_clean = false;
	}
}

LinearContainer::Alignment LinearContainer::getAlignment() const
{
	return m_alignment;
}

void LinearContainer::setSpacing( float spacing )
{
	if( spacing < 0.0f )
	{
		throw IECore::InvalidArgumentException( "Invalid spacing" );	
	}
	if( spacing!=m_spacing )
	{
		m_spacing = spacing;
		renderRequestSignal()( this );
		m_clean = false;
	}
}

float LinearContainer::getSpacing() const
{
	return m_spacing;
}
		
void LinearContainer::renderRequested( GadgetPtr gadget )
{
	/// \todo We don't need to recalculate the offsets every time a rerender is needed.
	/// the render request can be made for many reasons - a child changing its colour for
	/// instance. if there were a boundChanged() signal then we could attach to that instead,
	/// and potentially get some optimisation. it's not clear that that is necessary yet though.
	m_clean = false;
}

void LinearContainer::doRender( IECore::RendererPtr renderer ) const
{
	calculateChildTransforms();
	ContainerGadget::doRender( renderer );
}

void LinearContainer::calculateChildTransforms() const
{
	if( m_clean )
	{
		return;
	}
		
	int axis = m_orientation - 1;
	V3f size( 0 );
	vector<Box3f> bounds;
	for( ChildContainer::const_iterator it=children().begin(); it!=children().end(); it++ )
	{
		Box3f b = static_cast<const Gadget *>(it->get())->bound();
		for( int a=0; a<3; a++ )
		{
			if( a==axis )
			{
				size[a] += b.size()[a];
			}
			else
			{
				size[a] = max( size[a], b.size()[a] );
			}
		}
		bounds.push_back( b );
	}
	size[axis] += (children().size() - 1) * m_spacing;

	float offset = -size[axis]/2.0f;
	
	int i = 0;
	for( ChildContainer::const_iterator it=children().begin(); it!=children().end(); it++ )
	{
		const Box3f &b = bounds[i++];
		
		V3f childOffset;
		for( int a=0; a<3; a++ )
		{
			if( a==axis )
			{
				childOffset[a] = offset - b.min[a];
			}
			else
			{
				switch( m_alignment )
				{
					case Min :
						childOffset[a] = -size[a]/2.0f - b.min[a];
						break;
					case Centre :
						childOffset[a] = -b.center()[a];
						break;
					default :
						// max
						childOffset[a] = size[a]/2.0f - b.max[a];
				}
			}
		}
		
		M44f m; m.translate( childOffset );
		static_cast<const Gadget *>(it->get())->setTransform( m );
		
		offset += b.size()[axis] + m_spacing;
	}
	
	m_clean = true;
}
