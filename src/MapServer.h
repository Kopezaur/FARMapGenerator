typedef struct Position
{
    int x;
    int y;
} Position;

typedef struct ObjectivePosition
{
    int nbVillager;
    Position mercenaries[6];
    Position thebes;
    Position oedipe;
    Position sphinx;
} ObjectivePosition;

typedef struct Map
{
	int matrix[15][15];
	int mapSize;
	ObjectivePosition objPos;
} Map;