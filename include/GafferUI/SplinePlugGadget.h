#ifndef GAFFERUI_SPLINEPLUGGADGET_H
#define GAFFERUI_SPLINEPLUGGADGET_H

#include "GafferUI/Gadget.h"

#include "Gaffer/SplinePlug.h"
#include "Gaffer/Set.h"

namespace GafferUI
{

class SplinePlugGadget : public Gadget
{

	public :

		SplinePlugGadget( const std::string &name=staticTypeName() );
		virtual ~SplinePlugGadget();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( SplinePlugGadget, SplinePlugGadgetTypeId, Gadget );

		/// The splines to be edited
		Gaffer::PlugSetPtr splines();
		Gaffer::ConstPlugSetPtr splines() const;

		/// The selected spline points
		Gaffer::PlugSetPtr selection();
		Gaffer::ConstPlugSetPtr selection() const;

		virtual Imath::Box3f bound() const;
		
	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;
		
	private :
	
		void splineAdded( Gaffer::PlugSetPtr splineSet, Gaffer::PlugPtr splinePlug );
		void splineRemoved( Gaffer::PlugSetPtr splineSet, Gaffer::PlugPtr splinePlug );
		void plugSet( Gaffer::PlugPtr plug );
		Gaffer::PlugSetPtr m_splines;
		Gaffer::PlugSetPtr m_selection;
		
		void pointAdded( Gaffer::GraphComponentPtr spline, Gaffer::GraphComponentPtr point );
		void pointRemoved( Gaffer::GraphComponentPtr spline, Gaffer::GraphComponentPtr point );
		
		struct UI;
		typedef std::map<Gaffer::Plug *, UI> SplineUIMap;
		mutable SplineUIMap m_uis;
		void updateCurve( SplineUIMap::iterator it ) const;
		
		bool buttonPress( GadgetPtr gadget, const ButtonEvent &event );
		IECore::RunTimeTypedPtr dragBegin( GadgetPtr gadget, const ButtonEvent &event );	
		bool dragUpdate( GadgetPtr gadget, const ButtonEvent &event );
		Imath::V2f m_lastDragPosition;
		
		bool keyPress( GadgetPtr gadget, const KeyEvent &event );
				
};

} // namespace GafferUI

#endif // GAFFERUI_SPLINEPLUGGADGET_H
