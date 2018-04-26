#include <stdio.h>
#include <stdlib.h>

typedef struct Position{
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
