#include "GafferUI/SplinePlugGadget.h"
#include "GafferUI/Style.h"

#include "Gaffer/UndoContext.h"
#include "Gaffer/ScriptNode.h"

#include "IECore/CurvesPrimitive.h"
#include "IECore/AttributeBlock.h"
#include "IECore/SimpleTypedData.h"

#include "boost/bind.hpp"
#include "boost/bind/placeholders.hpp"

#include <algorithm>

using namespace GafferUI;
using namespace Gaffer;
using namespace Imath;

IE_CORE_DEFINERUNTIMETYPED( SplinePlugGadget );

struct SplinePlugGadget::UI
{
	IECore::CurvesPrimitivePtr curve;
};

SplinePlugGadget::SplinePlugGadget( const std::string &name )
	:	Gadget( name ), m_splines( new Set ), m_selection( new Set )
{
	m_splines->memberAddedSignal().connect( boost::bind( &SplinePlugGadget::splineAdded, this, ::_1,  ::_2 ) );
	m_splines->memberRemovedSignal().connect( boost::bind( &SplinePlugGadget::splineRemoved, this, ::_1,  ::_2 ) );

	buttonPressSignal().connect( boost::bind( &SplinePlugGadget::buttonPress, this, ::_1,  ::_2 ) );
	dragBeginSignal().connect( boost::bind( &SplinePlugGadget::dragBegin, this, ::_1, ::_2 ) );
	dragUpdateSignal().connect( boost::bind( &SplinePlugGadget::dragUpdate, this, ::_1, ::_2 ) );
	keyPressSignal().connect( boost::bind( &SplinePlugGadget::keyPress, this, ::_1,  ::_2 ) );

}

SplinePlugGadget::~SplinePlugGadget()
{
}

SetPtr SplinePlugGadget::splines()
{
	return m_splines;
}

ConstSetPtr SplinePlugGadget::splines() const
{
	return m_splines;
}

SetPtr SplinePlugGadget::selection()
{
	return m_selection;
}

ConstSetPtr SplinePlugGadget::selection() const
{
	return m_selection;
}

Imath::Box3f SplinePlugGadget::bound() const
{
	Box3f result;
	
	Set::SequencedIndex::iterator it;
	for( it=m_splines->sequencedMembers().begin(); it!=m_splines->sequencedMembers().end(); it++ )
	{
		SplineffPlugPtr spline = IECore::runTimeCast<SplineffPlug>( *it );
		if( spline )
		{
			unsigned n = spline->numPoints();
			for( unsigned i=0; i<n; i++ )
			{
				V3f p( 0 );
				p.x = spline->pointXPlug( i )->getValue();
				p.y = spline->pointYPlug( i )->getValue();
				result.extendBy( p );
			}
		}
	}
	return result;
}

void SplinePlugGadget::doRender( IECore::RendererPtr renderer ) const
{
	Set::SequencedIndex::iterator it;
	for( it=m_splines->sequencedMembers().begin(); it!=m_splines->sequencedMembers().end(); it++ )
	{
		SplineffPlugPtr spline = IECore::runTimeCast<SplineffPlug>( *it );
		if( spline )
		{
			{
				IECore::AttributeBlock ab( renderer );

				renderer->setAttribute( "gl:curvesPrimitive:useGLLines", new IECore::BoolData( true ) );
				renderer->setAttribute( "gl:smoothing:lines", new IECore::BoolData( true ) );

				// draw all the curves				
				SplineUIMap::iterator uiIt = m_uis.find( spline.get() );
				assert( uiIt!=m_uis.end() );

				//if( !uiIt->second.curve )
				//{
					updateCurve( uiIt );
				//}
				uiIt->second.curve->render( renderer );
			
				// draw handles for the points
				unsigned n = spline->numPoints();
				for( unsigned i=0; i<n; i++ )
				{
					if( m_selection->contains( spline->pointPlug( i ) ) )
					{
						renderer->setAttribute( Style::stateAttribute(), Style::stateValueSelected() );
					}
					else
					{
						renderer->setAttribute( Style::stateAttribute(), Style::stateValueNormal() );
					}
					
					V3f p( 0 );
					p.x = spline->pointXPlug( i )->getValue();
					p.y = spline->pointYPlug( i )->getValue();
					getStyle()->renderHandle( renderer, p );
				}
			}
		}
	}
}

void SplinePlugGadget::splineAdded( SetPtr splineSet, IECore::RunTimeTypedPtr splinePlug )
{
	SplineffPlugPtr s = IECore::runTimeCast<SplineffPlug>( splinePlug );
	if( s )
	{
		m_uis[s.get()] = UI();
		NodePtr node = s->node();
		/// \todo Only connect this once when there are many splines from the same node.
		/// Also remove the connections when all the splines from that node are removed.
		node->plugSetSignal().connect( boost::bind( &SplinePlugGadget::plugSet, this, ::_1 ) );
		/// \todo Remove these when the spline is removed.
		s->childAddedSignal().connect( boost::bind( &SplinePlugGadget::pointAdded, this, ::_1, ::_2 ) );
		s->childRemovedSignal().connect( boost::bind( &SplinePlugGadget::pointRemoved, this, ::_1, ::_2 ) );
	}
}

void SplinePlugGadget::pointAdded( GraphComponentPtr spline, GraphComponentPtr pointPlug )
{
	renderRequestSignal()( this );
}

void SplinePlugGadget::pointRemoved( GraphComponentPtr spline, GraphComponentPtr pointPlug )
{
	renderRequestSignal()( this );
}

void SplinePlugGadget::splineRemoved( SetPtr splineSet, IECore::RunTimeTypedPtr splinePlug )
{
	m_uis.erase( static_cast<Plug *>( splinePlug.get() ) );
}

void SplinePlugGadget::plugSet( PlugPtr plug )
{
	if( m_splines->contains( plug ) )
	{
		renderRequestSignal()( this );
	}
}

void SplinePlugGadget::updateCurve( SplineUIMap::iterator it ) const
{
	SplineffPlugPtr splinePlug = IECore::runTimeCast<SplineffPlug>( it->first );
	IECore::Splineff spline = splinePlug->getValue();
	IECore::Splineff::XInterval interval = spline.interval();
	
	unsigned numPoints = 100;
	
	IECore::V3fVectorDataPtr pd = new IECore::V3fVectorData();
	std::vector<V3f> &p = pd->writable();
	p.resize( numPoints );
	
	for( unsigned i=0; i<numPoints; i++ )
	{
		float x = lerp( interval.lower(), interval.upper(), (float)i / (float)(numPoints-1) );
		float y = spline( x );
		p[i] = V3f( x, y, 0.0f );
	}
	
	IECore::IntVectorDataPtr vertsPerCurve = new IECore::IntVectorData;
	vertsPerCurve->writable().push_back( numPoints );
	
	IECore::CurvesPrimitivePtr curve = new IECore::CurvesPrimitive(
		vertsPerCurve,
		IECore::CubicBasisf::linear(),
		false,
		pd
	);
	
	it->second.curve = curve;
}

bool SplinePlugGadget::buttonPress( GadgetPtr, const ButtonEvent &event )
{
	if( event.buttons!=ButtonEvent::Left )
	{
		return false;
	}
	
	if( event.modifiers && ModifiableEvent::Control )
	{
		// control click to make new points
		V3f intersection;
		if( !event.line.intersect( Plane3f( V3f( 0, 0, 1 ), 0 ), intersection ) )
		{
			return false;
		}
		if( !m_splines->size() )
		{
			return false;
		}
		SplineffPlugPtr spline = IECore::runTimeCast<SplineffPlug>( m_splines->lastAdded() );
		if( !spline )
		{
			return false;
		}

		UndoContext undoEnabler( spline->ancestor<ScriptNode>() );

		unsigned pointIndex = spline->addPoint();
		spline->pointXPlug( pointIndex )->setValue( intersection.x );
		spline->pointYPlug( pointIndex )->setValue( intersection.y );
		
		m_selection->clear();
		m_selection->add( spline->pointPlug( pointIndex ) );
		return true;
	}
	else
	{
		// click or shift click to choose points etc.
		bool clearedOnce = false;
		bool handled = false;
		bool shiftHeld = event.modifiers && ButtonEvent::Shift;
		Set::SequencedIndex::iterator it;
		for( it=m_splines->sequencedMembers().begin(); it!=m_splines->sequencedMembers().end(); it++ )
		{
			SplineffPlugPtr spline = IECore::runTimeCast<SplineffPlug>( *it );
			if( spline )
			{
				unsigned n = spline->numPoints();
				for( unsigned i=0; i<n; i++ )
				{
					V3f p( 0 );
					p.x = spline->pointXPlug( i )->getValue();
					p.y = spline->pointYPlug( i )->getValue();

					float d = event.line.distanceTo( p );
					PlugPtr pointPlug = spline->pointPlug( i );
					if( d < 0.25f ) /// \todo This ain't right
					{
						if( m_selection->contains( pointPlug ) )
						{
							if( shiftHeld )
							{
								m_selection->remove( pointPlug );
							}
						}
						else
						{
							if( !shiftHeld && !clearedOnce )
							{
								m_selection->clear();
								clearedOnce = true;
							}
							m_selection->add( pointPlug );
						}
						handled = true;
					}
				}
			}
		}

		if( handled )
		{
			renderRequestSignal()( this );
		}

		return handled;
	}
	// shouldn't get here
	return false;
}

IECore::RunTimeTypedPtr SplinePlugGadget::dragBegin( GadgetPtr gadget, const ButtonEvent &event )
{
	if( gadget!=this )
	{
		return 0;
	}
	
	if( !m_selection->size() )
	{
		return 0;
	}
	
	V3f i;
	if( event.line.intersect( Plane3f( V3f( 0, 0, 1 ), 0 ), i ) )
	{
		m_lastDragPosition = V2f( i.x, i.y );
		return m_selection;
	}
	
	return 0;
}

bool SplinePlugGadget::dragUpdate( GadgetPtr gadget, const ButtonEvent &event )
{
	/// \todo Undo support!
	V3f i;
	if( event.line.intersect( Plane3f( V3f( 0, 0, 1 ), 0 ), i ) )
	{
		V2f pos = V2f( i.x, i.y );
		V2f delta = pos - m_lastDragPosition;
	
		for( Set::OrderedIndex::iterator it=m_selection->members().begin(); it!=m_selection->members().end(); it++ )
		{
			PlugPtr plug = IECore::runTimeCast<Plug>( *it );
			FloatPlugPtr xPlug = plug->getChild<FloatPlug>( "x" );
			FloatPlugPtr yPlug = plug->getChild<FloatPlug>( "y" );
			xPlug->setValue( xPlug->getValue() + delta.x );
			yPlug->setValue( yPlug->getValue() + delta.y );
		}
	
		m_lastDragPosition = pos;
		return true;
	}
	return false;
}

bool SplinePlugGadget::keyPress( GadgetPtr gadget, const KeyEvent &event )
{
	if( event.key=="BackSpace" && m_selection->size() )
	{
		Plug *firstPlug = static_cast<Plug *>( (*(m_selection->members().begin())).get() );
		UndoContext undoEnabler( firstPlug->ancestor<ScriptNode>() );
		
		for( Set::OrderedIndex::iterator it=m_selection->members().begin(); it!=m_selection->members().end(); it++ )
		{
			Plug *pointPlug = static_cast<Plug *>( (*it).get() );
			GraphComponentPtr parent = pointPlug->parent<GraphComponent>();
			if( parent )
			{
				parent->removeChild( pointPlug );
			}
		}
	}
	return true;
}
