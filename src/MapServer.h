typedef struct Position
{
    int x;
    int y;
} Position;

typedef struct ObjectivePosition
{
    int nbVillager;
    struct Position mercenaires[6];
    struct Position thebes;
    struct Position oedipe;
    struct Position sphinx;
};

typedef struct Map
{
	int matrix[10][10];
	ObjectivePosition objPos;
} Map;