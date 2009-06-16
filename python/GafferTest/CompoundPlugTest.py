import unittest
import gc

import IECore
import Gaffer
import GafferTest

class CompoundPlugTest( unittest.TestCase ) :

	def testContructor( self ) :

		p = Gaffer.CompoundPlug()
		self.assertEqual( p.getName(), "CompoundPlug" )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.In )

		p = Gaffer.V3fPlug( name="b", direction=Gaffer.Plug.Direction.Out )
		self.assertEqual( p.getName(), "b" )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.Out )

	def testSerialisation( self ) :
	
		s = Gaffer.ScriptNode()
		s["n1"] = GafferTest.CompoundPlugNode()
		s["n2"] = GafferTest.CompoundPlugNode()
		
		s["n1"]["p"]["f"].setValue( 10 )
		s["n1"]["p"]["s"].setInput( s["n2"]["p"]["s"] )
		
		ss = s.serialise()
				
		s = Gaffer.ScriptNode()
		s.execute( ss )
		
		self.assertEqual( s["n1"]["p"]["f"].getValue(), 10 )
		self.failUnless( s["n1"]["p"]["s"].getInput().isSame( s["n2"]["p"]["s"] ) )
		
	def testDynamicSerialisation( self ) :
	
		s = Gaffer.ScriptNode()
		s["n1"] = Gaffer.Node()
		s["n1"]["p"] = Gaffer.CompoundPlug( flags = Gaffer.Plug.Flags.Dynamic )
		s["n1"]["p"]["f"] = Gaffer.FloatPlug( flags = Gaffer.Plug.Flags.Dynamic )
		s["n1"]["p"]["f"].setValue( 10 )
		
		ss = s.serialise()
				
		s = Gaffer.ScriptNode()
		s.execute( ss )
		
		self.assertEqual( s["n1"]["p"]["f"].getValue(), 10 )
		
	def testMasterConnectionTracksChildConnections( self ) :
	
		c = Gaffer.CompoundPlug( "c" )
		c["f1"] = Gaffer.FloatPlug()
		c["f2"] = Gaffer.FloatPlug()
		n = Gaffer.Node()
		n["c"] = c		

		c2 = Gaffer.CompoundPlug( "c" )
		c2["f1"] = Gaffer.FloatPlug()
		c2["f2"] = Gaffer.FloatPlug()
		n2 = Gaffer.Node()
		n2["c"] = c2		
		
		n2["c"]["f1"].setInput( n["c"]["f1"] )
		n2["c"]["f2"].setInput( n["c"]["f2"] )
		self.failUnless( n2["c"].getInput().isSame( n["c"] ) )
		
		n2["c"]["f2"].setInput( None )
		self.failUnless( n2["c"].getInput() is None )

		n2["c"]["f2"].setInput( n["c"]["f2"] )
		self.failUnless( n2["c"].getInput().isSame( n["c"] ) )

		c["f3"] = Gaffer.FloatPlug()
		c2["f3"] = Gaffer.FloatPlug()
		
		self.failUnless( n2["c"].getInput() is None )
		
		n2["c"]["f3"].setInput( n["c"]["f3"] )
		self.failUnless( n2["c"].getInput().isSame( n["c"] ) )
		

	def testInputChangedCrash( self ) :
	
		ca = Gaffer.CompoundPlug( "ca" )
		ca["fa1"] = Gaffer.FloatPlug()
		ca["fa2"] = Gaffer.FloatPlug()
		na = Gaffer.Node()
		na["ca"] = ca		

		cb = Gaffer.CompoundPlug( "cb" )
		cb["fb1"] = Gaffer.FloatPlug()
		cb["fb2"] = Gaffer.FloatPlug()
		nb = Gaffer.Node()
		nb["cb"] = cb		
		
		nb["cb"]["fb1"].setInput( na["ca"]["fa1"] )
				
		del ca, na, cb, nb
		while gc.collect() :
			pass
		IECore.RefCounted.collectGarbage()
		
	def testDirtyPropagation( self ) :
	
		c = Gaffer.CompoundPlug( direction=Gaffer.Plug.Direction.Out )
		c["f1"] = Gaffer.FloatPlug( direction=Gaffer.Plug.Direction.Out )
		c["f2"] = Gaffer.FloatPlug( direction=Gaffer.Plug.Direction.Out )
		
		n = Gaffer.Node()
		n["c"] = c

		c["f1"].setValue( 10 )
		c["f2"].setValue( 10 )
		
		self.failIf( c["f1"].getDirty() )
		self.failIf( c["f2"].getDirty() )
		self.failIf( n["c"].getDirty() )
		
		c["f1"].setDirty()

		self.failUnless( c["f1"].getDirty() )
		self.failIf( c["f2"].getDirty() )
		self.failUnless( n["c"].getDirty() )
		
		c["f1"].setValue( 10 )
						
		self.failIf( c["f1"].getDirty() )
		self.failIf( c["f2"].getDirty() )
		self.failIf( n["c"].getDirty() )

		c.setDirty()
		self.failUnless( c["f1"].getDirty() )
		self.failUnless( c["f2"].getDirty() )
		self.failUnless( n["c"].getDirty() )

		c["f1"].setValue( 10 )
		c["f2"].setValue( 10 )

		self.failIf( c["f1"].getDirty() )
		self.failIf( c["f2"].getDirty() )
		self.failIf( n["c"].getDirty() )
		
	def testPlugSetPropagation( self ) :
	
		c = Gaffer.CompoundPlug()
		c["f1"] = Gaffer.FloatPlug()
		
		n = Gaffer.Node()
		n["c"] = c
		
		def setCallback( plug ) :
			
			if plug.isSame( c ) :
				self.set = True

		cn = n.plugSetSignal().connect( setCallback )

		self.set = False
		
		c["f1"].setValue( 10 )
		
		self.failUnless( self.set )
		
if __name__ == "__main__":
	unittest.main()
	
