#include "GafferUI/Gadget.h"
#include "GafferUI/Style.h"

#include "IECore/SimpleTypedData.h"

#include "OpenEXR/ImathBoxAlgo.h"

using namespace GafferUI;
using namespace Imath;
using namespace std;

Gadget::Gadget( const std::string &name )
	:	GraphComponent( name ), m_style( Style::getDefaultStyle() )
{
}

Gadget::~Gadget()
{
}

bool Gadget::acceptsChild( Gaffer::ConstGraphComponentPtr potentialChild ) const
{
	return false;
}

bool Gadget::acceptsParent( const Gaffer::GraphComponent *potentialParent ) const
{
	return potentialParent->isInstanceOf( staticTypeId() );
}

ConstStylePtr Gadget::getStyle() const
{
	return m_style;
}

void Gadget::setStyle( ConstStylePtr style )
{
	if( style!=m_style )
	{
		m_style = style;
		renderRequestSignal()( this );
	}
}

void Gadget::setTransform( const Imath::M44f &matrix )
{
	if( matrix!=m_transform )
	{
		m_transform = matrix;
		renderRequestSignal()( this );
	}	
}

const Imath::M44f &Gadget::getTransform() const
{
	return m_transform;
}

Imath::M44f Gadget::fullTransform( ConstGadgetPtr ancestor ) const
{
	M44f result;
	const Gadget *g = this;
	do
	{
		result *= g->m_transform;
		g = g->parent<Gadget>();
	} while( g && g!=ancestor );
	
	return result;
}

void Gadget::render( IECore::RendererPtr renderer ) const
{
	renderer->attributeBegin();
		renderer->concatTransform( m_transform );
		renderer->setAttribute( "name", new IECore::StringData( fullName() ) );
		doRender( renderer );
	renderer->attributeEnd();
}

Imath::Box3f Gadget::transformedBound() const
{
	Box3f b = bound();
	return transform( b, getTransform() );
}

Imath::Box3f Gadget::transformedBound( ConstGadgetPtr ancestor ) const
{
	Box3f b = bound();
	return transform( b, fullTransform( ancestor ) );
}

Gadget::RenderRequestSignal &Gadget::renderRequestSignal()
{
	return m_renderRequestSignal;
}

Gadget::ButtonSignal &Gadget::buttonPressSignal()
{
	return m_buttonPressSignal;
}

Gadget::ButtonSignal &Gadget::buttonReleaseSignal()
{
	return m_buttonReleaseSignal;
}

Gadget::DragBeginSignal &Gadget::dragBeginSignal()
{
	return m_dragBeginSignal;
}

Gadget::DragDropSignal &Gadget::dragUpdateSignal()
{
	return m_dragUpdateSignal;
}

Gadget::DragDropSignal &Gadget::dropSignal()
{
	return m_dropSignal;
}

Gadget::DragDropSignal &Gadget::dragEndSignal()
{
	return m_dragEndSignal;
}
		
Gadget::KeySignal &Gadget::keyPressSignal()
{
	return m_keyPressSignal;
}

Gadget::KeySignal &Gadget::keyReleaseSignal()
{
	return m_keyReleaseSignal;
}
