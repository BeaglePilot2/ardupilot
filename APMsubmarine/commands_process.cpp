/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "Submarine.h"

// called by update navigation at 10Hz
// --------------------
void Submarine::update_commands(void)
{
    if(control_mode == AUTO) {
        if(home_is_set == true && mission.num_commands() > 1) {
            mission.update();
        }
    }
}
