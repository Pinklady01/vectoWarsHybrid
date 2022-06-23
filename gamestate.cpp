#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "vectorwar.h"
#include "gamestate.h"

extern GGPOSession *ggpo;

static double
degtorad(double deg)
{
   return PI * deg / 180;
}

static double
distance(Position *lhs, Position *rhs)
{
   double x = rhs->x - lhs->x;
   double y = rhs->y - lhs->y;
   return sqrt(x*x + y*y);
}

/*
 * InitGameState --
 *
 * Initialize our game state.
 */

void
GameState::Init(HWND hwnd, int num_players)
{
   int i, w, h, r;

   GetClientRect(hwnd, &_bounds);
   InflateRect(&_bounds, -8, -8);

   w = _bounds.right - _bounds.left;
   h = _bounds.bottom - _bounds.top;
   r = h / 4;

   _framenumber = 0;
   _num_ships = num_players;
   for (i = 0; i < _num_ships; i++) {
      int heading = i * 360 / num_players;
      double cost, sint, theta;

      theta = (double)heading * PI / 180;
      cost = ::cos(theta);
      sint = ::sin(theta);

      _ships[i].position.x = (w / 2) + r * cost;
      _ships[i].position.y = (h / 2) + r * sint;
      _ships[i].heading = (heading + 180) % 360;
      _ships[i].health = STARTING_HEALTH;
      _ships[i].radius = SHIP_RADIUS;
   }

   InflateRect(&_bounds, -8, -8);
   InitCsvColumns();
}

void GameState::GetShipAI(int i, double *heading, double *thrust, int *fire)
{
   *heading = (_ships[i].heading + 5) % 360;
   *thrust = 0;
   *fire = 0;
}

void GameState::ParseShipInputs(int inputs, int i, double *heading, double *thrust, int *fire)
{
   Ship *ship = _ships + i;

   ggpo_log(ggpo, "parsing ship %d inputs: %d.\n", i, inputs);

   if (inputs & INPUT_ROTATE_RIGHT) {
      *heading = (ship->heading + ROTATE_INCREMENT) % 360;
   } else if (inputs & INPUT_ROTATE_LEFT) {
      *heading = (ship->heading - ROTATE_INCREMENT + 360) % 360;
   } else {
      *heading = ship->heading;
   }

   if (inputs & INPUT_THRUST) {
      *thrust = SHIP_THRUST;
   } else if (inputs & INPUT_BREAK) {
      *thrust = -SHIP_THRUST;
   } else {
      *thrust = 0;
   }
   *fire = inputs & INPUT_FIRE;
}

void GameState::MoveShip(int which, double heading, double thrust, int fire)
{
   Ship *ship = _ships + which;
   
   ggpo_log(ggpo, "calculation of new ship coordinates: (thrust:%.4f heading:%.4f).\n", thrust, heading);

   ship->heading = (int)heading;

   if (ship->cooldown == 0) {
      if (fire) {
         ggpo_log(ggpo, "firing bullet.\n");
         for (int i = 0; i < MAX_BULLETS; i++) {
            double dx = ::cos(degtorad(ship->heading));
            double dy = ::sin(degtorad(ship->heading));
            if (!ship->bullets[i].active) {
               ship->bullets[i].active = true;
               ship->bullets[i].position.x = ship->position.x + (ship->radius * dx);
               ship->bullets[i].position.y = ship->position.y + (ship->radius * dy);
               ship->bullets[i].velocity.dx = ship->velocity.dx + (BULLET_SPEED * dx);
               ship->bullets[i].velocity.dy = ship->velocity.dy + (BULLET_SPEED * dy);
               ship->cooldown = BULLET_COOLDOWN;
               break;
            }
         }
      }
   }

   if (thrust) {
      double dx = thrust * ::cos(degtorad(heading));
      double dy = thrust * ::sin(degtorad(heading));

      ship->velocity.dx += dx;
      ship->velocity.dy += dy;
      double mag = sqrt(ship->velocity.dx * ship->velocity.dx + 
                       ship->velocity.dy * ship->velocity.dy);
      if (mag > SHIP_MAX_THRUST) {
         ship->velocity.dx = (ship->velocity.dx * SHIP_MAX_THRUST) / mag;
         ship->velocity.dy = (ship->velocity.dy * SHIP_MAX_THRUST) / mag;
      }
   }
   ggpo_log(ggpo, "new ship velocity: (dx:%.4f dy:%2.f).\n", ship->velocity.dx, ship->velocity.dy);

   ship->position.x += ship->velocity.dx;
   ship->position.y += ship->velocity.dy;
   ggpo_log(ggpo, "new ship position: (dx:%.4f dy:%2.f).\n", ship->position.x, ship->position.y);

   if (ship->position.x - ship->radius < _bounds.left || 
       ship->position.x + ship->radius > _bounds.right) {
      ship->velocity.dx *= -1;
      ship->position.x += (ship->velocity.dx * 2);
   }
   if (ship->position.y - ship->radius < _bounds.top || 
       ship->position.y + ship->radius > _bounds.bottom) {
      ship->velocity.dy *= -1;
      ship->position.y += (ship->velocity.dy * 2);
   }
   for (int i = 0; i < MAX_BULLETS; i++) {
      Bullet *bullet = ship->bullets + i;
      if (bullet->active) {
         bullet->position.x += bullet->velocity.dx;
         bullet->position.y += bullet->velocity.dy;
         if (bullet->position.x < _bounds.left ||
             bullet->position.y < _bounds.top ||
             bullet->position.x > _bounds.right ||
             bullet->position.y > _bounds.bottom) {
            bullet->active = false;
         } else {
            for (int j = 0; j < _num_ships; j++) {
               Ship *other = _ships + j;
               if (distance(&bullet->position, &other->position) < other->radius) {
                  ship->score++;
                  other->health -= BULLET_DAMAGE;
                  bullet->active = false;
                  break;
               }
            }
         }
      }
   }
}

void
GameState::Update(int inputs[], int disconnect_flags)
{
   _framenumber++;
   for (int i = 0; i < _num_ships; i++) {
      double thrust, heading;
      int fire;

      if (disconnect_flags & (1 << i)) {
         GetShipAI(i, &heading, &thrust, &fire);
      } else {
         ParseShipInputs(inputs[i], i, &heading, &thrust, &fire);
      }
      MoveShip(i, heading, thrust, fire);

      if (_ships[i].cooldown) {
         _ships[i].cooldown--;
      }
   }

   StoreFrameInCsv(inputs);
}

void GameState::InitCsvColumns() {
    std::ifstream ifile;
    ifile.open(CSV_FILE_PATH);
    bool exists = false;
    if(ifile)
        exists = true;
    ifile.close();
    file.open(CSV_FILE_PATH, std::ios::app);
    if(exists)
        return;
    std::vector<std::string> columns;
    columns.emplace_back("frame_number");

    for(int i = 1; i < _num_ships + 1; ++i)
    {
        std::string playerPrefix = "p" + std::to_string(i) + "_";
        columns.emplace_back(playerPrefix + "posX");
        columns.emplace_back(playerPrefix + "posY");
        columns.emplace_back(playerPrefix + "velocityX");
        columns.emplace_back(playerPrefix + "velocityY");
        columns.emplace_back(playerPrefix + "heading");
        columns.emplace_back(playerPrefix + "health");
        columns.emplace_back(playerPrefix + "speed");
        columns.emplace_back(playerPrefix + "cooldown");
        for(int bulletIndex = 1; bulletIndex < MAX_BULLETS + 1; ++bulletIndex)
        {
            std::string bulletPrefix = playerPrefix + "bullet" + std::to_string(bulletIndex) + "_";
            columns.emplace_back(bulletPrefix + "active");
            columns.emplace_back(bulletPrefix + "posX");
            columns.emplace_back(bulletPrefix + "posY");
            columns.emplace_back(bulletPrefix + "velocityX");
            columns.emplace_back(bulletPrefix + "velocityY");
        }
        columns.emplace_back(playerPrefix + "score");
        columns.emplace_back(playerPrefix + "input_rotate_left");
        columns.emplace_back(playerPrefix + "input_rotate_right");
        columns.emplace_back(playerPrefix + "input_thrust");
        columns.emplace_back(playerPrefix + "input_break");
        columns.emplace_back(playerPrefix + "input_fire");
    }
    std::string headerLine;
    for(auto column : columns)
    {
        headerLine += column + ";";
    }
    headerLine += "\n";
    file << headerLine;
}

void GameState::StoreFrameInCsv(int inputs[])  {
    std::string frameLine;

    frameLine += std::to_string(_framenumber) + ";";
    for(int i = 0; i < _num_ships; ++i)
    {
        frameLine += std::to_string(_ships[i].position.x) + ";";
        frameLine += std::to_string(_ships[i].position.y) + ";";
        frameLine += std::to_string(_ships[i].velocity.dx) + ";";
        frameLine += std::to_string(_ships[i].velocity.dy) + ";";
        frameLine += std::to_string(_ships[i].heading) + ";";
        frameLine += std::to_string(_ships[i].health) + ";";
        frameLine += std::to_string(_ships[i].speed) + ";";
        frameLine += std::to_string(_ships[i].cooldown) + ";";
        for(auto bullet : _ships[i].bullets)
        {
            frameLine += std::to_string(bullet.active) + ";";
            frameLine += std::to_string(bullet.position.x) + ";";
            frameLine += std::to_string(bullet.position.y) + ";";
            frameLine += std::to_string(bullet.velocity.dx) + ";";
            frameLine += std::to_string(bullet.velocity.dy) + ";";
        }
        frameLine += std::to_string(_ships[i].score) + ";";
        SerializeShipInputs(inputs[i]);
        for(int j = 0; j < 5; ++j)
        {
            frameLine += std::to_string(inputsBool[j]) + ";";
        }
    }

    frameLine += "\n";
    file << frameLine;
}

void GameState::SerializeShipInputs(int inputs) {

    inputsBool[0] = inputs & INPUT_ROTATE_RIGHT;
    inputsBool[1] = inputs & INPUT_ROTATE_LEFT;
    inputsBool[2] = inputs & INPUT_THRUST;
    inputsBool[3] = inputs & INPUT_BREAK;
    inputsBool[4] = inputs & INPUT_FIRE;
}
