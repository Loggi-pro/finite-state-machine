#pragma once
#include "fsm/hsm.h"
#include <iostream>
class HsmTest : public hsm::Hsm {
	public:
		enum HsmTestSignals {
			SIG_A = hsm::ReservedSignals::SIG_USER, // user sisngals start with SIG_USER
			SIG_B, SIG_C, SIG_D, SIG_E, SIG_F, SIG_G, SIG_H
		};
		HsmTest() : Hsm(hsm::as_state(&HsmTest::initial)) {}
	private:
		int _foo;
		hsm::EventResult initial(const hsm::Event* e) {
			std::cout << "top-INIT" << std::endl;
			_foo = 0;
			return eventTransition(&HsmTest::s0);
		}
		hsm::EventResult s0(const  hsm::Event* e) { //state handler
			switch (e->sig) {
			case hsm::ReservedSignals::SIG_ENTRY: std::cout << "s0-ENTRY;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT: std::cout << "s0-EXIT;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_INIT: std::cout << "s0-INIT;" << std::endl; return eventTransition(
				            &HsmTest::s1); //init

			case SIG_E: std::cout << "s0-E;" << std::endl; return  eventTransition(&HsmTest::s211);
			}

			return eventSuper(&Hsm::top);
		}
		/**/ hsm::EventResult s1(const  hsm::Event* e) {  //state handler
			switch (e->sig) {
			case hsm::ReservedSignals::SIG_ENTRY: std::cout << "s1-ENTRY;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT: std::cout << "s1-EXIT;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_INIT: std::cout << "s1-INIT;" << std::endl; return eventTransition(
				            &HsmTest::s11); //init

			case SIG_A: std::cout << "s1-A;" << std::endl; return eventHandled();

			case SIG_B: std::cout << "s1-B;" << std::endl; return eventTransition(&HsmTest::s11);

			case SIG_C: std::cout << "s1-C;" << std::endl; return eventTransition(&HsmTest::s2);

			case SIG_D: std::cout << "s1-D;" << std::endl; return eventTransition(&HsmTest::s0);

			case SIG_F: std::cout << "s1-F;" << std::endl; return eventTransition(&HsmTest::s211);
			}

			return eventSuper(&HsmTest::s0);
		}
		/*  */ hsm::EventResult s11(const  hsm::Event* e) {  //state handler
			switch (e->sig) {
			case hsm::ReservedSignals::SIG_ENTRY: std::cout << "s11-ENTRY;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT: std::cout << "s11-EXIT;" << std::endl; return eventHandled();

			case SIG_G: std::cout << "s11-G;" << std::endl; return eventTransition(&HsmTest::s211);

			case SIG_H:
				if (_foo) { //guard
					std::cout << "s11-H;" << std::endl;
					_foo = 0;
					return eventHandled();
				}

				break;
			}

			return eventSuper(&HsmTest::s1);
		}
		/**/ hsm::EventResult s2(const  hsm::Event* e) {  //state handler
			switch (e->sig) {
			case hsm::ReservedSignals::SIG_ENTRY: std::cout << "s2-ENTRY;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT: std::cout << "s2-EXIT;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_INIT: std::cout << "s2-INIT;" << std::endl; return eventTransition(
				            &HsmTest::s21); //init

			case SIG_C: std::cout << "s2-C;" << std::endl; return eventTransition(&HsmTest::s1);

			case SIG_F: std::cout << "s2-F;" << std::endl; return eventTransition(&HsmTest::s11);
			}

			return eventSuper(&HsmTest::s0);
		}
		/*  */ hsm::EventResult s21(const  hsm::Event* e) {  //state handler
			switch (e->sig) {
			case hsm::ReservedSignals::SIG_ENTRY: std::cout << "s21-ENTRY;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT: std::cout << "s21-EXIT;" << std::endl; return eventHandled();

			case hsm::ReservedSignals::SIG_INIT: std::cout << "s21-INIT;" << std::endl; return eventTransition(
				            &HsmTest::s211); //init

			case SIG_B: std::cout << "s21-B;" << std::endl; return eventTransition(&HsmTest::s211);

			case SIG_H:
				if (!_foo) { //guard
					std::cout << "s21-H;" << std::endl;
					_foo = 1;
					return eventHandled();
				}

				break;
			}

			return eventSuper(&HsmTest::s2);
		}
		/*    */ hsm::EventResult s211(const  hsm::Event* e) {  //state handler
			switch (e->sig) {
			case hsm::ReservedSignals::SIG_ENTRY: std::cout << "s211-ENTRY;" << std::endl;
				return eventHandled();

			case hsm::ReservedSignals::SIG_EXIT: std::cout << "s211-EXIT;" << std::endl; return eventHandled();

			case SIG_D: std::cout << "s211-D;" << std::endl; return eventTransition(&HsmTest::s21);

			case SIG_G: std::cout << "s211-G;" << std::endl; return eventTransition(&HsmTest::s0);
			}

			return eventSuper(&HsmTest::s21);
		}

};