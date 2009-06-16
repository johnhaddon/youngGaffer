#ifndef GAFFER_TYPEIDS_H
#define GAFFER_TYPEIDS_H

namespace Gaffer
{

enum TypeId
{

	GraphComponentTypeId = 400000,
	NodeTypeId = 400001,
	PlugTypeId = 400002,
	ValuePlugTypeId = 400003,
	FloatPlugTypeId = 400004,
	IntPlugTypeId = 400005,
	StringPlugTypeId = 400006,
	ScriptNodeTypeId = 400007,
	ApplicationRootTypeId = 400008,
	ScriptContainerTypeId = 400009,
	NodeSetTypeId = 400010,
	ObjectPlugTypeId = 400011,
	GraphComponentSetTypeId = 400012,
	CompoundPlugTypeId = 400013,
	V2fPlugTypeId = 400014,
	V3fPlugTypeId = 400015,
	V2iPlugTypeId = 400016,
	V3iPlugTypeId = 400017,
	Color3fPlugTypeId = 400018,
	Color4fPlugTypeId = 400019,
	
	FirstPythonTypeId = 405000,
	
	LastTypeId = 409999
	
};

} // namespace Gaffer

#endif // GAFFER_TYPEIDS_H
