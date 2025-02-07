#ifndef REMOTESTATE_H_
#define REMOTESTATE_H_

#include "Shared/RobotInfo.h"
#include <vector>

class RemoteRouter;

/*! This class represents remote state information recieved from a
 *  remote dog, and can be treated like a WorldState object */
class RemoteState {
    public:
    RemoteState(const RemoteRouter *p);
    virtual ~RemoteState();
    float outputs[NumOutputs];
	float buttons[NumButtons];
	float sensors[NumSensors];

    void update(char *data);
    
    enum StateType {
        OutputState,
		ButtonState,
		SensorState,
    };

    static const int sizes[];
    
    private:
    const RemoteRouter *parent;

    
    RemoteState(RemoteState&);
    RemoteState &operator=(const RemoteState&);
};

#endif /* REMOTESTATE_H_ */
