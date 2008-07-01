import IECore
import Gaffer
import GafferUI
import unittest

class LinearContainerTest( unittest.TestCase ) :

	def testConstruction( self ) :
	
		c = GafferUI.LinearContainer()
		self.assertEqual( c.getName(), "LinearContainer" )
		self.assertEqual( c.getOrientation(), GafferUI.LinearContainer.Orientation.X )
		self.assertEqual( c.getAlignment(), GafferUI.LinearContainer.Alignment.Centre )
		self.assertEqual( c.getSpacing(), 0 )
		
		c = GafferUI.LinearContainer( name="a" )
		self.assertEqual( c.getName(), "a" )
		self.assertEqual( c.getOrientation(), GafferUI.LinearContainer.Orientation.X )
		self.assertEqual( c.getAlignment(), GafferUI.LinearContainer.Alignment.Centre )
		self.assertEqual( c.getSpacing(), 0 )

		c = GafferUI.LinearContainer( spacing=10 )
		self.assertEqual( c.getName(), "LinearContainer" )
		self.assertEqual( c.getOrientation(), GafferUI.LinearContainer.Orientation.X )
		self.assertEqual( c.getAlignment(), GafferUI.LinearContainer.Alignment.Centre )
		self.assertEqual( c.getSpacing(), 10 )

		c = GafferUI.LinearContainer( orientation=GafferUI.LinearContainer.Orientation.Y )
		self.assertEqual( c.getName(), "LinearContainer" )
		self.assertEqual( c.getOrientation(), GafferUI.LinearContainer.Orientation.Y )
		self.assertEqual( c.getAlignment(), GafferUI.LinearContainer.Alignment.Centre )
		self.assertEqual( c.getSpacing(), 0 )

		c = GafferUI.LinearContainer( alignment=GafferUI.LinearContainer.Alignment.Min )
		self.assertEqual( c.getName(), "LinearContainer" )
		self.assertEqual( c.getOrientation(), GafferUI.LinearContainer.Orientation.X )
		self.assertEqual( c.getAlignment(), GafferUI.LinearContainer.Alignment.Min )
		self.assertEqual( c.getSpacing(), 0 )
		
		self.assert_( c.bound().isEmpty() )
	
	def testHorizontalCentred( self ) :
	
		twoByFour = GafferUI.RenderableGadget(
			IECore.MeshPrimitive.createPlane( IECore.Box2f( IECore.V2f( -1, -2 ), IECore.V2f( 1, 2 ) ) )
		)
		
		fourByFour = GafferUI.RenderableGadget(
			IECore.MeshPrimitive.createPlane( IECore.Box2f( IECore.V2f( -2, -2 ), IECore.V2f( 2, 2 ) ) )
		)
		
		fourByTwo = GafferUI.RenderableGadget(
			IECore.MeshPrimitive.createPlane( IECore.Box2f( IECore.V2f( -2, -1 ), IECore.V2f( 2, 1 ) ) )
		)
		
		c = GafferUI.LinearContainer()
		
		c.c1 = twoByFour
		self.assertEqual( c.bound(), IECore.Box3f( IECore.V3f( -1, -2, 0 ), IECore.V3f( 1, 2, 0 ) ) )
		self.assertEqual( c.childTransform( twoByFour ), IECore.M44f.createTranslated( IECore.V3f( 0 ) ) )
		
		c.c2 = fourByFour
		self.assertEqual( c.bound(), IECore.Box3f( IECore.V3f( -3, -2, 0 ), IECore.V3f( 3, 2, 0 ) ) )
		self.assertEqual( c.childTransform( twoByFour ), IECore.M44f.createTranslated( IECore.V3f( -2, 0, 0 ) ) )
		self.assertEqual( c.childTransform( fourByFour ), IECore.M44f.createTranslated( IECore.V3f( 1, 0, 0 ) ) )
		
		c.c3 = fourByTwo
		self.assertEqual( c.bound(), IECore.Box3f( IECore.V3f( -5, -2, 0 ), IECore.V3f( 5, 2, 0 ) ) )
		self.assertEqual( c.childTransform( twoByFour ), IECore.M44f.createTranslated( IECore.V3f( -4, 0, 0 ) ) )
		self.assertEqual( c.childTransform( fourByFour ), IECore.M44f.createTranslated( IECore.V3f( -1, 0, 0 ) ) )
		self.assertEqual( c.childTransform( fourByTwo ), IECore.M44f.createTranslated( IECore.V3f( 3, 0, 0 ) ) )
		
	def testVerticalMin( self ) :
	
		twoByFour = GafferUI.RenderableGadget(
			IECore.MeshPrimitive.createPlane( IECore.Box2f( IECore.V2f( -1, -2 ), IECore.V2f( 1, 2 ) ) )
		)
		
		fourByFour = GafferUI.RenderableGadget(
			IECore.MeshPrimitive.createPlane( IECore.Box2f( IECore.V2f( -2, -2 ), IECore.V2f( 2, 2 ) ) )
		)
		
		fourByTwo = GafferUI.RenderableGadget(
			IECore.MeshPrimitive.createPlane( IECore.Box2f( IECore.V2f( -2, -1 ), IECore.V2f( 2, 1 ) ) )
		)
		
		c = GafferUI.LinearContainer( orientation=GafferUI.LinearContainer.Orientation.Y, alignment=GafferUI.LinearContainer.Alignment.Min)
		
		c.c1 = twoByFour
		self.assertEqual( c.bound(), IECore.Box3f( IECore.V3f( -1, -2, 0 ), IECore.V3f( 1, 2, 0 ) ) )
		self.assertEqual( c.childTransform( twoByFour ), IECore.M44f.createTranslated( IECore.V3f( 0 ) ) )
		
		c.c2 = fourByFour
		self.assertEqual( c.bound(), IECore.Box3f( IECore.V3f( -2, -4, 0 ), IECore.V3f( 2, 4, 0 ) ) )
		self.assertEqual( c.childTransform( twoByFour ), IECore.M44f.createTranslated( IECore.V3f( -1, -2, 0 ) ) )
		self.assertEqual( c.childTransform( fourByFour ), IECore.M44f.createTranslated( IECore.V3f( 0, 2, 0 ) ) )
		
		c.c3 = fourByTwo
		self.assertEqual( c.bound(), IECore.Box3f( IECore.V3f( -2, -5, 0 ), IECore.V3f( 2, 5, 0 ) ) )
		self.assertEqual( c.childTransform( twoByFour ), IECore.M44f.createTranslated( IECore.V3f( -1, -3, 0 ) ) )
		self.assertEqual( c.childTransform( fourByFour ), IECore.M44f.createTranslated( IECore.V3f( 0, 1, 0 ) ) )
		self.assertEqual( c.childTransform( fourByTwo ), IECore.M44f.createTranslated( IECore.V3f( 0, 4, 0 ) ) )
				
if __name__ == "__main__":
	unittest.main()
	
