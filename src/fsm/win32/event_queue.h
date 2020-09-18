#pragma once
#include "fsm/hsm.h"
#include <list>
#include <cstddef>
//Implementation for WIN32 stdlib
//For embedded arch must be implemented ring buffer with background check
class HsmEventQueue {
	private:
		struct EventPack {
			hsm::Hsm& object;
			const fsm::Event& event;
			EventPack(hsm::Hsm& obj, const fsm::Event& ev) : object(obj), event(ev) {}
		};
		std::list<EventPack> _q;
	public:
		inline void putEvent(const fsm::Event& ev, hsm::Hsm& obj) {
			_q.emplace_back(obj, ev);
		}

		void checkQueue() {
			while (!_q.empty()) {
				EventPack& f = _q.front();
				f.object.dispatch(f.event);
				_q.pop_front();
			}
		}
		void clear() {
			_q.clear();
		}
		size_t size()const {
			return _q.size();
		}
};

extern HsmEventQueue eventQueue;

