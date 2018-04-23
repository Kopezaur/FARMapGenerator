// Auxiliary Structures
struct Position{
    int x;
    int y;

    // Getters and Setters
    void set_x (int n)
    {
      x = n;
    }

    int get_x (void)
    {
      return x;
    }

    void set_y (int n)
    {
      y = n;
    }

    int get_y (void)
    {
      return y;
    }
};

struct ObjectivePosition{
    int nbVillager;
    struct Position mercenaires[6];
    Position thebes;
    Position oedipe;
    Position sphinx;

    //Getters and Setters
    void set_nbVillager (int n)
    {
      nbVillager = n;
    }

    int get_nbVillager (void)
    {
      return nbVillager;
    }

    void set_thebes (Position n)
    {
      thebes = n;
    }

    int get_thebes (void)
    {
      return thebes;
    }
    void set_oedipe (Position n)
    {
      oedipe = n;
    }

    int get_oedipe (void)
    {
      return oedipe;
    }
    void set_sphinx (Position n)
    {
      sphinx = n;
    }

    int get_sphinx(void)
    {
      return sphinx;
    }
};

int a[][];
ObjectivePosition charPos;

