#include "ship.h"
#include <stdio.h>
#include <stdlib.h>

Ship *createShip(Ship_Type type, Orientation orientation) {
  Ship *ship = malloc(sizeof(Ship));

  ship->type = type;
  ship->orientation = orientation;
  ship->name = NULL;
  ship->hits = 0;

  return ship;
}

Ship *createShipWithName(Ship_Type type, Orientation orientation, char *name) {
  Ship *ship = createShip(type, orientation);
  ship->name = name;
  return ship;
}

void freeShip(Ship *ship) {
  if (ship->name != NULL)
    free(ship->name);

  free(ship);
}