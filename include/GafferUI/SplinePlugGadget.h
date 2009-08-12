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
		Gaffer::SetPtr splines();
		Gaffer::ConstSetPtr splines() const;

		/// The selected spline points
		Gaffer::SetPtr selection();
		Gaffer::ConstSetPtr selection() const;

		virtual Imath::Box3f bound() const;
		
	protected :
	
		virtual void doRender( IECore::RendererPtr renderer ) const;
		
	private :
	
		void splineAdded( Gaffer::SetPtr splineSet, IECore::RunTimeTypedPtr splinePlug );
		void splineRemoved( Gaffer::SetPtr splineSet, IECore::RunTimeTypedPtr splinePlug );
		void plugSet( Gaffer::PlugPtr plug );
		Gaffer::SetPtr m_splines;
		Gaffer::SetPtr m_selection;
		
		void pointAdded( Gaffer::GraphComponentPtr spline, Gaffer::GraphComponentPtr point );
		void pointRemoved( Gaffer::GraphComponentPtr spline, Gaffer::GraphComponentPtr point );
		
		bool selectionAcceptance( Gaffer::ConstSetPtr selection, IECore::ConstRunTimeTypedPtr point );
		
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
