#ifndef GAFFER_VALUEPLUG_H
#define GAFFER_VALUEPLUG_H

#include "Gaffer/Plug.h"

namespace Gaffer
{

/// The Plug base class defines the concept of a connection
/// point with direction. The ValuePlug class extends this concept
/// to allow the connections to pass values between connection
/// points. The ValuePlug doesn't dictate how the value is stored,
/// but defines the concept of whether the value is up to date (clean)
/// or not (dirty).
class ValuePlug : public Plug
{

	public :
	
		ValuePlug( const std::string &name=staticTypeName(), Direction direction=In );
		virtual ~ValuePlug();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( ValuePlug, ValuePlugTypeId, Plug );

		/// Accepts the input only if it is derived from ValuePlug.
		/// Derived classes may accept more types provided they
		/// derive from ValuePlug too, and they can deal with them
		/// in setFromInput().
		virtual bool acceptsInput( ConstPlugPtr input ) const;
		/// Reimplemented so that values and dirty status can be
		/// propagated from inputs.
		virtual void setInput( PlugPtr input );

		/// Must be implemented by derived classes to set the value
		/// to the default for this Plug.
		/// \todo I think we need this for when an input connection
		/// is removed, so we can revert to default.
		///virtual void setToDefault() = 0;
		
		/// Marks the Plug as dirty (ie the value not being up to
		/// date) and propagates the dirty status to any dependent
		/// Plugs. This takes no arguments as a Plug is made clean
		/// by using the implementation specific setValue() function.
		void setDirty();
		/// Returns true if the Plug is dirty - ie the value held is
		/// not valid.
		bool getDirty() const;

	protected :
	
		/// Must be called by derived classes when the value has been
		/// set. This sets the Plug clean, emits any relevant signals
		/// and propagates the value to the Plug's outputs.
		void valueSet();
		/// Must be called by derived classes before accessing the value
		/// (for instance in a getValue() method). If the plug is dirty this
		/// either calls node()->compute() if there is no input connection, or
		/// setFromInput() if there is. This ensures that the value is updated before
		/// it's accessed.
		void computeIfDirty();
		
		/// Must be implemented to set the value of this Plug from the
		/// value of its input. This allows the Plug to perform any
		/// necessary conversions of the input value. Called by the valueSet()
		/// function to propagate values through the graph.
		virtual void setFromInput() = 0;

	private :

		bool m_dirty;

};

IE_CORE_DECLAREPTR( ValuePlug )

} // namespace Gaffer

#endif // GAFFER_VALUEPLUG_H
