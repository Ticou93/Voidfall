#pragma once

#include "TaskSystem.h"
#include "Entity.h"
#include <chrono>
#include "USRArray.h"

extern TaskSystem Tasker;

TaskSystem Tasker;//Create a task system, do not use from other threads

class VoidEngine {
public:
	VoidEngine() : m_framecount(0){}

	void Engage() {
		Init();

		//TESTING!!!!!!!
		//Add scene
		//Add world(includes grid)
		//Add ground -> static game entity
		//Add camera -> noninterfering game entity
		//Add gravity -> affector game entity
		//Add humanunit -> interfering game entity
		
		USRArray<Position>* parr = new USRArray<Position>();
		USRArray<Velocity>* varr = new USRArray<Velocity>();
		
		Position pos;
		Velocity vel;
		for (UINT i = 0; i < parr->Size(); ++i) parr->Add(pos);
		for (UINT i = 0; i < varr->Size(); ++i) varr->Add(vel);

		std::chrono::duration<float> dt;
		int i = 0;
		while(true){
			auto start = std::chrono::steady_clock::now();
			//entitysys.Update();

			
			if(i<5000000) Tasker.Execute([=]() {
				m_physics.Update(*parr, *varr, parr->Count(), dt.count());
				});
			
			if (i == 5000000) {
				std::this_thread::sleep_for(5s);
				while (!Tasker.IsAllDone());
			}
			if (i > 5000000) Tasker.Execute([=]() {
				m_physics.Update(*parr, *varr, parr->Count(), dt.count());
				});
			if (i > 10000000) {
				while (!Tasker.IsAllDone());
				break;
			}
			++i;
			auto end = std::chrono::steady_clock::now();
			//std::chrono::nanoseconds dt = (end - start);
			dt = (end - start);
			//cout << std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count() << endl;
			//std::this_thread::sleep_for(500ms);
		}

		Deinit();
	}
protected:
	void Init() {
		Tasker.Init();//Set up the task system properly
	}
	void Deinit() {
		Tasker.Deinit();
	}

	PhysicsSystem m_physics;
	//EntitySystem entitysys;
	uint64_t m_framecount;
};

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
