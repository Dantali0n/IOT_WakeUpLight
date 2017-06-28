class SpringyValue 
{
  public:
    float x = 0, v = 0, a = 0, // x (value), v (velocity) and a (acceleration) define the state of the system
          o = 0, // o defines the temporary spring offset w.r.t. its resting position
          c = 20.0, k = 1.5, m = 1.0; // c (spring constant), k (damping constant) and m (mass) define the behavior of the system

    // Perturb the system to change the "length" of the spring temporarlily
    void perturb(float offset);

    // Call "update" every now and then to update the system
    // parameter dt specifies the elapsed time since the last update
    void update(float dt);
};