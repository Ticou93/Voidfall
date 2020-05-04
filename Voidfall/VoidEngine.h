#pragma once

#include "TaskSystem.h"
#include "Entity.h"
#include <chrono>
#include "USRArray.h"

extern TaskSystem Tasker;

TaskSystem Tasker;//Create a task system, do not use from other threads


//!!!!!!!!VOIDEXECUTE TEMPLATE
//class VoidGame {
//public:
//	bool Init() {}
//	bool Update() {}
//	void Deinit() {}
//protected:
//};
//!!!!!!!!Template


template <class VoidExecute>
class VoidEngine {
public:
	VoidEngine() : m_framecount(0){}
	
	//Init systems
	bool Init() {
		Tasker.Init();//Set up the task system properly
	}

	//Deinit systems
	void Deinit() {
		Tasker.Deinit();
	}

	void Engage() {
		USRArray<Position>* parr = new USRArray<Position>();
		USRArray<Velocity>* varr = new USRArray<Velocity>();
		
		Position pos;
		Velocity vel;
		for (UINT i = 0; i < parr->Size(); ++i) parr->Add(pos);
		for (UINT i = 0; i < varr->Size(); ++i) varr->Add(vel);

		std::chrono::duration<float> dt;
		int i = 0;
		//!!!!INIT PROGRAM
		m_program.Init();
		//!!!!!!!!!!!!!!!!
		while(true){
			auto start = std::chrono::steady_clock::now();

			if (!m_program.Update()) break;



			auto end = std::chrono::steady_clock::now();
			//std::chrono::nanoseconds dt = (end - start);
			dt = (end - start);

			//cout << std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count() << endl;
			//std::this_thread::sleep_for(500ms);
		}

		//!!!!DEINIT PROGRAM
		m_program.Deinit();
		//!!!!!!!!!!!!!!!!!!
	}
	
protected:
	PhysicsSystem m_physics;
	//EntitySystem entitysys;
	uint64_t m_framecount;
	VoidExecute m_program;//The executing program

};

////Define VOIDEXECUTE with your execution class name
//#ifdef VOIDEXECUTE
//int main() {
//	VoidEngine<VOIDEXECUTE> voidengine;
//	if(!voidengine.Init()) return 1;
//	voidengine.Engage();
//	voidengine.Deinit();
//	return 0;
//}
//#endif













//Staggered update, possible?
////Logic update, paths, sets incentives and applies forces
//SystemLogic.Update(0ms);//Updates ~2 times a minute
////Wait 100ms
//Physics.Update(100ms);//Updates 5 times a minute
////Wait 100ms
//UnitLogic.Update(200ms);
////Wait 100ms
//Physics.Update(300ms);//Updates 5 times a minute
////Wait 100ms
//GameLogic.Update(400ms);
////Wait 100ms
////Physics update, applies forces and actually moves things
//Physics.Update(400ms);//Updates 5 times a minute
////Render, read only
//Render.Update(10ms);//Tries to render as often as possible, refresh rate controlled
