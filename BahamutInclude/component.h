#pragma once

//================================================
//Description: An abstract component class for 
//	entities. Components will be sorted in a map
//	by their type. So an entity can only have
//	one component of any given type.
//
//Comments: All components are pure data. Systems
//	will take the data stored in Entities and
//	do something with them, assuming the Entity
//	has all the required components.
//================================================
struct Component {

};