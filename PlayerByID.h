//
// Created by ameer on 12/28/2022.
//

#ifndef UNTITLED1_PLAYERBYID_H
#define UNTITLED1_PLAYERBYID_H
#include "PlayerByGoals.h"
#include <memory>

class Team;
using std::shared_ptr;

class PlayerByID{
    private:
        int id;
        shared_ptr<Team> ptr_my_team;
        shared_ptr<PlayerByGoals> ptr_goals_player;

    public:

        PlayerByID(int id ,shared_ptr<Team> ptr_my_team,  shared_ptr<PlayerByGoals> ptr_goals_player = nullptr);
        PlayerByID(const PlayerByID& player) = default;
        ~PlayerByID () = default;

        int getID();
        shared_ptr<Team> getTeamPtr();
        shared_ptr<PlayerByGoals> getGoalsPtr();

        void setGoalsPtr(shared_ptr<PlayerByGoals> new_ptr);
        void setTeamPtr(shared_ptr<Team> new_ptr);

        friend bool operator< (const PlayerByID& player_a, const PlayerByID& player_b);
        friend bool operator== (const PlayerByID& player_a, const PlayerByID& player_b);


};

bool operator!= (const PlayerByID& player_a, const PlayerByID& player_b);
bool operator<= (const PlayerByID& player_a, const PlayerByID& player_b);
bool operator>= (const PlayerByID& player_a, const PlayerByID& player_b);
bool operator> (const PlayerByID& player_a, const PlayerByID& player_b);




#endif //UNTITLED1_PLAYERBYID_H
