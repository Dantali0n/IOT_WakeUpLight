#include "springyvalue.h";

// Perturb the system to change the "length" of the spring temporarlily
void SpringyValue::perturb(float offset) {
    this->o = offset;
}

// Call "update" every now and then to update the system
// parameter dt specifies the elapsed time since the last update
void SpringyValue::update(float dt) {
    a = (-this->c * this->x - this->k * this->v ) / this->m;
    this->v += this->a * dt;
    this->x += this->v * dt + this->o;
    this->o = 0; // a spring offet only takes one frame
}
