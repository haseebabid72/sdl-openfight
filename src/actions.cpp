#include "actions.h"
#include "player.h"

using namespace std;

Action::Action()
{
}

Action::~Action()
{
}

MoveAction::MoveAction(GLfloat x_vel, GLfloat y_vel)
{
   this->x_vel  = x_vel;
   this->y_vel  = y_vel;
}

MoveAction::~MoveAction()
{
}

void MoveAction::doAction(Player *player, Player *opponent)
{
   player->move(x_vel, y_vel);
}


CreateObjectAction::CreateObjectAction(string object, GLfloat x_pos, GLfloat y_pos, int index)
{
   this->object = object;
   this->x_pos  = x_pos;
   this->y_pos  = y_pos;
   this->index  = index;
}

CreateObjectAction::~CreateObjectAction()
{
}

void CreateObjectAction::doAction(Player *player, Player *opponent)
{
   player->copyObject(object, x_pos, y_pos, index);
}

CollisionAction::CollisionAction(string object, GLfloat x_pos, GLfloat y_pos, int index, string to, string to_opponent)
{
   this->object      = object;
   this->x_pos       = x_pos;
   this->y_pos       = y_pos;
   this->index       = index;
   this->to          = to;
   this->to_opponent = to_opponent;
}

CollisionAction::~CollisionAction()
{
}

void CollisionAction::doAction(Player *player, Player *opponent)
{
   bool opponent_updated = false;

   if(to.size() > 0)
      player->updateState(to);

   if(to_opponent.size() > 0)
      opponent_updated = opponent->updateState(to_opponent, true);

   if(opponent_updated)
      player->copyObject(object, x_pos, y_pos, index);
}

Actions::Actions()
{
}

Actions::Actions(const Actions &a)
{
   for(int i = 0; i < a.actions.size(); i++)
   {
      if(typeid(*a.actions[i]) == typeid(CreateObjectAction))
      {
         actions.push_back(new CreateObjectAction(static_cast<const CreateObjectAction&>(*a.actions[i])));
      }
      else if(typeid(*a.actions[i]) == typeid(CollisionAction))
      {
         actions.push_back(new CollisionAction(static_cast<const CollisionAction&>(*a.actions[i])));
      }
      else if(typeid(*a.actions[i]) == typeid(MoveAction))
      {
         actions.push_back(new MoveAction(static_cast<const MoveAction&>(*a.actions[i])));
      }
   }
}

Actions::~Actions()
{
   for(int i = 0; i < actions.size(); i++)
      delete actions[i];
}

void Actions::addAction(Action *a)
{
   actions.push_back(a);
}

bool Actions::doActions(Player *player, Player *opponent, const type_info& type)
{
   bool result = false;

   for(int i = 0; i < actions.size(); i++)
   {
      if(typeid(*actions[i]) == type)
      {
         actions[i]->doAction(player, opponent);
         result = true;
      }
   }

   return result;
}



