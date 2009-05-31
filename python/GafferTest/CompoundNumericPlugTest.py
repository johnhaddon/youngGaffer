import unittest

import IECore

import Gaffer
import GafferTest

class CompoundNumericPlugTest( unittest.TestCase ) :

	def testContructor( self ) :
	
		p = Gaffer.V3fPlug()
		self.assertEqual( p.getName(), "V3fPlug" )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.In )
		
		p = Gaffer.V3fPlug( name="b", direction=Gaffer.Plug.Direction.Out )
		self.assertEqual( p.getName(), "b" )
		self.assertEqual( p.direction(), Gaffer.Plug.Direction.Out )
	
	def testChildRejection( self ) :
	
		p = Gaffer.V3fPlug()
		c = Gaffer.FloatPlug()
		self.assertRaises( Exception, p.addChild, c )
	
	def testChildNames( self ) :
	
		p = Gaffer.V3fPlug()
		p["x"].setValue( 1 )
		p["y"].setValue( 2 )
		p["z"].setValue( 3 )
		
		self.assertEqual( p.getValue(), IECore.V3f( 1, 2, 3 ) )
	
	def testSetFromInput( self ) :
	
		p1 = Gaffer.V3fPlug( direction=Gaffer.Plug.Direction.Out )
		p2 = Gaffer.V3fPlug( direction=Gaffer.Plug.Direction.In )
		
		p1.setValue( IECore.V3f( 1, 2, 3 ) )
		
		p2.setInput( p1 )
		
		self.assertEqual( p2.getValue(), IECore.V3f( 1, 2, 3 ) )
	
	def testMinMaxValues( self ) :
	
		p = Gaffer.V3fPlug()
		self.failIf( p.hasMinValue() )
		self.failIf( p.hasMaxValue() )
		for a in ( "x", "y", "z" ) :
			self.failIf( p[a].hasMinValue() )
			self.failIf( p[a].hasMaxValue() )
		
		p = Gaffer.V3fPlug(
			minValue = IECore.V3f( -1, -2, -3 ),
			maxValue = IECore.V3f( 1, 2, 3 )
		)
		
		self.failUnless( p.hasMinValue() )
		self.failUnless( p.hasMaxValue() )
		for a in ( "x", "y", "z" ) :
			self.failUnless( p[a].hasMinValue() )
			self.failUnless( p[a].hasMaxValue() )
		
		minValue = p.minValue()
		maxValue = p.maxValue()
		self.assertEqual( minValue, IECore.V3f( -1, -2, -3 ) )
		self.assertEqual( maxValue, IECore.V3f( 1, 2, 3 ) )
		i = 0
		for a in ( "x", "y", "z" ) :
			self.assertEqual( p[a].minValue(), minValue[i] )
			self.assertEqual( p[a].maxValue(), maxValue[i] )
			i += 1
	
	def testDefaultValue( self ) :
	
		p = Gaffer.V3fPlug( defaultValue = IECore.V3f( 1, 2, 3 ) )
		self.assertEqual( p.defaultValue(), IECore.V3f( 1, 2, 3 ) )
		self.assertEqual( p["x"].defaultValue(), 1 )
		self.assertEqual( p["y"].defaultValue(), 2 )
		self.assertEqual( p["z"].defaultValue(), 3 )
		
	def testSerialisation( self ) :
	
		s = Gaffer.ScriptNode()		
		n = GafferTest.CompoundNumericNode()
		n["p"].setValue( IECore.V3f( 1, 2, 3 ) )
		s["n"] = n
		
		ss = s.serialise()
								
		s = Gaffer.ScriptNode()
		s.execute( ss )
				
		self.assertEqual( s["n"]["p"].getValue(), IECore.V3f( 1, 2, 3 ) )		

	def testSerialisationWithConnection( self ) :
	
		s = Gaffer.ScriptNode()
		n1 = GafferTest.CompoundNumericNode()
		n2 = GafferTest.CompoundNumericNode()
		n1["p"].setInput( n2["p"] )
		s["n1"] = n1
		s["n2"] = n2
		
		ss = s.serialise()
				
		s = Gaffer.ScriptNode()
		s.execute( ss )
		
		self.failUnless( s["n1"]["p"].getInput().isSame( s["n2"]["p"] ) )
		self.failUnless( s["n1"]["p"]["x"].getInput().isSame( s["n2"]["p"]["x"] ) )
		self.failUnless( s["n1"]["p"]["y"].getInput().isSame( s["n2"]["p"]["y"] ) )
		self.failUnless( s["n1"]["p"]["z"].getInput().isSame( s["n2"]["p"]["z"] ) )

	def testSerialisationWithPartialConnections( self ) :
	
		s = Gaffer.ScriptNode()
		n = GafferTest.CompoundNumericNode()
		a = Gaffer.AddNode()
		n["p"]["x"].setValue( 10 )
		n["p"]["y"].setInput( a["sum"] )
		s["n"] = n
		a["a"] = a
		
		ss = s.serialise()
		
		s = Gaffer.ScriptNode()
		s.execute( ss )
		
		self.assertEqual( s["n"]["p"]["x"].getValue(), 10 )
		self.failUnless( s["n"]["p"]["y"].getInput().isSame( s["a"]["sum"] ) )
		self.assertEqual( s["n"]["p"]["z"].getValue(), 0 )
		
	def testDynamicSerialisation( self ) :
	
		s = Gaffer.ScriptNode()
		n = Gaffer.Node()
		n["p"] = Gaffer.V3fPlug( flags=Gaffer.Plug.Flags.Dynamic )
		s["n"] = n
		
		ss = s.serialise()
		
		s = Gaffer.ScriptNode()
		s.execute( ss )
		
		self.assertEqual( s["n"]["p"].getValue( IECore.V3f( 1, 2, 3 ) ) )

if __name__ == "__main__":
	unittest.main()
	
