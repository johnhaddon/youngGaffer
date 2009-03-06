import GafferUI

GafferUI.Layouts.add( "Default", GafferUI.CompoundEditor( children = ( GafferUI.Splittable.SplitDirection.Vertical, ( ( GafferUI.Splittable.SplitDirection.Horizontal, ( ( GafferUI.Splittable.SplitDirection.None, (GafferUI.Viewer(),) ), ( GafferUI.Splittable.SplitDirection.None, (GafferUI.GraphEditor(),) ) ) ), ( GafferUI.Splittable.SplitDirection.Horizontal, ( ( GafferUI.Splittable.SplitDirection.None, (GafferUI.NodeEditor(),) ), ( GafferUI.Splittable.SplitDirection.None, (GafferUI.ScriptEditor(),) ) ) ) ) ) ) )
