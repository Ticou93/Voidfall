#pragma once

#include <bitset>
#include "USRArray.h"

//Gravitational and other affector hierarchies such as radiation etc.
//an affector source must be exposed to all the entities it is supposed to affect
//

//Structural hierarchies such as building>unit when a unit is inside a building etc.
//We could sort hierarchies into different entity lists, such that they are guaranteed to be executed first

//Data locality
//Data similarity


//If we run all entities in sequence, then also all components will be sorted

//Separate by properties such that unique behaviour is also processed separately
//WorldPosition
//SystemPosition


struct Position{
	float x{}, y{}, z{};
};
struct Velocity{
	float x{}, y{}, z{};
};

struct Vec3 {
	float x, y, z;
};

struct Transform {
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};

struct Dynamics {
	Vec3 velocity;
	Vec3 acceleration;
};

struct Incentives {
	Vec3 movedirection;

};

//idea is to have component lists for all differing types of entities
//that all have the equal set of components
//Every component system is fully linear in design all the time, with instant access to all data members and ownership

const uint64_t default_array_size = 64;


class EntityFrame{
public:

	EntityFrame(){

	}

	void Update() {
		//Get all systems that apply to these entities, run them
		//If enough components for multiple processes, make a for loop jumping enough and 
		//TaskSystem.Execute() a parallel batch for each iteration

	}

protected:

	USRArray<Position> position;
	USRArray<Velocity> velocity;

	uint64_t m_entitiescount;//Applies to all components
};


class EntitySystem {
public:

	void Update() {
	
		//ents.Update();
	}

private:

	//EntityFrame<> ents;
	//EntityFrame<> gents;
};

class PhysicsSystem {
public:

	//using PosArr = USRArray<Position>;
	//using VelArr = USRArray<Velocity>;

	//Takes array references as input
	void Update(USRArray<Position>& a_posarr, USRArray<Velocity>& a_velarr, uint64_t a_count, float a_dt) {
		for (int i = 0; i < a_count; ++i) {
			a_posarr[i].x += a_velarr[i].x * a_dt;
			a_posarr[i].y += a_velarr[i].y * a_dt;
			a_posarr[i].z += a_velarr[i].z * a_dt;
		}
	}

protected:
};

//CreateEntity(UNIT42)
//Needs a spawn location
//Needs a hierarchy owner


//Message system is a component system as well
class MessageSystem {
public:

};


//
//
//
//struct Entity32{
//	uint32_t data;
//};
//
//struct PositionComponent {
//	float x,y,z;
//};
//
////sphere
//struct CollisionComponent {
//	float radius;
//};
//
////We want to contain entities such that they can be batch run, in localized contiguous data sequences
////Entities must be as small as possible
////Plan for saturation
//
//
////!!!DESIGN NOTES
////Use vertical design(data separated into separated arrays) with data optimized alignments for each
////Cache line is assumed 64 bytes, optimize for it
////Array sizes preferably less than 4MB, 1-2MB could be reasonable.
//
//struct Entity{
//	uint8_t type;//0 = NOTHING
//};
//
//
//
//
//
//
////Entity manager decides what an entity represents and how it is processed
//template <class EntityType> //, class EntityScheme>
//class EntityManager {
//public:
//	EntityManager() = delete;
//	EntityManager(uint64_t a_maxentities = 2048) : m_entitycount(0), m_maxentities(a_maxentities) {
//		m_entitybuffer = new EntityType[m_maxentities];//Use custom allocator instead of calling new
//		//for (int i = 0; i < m_maxentities; ++i) m_entitybuffer[i] = 0;
//		memset(m_entitybuffer, 0, m_maxentities * sizeof(m_entitybuffer[0]));
//	}
//
//	void AddEntity(EntityType & _e) {
//
//	}
//
//	~EntityManager() {
//		if (m_entitybuffer != NULL) delete[] m_entitybuffer;
//	}
//protected:
//
//	//EntityScheme m_entityscheme;
//	EntityType* m_entitybuffer;
//	uint64_t m_entitycount;
//	uint64_t m_maxentities;
//};
//
//
////Only wants to affect physics based entities
//void Physics() {
//
//
//}
