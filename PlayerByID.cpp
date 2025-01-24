#include "PlayerByID.h"

PlayerByID::PlayerByID (int id ,shared_ptr<Team> ptr_my_team,  shared_ptr<PlayerByGoals> ptr_goals_player):
        id(id),
        ptr_my_team(ptr_my_team),
        ptr_goals_player(ptr_goals_player)
{
}

int PlayerByID::getID()
{
    return this->id;
}

void PlayerByID::setGoalsPtr(shared_ptr<PlayerByGoals> new_ptr)
{
    ptr_goals_player = new_ptr;
    return;
}

void PlayerByID::setTeamPtr(shared_ptr<Team> new_ptr)
{
    ptr_my_team = new_ptr;
    return;
}

bool operator< (const PlayerByID& player_a, const PlayerByID& player_b)
{
    return player_a.id < player_b.id;
}

bool operator== (const PlayerByID& player_a, const PlayerByID& player_b)
{
    return player_a.id == player_b.id;
}

bool operator!= (const PlayerByID& player_a, const PlayerByID& player_b)
{
    return !(player_a == player_b);
}

bool operator<= (const PlayerByID& player_a, const PlayerByID& player_b)
{
    return !(player_b < player_a);
}

bool operator>= (const PlayerByID& player_a, const PlayerByID& player_b)
{
    return player_b <= player_a;
}

bool operator> (const PlayerByID& player_a, const PlayerByID& player_b)
{
    return player_b < player_a;
}
shared_ptr<Team> PlayerByID::getTeamPtr()
{
    return ptr_my_team;
}
shared_ptr<PlayerByGoals> PlayerByID::getGoalsPtr()
{
    return ptr_goals_player;
}
