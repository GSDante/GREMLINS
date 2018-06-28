#ifdef _TIMEEVENT_HPP_
#define _TIMEEVENT_HPP_

class TimeEvent
{
public:
	TimeEvent(){ /*Empty */}

	TimeEvent( void *pointer, std::time_t _time){
		ptr = pointer;
		time = _time;
	}

	void *getPointer(){
		return this->ptr;
	}

	std::time_t get_time(){
		return this->time;
	}

	bool operator<(TimeEvent timeevent) const{
		return ( this->time < timeevent.get_time());
	}
	
	private:
		void *pointer;
		std::time_t _time;
};

#endif